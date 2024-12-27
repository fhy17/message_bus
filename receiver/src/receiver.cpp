// #include "message_bus_interface.h"
#include "receiver.h"
#include "dir_opt.h"
#include "dll_proxy.h"

namespace Receiver {
typedef void (*SubscribeFunc)(const char*, void (*)(const char*));

static DllProxyPtr receiver_ptr = nullptr;

void receiveMessageFromBus(const char* message, size_t len) {
    // std::cout << "received at " << time(0) << std::endl;
    std::cout << "receive: " << message << ", len: " << len << std::endl;
}

extern "C" RECEIVER_API void init() {
    auto curr_path = getCurrentModPath();
    auto file = curr_path + "\\message_bus.dll";
    receiver_ptr = std::make_shared<DllProxy>(file);
    if (receiver_ptr) {
        using type = void(const char*, void (*)(const char*, size_t));
        receiver_ptr->call<type>("subscribe", "topic1", receiveMessageFromBus);
    }
}

extern "C" RECEIVER_API void uninit() {
    if (receiver_ptr) receiver_ptr.reset();
}

// extern "C" RECEIVER_API void startListening() {
//     // getMessageBus().subscribe("topic1", receiveMessageFromBus);
//     auto handle = LoadLibrary("message_bus.dll");
//     if (handle == NULL) {
//         std::cerr << "Failed to load message_bus.dll" << std::endl;
//         return;
//     }

//     SubscribeFunc subscribe = (SubscribeFunc)GetProcAddress(handle, "subscribe");

//     if (subscribe) {
//         subscribe("topic1", receiveMessageFromBus);
//     } else {
//         std::cerr << "Failed to get functions \"subscribe\" from message_bus.dll" << std::endl;
//     }

//     // FreeLibrary(handle);
// }
}  // namespace Receiver
