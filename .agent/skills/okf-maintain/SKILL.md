---
name: okf-maintain
description: OKF 存量头维护入口：检查指定文档的三类头（front matter / include Doxygen / 样例 marker）是否漂移，出具报告，并按真实语义修复。用户提到“头过期/漂移/内容变了头要不要更新/巡检某批文档/升级文档后维护头”时使用；首次建设头请用 okf-build，检索请用 okf-search。
---

# OKF 存量头维护（okf-maintain）

本 Skill 面向"头已存在、源内容发生变化"的场景：对**指定文档**体检三类头，出漂移报告，
确定性漂移自动修复，语义漂移按正文真实含义重写。只查指定的文档/目录，不做全仓审计、
不清理孤儿头。流程细节见 [references/drift.md](references/drift.md)。

依赖同仓建设侧 skill `okf-build`（`.agent/skills/okf-build/`）：复用其脚本与字段契约，
本 skill 不复制解析实现。三个 skill 的分工：okf-build 建头、okf-maintain 维护头、okf-search 检索。

## 何时使用

- 合入新版本文档、Release Notes 更新后（version/okftype 等可能漂移）
- 词表、profile registry、type registry 换版后
- 批量编辑/修复文档正文后，怀疑头语义过期
- 定期巡检（`check` 退出码可用于脚本）

## 快速命令

从本 skill 目录执行：

```bash
# 体检指定文档（默认检查文档头 + include + examples；--checks doc 可只做快速确定性检查）
python3 scripts/maintain.py check --devkit-root /path/to/asc-devkit \
  --document docs/zh/api/SIMD-API/c_api/.../asc_xxx.md

# 确定性修复（默认 dry-run；--write 落盘：okftype/version 原位刷新 + include/marker 幂等写入）
python3 scripts/maintain.py update --devkit-root /path/to/asc-devkit \
  --document docs/.../asc_xxx.md --write

# 建立/刷新账本（首次使用或语义修复后）
python3 scripts/maintain.py baseline --devkit-root /path/to/asc-devkit
python3 scripts/maintain.py baseline --from-run tmp/maintain/<ts>/annotate
```

## 修复流程

1. `check` 出报告（`tmp/maintain/<ts>/drift-report.md|json`），按级别分流：
   - **DET**：跑 `update --write`；
   - **SEM-HIGH/LOW**：进入第 2 步；
   - **INFO**：记录在册，不阻断。
2. 语义修复走 okf-build 标注管线（不在本 skill 内另建标注机器）：
   `controller.py prepare --document <漂移文档>` → Agent 按
   [annotation-contract.md](../okf-build/references/annotation-contract.md) 写逐字证据
   → `validate` → `write --write`。
3. `baseline --from-run <run>` 刷新账本（记录新正文哈希与新证据）。
4. 复检 `check` 清零；检索侧无需动作（okf-search `ensure_index` 按指纹自行判废重建）。

## 全局约束

- **只碰指定文档**；不自动删除既有 include 注解或样例 marker（孤儿不在范围内）。
- **写前必 dry-run**；不改正文与代码逻辑，只改头所在位置（front matter / Doxygen 块 / marker 行）。
- **语义修复必须有正文逐字证据**；证据不足阻断并登记
  [document-issues.md](../okf-build/references/document-issues.md)，不得用临时值绕过。
- 字段契约与写入面以 okf-build 的
  [头字段与标注规范.md](../okf-build/references/头字段与标注规范.md) 为准。
