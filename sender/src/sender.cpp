// #include "message_bus_interface.h"
#include "sender.h"
#include "dir_opt.h"
#include "dll_proxy.h"

// #include <iostream>
// #include <Windows.h>

// typedef void (*PublishFunc)(const char*, const char*, const size_t);
namespace Sender {
static DllProxyPtr sender_ptr = nullptr;
extern "C" SENDER_API void init() {
    auto curr_path = getCurrentModPath();
    auto file = curr_path + "\\message_bus.dll";
    sender_ptr = std::make_shared<DllProxy>(file);
}

extern "C" SENDER_API void uninit() {
    if (sender_ptr) sender_ptr.reset();
}

extern "C" SENDER_API void sendMessageToBus(const char* message, const size_t len) {
    if (sender_ptr) {
        using type = void(const char*, const char*, const size_t);
        sender_ptr->call<type>("publish", "topic1", message, len);
    }
}
}  // namespace Sender
