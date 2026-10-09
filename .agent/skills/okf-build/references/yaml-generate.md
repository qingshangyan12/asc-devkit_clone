# 打标写入工作流（slim OKF 头）

为 Ascend C API 文档生成、校验并写入 slim OKF 头（okftype/description/tag/dependencies/version
5 字段），含 prepare→validate→write 管线、Release Notes 版本推导与字段闭包检查。
可推导字段一律不写，由索引侧推导。

本工作流是建设侧唯一打标记入口，统一由 `scripts/controller.py` 提供 CLI。
字段契约的唯一事实源是 [头字段与标注规范.md](头字段与标注规范.md)：
文档头固定 5 字段（`okftype, description, tag, dependencies, version`），
可推导字段（signature/archs/header/examples/navigation 等）一律不写。
标签词表与必填规则以 `config/tag-taxonomy.json`
（core-v10，3 语义 namespace，词表调研工作流冻结产出）为准；脚本机械校验，无需复述解析器逻辑。

## 范围与写入

仅处理 `docs/zh/api/SIMD-API/c_api` 下指定的文件或目录（当前适配范围）。
每次使用 `tmp/<run-id>` 作为唯一运行目录。写入面仅限选中文档的 front matter：
整体替换为 5 字段 slim 头，正文必须字节不变；不改头文件、样例源码或索引。
include Doxygen 与样例 marker 已由仓内既有标注覆盖，缺口由索引构建期
报告（属检索侧流程），修复属于源码维护动作。

## 管线

```
prepare → （Agent 按 annotation-task 产出语义标注）→ validate → write
```

1. `prepare`：扫描并分类文档（未分类即停止），固化哈希，推导 version
   （Release Notes + `config/version-overrides.yaml` 人工裁决覆盖），
   产出 `manifest.json`、`annotation-task.json`、`version-map.json`。
2. 标注：Agent 按 [标注工件契约](annotation-contract.md) 将
   `annotations/*.json` 写入 run 目录；只产出语义判断（description/tag/deps），
   证据必须正文逐字。
3. `validate`：确定性校验（哈希一致、词表值、逐字证据、必填与条件必填、
   依赖存在性与 requires 无环、version 一致）→ `validation.json`。
4. `write`：默认 dry-run；`--write` 整体替换 front matter，`--allow-partial`
   可只写校验就绪文档。写后重新解析确认 okftype。

```bash
python3 scripts/controller.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_compute \
  --run-dir tmp/<run-id>
# Agent 写 tmp/<run-id>/annotations/*.json 后：
python3 scripts/controller.py validate --run-dir tmp/<run-id>
python3 scripts/controller.py write --run-dir tmp/<run-id>            # dry-run
python3 scripts/controller.py write --run-dir tmp/<run-id> --write    # 写入
python3 scripts/controller.py check                                   # 字段闭包
```

## version 推导

`scripts/derive_version.py` 从 `docs/zh/api/appendix/Release_Notes/` 推导
（9.0.0 为 C API 首发基线；9.1.0/9.2.0「新增特性」抬升 floor；整接口废弃仅在
deprecated_interface 文档闭合区间）。线上 release_notes.html 与本地同源，本地为
唯一构建输入。规则无法裁决的个案登记 `config/version-overrides.yaml`（附依据摘录，
人工核对后维护）。

## 失败处理

- 未分类文档、taxonomy 非冻结、文档或词表在 prepare 后变化：停止，重跑 prepare。
- 证据不逐字、tag 值越界、缺必填 namespace、依赖目标缺失、requires 成环：
  修正标注后重跑 validate；validate 未通过不得 `--write`（`--allow-partial` 仅写
  就绪文档）。
- 写入后正文必须字节不变；front matter 解析失败立即报错。

## 维护边界

- 词表问题回流 [tag-research.md](tag-research.md) 新调研轮，不得临时创造未定义值。
- 字段集变更需先修订 [头字段与标注规范.md](头字段与标注规范.md) 与 Profile 注册表，并通过 `check` 闭包。
- 单文档维护：对该文档重跑 prepare→validate→write。
