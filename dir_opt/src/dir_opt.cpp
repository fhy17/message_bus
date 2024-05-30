#include "dir_opt.h"
#if defined(_MSC_VER)
#if _MSVC_LANG >= 201703L
#include <filesystem>
#else
#include <windows.h>
#endif
#endif

std::string getCurrentModPath() {
#if defined(_MSC_VER)
#if _MSVC_LANG >= 201703L
    return std::filesystem::current_path().string();
#else
    char path[MAX_PATH] = {};
    GetModuleFileNameA(0, path, MAX_PATH);
    std::string tmp_path = path;

    auto pos = tmp_path.rfind("\\");
    if (pos != tmp_path.npos) {
        return std::move(tmp_path.substr(0, pos));
    }
    return std::string("");
#endif
#endif
}