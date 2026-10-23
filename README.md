# CalcExpr

跨平台 C11 数学表达式计算器，使用递归下降解析器完成表达式求值。

[![C11](https://img.shields.io/badge/C-C11-blue)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CMake](https://img.shields.io/badge/CMake-3.21%2B-blue)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

## 项目简介

CalcExpr 是一个零依赖的 C11 命令行计算器，采用递归下降算法处理运算符优先级、括号、一元运算符、小数与科学计数法。计算内核以 `calcexpr_core` 库提供，命令行程序 `calcexpr` 负责交互与命令处理。

项目目标是保持模块边界清晰、构建可复现，并用 CTest 与质量门禁持续验证。

## 功能特性

- 支持加法、减法、乘法、除法与括号
- 正确的运算符优先级（乘除高于加减）
- 支持小数、科学计数法与一元正负号
- 中文交互界面与命令系统
- 编译期可裁剪、运行时可控的调试系统
- 安装与 `find_package` 消费支持
- 跨平台 CI、静态分析与质量门禁

## 项目结构

```text
include/calcexpr/       公共头文件（calculator.h、export.h、compiler.h）
src/calcexpr/           实现与内部头文件
src/main.c              CLI 入口
tests/                  CTest 测试与 vendored Unity
tests/package_smoke     安装包消费冒烟
tests/subproject_smoke  子项目消费冒烟
cmake/                  构建、安装与打包模块
scripts/                引导、检查与质量门禁脚本
docs/guides/            中文主题指南
docs/adr/               架构决策记录
```

## 快速开始

需要 CMake 3.21+、支持 C11 的编译器，以及 Ninja 预设所需的 Ninja。Windows 环境配置见[环境配置](docs/guides/environment.md)。

首次设置：

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
```

**Linux/macOS**

```sh
./scripts/bootstrap.sh
```

配置、构建与测试：

```sh
cmake --preset ninja-debug
cmake --build --preset ninja-debug
ctest --preset ninja-debug --output-on-failure
```

运行：

```sh
# Windows
./build/ninja-debug/bin/calcexpr.exe

# Linux/macOS
./build/ninja-debug/bin/calcexpr
```

## 使用方式

```text
请输入表达式> 2+3*4
表达式: 2+3*4
结果:   14

请输入表达式> (2+3)*4
表达式: (2+3)*4
结果:   20

请输入表达式> (3-5)*6
表达式: (3-5)*6
结果:   -12
```

### 命令行选项

| 选项 | 说明 |
|---|---|
| `--version` | 显示版本号并退出 |
| `--help`、`-h` | 显示帮助并退出 |
| `--debug`、`-d` | 开启调试输出 |
| `--debug-level=N` | 设置调试级别（0-5） |
| `--debug-module=NAME` | 设置调试模块（lexer/parser/calc/main/all） |

### 交互命令

| 命令 | 说明 |
|---|---|
| `/quit`、`/exit`、`/q` | 退出程序 |
| `/help`、`/show help` | 显示帮助 |
| `/show process` | 交互选择调试级别并开启计算过程输出 |
| `/hide process` | 关闭计算过程输出 |

调试级别、编译开关与输出示例见[调试系统](docs/guides/debug-system.md)。

## 测试与质量门禁

测试使用 CTest 与 vendored Unity：

```sh
ctest --preset ninja-debug --output-on-failure

# 只运行计算器用例
ctest --preset ninja-debug -R calcexpr_calculator_tests --output-on-failure

# 只运行命令模块用例
ctest --preset ninja-debug -R calcexpr_command_tests --output-on-failure
```

本地完整检查：

**Windows PowerShell**

```powershell
./scripts/check.ps1
```

**Linux/macOS**

```sh
./scripts/check.sh
```

门禁覆盖公共声明与定义一致性、版本一致性、文档链接与索引、门禁自身可失败性，以及安装与子项目冒烟。详细说明见[测试与质量门禁](docs/guides/testing.md)。

## 安装与包消费

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
cmake --install build/ninja-release --prefix install
```

```cmake
find_package(CalcExpr CONFIG REQUIRED)
target_link_libraries(app PRIVATE CalcExpr::calcexpr_core)
```

公共 API 位于 `include/calcexpr/calculator.h`，内部头文件不会安装。

## 持续集成

项目包含 GitHub Actions 的构建、测试、共享库、sanitizer、覆盖率、包消费、子项目消费与静态分析工作流。依赖更新由 Dependabot 覆盖。

## 文档

| 文档 | 说明 |
|---|---|
| [环境配置](docs/guides/environment.md) | Windows 开发环境与工具安装 |
| [CMake 与构建](docs/guides/cmake.md) | 预设、构建、安装与故障排除 |
| [编码风格](docs/guides/coding-style.md) | 命名、头文件、格式与 API 文档约定 |
| [测试与质量门禁](docs/guides/testing.md) | 测试分层、静态分析与门禁说明 |
| [调试系统](docs/guides/debug-system.md) | 编译开关、级别、参数与输出示例 |
| [递归下降解析器原理详解](docs/guides/recursive-descent-parser.md) | 解析算法与代码结构 |
| [架构决策记录](docs/adr/README.md) | ADR 索引与说明 |
| [C11 与 CMake 工具链策略](docs/adr/0001-c11-cmake-toolchain-policy.md) | 语言标准与工具链决策 |
| [C 项目工程标准](C_PROJECT_STANDARD.md) | 本项目遵循的工程基线 |
| [贡献指南](CONTRIBUTING.md) | 分支、提交与 PR 流程 |
| [安全策略](SECURITY.md) | 漏洞报告与依赖处理 |

## 贡献

欢迎提交 Issue 与 Pull Request。提交前请阅读[贡献指南](CONTRIBUTING.md)，并运行完整本地检查。

## 许可证

本项目采用 MIT 许可证，详见 [LICENSE](LICENSE)。
