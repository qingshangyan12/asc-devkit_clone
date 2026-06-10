---
name: asc-api-doc-mapper
description: AscendC API 头文件与文档映射查询技能。覆盖 basic_api、adv_api、c_api、simt_api、tensor_api、aicpu_api、utils 全部 7 个层级。触发：当用户需要查找 API 函数的声明头文件、文档页面，或查询某个头文件包含哪些函数，或询问哪些接口没有文档覆盖时使用。Triggers: "which header declares X", "where is the doc for X", "what functions are in header Y", "查找函数对应的文档", "find the header for ReduceSum", "这个头文件有哪些文档", "SIMT API 的文档在哪", "哪些接口没写文档"。
---

# AscendC API 头文件-文档映射

## 为什么需要这个映射

AscendC 的头文件名和文档分类方式不对应。`kernel_operator_vec_binary_intf.h` 声明了 Add、Sub、Mul、Div 等函数，文档却分散在"基础算术"、"逻辑计算"、"复合计算"等多个子目录。加上 7 个 API 层级共 2500+ 文档页和 600+ 头文件，手动逆向搜索效率极低。

本 skill 维护一份预计算的 JSON 索引，通过 `query.py` 提供结构化查询，避免逐层递归搜索。

## 使用方式

用户提问后，直接用 `query.py` 查询：

```bash
# 查函数所在的头文件和文档
python .agent/skills/asc-api-doc-mapper/scripts/query.py --function Add

# 查头文件包含哪些函数
python .agent/skills/asc-api-doc-mapper/scripts/query.py --header kernel_operator_vec_reduce_intf

# 查某个层级的所有函数
python .agent/skills/asc-api-doc-mapper/scripts/query.py --tier simt_api

# 查哪些头文件没有文档覆盖
python .agent/skills/asc-api-doc-mapper/scripts/query.py --unreferenced

# 过滤特定层级的未覆盖头文件
python .agent/skills/asc-api-doc-mapper/scripts/query.py --unreferenced-tier basic_api

# 查看统计信息
python .agent/skills/asc-api-doc-mapper/scripts/query.py --stats
```

`--function` 和 `--header` 均支持部分匹配。结果包含 `tier`、`header`、`doc_path` 和 `confidence`。始终向用户展示置信度标签。

### JSON 中未命中时

如果 `--function` 返回空，用以下命令 fallback 搜索源码：

```bash
# 在文档中搜索函数名
grep -r "^# 目标函数名" docs/api/ --include="*.md" -l

# 在头文件中搜索声明
grep -rn "目标函数名(" include/ --include="*.h"

# 在文档中搜索头文件引用
grep -r "头文件路径为：.*目标头文件\|包含.*目标头文件.*头文件" docs/api/ --include="*.md" -l
```

告知用户"映射数据中未收录，以下是实时搜索结果"。

## 置信度

| 置信度 | 来源 | 含义 |
|--------|------|------|
| `HIGH` | 文档显式指明了声明该函数的头文件（`头文件路径为：xxx.h` 或 `包含"xxx.h"头文件`） | 确定映射，可放心引用 |
| `MEDIUM` | 函数名在对应层级头文件中唯一匹配 | 推断映射，建议确认 |

## 覆盖的 7 个层级

所有层级统一识别两种文档中的显式头文件引用格式：`头文件路径为：xxx.h` 和 `包含"xxx.h"头文件`。有 HIGH 条目的层级即文档中使用了以上格式。

| tier | include/ | docs/ |
|------|----------|-------|
| basic_api | `include/basic_api/` | `docs/api/SIMD-API/基础API/` |
| adv_api | `include/adv_api/` | `docs/api/SIMD-API/高阶API/` |
| c_api | `include/c_api/` | `docs/api/SIMD-API/C-API/` |
| simt_api | `include/simt_api/` | `docs/api/SIMT-API/` |
| tensor_api | `include/tensor_api/` | `docs/api/tensor_api/` |
| aicpu_api | `include/aicpu_api/` | `docs/api/AI-CPU-API/` |
| utils | `include/utils/` | `docs/api/Utils-API/` |

## 数据更新

新增 API 或文档结构调整后，重新生成索引：

```bash
python .agent/skills/asc-api-doc-mapper/scripts/regenerate.py
```

正常查询时不要运行此脚本。

## 特殊情况

- 结构体/类型定义头文件（如 `kernel_base_types.h`）不会有函数文档，出现在 `--unreferenced` 中
- 索引/导航页面（`README.md`、`xxx列表.md`、`概述.md`）在生成时自动跳过
- 同名函数跨层级通过 `tier` 字段区分，`--function ReduceSum` 会同时返回 basic_api 和 adv_api 的结果
- SIMT 中 `asc_` 前缀的函数可能在 `device_functions.h`、`asc_fp16.h`、`asc_bf16.h` 中都出现，标记为 LOW 置信度
