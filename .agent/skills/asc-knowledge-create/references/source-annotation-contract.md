# 源码标注契约（include Doxygen + 样例 marker）

本契约定义打标写入工作流第二管线「源码标注」的写入面、记录格式与门禁。
驱动为 `scripts/annotate_sources.py`（默认 dry-run，`--write` 落盘），
渲染/校验实现为恢复自 slim 重构前工具链的 `doxygen_annotations.py` 与
`example_snippet_markers.py`，解析实现为 `extract_api_yaml_metadata.py`。

## 写入面

| 目标 | 载体 | 允许的修改 |
| --- | --- | --- |
| include 头 | `include/c_api/**/*.h` | 在具体声明上方补 Doxygen 块、补缺 tag；不得改声明与其它内容 |
| 样例源码 | `examples/02_simd_c_api/**`（`.asc/.c/.cpp/.h/...`） | 每个源文件用一对 `//! [id1, id2, ...]` 包住重点 API 的共同范围；不得改逻辑 |

文档 slim 头仍由 prepare→validate→write 管线负责，本管线只读文档正文。

样例标注仅修改仓库已有源码的 marker 注释，不新建样例文件、不扩写文档调用片段。
没有原有样例时登记缺失，保留空样例关联。

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
  "doxygen": {
    "brief": {"text": "Updates the value at the destination address.", "evidence": ["正文中支持此重载功能的逐字摘录"]},
    "params": [{"name": "...", "direction": "in", "description": "...", "evidence": ["| 参数名 | 输入/输出 | 描述 |"]}]
  }
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
7. 每个具体重载需要一句英文 `@brief`：依据正文「功能说明」「返回值说明」及该声明撰写，
   区分输出参数与返回值形式；术语参考 `docs/en/asc_api_writing_guidelines.md`、
   `docs/en/contributing/asc_c_api_contributing.md`。证据是正文逐字摘录，不能仅引用 front matter。
   摘要为单行纯文本，不含 Doxygen 命令或注释终止符。缺少证据或摘要时跳过该声明并报告。

写入规则：块已存在则只替换/补齐基础 tag、缺失 brief 与缺失 param，不删除既有内容。
新 `@brief` 放在 `@ingroup` 后，随后保留一行空注释 ` *`，再写 `@override_id`、`@archs`、
`@param`；已有 `@brief` 或 `\brief` 保留，不覆盖、不重复添加。重复执行幂等。

### 英文摘要工件

Agent 先为本次选定的具体重载生成 `<run-dir>/include-briefs.json`；脚本负责验证和写入，
不在运行过程中调用模型翻译。可通过 `--brief-records <file>` 指定其他文件。
JSON 为列表，每项按文档路径和 canonical `override_id` 定位唯一声明（宏使用 `macro_id`）：

```json
[
  {
    "source_document": "docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_abs.md",
    "override_id": "asc_abs__Rvf32_vf32_vb",
    "brief": {
      "text": "Computes element-wise absolute values of src under mask and stores the results in dst.",
      "evidence": ["根据`mask`对源操作数`src`按元素取绝对值，得到计算结果。", "计算结果写入`dst`。"]
    }
  }
]
```

返回值形式对应 `Returns element-wise absolute values of src under mask.`，并引用返回值说明。
同一数据类型的两种形式分别定位；不能把输出参数形式的摘要套用到返回值形式。
逐字证据、英文单行、目标匹配及重复目标检查在任何头文件写入前完成；不合格工件拒绝写入。

## 样例 marker 契约

1. **featured 名单**（agent 产出，随包版本化在 `featured/*.json`，默认目录）：
   `{"example": "examples/...", "apis": ["asc_xxx"], "evidence": ["README 逐字摘录"]}`；
   只收录 README 明示重点展示的 API；无 API 线索时收样例主体接口或置空。
2. **先确定范围**：扫描 featured API 的全部调用；每个源文件从首次调用所在行到末次调用结束行
   合成一个连续范围，允许跨函数，并包含中间的配置、循环、同步及其它源码。
   范围是展示片段，不保证可独立编译；不把跨文件调用合成一个范围。
3. **再确定 ID**：只有 `overload_status=exact`、独立语句且实参类型可推断的调用生成
   canonical `override_id`；同一 ID 在文件中调用多次只列一次。未确认重载的重点调用参与
   范围定位，但不猜测 ID，保留在调用问题报告中。
4. **集中渲染**：范围首尾各一行 `//! [id1, id2, ...]`，两侧列表相同，ID 去重后排序。
   单 ID 的 `//! [id]` 仍兼容。读取时将列表展开为多个 ID 与同一范围的关联，
   再按调用的精确 ID 匹配，不能把整个列表当成一个重载。
5. **迁移与幂等**：移除原来分散的 marker 行，集中到共同范围首尾；代码主体保持不变。
   已有标记范围会并入新范围，未在本次确认的其它 ID 或孤儿 ID 保留；只有同符号的精确调用
   能证明旧 ID 过期时才替换。再次执行无 diff；既有 marker 无效、嵌套或不闭合时跳过该文件并报告。

## 运行与报告

```bash
python3 scripts/annotate_sources.py include   --run-dir tmp/<run-id> [--brief-records <file>] [--only <doc子串>] [--refresh] [--write]
python3 scripts/annotate_sources.py examples  [--featured-dir <dir>] [--refresh] [--write]
```

- 运行目录默认 `tmp/source-annotate/`：`signatures.json`（全量解析缓存）、
  `include-report.json|md`、`examples-report.json`。
- dry-run 报告含逐头文件/逐文件的 diff 预览、跳因与问题清单；样例文件记录还包含
  `featured_calls`、`override_ids`、`source_start_line/source_end_line`、`range_lines`，
  行号指向本次写入前的源文件范围；`--write` 前不改动任何文件。
- 文档自身缺陷（README 与源码不符、参数表缺失、命名歧义）记入问题清单并登记
  `document-issues.md`，不阻塞其它记录。

## 与索引的关系

- include 注解中的 `@override_id`/`@archs`/`@param`/`ASC_DEPRECATED` 是索引侧
  结构推导的输入；样例 marker 的 id 与签名 override_id 同一套生成规则。
- `@param [in,out]` 由索引侧 `include_db` 解析（含方向归一）。
- `@brief` 为开发者提供重载功能摘要；当前 search 不将它写入索引或替代文档 description。
