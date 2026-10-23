# ---------------------------------------------------------------------------
# 跨平台安装规则。
#
# 由 cmake/Packaging.cmake 引入。库与 CMake 包布局对生成项目通用；
# 模板专属的向导与 MSI 文件不在本项目范围内。
# ---------------------------------------------------------------------------

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CALCEXPR_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/CalcExpr")

install(TARGETS calcexpr_core
    EXPORT CalcExprTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CALCEXPR_GENERATED_INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CalcExprConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CalcExprConfig.cmake"
    INSTALL_DESTINATION "${CALCEXPR_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CalcExprConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(EXPORT CalcExprTargets
    NAMESPACE CalcExpr::
    DESTINATION ${CALCEXPR_INSTALL_CMAKEDIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/CalcExprConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/CalcExprConfigVersion.cmake"
    DESTINATION ${CALCEXPR_INSTALL_CMAKEDIR}
)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

if(TARGET calcexpr)
    install(TARGETS calcexpr
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
