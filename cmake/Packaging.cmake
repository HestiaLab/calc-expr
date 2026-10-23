# 跨平台安装与打包规则。
#
# 本项目统一使用 CPack；模板的 MSI/WiX 链路不进入生成项目。
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/InstallRules.cmake")

string(TOLOWER "${PROJECT_NAME}" CALCEXPR_PROJECT_ID)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "${CALCEXPR_PROJECT_AUTHOR}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;TGZ")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # CPack 的 productbuild 生成器在当前 CMake 版本上可能产出空分发；
    # 本项目的本地 CPack 统一产出 TGZ。
    set(CPACK_GENERATOR "TGZ")
else()
    find_program(CPACK_DEB_EXECUTABLE dpkg)
    find_program(CPACK_RPM_EXECUTABLE rpmbuild)
    set(CPACK_GENERATOR "TGZ")
    if(CPACK_DEB_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CALCEXPR_PROJECT_AUTHOR}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    endif()
    if(CPACK_RPM_EXECUTABLE)
        list(APPEND CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_LICENSE "${CALCEXPR_PROJECT_LICENSE}")
        set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
    endif()
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
    "/build/"
    "/_CPack_Packages/"
    "/\\\\.git/"
    "/\\\\.vs/"
    "/\\\\.cache/"
    "/\\\\.hermes/"
    "/install/"
    "/install-"
    "/dist/"
    "/out/"
    "\\\\.swp$"
    "\\\\.swo$"
    "\\\\.user$"
    "\\\\.idx$"
)

include(CPack)
