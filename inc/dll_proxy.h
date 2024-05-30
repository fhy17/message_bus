#pragma once

#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <string>
#include <map>
#include <functional>
#include <unordered_map>

class DllProxy {
public:
    // DllProxy() : DllProxy("") {}

    DllProxy(const std::string &dll_path) : handle_(nullptr) {
        if (dll_path.size() > 0) load(dll_path);
    }

    ~DllProxy() { unload(); }

    template <typename T, typename... Args>
    typename std::result_of<std::function<T>(Args...)>::type call(const std::string &func_name, Args... args) {
        auto func = getFunc<T>(func_name);
        if (func == nullptr) {
            std::string err = "can not find this function " + func_name;
            throw std::exception(err.c_str());
        }
        return func(std::forward<Args>(args)...);
    }

private:
    bool load(const std::string &dll_path) {
        handle_ = LoadLibraryA(dll_path.c_str());
        if (handle_ == nullptr) {
            std::string err = "Failed to load DLL " + dll_path;
            throw std::runtime_error(err);
            return false;
        }
        return true;
    }

    bool unload() {
        if (handle_ == nullptr) return true;

        auto ret = FreeLibrary(handle_);
        if (!ret) return false;

        handle_ = nullptr;
        return true;
    }

    template <typename T>
    std::function<T> getFunc(const std::string &func_name) {
        auto it = func_.find(func_name);
        if (it == func_.end()) {
            auto addr = GetProcAddress(handle_, func_name.c_str());
            if (!addr) {
                throw std::runtime_error("Failed to get function address");
                return nullptr;
            }
            it = func_.emplace(func_name, addr).first;
        } else {
            // if (IsBadReadPtr(it->second, sizeof(T))) {
            //     // if (IsBadReadPtr(it->second, size)) {
            //     func_.erase(it);
            //     it = func_.end();
            // }
        }

        return std::function<T>((T *)(it->second));
    }

private:
    HMODULE handle_;
    mutable std::unordered_map<std::string, FARPROC> func_;
};

typedef std::shared_ptr<DllProxy> DllProxyPtr;
