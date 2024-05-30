#pragma once

#ifdef MESSAGE_BUS_EXPORTS
#define MESSAGE_BUS_API __declspec(dllexport)
#else
#define MESSAGE_BUS_API __declspec(dllimport)
#endif

// #include <string>
// #include <functional>
// #include <unordered_map>
// #include <vector>
// #include <mutex>

// class MESSAGE_BUS_API MessageBus {
// public:
//     using Callback = std::function<void(const std::string&)>;

//     static MessageBus& getInstance() {
//         static MessageBus instance;
//         return instance;
//     }

//     void subscribe(const std::string& topic, Callback callback);
//     void publish(const std::string& topic, const std::string& message);

// private:
//     MessageBus() = default;
//     ~MessageBus() = default;

//     MessageBus(const MessageBus&) = delete;
//     MessageBus& operator=(const MessageBus&) = delete;

//     std::unordered_map<std::string, std::vector<Callback>> subscribers_;
//     std::mutex mtx_;
// };

// extern "C" MESSAGE_BUS_API MessageBus& getMessageBus();

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*Callback)(const char*);

MESSAGE_BUS_API void init();
MESSAGE_BUS_API void uninit();
MESSAGE_BUS_API void subscribe(const char* topic, Callback callback);
MESSAGE_BUS_API void publish(const char* topic, const char* message, const size_t len);

#ifdef __cplusplus
}
#endif

#ifdef SYNC_MESSAGE_BUS
#ifdef __cplusplus
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <mutex>

class MessageBus {
public:
    using Callback = std::function<void(const std::string&)>;

    static MessageBus& getInstance() {
        static MessageBus instance;
        return instance;
    }

    void subscribe(const std::string& topic, Callback callback);
    void publish(const std::string& topic, const std::string& message);

private:
    MessageBus() = default;
    ~MessageBus() = default;

    MessageBus(const MessageBus&) = delete;
    MessageBus& operator=(const MessageBus&) = delete;

    std::unordered_map<std::string, std::vector<Callback>> subscribers_;
    std::mutex mtx_;
};
#endif
#else
#ifdef __cplusplus
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

class MessageBus {
public:
    using Callback = std::function<void(const std::string&)>;

    static MessageBus& getInstance() {
        static MessageBus instance;
        return instance;
    }

    void subscribe(const std::string& topic, Callback callback);
    void publish(const std::string& topic, const std::string&& message);

private:
    MessageBus() : running_(true) {
        worker_ = std::thread([this] { this->processMessages(); });
    }

    ~MessageBus() {
        running_ = false;
        if (worker_.joinable()) worker_.join();
    }

    MessageBus(const MessageBus&) = delete;
    MessageBus& operator=(const MessageBus&) = delete;

    void processMessages();

    std::unordered_map<std::string, std::vector<Callback>> subscribers_;
    std::queue<std::pair<std::string, std::string>> message_queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool running_;
    std::thread worker_;
    std::mutex subscribers_mtx_;
};

#endif

#endif
