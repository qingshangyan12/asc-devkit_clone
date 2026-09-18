# 子代理协议

主代理通过 `tag_research.py prepare` 创建任务包。每个子代理都必须在全新上下文中启动，并且只收到所列文件位置。

## 发现子代理提示词

对 `discovery-a` 和 `discovery-b` 分别使用以下提示词，仅替换 worker 名称与路径：

```text
你是 <worker>。只读取你的 assignment JSON、其中指定的基础 taxonomy，以及指定 DevKit 根目录下被分配的 API Markdown。不得读取其他 worker 的输出、任何 candidate summary、taxonomy review、release bundle 或历史 annotation。

对每篇被分配 API，在可用时比较至少两篇相关的被分配或邻近 API。只提出能够改进相似 API 检索的文档级语义 namespace:value 候选。必须使用基础 taxonomy 已有 namespace，但可提出新 value。每个 proposal 都需要一段短小、逐字匹配 Markdown 正文的证据。不得从路径、API 名称、dtype、架构、头文件或样例推导非 surface 语义；不得提出生命周期、配置、绑定、模板或实现事实。

严格按 assignment 的输出契约，将唯一 JSON 结果写入 <output-path>。不得修改原文档或 taxonomy。完成时只报告输出路径。
```

## 主代理裁决规则

全部 worker 输出通过校验后，主代理才可比较它们。只有明显拼写差异可合并为一个 `namespace:value`，每条合并必须记录在 `normalizations`。只有两个发现子代理独立支持归一化后的值、证据逐字匹配且定义和选择规则明确时才可 `approve`。其他候选必须写 `defer` 或 `reject` 并说明原因。

词表冻结后进入逐篇标注时，不能因为某篇 API 没有明显证据就省略强制概念。taxonomy 中每个
`required: true` 的 namespace 都必须填入至少一个冻结值；如果正文确实不足以判断，必须阻断该文档并进入补证或词表修订队列，不能填写临时值或空数组绕过校验。
