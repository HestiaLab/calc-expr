# CalcExpr

跨平台 C11 数学表达式计算器。

作者/维护者：wubing7755

## 快速开始

```sh
./scripts/bootstrap.sh
./scripts/check.sh
```

**Windows PowerShell**

```powershell
./scripts/bootstrap.ps1
./scripts/check.ps1
```

构建后运行 CLI：

```sh
cmake --build --preset ninja-debug
./build/ninja-debug/bin/calcexpr
```

## 项目结构

```text
include/        公共头文件
src/            实现
tests/          CTest 测试
cmake/          构建模块
scripts/        本地引导与检查脚本
docs/           项目文档
```

## 安装与消费

```sh
cmake --preset ninja-release
cmake --build --preset ninja-release
cmake --install build/ninja-release --prefix install
```

```cmake
find_package(CalcExpr CONFIG REQUIRED)
target_link_libraries(app PRIVATE CalcExpr::calcexpr_core)
```

## 持续集成

本项目包含 GitHub Actions 构建、测试与静态分析工作流。

## 文档

- [环境配置](docs/guides/environment.md)
- [CMake 与构建](docs/guides/cmake.md)
- [编码风格](docs/guides/coding-style.md)
- [测试与质量门禁](docs/guides/testing.md)
- [架构决策记录](docs/adr/README.md)
- [工具链策略 ADR](docs/adr/0001-c11-cmake-toolchain-policy.md)
- [工程标准](C_PROJECT_STANDARD.md)
- [贡献指南](CONTRIBUTING.md)
- [安全策略](SECURITY.md)

## 许可证

MIT
