---
name: okf-dependency-generate
description: 根据 Ascend DevKit API Markdown 正文证据生成并写入唯一的 OKF dependencies 字段。用于指定 API 文件或目录的依赖标注；先校验和 dry-run，再替换旧 usage_dependencies 并写入 dependencies，不生成索引、不修改正文或源代码。
---

# OKF Dependencies 生成

本 Skill 负责 API YAML 中的依赖关系阶段。基础 OKF 由项目 OKF 工作流生成，tag 由
`okf-tag-generate` 生成；本 Skill 只生成 `dependencies`，不处理 tag、索引、检索策略、头文件或样例源码。

## 规范来源

字段形状以同级的 [`OKF字段规范.md`](../OKF字段规范.md) 为准；详细契约见
[`references/dependency-contract.md`](references/dependency-contract.md)，证据规则见
[`references/evidence-rules.md`](references/evidence-rules.md)。旧 guard 工具只作为关系识别、路径校验和环检测的参考，
本 Skill 不调用它的 `usage_dependencies` 写入器，也不迁移旧工具。

## 输出字段

每个目标 API 都必须得到一个 `dependencies` 数组：

- 有明确关系时，数组包含 `path`、`kind`，必要时包含 `condition`。
- 没有明确关系时，写入 `dependencies: []`。
- 目标文档已有 `usage_dependencies` 时，删除旧字段并写入新的 `dependencies`；最终不能并存。
- 当前字段不表达 `status`、`example_before`、`all_of` 或 `any_of`。

## 工作流程

1. 确认精确 DevKit 根目录和用户指定的 API 文件或目录。只处理 `docs/zh/api/SIMD-API/` 下的
   `c_api`、`basic_api`、`adv_api` 和 `tensor_api` 文档。
2. 阅读执行 API 的正文、约束、参数说明和代码示例，并读取正文明确指向的候选 API。
3. 只根据正文中的明确关系生成候选；不能凭名称、头文件、dtype、架构或一次示例调用推断依赖。
4. 为每个依赖保存逐字证据。证据格式和关系边界见两个 reference 文件，主代理在写工件前必须阅读它们。
5. 生成 annotation artifact，并运行默认 dry-run：

   ```bash
   python3 scripts/generate_dependencies.py \
     --devkit-root /path/to/asc-devkit \
     --input tmp/okf-agent/<run-id>/dependencies.json
   ```

6. dry-run 成功后，使用完全相同的工件执行写入：

   ```bash
   python3 scripts/generate_dependencies.py \
     --devkit-root /path/to/asc-devkit \
     --input tmp/okf-agent/<run-id>/dependencies.json \
     --write
   ```

7. 写入后重新解析目标 front matter，确认 `usage_dependencies` 已移除、`dependencies` 与工件一致，且正文未变化。

## Annotation artifact

工件必须是以下结构的 JSON。每个 dependency 条目都必须带证据；证据只用于校验，不会写入 YAML：

```json
{
  "schema_version": 1,
  "annotations": [
    {
      "document_path": "docs/zh/api/SIMD-API/c_api/.../execute.md",
      "dependencies": [
        {
          "path": "docs/zh/api/SIMD-API/c_api/.../configure.md",
          "kind": "requires",
          "condition": "量化模式下",
          "evidence": {
            "source_path": "docs/zh/api/SIMD-API/c_api/.../execute.md",
            "excerpt": "量化模式下需要先完成配置。"
          }
        }
      ]
    }
  ]
}
```

没有依赖的文档使用 `"dependencies": []`，不能省略该键。

## 安全边界

- writer 只修改目标 Markdown 的 `dependencies` 和旧 `usage_dependencies` 块。
- 不生成 `api-dependency-index.yaml` 或任何其他索引。
- 不修改 Markdown 正文、头文件、样例源码、实现代码或其他未指定 API 文档。
- 任一路径、证据、关系类型、YAML 或循环校验失败，整个写入停止。
