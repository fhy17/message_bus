# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
cmake_minimum_required (VERSION 3.20)

# 设置MSVC编译编码 默认多字节编码
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
# add_definitions(-DUNICODE -D_UNICODE)
# add_definitions(-DUNICODE -D_UNICODE -DGOOGLE_GLOG_DLL_DECL=)


# 配置编译选项
# 通过命令add_compile_options命令可以为所有编译器配置编译选项（同时对多个编译器生效）
# 通过设置变量CMAKE_C_FLAGS可以配置c编译器的编译选项
# 而设置变量CMAKE_CXX_FLAGS可配置针对c++编译器的编译选项

# 声明C使用c99标准，C++使用c++17标准
# message(STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID})
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
else()
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_CXX_STANDARD 17)
endif()
#  message(STATUS "CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /EHsc")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")


# 设置多线程运行时选项
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# 编译宏
add_definitions(-DWIN32_LEAN_AND_MEAN)
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDebug)
endif()


# 为release版本添加调试信息，生成pdb文件
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

find_program(CPPCHECK_PATH cppcheck REQUIRED)
if(CPPCHECK_PATH)
    message("Found cppcheck: ${CPPCHECK_PATH}")
else()
    message(FATAL_ERROR "cppcheck not found.")
endif()

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})


if(${CMAKE_GENERATOR_PLATFORM} MATCHES [xX]64)
    set(CURRENT_PLATFORM "x64")
elseif(${CMAKE_GENERATOR_PLATFORM} MATCHES [wW]in32)
    set(CURRENT_PLATFORM "x86")
else()
	set(CURRENT_PLATFORM "x64")
	message(STATUS "CMAKE_GENERATOR_PLATFORM is NULL, set default value \"x64\" for CURRENT_PLATFORM")
endif()
# message(STATUS "CURRENT_PLATFORM: " ${CURRENT_PLATFORM})

# message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(BUILD_TYPE "debug")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(BUILD_TYPE "release")
endif()

set(CMAKE_INSTALL_PREFIX  ${CMAKE_SOURCE_DIR}/install/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX "  ${CMAKE_INSTALL_PREFIX})

project ("test" VERSION 1.0.0.0)

# 包含子项目
add_subdirectory("dir_opt")

add_subdirectory("call")

add_subdirectory("message_bus")

add_subdirectory("sender")

add_subdirectory("receiver")