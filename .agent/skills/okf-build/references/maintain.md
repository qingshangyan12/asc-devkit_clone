# 新范围接入编排工作流

把已在 c_api 试点验证的三段流水线（词表调研、重标注、建立索引）打包为新 API 文档范围的
标准接入路径。只编排不建规则——词表归 [tag-research.md](tag-research.md)，文档头与源码标注归
[yaml-generate.md](yaml-generate.md)，字段契约归 [头字段与标注规范.md](头字段与标注规范.md)；
索引段由检索侧 skill（okf-search）独立完成，本工作流只分派与复核，不引用其文件。
既有范围内文档增删改的局部修正直接复用对应工作流的维护边界（单文档重跑、重建索引、
词表新轮次）；存量头的漂移体检与修复由建设侧 okf-maintain skill 负责任意指定文档，
本工作流不设集中维护环节。

## 输入

- 新 API 文档范围（文档根目录及角色分布；范围基线统计参考 `README.md` 第 4 节）。
- 前置检查项：目标目录在打标写入工作流的源上下文范围内；
  索引侧能表达该范围（必要时由其登记新 surface 或放开分类）。
- 既有字段契约、冻结词表（core-v10）与各工作流的运行目录约定。

## 三段流水线

| 段 | 负责 | 交付物 | 关键门槛 |
| --- | --- | --- | --- |
| 1 调研 | 本 skill（[tag-research.md](tag-research.md)） | 冻结词表（3 语义 namespace；可推导维度不入词表） | 双盲发现→校验→**人工批准**后冻结 |
| 2 重标注 | 本 skill（[yaml-generate.md](yaml-generate.md)） | 范围内全部文档的 5 字段 slim 头（version 由 Release Notes 推导）+ 源码标注补齐 | prepare→标注→validate→write；validate 未过不得写入 |
| 3 建立索引 | okf-search skill | 门控化持久索引 + 查询 CLI | 校验不过不出索引；单测+dryrun 全过 |

## 接入清单

1. **确定范围与角色判定**：确认文档根目录，编制范围清单与基线统计。
2. **词表覆盖评估**：core-v10 不足时先执行调研轮次并人工批准（遗留候选见
   `runs/core-v10/` 的 review notes，如 `strided_transfer`）。
3. **分批重标注**：超过 25 篇按批组织（标注 Agent 每批产出 `annotations/*.json`，
   证据正文逐字），每批走完 validate 再 write。
4. **索引接入**：交由 okf-search 完成范围登记与重建；本工作流接收其 `unresolved` 结论，
   需要时按接口事实提供裁决依据，由其登记 `derive-overrides`。
5. **复核**：`scripts/controller.py check` 字段闭包通过；检索侧单测与 dryrun 全过。

## 强制约束

- 禁止跳过任一段直接写头或建索引；词表未冻结不得进入标注。
- 禁止临时创造未定义标签值；禁止手改索引构建产物。
- 任一校验失败必须回到对应步骤重做；文档问题登记 [document-issues.md](document-issues.md)
  并回流源文档修复。
