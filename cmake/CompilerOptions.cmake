function(calcexpr_target_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
        )
    endif()
endfunction()

function(calcexpr_target_sanitizers target)
    if(CALCEXPR_ENABLE_ASAN)
        if(MSVC)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            target_link_options(${target} PRIVATE /fsanitize=address)
        else()
            target_compile_options(${target} PRIVATE -fsanitize=address)
            target_link_options(${target} PRIVATE -fsanitize=address)
        endif()
    endif()

    if(CALCEXPR_ENABLE_UBSAN)
        if(NOT MSVC)
            target_compile_options(${target} PRIVATE -fsanitize=undefined)
            target_link_options(${target} PRIVATE -fsanitize=undefined)
        else()
            message(WARNING
                "CALCEXPR_ENABLE_UBSAN 已开启，但本模板未为 MSVC 配置 UBSAN。")
        endif()
    endif()
endfunction()

function(calcexpr_target_coverage target)
    if(NOT CALCEXPR_ENABLE_COVERAGE)
        return()
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE --coverage -O0 -g)
        target_link_options(${target} PRIVATE --coverage)
    else()
        message(WARNING
            "CALCEXPR_ENABLE_COVERAGE 已开启，但覆盖率选项只对 GCC 与 Clang 系编译器生效。")
    endif()
endfunction()

function(calcexpr_target_defaults target)
    target_compile_features(${target} PRIVATE c_std_11)
    calcexpr_target_warnings(${target})
    calcexpr_target_sanitizers(${target})
    calcexpr_target_coverage(${target})
endfunction()
