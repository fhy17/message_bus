#include "dir_opt.h"
#include "dll_proxy.h"

#include <iostream>
#include <string>

int main() {
    auto curr_path = getCurrentModPath();

    // init message bus dll
    auto file = curr_path + "\\message_bus.dll";
    auto message_bus_ptr = std::make_shared<DllProxy>(file);
    if (!message_bus_ptr) {
        std::cerr << "failed to load message_bus.dll" << std::endl;
        return 0;
    }
    using message_bus_init_type = void();
    using message_bus_uninit_type = void();
    message_bus_ptr->call<message_bus_init_type>("init");

    // init receiver dll
    file = curr_path + "\\receiver.dll";
    auto receiver_ptr = std::make_shared<DllProxy>(file);
    if (!receiver_ptr) {
        std::cerr << "failed to load receiver.dll" << std::endl;
        return 0;
    }
    using receiver_init_type = void();
    receiver_ptr->call<receiver_init_type>("init");

    // init sender dll
    file = curr_path + "\\sender.dll";
    auto sender_ptr = std::make_shared<DllProxy>(file);
    if (!sender_ptr) {
        std::cerr << "failed to load sender.dll" << std::endl;
        return 0;
    }
    using sender_init_type = void();
    sender_ptr->call<sender_init_type>("init");

    // send messages
    std::string msg = "Hello from main program";
    using sender_type = void(const char*, const size_t);
    sender_ptr->call<sender_type>("sendMessageToBus", msg.c_str(), msg.length());
    for (int i = 0; i < 11; ++i) {
        std::string s = "test " + std::to_string(i);
        sender_ptr->call<sender_type>("sendMessageToBus", s.c_str(), s.length());
    }

    // uninit sender dll
    using sender_uninit_type = void();
    sender_ptr->call<sender_uninit_type>("uninit");

    // wait for the user to press any key
    std::cin.get();

    // uninit receiver dll
    using receiver_uninit_type = void();
    receiver_ptr->call<receiver_uninit_type>("uninit");

    // uninit message bus dll
    using message_bus_uninit_type = void();
    message_bus_ptr->call<message_bus_uninit_type>("uninit");

    return 0;
}