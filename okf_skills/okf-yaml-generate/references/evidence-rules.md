# Dependencies 证据规则

## 可接受证据

每个生成的依赖条目都必须有一条逐字证据：

```json
{
  "source_path": "docs/zh/api/SIMD-API/c_api/.../execute.md",
  "excerpt": "调用本接口前必须先完成配置。"
}
```

证据必须来自已存在的 Markdown 正文，不得只来自 YAML front matter、文件名、API 名称、头文件、dtype、架构或样例注释。

## 判断规则

- 只有正文明确表达“必须先调用”“需要预先配置”“受某接口约束”等关系时才生成依赖。
- 示例中的调用先后只能证明一次观察到的顺序，不能单独证明 `requires`。
- “配合使用”或“成对使用”只有在正文明确表达关系时才使用 `related` 或 `configures`，不能凭 API 名称推断。
- 模式、产品或架构条件必须保留为 `condition`，不得扩大成全局关系。
- 无法唯一确定目标 API 时不生成条目，记录为待审核。

## 写入边界

生成器只改目标 Markdown front matter 中的 `dependencies` 块。不得修改正文、声明、参数、头文件、样例源码或其他 API 文档。
