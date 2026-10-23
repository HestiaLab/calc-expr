# CalcExpr 源文件清单。core 提供可复用计算内核，CLI 负责终端交互。

set(CALCEXPR_CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/calculator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/lexer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/parser.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/logger.c
)

if(CALCEXPR_ENABLE_DEBUG)
    list(APPEND CALCEXPR_CORE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/debug.c
    )
endif()

set(CALCEXPR_CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/command.c
)

if(WIN32)
    list(APPEND CALCEXPR_CLI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/platform/platform_win.c
    )
elseif(APPLE)
    list(APPEND CALCEXPR_CLI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/platform/platform_mac.c
    )
elseif(UNIX)
    list(APPEND CALCEXPR_CLI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/calcexpr/platform/platform_linux.c
    )
endif()
