cmake_minimum_required(VERSION 3.21...3.31)

project(CalcExpr
    VERSION 0.1.0
    DESCRIPTION "跨平台 C11 数学表达式计算器"
    HOMEPAGE_URL "https://github.com/wubing7755/calc-expr"
    LANGUAGES C
)
set(CALCEXPR_PROJECT_AUTHOR "wubing7755")
set(CALCEXPR_PROJECT_LICENSE "MIT")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CALCEXPR_IS_TOP_LEVEL_DEFAULT ON)
else()
    set(CALCEXPR_IS_TOP_LEVEL_DEFAULT OFF)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "构建类型" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(CALCEXPR_ENABLE_DEBUG "编译调试系统（关闭时零开销）" OFF)
option(CALCEXPR_ENABLE_ASAN "启用 AddressSanitizer（平台支持时）" OFF)
option(CALCEXPR_ENABLE_UBSAN "启用 UndefinedBehaviorSanitizer（平台支持时）" OFF)
option(CALCEXPR_ENABLE_COVERAGE "启用覆盖率编译选项（平台支持时）" OFF)
option(CALCEXPR_BUILD_CLI "构建 calcexpr 命令行程序" ${CALCEXPR_IS_TOP_LEVEL_DEFAULT})
option(CALCEXPR_BUILD_TESTING "构建 CTest 测试" ${CALCEXPR_IS_TOP_LEVEL_DEFAULT})
option(CALCEXPR_INSTALL "生成安装规则" ${CALCEXPR_IS_TOP_LEVEL_DEFAULT})

set(CALCEXPR_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/calcexpr_version.h.in"
    "${CALCEXPR_GENERATED_INCLUDE_DIR}/calcexpr/version.h"
    @ONLY
)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerOptions.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Sources.cmake")

add_library(calcexpr_core ${CALCEXPR_CORE_SOURCES})
add_library(CalcExpr::calcexpr_core ALIAS calcexpr_core)

target_compile_features(calcexpr_core PUBLIC c_std_11)

target_compile_definitions(calcexpr_core
    PRIVATE
        CALCEXPR_BUILDING_LIBRARY
    PUBLIC
        $<$<BOOL:${CALCEXPR_ENABLE_DEBUG}>:DEBUG_ENABLE=1>
        $<$<NOT:$<BOOL:${CALCEXPR_ENABLE_DEBUG}>>:DEBUG_ENABLE=0>
        $<$<BOOL:${CALCEXPR_ENABLE_DEBUG}>:DEBUG_DEFAULT_LEVEL=5>
        $<$<BOOL:${CALCEXPR_ENABLE_DEBUG}>:DEBUG_DEFAULT_MODULE=0xFF>
)
if(BUILD_SHARED_LIBS)
    target_compile_definitions(calcexpr_core PUBLIC CALCEXPR_SHARED)
endif()
set_target_properties(calcexpr_core PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(calcexpr_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CALCEXPR_GENERATED_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr
)
if(UNIX)
    target_link_libraries(calcexpr_core PUBLIC m)
endif()
calcexpr_target_defaults(calcexpr_core)

if(CALCEXPR_BUILD_CLI)
    if(WIN32)
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/res/calcexpr.rc.in"
            "${CMAKE_CURRENT_BINARY_DIR}/generated/calcexpr.rc"
            @ONLY
        )
    endif()
    add_executable(calcexpr ${CALCEXPR_CLI_SOURCES})
    if(WIN32)
        target_sources(calcexpr PRIVATE
            "${CMAKE_CURRENT_BINARY_DIR}/generated/calcexpr.rc"
        )
    endif()
    target_include_directories(calcexpr PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/platform
    )
    target_link_libraries(calcexpr PRIVATE calcexpr_core)
    calcexpr_target_defaults(calcexpr)
    set_target_properties(calcexpr PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

if(CALCEXPR_BUILD_TESTING)
    enable_testing()
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Tests.cmake")
endif()

if(CALCEXPR_INSTALL)
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packaging.cmake")
endif()

message(STATUS "CalcExpr ${PROJECT_VERSION} | ${CMAKE_BUILD_TYPE} | C11 | 调试系统: ${CALCEXPR_ENABLE_DEBUG}")
