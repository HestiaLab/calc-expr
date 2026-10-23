# 测试注册辅助函数。
#
# 每个测试源文件生成一个独立可执行文件并注册到 CTest。
# 测试断言使用 Unity（vendored 到 tests/vendor/unity/，MIT）。

set(CALCEXPR_UNITY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity/unity.c
)

function(calcexpr_add_test target)
    cmake_parse_arguments(CALCEXPR_TEST
        ""
        ""
        "SOURCES;LIBS"
        ${ARGN}
    )

    if(NOT CALCEXPR_TEST_SOURCES)
        message(FATAL_ERROR "calcexpr_add_test(${target}) requires SOURCES")
    endif()

    add_executable(${target}
        ${CALCEXPR_TEST_SOURCES}
        ${CALCEXPR_UNITY_SOURCES}
    )

    target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/vendor/unity
    )

    if(CALCEXPR_TEST_LIBS)
        target_link_libraries(${target} PRIVATE ${CALCEXPR_TEST_LIBS})
    endif()

    calcexpr_target_defaults(${target})

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    add_test(NAME ${target} COMMAND $<TARGET_FILE:${target}>)
    set_tests_properties(${target} PROPERTIES LABELS "unit")
    list(APPEND CALCEXPR_TEST_TARGETS ${target})
    set(CALCEXPR_TEST_TARGETS "${CALCEXPR_TEST_TARGETS}" PARENT_SCOPE)
endfunction()

set(CALCEXPR_TEST_TARGETS "")

# 架构门禁：include/ 中每个公共函数声明都必须在 src/ 中有定义。
# Python 解释器是必需项：缺少时不能静默少注册门禁。
find_package(Python3 COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    add_test(NAME calcexpr_decl_def_check
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_decl_def.py
            ${CMAKE_CURRENT_SOURCE_DIR}
            --min-declarations 1
            --min-definitions 1)
    set_tests_properties(calcexpr_decl_def_check PROPERTIES LABELS "architecture")
else()
    message(FATAL_ERROR
        "找不到 Python3 解释器，但 CALCEXPR_BUILD_TESTING 已开启，架构门禁依赖它。"
        "请安装 Python 3，或使用 -DCALCEXPR_BUILD_TESTING=OFF 关闭测试套件。")
endif()

# 门禁自检：验证质量门禁脚本确实能失败。
add_test(NAME calcexpr_gate_behaviors
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/tests/test_gate_behaviors.py)
set_tests_properties(calcexpr_gate_behaviors PROPERTIES LABELS "architecture")

calcexpr_add_test(calcexpr_calculator_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_calculator.c
    LIBS
        calcexpr_core
)

calcexpr_add_test(calcexpr_command_tests
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_command.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/command.c
    LIBS
        calcexpr_core
)

# 版本门禁：CMake 版本、生成头、CHANGELOG 与 CLI --version 必须一致。
set(CALCEXPR_VERSION_GATE_ARGS
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_version_consistency.py
    ${CMAKE_CURRENT_SOURCE_DIR}
    --build-dir ${CMAKE_BINARY_DIR}
)
if(TARGET calcexpr)
    list(APPEND CALCEXPR_VERSION_GATE_ARGS --exe $<TARGET_FILE:calcexpr>)
endif()
add_test(NAME calcexpr_version_consistency
    COMMAND ${Python3_EXECUTABLE} ${CALCEXPR_VERSION_GATE_ARGS}
)
set_tests_properties(calcexpr_version_consistency PROPERTIES LABELS "architecture")
if(TARGET calcexpr)
    set_tests_properties(calcexpr_version_consistency PROPERTIES DEPENDS calcexpr)
endif()

# 文档门禁：相对链接、锚点、代码围栏与 README 文档索引完整性。
add_test(NAME calcexpr_docs_check
    COMMAND ${Python3_EXECUTABLE}
        ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_docs.py
        ${CMAKE_CURRENT_SOURCE_DIR}
        --mode all)
set_tests_properties(calcexpr_docs_check PROPERTIES LABELS "documentation")

# CPack 配置门禁：生成项目通用检查（包名、版本、vendor、generator）。
if(CALCEXPR_INSTALL)
    find_package(Python3 COMPONENTS Interpreter REQUIRED)
    add_test(NAME calcexpr_cpack_config
        COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-cpack-config.py
            ${CMAKE_CURRENT_BINARY_DIR}/CPackConfig.cmake
    )
    set_tests_properties(calcexpr_cpack_config PROPERTIES LABELS "packaging")
endif()

add_custom_target(calcexpr_tests
    DEPENDS
        ${CALCEXPR_TEST_TARGETS}
)
