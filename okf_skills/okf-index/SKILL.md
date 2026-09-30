---
name: okf-index
description: 把 slim OKF 头（5字段）+ include Doxygen + examples marker 三类头推导为门控化持久索引（CANN 版本 × 架构 × 层级，默认 9.x 自动检测 × A5 × c_api），提供统一查询 CLI（阶段/操作/分类/参数反查/枚举反查/特性/版本/产品/依赖图/缺口）。脚本为主 + derive-overrides 兜底，构建确定性。
---

# OKF 索引建设

本 Skill 把已打标文档构建为持久索引，支撑 `okf-retrieve` 的算子需求检索。索引是三类头的
**纯派生数据**：文档 slim 头（语义）+ include Doxygen/声明（结构）+ examples marker（关联），
推导规则以 [`../字段与推导规范.md`](../字段与推导规范.md) 第 3 节为唯一契约。
当前实例为 c_api 试点范围（vector_compute + vector_datamove，140 篇；全量轮次在
`SCOPES` 注册表放开 categories）。

## 资产结构

```
okf-index/
  scripts/build_index.py        # 构建 + 校验 + 漂移检测（--check）；--arch/--cann-version 门控
  scripts/query.py              # 查询 CLI（唯一检索入口）
  scripts/derive/               # 推导引擎：include_db / docparse / examples_db / gate / overrides
  derive-overrides/c_api.json   # LLM/人工裁决残留（unresolved.json 的固化结论，可审查可回放）
  index/c_api/index.json        # 索引实例（构建产物）：documents + 16 视图 + 依赖图 + defs 目录
  index/c_api/build-meta.json   # 源 commit、文档 sha256、词表/映射指纹
  index/c_api/gaps.json         # 缺口登记（人工维护、构建校验）
  index/c_api/概览.md            # 人读概览（构建产物）
  tests/test_index.py           # 门控 + 推导正确性 + 确定性 + dryrun 回归
  tests/dryrun.yaml             # 查询清单（A5/A3 双实例）
```

## 构建门控与流程

```bash
python3 scripts/build_index.py --scope c_api [--arch A5] [--cann-version 9.2.0] [--check]
```

1. **CANN 版本检测链**：`--cann-version` > `$ASCEND_HOME_PATH` 安装目录 > 仓 `version.cmake`。
2. **架构映射**：`--arch`（默认 A5）经 `okf_skills/config/product-map.yaml` 映射架构号（A5→3510）。
3. 只索引同时满足三条件的 API：头内 `version` 区间覆盖本地 CANN 版本 ∧ 推导 archs 含目标架构
   ∧ 属于目标层级；被过滤 API 计入 `meta.filtered_by_version/arch`。
4. 推导（脚本为主）：
   - include 声明 + Doxygen → 签名（override_id/archs/dtype/位置/参数/掩码/废弃）、
     `api_class`（include 子目录）、`execution_model`、defs 目录（枚举/常量/宏/别名/union）；
   - examples `//! [override_id]` marker → 样例关联；
   - 正文 → 标题、导航链接、npu 产品标记 → 产品支持；
   - 架构变体页只保留本架构声明（无 @archs 声明继承页面架构）；家族页名不带序号时从正文
     函数原型提取真实 API 名回查 include。
5. 残留进 `unresolved.json`（静默重载无示例链接、archs 需 npu 兜底等）；单轮 LLM/人工裁决后
   固化为 `derive-overrides/<surface>.json`，构建确定性消费——**构建过程不内联调用 LLM**。
6. 构建内联全量校验，任一失败拒绝出索引；输出不含时钟，同输入两次构建字节一致；
   `--check` 做漂移检测（文档改动未重建时退出码 1）。

## 查询接口

`query.py` 过滤维度间 AND、逗号值内 OR：

```
--surface c_api                # 索引实例（默认；--index-dir 显式指定目录）
--stage in,out,compute         # 分阶段候选（roles + 推导位置判定）
--operation copy,divide        # operations 维度
--category vector_compute      # 文档目录分类
--api-class vector_datamove    # API 分类 = include 子目录（reg_compute/vector_datamove/...）
--dtype half,fp8_e4m3fn        # dtype（别名归一：f32→float、e4m3→fp8_e4m3fn）
--kw 非对齐,填充                # title+description 子串
--convert half int4            # 类型转换对
--feature low_precision        # 特性（low_precision/low_precision_matrix/int4...）
--version 9.1.0-               # CANN 版本区间
--product 950,A3               # 产品 npu 标记
--api asc_copy_gm2ub           # 家族卡：主页+同族成员+分类页
--deps-of <api|path> [--kind K] [--reverse]   # 依赖图遍历
--param round_mode             # 参数名或枚举类型反查（如 asc_cast_round_mode）
--enum asc_cast_round_mode     # 枚举名或枚举值反查（如 ASC_CAST_RN → 使用方 API）
--gap 同步,寄存器               # 缺口登记检索
--json                         # 机器可读输出
```

输出统一为 `path | API 名 | 命中原因`；枚举反查返回值目录与使用方。

## 维护边界

- 文档头变化（okf-yaml-generate 重跑）或 include/examples 变化后：重跑
  `build_index.py --scope c_api` 并提交索引 diff；`--check` 报漂移属预期报警。
- `derive-overrides/<surface>.json` 只登记脚本无法裁决、经人工核对的事实，每条附 reason；
  新增推导规则可消除的条目应删除。
- gaps.json 人工按证据增补（hint/evidence 必须是真实仓库路径），构建只做存在性校验。
- 词表合法性以 `okf-tag-research/taxonomy/tag-taxonomy.json`（core-v10）校验；
  全量轮次接入新范围时在 `SCOPES` 登记、放开 categories 并扩充 dryrun 清单。
