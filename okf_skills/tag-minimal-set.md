# OKF Tag 最小集

## 1. 为什么这样设计

`api_surfaces` 由稳定资源路径确定，和 `roles` 一起构成所有 API 文档的最小导航信息。计算、搬运和初始化 API 再要求 `operations`；计算 API 再要求 `execution_models`，以区分内存矢量与寄存器矢量等同名接口。其他 namespace 只在证据和检索用途同时存在时填写。

## 2. 以后扩展规则

新增 namespace 或值必须先有真实检索用例、正文证据和冻结 taxonomy 版本变更。可选字段缺失时省略，不写空数组；配置 API 不因下游行为被强加 `operations`。

## 3. 标注示例

```yaml
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - operations: [add]
  - execution_models: [memory_vector]
```

## 4. 相比其他方案的收益

在 140 篇已标注文档中，保守投影后非空字段减少 35.48%，标签值记录减少 31.41%；A1 查找中推荐字段覆盖 3/3 必需 API，候选数少于只使用 `operations` 或简单文本关键词的方案。
