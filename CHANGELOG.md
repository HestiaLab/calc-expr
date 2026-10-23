# 更新日志

> 新增段落用中文书写；已发布段落保留发布时的原文，不重写。

## 1.0.0 — 2026-10-22

- 对齐 c-project-standard v0.2.1 工程结构：CMake 模块、CTest/Unity、CI、安装与打包。
- 重排为 `include/calcexpr/` 公共 API 与 `src/calcexpr/` 内部实现，新增 `calcexpr_core` 库与 `calcexpr` CLI。
- 调试系统改为 `CALCEXPR_ENABLE_DEBUG`，Debug 预设默认开启，Release 保持零开销。
- 新增 `--version`/`--help`，版本统一为 1.0.0。
- 文档改为中文单语，删除英文与西语镜像。
- 新增声明/定义、版本一致性、文档、CPack 与门禁自检。
- 新增安装包与子项目消费冒烟测试。
