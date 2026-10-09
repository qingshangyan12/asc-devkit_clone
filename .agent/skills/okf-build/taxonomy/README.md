# Tag 词表工件说明

本目录保存词表调研资产；冻结生产词表位于本 skill 配置 [`../config/tag-taxonomy.json`](../config/tag-taxonomy.json)：

- `tag-rules.yaml`：必填、证据与可推导边界规则。
- `releases/`：经人工批准发布的冻结版本；当前生产版本为 `core-v10-frozen.json`。

调研、标注和写入流程都必须从 `config/tag-taxonomy.json` 读取 namespace 和值，
不得另建并行词表。provisional 文件只存在于对应 `runs/<run-id>/`。

## 词表结构（core-v10 起）

tag 只覆盖脚本不可推导的语义概念，固定 3 个 namespace：

| namespace | 必填规则 | 内容 |
| --- | --- | --- |
| `roles` | 实体页必填 | API 在调用/执行流程中的角色（compute、data_movement、configuration 等） |
| `operations` | roles 含 compute/data_movement/data_initialization 时必填 | 直接执行或配置的操作（add、copy、interleave 等） |
| `traits` | 选填 | 影响检索的稳定行为特征（对齐/连续性、量化、填充、饱和/舍入模式等） |

历史 namespace（api_surfaces、families、objects、execution_models、source_locations、
destination_locations）与历史 trait 值（masked、deprecated）已改为索引期推导，
不再进入词表；可推导边界见 `tag-rules.yaml` 的 `derivation_boundary`。

## 设计要点

- 最小标注集 = 全局必填 `roles` + 条件必填 `operations` + 按需 `traits`；
  不适用 namespace 直接省略，不用空数组、`unknown`、`other` 伪造信息。
- 候选值先复用已有值或查询别名（GM/Global Memory/全局内存同一概念不新增多个值）；
  新值须两个独立发现支持、两个查询场景证明收益，经人工批准后冻结。
- 每个标签值必须有目标 Markdown 正文逐字证据；目录、API 名称、dtype、架构、头文件属性
  和样例注释不能单独证明语义标签。
- 新增、删除、合并或重新定义标签必须开新调研 run 并生成新的冻结版本；`releases/` 只保留
  当前生产版本，历史版本由版本管理回溯。

## 版本规则

- 冻结生产词表维护在 `../config/tag-taxonomy.json`（词表调研流程唯一写入）；
  调研 `approve` 的冻结产物写入本目录 `releases/`。
- 发布 registry 切换生产版本是单独的人工批准动作，不由调研流程自动完成。
