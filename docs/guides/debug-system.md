# 调试系统

CalcExpr 提供一个编译期可裁剪、运行时可控的调试系统。关闭编译开关时，所有调试宏展开为空操作，不产生运行时开销。

## 编译期开关

调试运行时只在 `CALCEXPR_ENABLE_DEBUG=ON` 时编译进 `calcexpr_core`：

- `debug` 与 `ninja-debug` 预设默认开启；
- `ninja-shared`、`asan`、`coverage` 继承 Debug 预设，因此同样开启；
- `release` 与 `ninja-release` 默认关闭，保证发布构建零开销。

需要手动开启时：

```sh
cmake -S . -B build/debug-trace -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCALCEXPR_ENABLE_DEBUG=ON
cmake --build build/debug-trace
```

如果发布构建中传入 `--debug`，程序会提示调试系统未启用，并给出重新编译的提示。

## 命令行参数

| 参数 | 说明 |
|---|---|
| `--debug`、`-d` | 开启调试输出 |
| `--debug-level=N` | 设置调试级别（0-5） |
| `--debug-module=NAME` | 设置模块：`lexer`、`parser`、`calc`、`main`、`all` |

这些参数在进入 REPL 前解析；调试输出默认写入标准错误，避免污染表达式结果。

## 调试级别

| 级别 | 名称 | 说明 |
|---|---|---|
| 0 | NONE | 完全关闭 |
| 1 | ERROR | 仅错误 |
| 2 | WARN | 警告及以上 |
| 3 | INFO | 信息及以上，包含计算步骤 |
| 4 | DEBUG | 调试及以上，包含解析器细节 |
| 5 | TRACE | 最详细，包含完整调用链与缩进 |

## 运行时命令

| 命令 | 说明 |
|---|---|
| `/show process` | 交互选择调试级别并开启计算过程输出 |
| `/hide process` | 关闭计算过程输出 |
| `/help`、`/show help` | 查看命令列表 |

`/show process` 会依次提示选择 1-5 级；选择后，解析器会按对应级别输出步骤、上下文或调用树。

## 输出示例

输入 `2+3*4` 并开启 TRACE 级别时，输出类似：

```text
[PARSER] → parser_parse_expression()
[PARSER] → parser_parse_term()
[PARSER] [步骤1] 读取数字 2
[PARSER] → parser_parse_term()
[PARSER] [步骤2] 读取数字 3
[PARSER] [步骤3] 读取数字 4
[PARSER] [步骤4] 3 * 4 = 12
[PARSER] [步骤5] 2 + 12 = 14
表达式: 2+3*4
结果:   14
```

TRACE 级别下每层括号会增加两个空格的缩进，便于观察递归深度。

## 相关文档

- 递归下降解析器的实现原理见[递归下降解析器原理详解](recursive-descent-parser.md)。
- 构建预设与编译选项见 [CMake 与构建](cmake.md)。
