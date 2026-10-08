---
name: okf-search
description: OKF 用户侧入口：把 Ascend C API 文档的三类头建立为门控化持久索引，并按算子需求检索候选接口文档。用户需要建索引/检查索引新鲜度、询问“用哪个接口/怎么找 API/某接口配什么用/哪个版本可用”时使用；写头、改词表、标注文档请用 okf-build。
---

# OKF 索引与检索（okf-search）

本 Skill 覆盖「建立索引 → 使用索引」全流程：索引是三类头（文档 slim 头、include Doxygen、
examples marker）的纯派生数据，查询统一走本 skill 的 `scripts/query.py`。推导规则唯一事实源是
[references/索引推导契约.md](references/索引推导契约.md)；
建索引细节见 [references/index.md](references/index.md)，检索流程细节见
[references/retrieve.md](references/retrieve.md)。命令均从仓库根目录执行。

## 建立索引

```bash
python3 okf_skills/okf-search/scripts/build_index.py --scope c_api [--arch A5] [--cann-version 9.2.0] [--check]
```

构建按「CANN 版本 × 架构 × 层级」门控，只索引同时满足 version 覆盖、archs 命中、层级命中的 API；
校验不过不出索引，同输入两次构建字节一致。`--check` 做文档漂移检测。

## 使用索引

### 步骤 0：索引前置保障

任何检索开始前先运行（用户显式指定架构/版本时透传参数，否则默认 A5 + 本地自动检测）：

```bash
python3 okf_skills/okf-search/scripts/ensure_index.py --json [--arch A3] [--cann-version 9.2.0]
```

门控匹配且指纹新鲜 → 零动作；缺失、门控不符或漂移 → 原地重建（本地 diff，不主动提交）。
成功后以输出 JSON（`cann_version/arch_label/categories`）作为本次检索的**文档范围**；
`actions` 含 `rebuilt` 时向用户说明一句。失败时如实报告并停止，不得绕过索引猜测文档。

### 检索循环

1. **意图分析**：抽取阶段/操作/分类/dtype/行为约束/版本产品/具名 API/参数枚举/配套依赖等信号，
   映射到查询维度；至少 1 个可映射信号且无影响候选集的歧义才检索，不明确时先反问（≤3 个问题，
   给出选项），不执行任何查询。
2. **初查与收窄**：主信号查询，结果过宽时加维度（维度间 AND、逗号值内 OR）。
3. **空结果降级链**：精确 `--operation` → `--kw` 同义词 → `--api-class` + `--kw` →
   `--gap` 确认范围外；仍无则如实说明。
4. **配套展开**：`--deps-of`（含 `--reverse`）、`--enum`/`--param` 反查，保留家族变体。
5. **收敛**：意图要素全部有候选或确认范围外、依赖闭合后按固定格式作答；按需只读候选文档
   支撑推荐所需的章节，禁止全量精读。

完整维度映射表、降级链与策略要点见 [references/retrieve.md](references/retrieve.md)；
查询 CLI 全部参数见 [references/index.md](references/index.md)。

### 输出格式（不落盘、不增删章节）

```text
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md 或 include/... | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

## 边界

- 只帮找文档，不生成算子代码、不替用户写 kernel。
- 当前覆盖范围为 `docs/zh/api/SIMD-API/c_api`；范围外层级（adv_api/tensor_api/basic_api/SIMT/
  AI-CPU 等）如实标注为未覆盖并给出线索。
- 不改写 `docs/` 正文、include、样例或索引构建产物。
