#!/usr/bin/env python3
"""校验生成项目的 CPack 配置是否可用于发布。

生成项目使用 CPack 的 TGZ/ZIP（Windows）或 TGZ/DEB/RPM（Linux）。
本脚本不针对模板的 NSIS 安装器，而是检查生成项目真正依赖的字段：

- ``CPACK_PACKAGE_NAME`` 非空；
- ``CPACK_PACKAGE_VERSION`` 为 ``x.y.z`` 且与 MAJOR/MINOR/PATCH 一致；
- ``CPACK_PACKAGE_VENDOR`` 非空；
- ``CPACK_GENERATOR`` 非空；
- Windows 上生成器必须包含 ``ZIP``。

用法::

    python3 scripts/check-cpack-config.py <CPackConfig.cmake>

退出码：0 = 通过；1 = 配置缺失或不合法；2 = 用法错误。
"""

import argparse
import re
import sys
from pathlib import Path


def _pin_utf8_console() -> None:
    for stream in (sys.stdout, sys.stderr):
        if hasattr(stream, "reconfigure"):
            stream.reconfigure(encoding="utf-8", errors="replace")


_pin_utf8_console()

_VERSION_RE = re.compile(r"^\d+\.\d+\.\d+$")
_SET_RE = r'^set\({name}\s+"([^"]*)"\)\s*$'


def cmake_value(text: str, name: str) -> str | None:
    match = re.search(_SET_RE.format(name=re.escape(name)), text, re.MULTILINE)
    return match.group(1) if match else None


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description="校验 CPack 配置")
    parser.add_argument("config", type=Path, help="CPackConfig.cmake 路径")
    args = parser.parse_args(argv[1:])

    path: Path = args.config
    if not path.is_file():
        print(f"缺少 CPack 配置文件: {path}", file=sys.stderr)
        return 1

    text = path.read_text(encoding="utf-8", errors="replace")
    problems: list[str] = []

    package_name = cmake_value(text, "CPACK_PACKAGE_NAME")
    version = cmake_value(text, "CPACK_PACKAGE_VERSION")
    vendor = cmake_value(text, "CPACK_PACKAGE_VENDOR")
    generator = cmake_value(text, "CPACK_GENERATOR")

    if not package_name:
        problems.append("CPACK_PACKAGE_NAME 缺失或为空")
    if not vendor:
        problems.append("CPACK_PACKAGE_VENDOR 缺失或为空")
    if not generator:
        problems.append("CPACK_GENERATOR 缺失或为空")

    if not version:
        problems.append("CPACK_PACKAGE_VERSION 缺失或为空")
    elif not _VERSION_RE.match(version):
        problems.append(f"CPACK_PACKAGE_VERSION 不是 x.y.z: {version}")
    else:
        major, minor, patch = version.split(".")
        for suffix, expected in (
            ("MAJOR", major),
            ("MINOR", minor),
            ("PATCH", patch),
        ):
            actual = cmake_value(text, f"CPACK_PACKAGE_VERSION_{suffix}")
            if actual is not None and actual != expected:
                problems.append(
                    f"CPACK_PACKAGE_VERSION_{suffix}={actual} 与版本 {version} 不一致")

    generators = [item.strip() for item in (generator or "").split(";") if item.strip()]
    if generator is not None and not generators:
        problems.append("CPACK_GENERATOR 不含任何生成器")
    if sys.platform == "win32" and "ZIP" not in generators:
        problems.append("Windows 打包生成器必须包含 ZIP")

    if problems:
        for problem in problems:
            print(f"{path}: {problem}", file=sys.stderr)
        return 1

    print(f"CPack 配置检查通过: {package_name} {version} ({', '.join(generators)})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
