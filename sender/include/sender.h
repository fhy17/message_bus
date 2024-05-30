#pragma once

#ifdef SENDER_EXPORTS
#define SENDER_API __declspec(dllexport)
#else
#define SENDER_API __declspec(dllimport)
#endif
namespace Sender {

extern "C" SENDER_API void init();
extern "C" SENDER_API void uninit();
extern "C" SENDER_API void sendMessageToBus(const char* message, const size_t len);

}  // namespace Sender
