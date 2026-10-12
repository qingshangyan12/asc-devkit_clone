# 源码标注契约（include Doxygen + 样例 marker）

本契约定义打标写入工作流第二管线「源码标注」的写入面、记录格式与门禁。
驱动为 `scripts/annotate_sources.py`（默认 dry-run，`--write` 落盘），
渲染/校验实现为恢复自 slim 重构前工具链的 `doxygen_annotations.py` 与
`example_snippet_markers.py`，解析实现为 `extract_api_yaml_metadata.py`。

## 写入面

| 目标 | 载体 | 允许的修改 |
| --- | --- | --- |
| include 头 | `include/c_api/**/*.h` | 在具体声明上方补 Doxygen 块、补缺 tag；不得改声明与其它内容 |
| 样例源码 | `examples/02_simd_c_api/**`（`.asc/.c/.cpp/...`） | 在调用点上下成对插入 `//! [override_id]`；不得改逻辑 |

文档 slim 头仍由 prepare→validate→write 管线负责，本管线只读文档正文。

## include 记录契约

每条记录定位头文件中的一个具体声明：

```json
{
  "source_document": "docs/zh/api/SIMD-API/c_api/<...>.md",
  "header": "include/c_api/<...>.h",
  "function": "asc_xxx(__gm__ int32_t*,int32_t)",
  "parameters": "asc_xxx(__gm__ int32_t* address,int32_t val)",
  "override_id": "asc_xxx__PGi32_i32",
  "archs": ["3510"],
  "template_parameters": [],
  "doxygen": {"params": [{"name": "...", "direction": "in", "description": "...", "evidence": ["| 参数名 | 输入/输出 | 描述 |"]}]}
}
```

生成门禁（缺一不写）：

1. 文档正文「函数原型」能解析出原型，且映射 status ∈ verified（提示头/闭包/回退单一声明）且唯一 header；
2. 文档原型与头文件声明的比对无 unsafe 差异（参数名/类型/数量等）；
3. `override_id` 由 canonical 签名（`prototype_id.signature_identifier`）确定性生成并匹配声明；
4. `@param` 描述只来自「参数说明」表：输入/输出→`in`/`out`/`in,out`，行内容逐字作证据；
   表缺参数或方向缺失时该声明的 params 不写并记问题（不猜测）；
5. `@archs` = 映射架构 ∪ 产品支持节 npu 标记；`@ingroup` = `simd_c_api.<文档路径>`；
6. 同一声明被多篇架构文档命中时合并 archs、保留 canonical ID。

写入规则：块已存在则只替换/补齐基础 tag 与缺失 param，不删除既有内容；重复执行幂等。

## 样例 marker 契约

1. **featured 名单**（agent 产出，随包版本化在 `featured/*.json`，默认目录）：
   `{"example": "examples/...", "apis": ["asc_xxx"], "evidence": ["README 逐字摘录"]}`；
   只收录 README 明示重点展示的 API；无 API 线索时收样例主体接口或置空。
2. **调用匹配**：featured 名单对应文档解析出的声明集合 → 扫描样例目录 → 只保留
   `overload_status=exact` 且为独立语句、实参类型可推断的调用。
3. **marker 渲染**：调用点上下各一行同 id 的 `//! [<override_id>]`（成对包住，无闭合标签）；
   id 为该调用点精确重载的 canonical `override_id`。
4. **幂等**：已有同 id marker 跳过；异 id 原地更新；区间重叠跳过并报告。

## 运行与报告

```bash
python3 scripts/annotate_sources.py include   [--only <doc子串>] [--refresh] [--write]
python3 scripts/annotate_sources.py examples  [--featured-dir <dir>] [--refresh] [--write]
```

- 运行目录默认 `tmp/source-annotate/`：`signatures.json`（全量解析缓存）、
  `include-report.json|md`、`examples-report.json`。
- dry-run 报告含逐头文件/逐文件的 diff 预览、跳因（无唯一声明、unknown API、无 exact 调用、
  重叠等）与问题清单；`--write` 前不改动任何文件。
- 文档自身缺陷（README 与源码不符、参数表缺失、命名歧义）记入问题清单并登记
  `document-issues.md`，不阻塞其它记录。

## 与索引的关系

- include 注解中的 `@override_id`/`@archs`/`@param`/`ASC_DEPRECATED` 是索引侧
  结构推导的输入；样例 marker 的 id 与签名 override_id 同一套生成规则。
- `@param [in,out]` 由索引侧 `include_db` 解析（含方向归一）。
