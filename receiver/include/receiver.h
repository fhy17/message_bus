#pragma once

#ifdef RECEIVER_EXPORTS
#define RECEIVER_API __declspec(dllexport)
#else
#define RECEIVER_API __declspec(dllimport)
#endif

namespace Receiver {
extern "C" RECEIVER_API void init();
extern "C" RECEIVER_API void uninit();
// extern "C" RECEIVER_API void stopListen();
}  // namespace Receiver
