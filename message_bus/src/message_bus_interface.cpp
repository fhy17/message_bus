#include "message_bus_interface.h"

#ifdef SYNC_MESSAGE_BUS  // synchronous model
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
#else  // asynchronous model
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

    std::unordered_map<std::string, std::vector<Callback>> subscribers_;  // <topic, message_callback>
    std::queue<std::pair<std::string, std::string>> message_queue_;       // <topic, message>
    std::mutex publish_mtx_;
    std::condition_variable cv_;
    bool running_;
    std::thread worker_;
    std::mutex subscribe_mtx_;
};
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

static MessageBus* bus = nullptr;

void init() { bus = &MessageBus::getInstance(); }

void uninit() { bus = nullptr; }

void subscribe(const char* topic, Callback callback) {
    if (bus)
        bus->subscribe(topic, [callback](const std::string& message) { callback(message.c_str(), message.length()); });
}

void publish(const char* topic, const char* message, const size_t len) {
    if (bus) bus->publish(topic, std::string(message, len));
}

#ifdef __cplusplus
}  // extern "C"
#endif

// C++ 类的实现
#ifdef SYNC_MESSAGE_BUS  // synchronous model
void MessageBus::subscribe(const std::string& topic, Callback callback) {
    std::lock_guard<std::mutex> lock(mtx_);
    subscribers_[topic].push_back(callback);
}

void MessageBus::publish(const std::string& topic, const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (subscribers_.find(topic) != subscribers_.end()) {
        for (const auto& callback : subscribers_[topic]) {
            callback(message);
        }
    }
}
#else  // asynchronous model
void MessageBus::subscribe(const std::string& topic, Callback callback) {
    std::lock_guard<std::mutex> lock(subscribe_mtx_);
    subscribers_[topic].push_back(callback);
}

void MessageBus::publish(const std::string& topic, const std::string&& message) {
    {
        std::lock_guard<std::mutex> lock(publish_mtx_);
        message_queue_.push(std::make_pair(topic, message));
    }
    cv_.notify_one();
}

void MessageBus::processMessages() {
    while (running_) {
        std::unique_lock<std::mutex> lock(publish_mtx_);
        cv_.wait(lock, [this] { return !message_queue_.empty() || !running_; });
        if (!running_) return;

        auto msg = message_queue_.front();
        message_queue_.pop();
        lock.unlock();

        std::lock_guard<std::mutex> guard(subscribe_mtx_);
        if (subscribers_.find(msg.first) != subscribers_.end()) {
            for (const auto& callback : subscribers_[msg.first]) {
                callback(msg.second);
            }
        }
    }
}
#endif