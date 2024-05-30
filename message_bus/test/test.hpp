#pragma once
#include <string>

#ifndef DEPENDENCY
#define DEPENDENCY ""
#endif

std::string getTestDependencyPath() {
    std::string dependency_path = DEPENDENCY;
    if (dependency_path.empty()) return "";
    return std::move(dependency_path);
}