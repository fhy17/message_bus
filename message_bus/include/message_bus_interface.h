#pragma once

#ifdef MESSAGE_BUS_EXPORTS
#define MESSAGE_BUS_API __declspec(dllexport)
#else
#define MESSAGE_BUS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Callback)(const char*, size_t len);

MESSAGE_BUS_API void init();
MESSAGE_BUS_API void uninit();
MESSAGE_BUS_API void subscribe(const char* topic, Callback callback);
MESSAGE_BUS_API void publish(const char* topic, const char* message, const size_t len);

#ifdef __cplusplus
}
#endif