cmake_minimum_required(VERSION 3.21...3.31)

project(CalcExprSubprojectSmoke LANGUAGES C)

if(NOT DEFINED CALCEXPR_SOURCE_DIR)
    message(FATAL_ERROR "CALCEXPR_SOURCE_DIR 必须指向 CalcExpr 源码树")
endif()

add_subdirectory("${CALCEXPR_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/calcexpr-standard")

if(NOT TARGET CalcExpr::calcexpr_core)
    message(FATAL_ERROR "Expected CalcExpr::calcexpr_core alias target")
endif()

foreach(option_name IN ITEMS
        CALCEXPR_BUILD_CLI
        CALCEXPR_BUILD_TESTING
        CALCEXPR_INSTALL)
    if(${option_name})
        message(FATAL_ERROR "${option_name} 在子项目构建中应默认关闭")
    endif()
endforeach()

foreach(target_name IN ITEMS
        calcexpr
        calcexpr_calculator_tests
        calcexpr_command_tests
        calcexpr_tests)
    if(TARGET ${target_name})
        message(FATAL_ERROR "${target_name} 不应在子项目构建中默认创建")
    endif()
endforeach()

add_executable(subproject_smoke main.c)
target_link_libraries(subproject_smoke PRIVATE CalcExpr::calcexpr_core)

enable_testing()
add_test(NAME subproject_smoke COMMAND subproject_smoke)
