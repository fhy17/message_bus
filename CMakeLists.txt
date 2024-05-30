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

# 设置googletest 运行时为 MD, 默认为MT
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)


# 编译宏
add_definitions(-DWIN32_LEAN_AND_MEAN)
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-DDebug)
endif()


# 为release版本添加调试信息，生成pdb文件
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")


# add_compile_options(-Wall)

# 配置编译类型 Debug、Release、RelWithDebInfo、MinSizeRel
# set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

# set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

find_program(CPPCHECK_PATH cppcheck REQUIRED)
if(CPPCHECK_PATH)
    message("Found cppcheck: ${CPPCHECK_PATH}")
else()
    message(FATAL_ERROR "cppcheck not found.")
endif()

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

# message("CMAKE_CURRENT_SOURCE_DIR " ${CMAKE_CURRENT_SOURCE_DIR})
# message("CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})

# if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#     # set the output directory for the debug version of the executable
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
#     # set the output directory for the debug version of the library file
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

#     # set the suffix of the library file of the debug version
#     # set(CMAKE_DEBUG_POSTFIX "_d")
# elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
#     # set the output directory for the release version of the executable
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
#     # set the output directory for the release version of the library file
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

#     # set the suffix of the library file of the release version
#     # set(CMAKE_RELEASE_POSTFIX "_r")
# endif()

# if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
#     message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
#     message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG})
# elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
#     message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
#     message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
# endif()

# message("EXECUTABLE_OUTPUT_PATH"  ${EXECUTABLE_OUTPUT_PATH})
# message("LIBRARY_OUTPUT_PATH" ${LIBRARY_OUTPUT_PATH})

message(STATUS "CMAKE_GENERATOR_PLATFORM: " ${CMAKE_GENERATOR_PLATFORM})
# message(STATUS "CMAKE_GENERATOR: " ${CMAKE_GENERATOR})

if(${CMAKE_GENERATOR_PLATFORM} MATCHES [xX]64)
    set(CURRENT_PLATFORM "x64")
elseif(${CMAKE_GENERATOR_PLATFORM} MATCHES [wW]in32)
    set(CURRENT_PLATFORM "x86")
else()
	set(CURRENT_PLATFORM "x64")
	message(STATUS "CMAKE_GENERATOR_PLATFORM is NULL, set default value \"x64\" for CURRENT_PLATFORM")
endif()
message(STATUS "CURRENT_PLATFORM: " ${CURRENT_PLATFORM})

message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    set(BUILD_TYPE "debug")
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
    set(BUILD_TYPE "release")
endif()

set(CMAKE_INSTALL_PREFIX  ${CMAKE_SOURCE_DIR}/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX "  ${CMAKE_INSTALL_PREFIX})

# set google test libraries path
set(GOOGLE_TEST_LIB_PATH ${CMAKE_SOURCE_DIR}/../extern/googletest/lib)
set(GOOGLE_TEST_LIB_PATH ${GOOGLE_TEST_LIB_PATH}/${CURRENT_PLATFORM}/${BUILD_TYPE})
message(STATUS "GOOGLE_TEST_LIB_PATH: " ${GOOGLE_TEST_LIB_PATH})

enable_testing()

project ("test" VERSION 1.0.0.0)

# 包含子项目
add_subdirectory("dir_opt")

add_subdirectory("call")

add_subdirectory("message_bus")

add_subdirectory("sender")

add_subdirectory("receiver")

#=============Copy Source files to Build Runtime Dir=============== 
#该内容一般放在项目顶层CMakelists.txt的最后
#目的是将项目生成后的执行文件所需的第三方库复制到执行程序目录
#并区分Debug和Release版本
#该方法中的COMMAND_EXPAND_LISTS参数值得关注，可以复制列表内所有文件
# set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# FILE(GLOB Glog_DLL CONFIGURE_DEPENDS 
#     ${CMAKE_SOURCE_DIR}/external_dependencies/glog/bin/*.dll
# ) 
   
# add_custom_target(target ALL
#     VERBATIM
#     COMMAND_EXPAND_LISTS
#     COMMAND ${CMAKE_COMMAND} -E 
#         make_directory "${PROJECT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/" 
#     COMMAND ${CMAKE_COMMAND} -E 
#         copy_if_different  
#             "$<$<CONFIG:Release>:${Glog_DLL}>"  
#             "$<$<CONFIG:Debug>:${Glog_DLL}>" 
#             "${PROJECT_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>/"
#     COMMENT "copy glog dll to runtime dir"
# )

# 设置工程多处理器编译
# IF (WIN32)
# 		if(MSVC)
# 				OPTION(USE_MP "use multiple" ON)
# 				OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP 
# 					"Set The Global Option COMPILE_FLAGS /MP to target." ON)
# 				if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
# 					set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
# 					set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
# 				endif()
# 				set(VS_STARTUP_PROJECT ${PROJECT_NAME})
# 		ENDIF(MSVC)
# ENDIF()