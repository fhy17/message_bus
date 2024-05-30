#include "message_bus_interface.h"

// MessageBus& getMessageBus() { return MessageBus::getInstance(); }

// void MessageBus::subscribe(const std::string& topic, Callback callback) {
//     std::lock_guard<std::mutex> lock(mtx_);
//     subscribers_[topic].push_back(callback);
// }

// void MessageBus::publish(const std::string& topic, const std::string& message) {
//     std::lock_guard<std::mutex> lock(mtx_);
//     if (subscribers_.find(topic) != subscribers_.end()) {
//         for (const auto& callback : subscribers_[topic]) {
//             callback(message);
//         }
//     }
// }

extern "C" {

static MessageBus* bus = nullptr;

void initializeMessageBus() { bus = &MessageBus::getInstance(); }

void destroyMessageBus() { bus = nullptr; }

void subscribe(const char* topic, Callback callback) {
    if (bus) {
        bus->subscribe(topic, [callback](const std::string& message) { callback(message.c_str()); });
    }
}

void publish(const char* topic, const char* message, const size_t len) {
    if (bus) {
        bus->publish(topic, std::string(message, len));
    }
}
}

// C++ 类的实现
#ifdef SYNC_MESSAGE_BUS
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
#else
void MessageBus::subscribe(const std::string& topic, Callback callback) {
    std::lock_guard<std::mutex> lock(subscribers_mtx_);
    subscribers_[topic].push_back(callback);
}

void MessageBus::publish(const std::string& topic, const std::string&& message) {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        message_queue_.push(std::make_pair(topic, message));
    }
    cv_.notify_one();
}

void MessageBus::processMessages() {
    while (running_) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this] { return !message_queue_.empty() || !running_; });
        if (!running_) return;

        auto msg = message_queue_.front();
        message_queue_.pop();
        lock.unlock();

        std::lock_guard<std::mutex> guard(subscribers_mtx_);
        if (subscribers_.find(msg.first) != subscribers_.end()) {
            for (const auto& callback : subscribers_[msg.first]) {
                callback(msg.second);
            }
        }
    }
}
#endif