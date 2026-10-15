# asc-knowledge-create reference 导航

本目录保存 asc-knowledge-create 三个流程工作流（词表调研、打标写入、接入编排）的流程文档与契约，
以及字段契约、独立评测功能文档和文档问题登记。运行所需配置、脚本与历史资产在各同级目录。

## 目录划分

| 文档 | 内容边界 |
|------|----------|
| [README.md](README.md) | 本 reference 导航和维护规则 |
| [tag-research.md](tag-research.md) | Tag 词表调研：双盲发现 → 校验 → 人工批准冻结的完整流程与命令 |
| [yaml-generate.md](yaml-generate.md) | 打标写入：文档 slim 头 prepare→标注→validate→write 管线与 version 推导 |
| [maintain.md](maintain.md) | 新 API 文档范围接入的三段流水线编排（只编排不建规则） |
| [evaluate.md](evaluate.md) | 检索评测（独立功能，不参与流程编排）：测试集、指标口径、基线与对照方法 |
| [头字段与标注规范.md](头字段与标注规范.md) | 字段契约唯一事实源：三类头写入面、5 字段、version 推导、证据规则 |
| [annotation-contract.md](annotation-contract.md) | 文档标注工件契约（annotations/*.json 记录格式与门禁） |
| [source-annotation-contract.md](source-annotation-contract.md) | 源码标注管线（include Doxygen / examples marker）写入面与记录格式 |
| [example-range-options.md](example-range-options.md) | examples 范围改进调研：按函数分组、集中 ID 与片段引用、检索时提取；候选方案，尚未实施 |
| [artifact-contract.md](artifact-contract.md) | 词表调研裁决工件契约（review/approval 格式） |
| [worker-protocol.md](worker-protocol.md) | 词表调研发起子代理的提示词协议与隔离要求 |
| [评测口径.md](评测口径.md) | 评测指标定义、隔离约束、测量流程与对照判据 |
| [document-issues.md](document-issues.md) | API 文档自身问题登记（非工具缺陷） |

## 维护规则

- 字段契约只在《头字段与标注规范.md》维护；流程文档只引用，不复述字段定义。
- 词表问题回流词表调研新轮次，不得临时创造未定义标签值。
- 检索评测为独立功能，不参与流程编排；流程文档不引用其结论作为门禁。
- 字段集变更需先修订字段契约与 `config/document-profile-registry.yaml`，并通过
  `scripts/controller.py check` 闭包。
