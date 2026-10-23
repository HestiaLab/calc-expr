# 测试

使用 CTest 进行自动化测试。

```sh
ctest --preset ninja-debug --output-on-failure
```

命令详情和生成器相关标志见 [CMake 指南](cmake.md)。

## 测试框架

测试使用 vendored 的 [Unity](https://github.com/ThrowTheSwitch/Unity) 框架
（MIT，位于 `tests/vendor/unity/` 下）。每个测试可执行文件在 `UNITY_BEGIN()`
和 `UNITY_END()` 之间用 `RUN_TEST()` 注册测试用例，并且必须提供 `setUp()` 和
`tearDown()`，即使它们为空。vendored 的 Unity 源码不参与 `clang-format`、
`clang-tidy` 和 `cppcheck` 检查。

当前测试目标：

- `calcexpr_calculator_tests`：表达式成功用例、错误用例、API 合约与递归上限。
- `calcexpr_command_tests`：命令分发、交互状态与默认值。
- `calcexpr_tests`：构建全部测试目标的便利目标。

Debug 预设默认启用调试系统，因此 `ninja-debug` 的测试会覆盖调试运行时代码路径；
Release 预设关闭调试系统，覆盖零开销桩路径。

## 期望

- 缺陷修复应包含回归测试。
- 新增公共 API 应包含成功与失败用例。
- 数值类 API 应覆盖相关的边界与溢出用例。
- 测试应断言行为，而非行覆盖率。
- 对于小型模块，优先编写聚焦的测试程序，而非宽泛的集成测试。
- 可安装的库应保留一个小型的下游包冒烟测试。
- 新增质量门禁必须证明自己能失败：破坏被守护项后必须得到指名 `文件:行`
  的非 0 退出，并恢复复绿。

## 静态分析

CI 运行 clang-format、clang-tidy、cppcheck 和 YAML lint 检查。本地
clang-tidy 检查是可选的，因为工具可用性因平台而异。clang-tidy 只分析当前
编译数据库中的源文件，避免在 Windows 上误分析 Linux/macOS 的 platform 实现。

**Linux/macOS**

```sh
./scripts/check.sh --enable-tidy
```

**Windows PowerShell**

```powershell
./scripts/check.ps1 -EnableTidy
```

## 需要运行的检查

| 变更类型 | 最低限度的有效检查 |
| --- | --- |
| C 源码或公共头文件 | `./scripts/check.ps1` 或 `./scripts/check.sh` |
| 动态库或导出宏变更 | `ninja-shared` 预设与动态库包冒烟测试 |
| 安装/包元数据变更 | 静态与动态包冒烟测试 |
| 子项目行为变更 | `tests/subproject_smoke/` |
| CI 或 YAML 变更 | `git diff --check` 与 CI 静态分析 |
| 发布敏感变更 | `ninja-release`、`ninja-shared`、`ninja-asan` 与包冒烟测试 |

## 包冒烟测试

`tests/package_smoke/` 是一个下游消费者，用于验证已安装的包元数据。它应保持
精简，只断言已安装的头文件、导出的目标、C 链接以及基本库调用能够通过
`find_package` 正常工作。在支持动态库时，为静态与动态两种安装运行它。

## 子项目冒烟测试

`tests/subproject_smoke/` 验证项目可以通过 `add_subdirectory()` 被消费，且
默认不启用顶层 CLI、测试或安装规则。

## 通用质量门禁

除单元测试外，CTest 还注册以下门禁。它们由模板生成时保留，适用于生成后的
项目。

| CTest 名称 | 脚本 | 守护什么 |
| --- | --- | --- |
| `calcexpr_decl_def_check` | `scripts/check_decl_def.py` | `include/` 每个公共声明在 `src/` 有定义；扫描面有绝对下限 |
| `calcexpr_version_consistency` | `scripts/check_version_consistency.py` | CMake VERSION、生成头、CHANGELOG 一致；有 CLI 时同时比较 `--version` |
| `calcexpr_docs_check` | `scripts/check_docs.py` | 相对链接/锚点可达、代码围栏配对、README 文档索引覆盖 `docs/` |
| `calcexpr_gate_behaviors` | `scripts/tests/test_gate_behaviors.py` | 门禁脚本自身能失败（unittest） |
| `calcexpr_cpack_config` | `scripts/check-cpack-config.py` | CPack 包名、版本、vendor 与生成器；Windows 要求 ZIP |

门禁的诊断输出固定为 UTF-8：Windows 控制台默认 GBK/cp1252，中文判定写到
stdout 会崩溃、写到 stderr 会退化成转义序列。门禁自测用
`PYTHONIOENCODING=cp1252` 复现旧控制台，锁定判定不随宿主编码变化。

`scripts/coverage_ratchet.py` 是可选入口，默认不接入 CI 与 check 脚本。覆盖率
基线只能有唯一平台口径，当前基线字段注明取数平台。

## 手动检查

当行为无法通过 CTest 验证时，在 PR 中记录手动检查。手动检查应包含命令、平台
与观察到的结果。
