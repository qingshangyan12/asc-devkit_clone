import json
import hashlib
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
CONTROLLER = ROOT / "scripts" / "controller.py"


class ControllerSmokeTest(unittest.TestCase):
    def _controller_module(self):
        from importlib.util import module_from_spec, spec_from_file_location

        spec = spec_from_file_location("okf_yaml_controller", CONTROLLER)
        module = module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(module)
        return module

    def test_source_analysis_uses_legacy_header_closure_and_macro_rules(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_example.md"
            facade = root / "include/c_api/asc_example.h"
            declaration = root / "include/c_api/detail/asc_example.h"
            document.parent.mkdir(parents=True)
            facade.parent.mkdir(parents=True)
            declaration.parent.mkdir(parents=True)
            facade.write_text('#include "detail/asc_example.h"\n', encoding="utf-8")
            declaration.write_text(
                "__aicore__ inline void asc_example(uint32_t value);\n"
                "#define ASC_EXAMPLE(value) asc_example(value)\n",
                encoding="utf-8",
            )
            markdown = (
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_example.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_example(uint32_t value);\n"
                "```\n\n## 参数说明\n\n"
                "| 参数名 | 输入/输出 | 描述 |\n"
                "| --- | --- | --- |\n"
                "| value | 输入 | 输入值。 |\n"
            )
            document.write_text(markdown, encoding="utf-8")

            signatures, _, _, doxygen, issues = module._source_signature_fields(
                root, document, markdown
            )

            self.assertEqual(issues, [])
            self.assertIn(
                {
                    "override_id": "asc_example__u32",
                    "archs": [],
                    "header": ["/include/c_api/detail/asc_example.h"],
                },
                signatures,
            )
            self.assertEqual(len(doxygen), 1)
            self.assertEqual(
                doxygen[0]["doxygen"]["params"],
                [{
                    "name": "value",
                    "direction": "in",
                    "description": "输入值。",
                    "evidence": ["| value | 输入 | 输入值。 |"],
                }],
            )
            updated, _ = module.apply_records_to_text(
                declaration.read_text(encoding="utf-8"), doxygen
            )
            self.assertIn("@param [in] value 输入值。", updated)

            macro_markdown = (
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_example.h\"`。\n\n"
                "## 函数原型\n\n```c\nASC_EXAMPLE(value)\n```\n"
            )
            macro_signatures, _, _, macro_doxygen, macro_issues = module._source_signature_fields(
                root, document, macro_markdown
            )
            self.assertEqual(macro_issues, [])
            self.assertEqual(macro_signatures, [{"macro_id": "ASC_EXAMPLE__value"}])
            self.assertEqual(len(macro_doxygen), 1)

    def test_signature_archs_fall_back_to_document_product_support(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_example.md"
            header = root / "include/c_api/asc_example.h"
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_example(uint32_t value);\n",
                encoding="utf-8",
            )
            markdown = (
                "## 产品支持情况\n\n<!-- npu=\"950\" -->\n- Ascend 950：支持\n\n"
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_example.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_example(uint32_t value);\n"
                "```\n\n## 参数说明\n\n"
                "| 参数名 | 输入/输出 | 描述 |\n"
                "| --- | --- | --- |\n"
                "| value | 输入 | 输入值。 |\n"
            )
            document.write_text(markdown, encoding="utf-8")

            signatures, _, _, doxygen, issues = module._source_signature_fields(
                root, document, markdown
            )

            self.assertEqual(issues, [])
            self.assertEqual(signatures[0]["archs"], ["3510"])
            self.assertEqual(doxygen[0]["archs"], ["3510"])

    def test_input_output_parameter_emits_inout_doxygen_tag(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_update.md"
            header = root / "include/c_api/asc_update.h"
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_update(__gm__ uint32_t* address, uint32_t value);\n",
                encoding="utf-8",
            )
            markdown = (
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_update.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_update(__gm__ uint32_t* address, uint32_t value);\n"
                "```\n\n## 参数说明\n\n"
                "| 参数名 | 输入/输出 | 描述 |\n"
                "| --- | --- | --- |\n"
                "| address | 输入/输出 | Global Memory的地址。 |\n"
                "| value | 输入 | 更新值。 |\n"
            )
            document.write_text(markdown, encoding="utf-8")

            _, _, _, doxygen, issues = module._source_signature_fields(root, document, markdown)

            self.assertEqual(issues, [])
            self.assertEqual(doxygen[0]["doxygen"]["params"][0]["direction"], "in,out")
            updated, applied = module.apply_records_to_text(
                header.read_text(encoding="utf-8"), doxygen
            )
            self.assertEqual(applied, 1)
            self.assertIn("@param [in,out] address Global Memory的地址。", updated)

    def test_documented_placeholder_overloads_emit_concrete_doxygen_ids(self):
        module = self._controller_module()
        source_root = ROOT.parents[1]
        source_document = source_root / "docs/zh/api/SIMD-API/c_api/cube_compute/asc_mmad.md"
        source_header = source_root / "include/c_api/cube_compute/cube_compute.h"
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / source_document.relative_to(source_root)
            header = root / source_header.relative_to(source_root)
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            markdown = source_document.read_text(encoding="utf-8")
            document.write_text(markdown, encoding="utf-8")
            header.write_text(source_header.read_text(encoding="utf-8"), encoding="utf-8")

            signatures, _, _, doxygen, issues = module._source_signature_fields(
                root, document, markdown
            )

            self.assertEqual(issues, [])
            self.assertTrue(any("prototype_id" in signature for signature in signatures))
            records = [record for record in doxygen if record["function"].startswith("asc_mmad(")]
            self.assertEqual(len(records), 22)
            self.assertTrue(all("override_id" in record for record in records))
            self.assertTrue(all("prototype_id" not in record for record in records))
            updated, applied = module.apply_records_to_text(
                header.read_text(encoding="utf-8"), records
            )
            self.assertEqual(applied, 22)
            for record in records:
                self.assertIn(f"@override_id {record['override_id']}", updated)

    def test_reverse_example_index_confirms_exact_call_for_markers(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_foo.md"
            header = root / "include/c_api/asc_foo.h"
            project = root / "examples/02_simd_c_api/demo/asc_foo"
            source = project / "main.asc"
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            project.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_foo(uint32_t value);\n",
                encoding="utf-8",
            )
            source.write_text(
                "void Kernel(uint32_t value) {\n    asc_foo(value);\n}\n",
                encoding="utf-8",
            )
            (project / "CMakeLists.txt").write_text(
                "set(SOC_VERSION dav-3510)\n", encoding="utf-8"
            )
            markdown = (
                "## 产品支持情况\n\n<!-- npu=\"950\" -->\n- Ascend 950：支持\n\n"
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_foo.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_foo(uint32_t value);\n"
                "```\n\n## 参数说明\n\n"
                "| 参数名 | 输入/输出 | 描述 |\n"
                "| --- | --- | --- |\n"
                "| value | 输入 | 输入值。 |\n"
            )
            document.write_text(markdown, encoding="utf-8")

            _, examples, marker_examples, _, issues = module._source_signature_fields(
                root,
                document,
                markdown,
                module.build_example_discovery_index(root),
            )

            self.assertEqual(issues, [])
            self.assertEqual(examples, [{
                "path": "/examples/02_simd_c_api/demo/asc_foo",
                "archs": ["3510"],
                "override_id": "asc_foo__u32",
                "file": "main.asc",
            }])
            confirmed = next(
                item for item in marker_examples
                if item["confidence"] == "authoritative" and not item["issues"]
            )
            self.assertEqual(confirmed["calls"][0]["canonical_id"], "asc_foo(uint32_t)")
            self.assertEqual(confirmed["calls"][0]["inferred_types"], ("uint32_t",))
            self.assertTrue(confirmed["calls"][0]["is_statement"])

    def test_missing_parameter_table_reports_include_doxygen_issue(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_example.md"
            header = root / "include/c_api/asc_example.h"
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_example(uint32_t value);\n",
                encoding="utf-8",
            )
            markdown = (
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_example.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_example(uint32_t value);\n"
                "```\n"
            )
            document.write_text(markdown, encoding="utf-8")

            _, _, _, doxygen, issues = module._source_signature_fields(
                root, document, markdown
            )

            self.assertEqual(len(doxygen), 1)
            self.assertEqual(doxygen[0]["doxygen"], {})
            include_issue = next(item for item in issues if item["field"] == "include_doxygen")
            self.assertEqual(include_issue["issue_type"], "document_defect")
            updated, applied = module.apply_records_to_text(
                header.read_text(encoding="utf-8"), doxygen
            )
            self.assertEqual(applied, 1)
            self.assertIn("@override_id asc_example__u32", updated)

    def test_partial_parameter_table_preserves_verified_doxygen_params(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/asc_example.md"
            header = root / "include/c_api/asc_example.h"
            document.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_example(uint32_t dst, uint32_t size);\n",
                encoding="utf-8",
            )
            markdown = (
                "## 功能说明\n\n头文件路径为：`\"c_api/asc_example.h\"`。\n\n"
                "## 函数原型\n\n```c\n"
                "__aicore__ inline void asc_example(uint32_t dst, uint32_t size);\n"
                "```\n\n## 参数说明\n\n"
                "| 参数名 | 输入/输出 | 描述 |\n"
                "| --- | --- | --- |\n"
                "| size | 输入 | 输入大小。 |\n"
            )
            document.write_text(markdown, encoding="utf-8")

            _, _, _, doxygen, issues = module._source_signature_fields(root, document, markdown)

            self.assertEqual(len(doxygen), 1)
            self.assertEqual(
                doxygen[0]["doxygen"]["params"],
                [{
                    "name": "size",
                    "direction": "in",
                    "description": "输入大小。",
                    "evidence": ["| size | 输入 | 输入大小。 |"],
                }],
            )
            include_issue = next(item for item in issues if item["field"] == "include_doxygen")
            self.assertIn("未覆盖参数 dst", include_issue["message"])
            updated, applied = module.apply_records_to_text(
                header.read_text(encoding="utf-8"), doxygen
            )
            self.assertEqual(applied, 1)
            self.assertIn("@param [in] size 输入大小。", updated)
            self.assertNotIn("@param [", updated.split("@param [in] size")[0])

    def test_architecture_siblings_suffix_only_shared_override_ids(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            header = root / "include/c_api/asc_foo.h"
            directory = root / "docs/zh/api/SIMD-API/c_api/foo"
            header.parent.mkdir(parents=True)
            directory.mkdir(parents=True)
            header.write_text(
                "__aicore__ inline void asc_foo(uint32_t size);\n"
                "__aicore__ inline void asc_foo(uint16_t size);\n",
                encoding="utf-8",
            )

            def write_document(architecture, declarations):
                document = directory / f"asc_foo_arch_{architecture}.md"
                document.write_text(
                    "# asc_foo\n\n"
                    "## 功能说明\n\n执行测试操作。\n\n"
                    "头文件路径为：`\"c_api/asc_foo.h\"`。\n\n"
                    "## 函数原型\n\n```c\n"
                    + declarations
                    + "```\n\n## 参数说明\n\n"
                    "| 参数名 | 输入/输出 | 描述 |\n"
                    "| --- | --- | --- |\n"
                    "| size | 输入 | 输入大小。 |\n",
                    encoding="utf-8",
                )
                return document

            older = write_document(
                "2201",
                "__aicore__ inline void asc_foo(uint32_t size);\n"
                "__aicore__ inline void asc_foo(uint16_t size);\n",
            )
            newer = write_document(
                "3510", "__aicore__ inline void asc_foo(uint32_t size);\n"
            )

            older_worker = module.build_worker_document(
                root, str(older.relative_to(root)), ("api_surfaces",)
            )
            newer_worker = module.build_worker_document(
                root, str(newer.relative_to(root)), ("api_surfaces",)
            )

            self.assertEqual(
                {item["override_id"] for item in older_worker["fields"]["signature"]},
                {"asc_foo__u32@2201", "asc_foo__u16"},
            )
            self.assertEqual(
                {item["override_id"] for item in newer_worker["fields"]["signature"]},
                {"asc_foo__u32@3510"},
            )
            self.assertIn(
                "asc_foo__u32@2201",
                {item.get("override_id") for item in older_worker["doxygen"]},
            )
            self.assertIn(
                "asc_foo__u32@3510",
                {item.get("override_id") for item in newer_worker["doxygen"]},
            )
            self.assertEqual(
                module._architecture_document_identity(
                    Path("asc_foo.md"), "# asc_foo_arch_2201\n"
                ),
                ("asc_foo", "2201"),
            )

    def test_doxygen_merges_shared_architecture_declaration(self):
        module = self._controller_module()
        header = "__aicore__ inline void asc_foo(uint32_t size);\n"
        records = [
            {
                "source_document": "docs/zh/api/SIMD-API/c_api/foo/asc_foo_arch_2201.md",
                "header": "include/c_api/asc_foo.h",
                "function": "asc_foo(uint32_t)",
                "parameters": "asc_foo(uint32_t size)",
                "override_id": "asc_foo__u32@2201",
                "archs": ["2201"],
                "template_parameters": [],
                "doxygen": {},
            },
            {
                "source_document": "docs/zh/api/SIMD-API/c_api/foo/asc_foo_arch_3510.md",
                "header": "include/c_api/asc_foo.h",
                "function": "asc_foo(uint32_t)",
                "parameters": "asc_foo(uint32_t size)",
                "override_id": "asc_foo__u32@3510",
                "archs": ["3510"],
                "template_parameters": [],
                "doxygen": {},
            },
        ]

        updated, count = module.apply_records_to_text(header, records)

        self.assertEqual(count, 2)
        self.assertEqual(updated.count("@override_id"), 1)
        self.assertIn("@override_id asc_foo__u32", updated)
        self.assertNotIn("@override_id asc_foo__u32@", updated)
        self.assertIn("@archs 2201 3510", updated)

    def test_definition_pages_extract_typed_metadata_without_function_signature_issues(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            enum_header = root / "include/c_api/defs/enum.h"
            union_header = root / "include/c_api/defs/union.h"
            constant_header = root / "include/c_api/defs/constant.h"
            macro_header = root / "include/c_api/defs/macro.h"
            for header in (enum_header, union_header, constant_header, macro_header):
                header.parent.mkdir(parents=True, exist_ok=True)
            enum_header.write_text(
                "enum asc_mode { DEFAULT = 0 };\nusing asc_mode_alias = asc_mode;\n",
                encoding="utf-8",
            )
            union_header.write_text("union asc_config { uint32_t value; };\n", encoding="utf-8")
            constant_header.write_text("constexpr uint8_t ASC_SIZE = 16;\n", encoding="utf-8")
            macro_header.write_text(
                "#define ASC_WRAP(value) (value)\n#define ASC_FLAG PLATFORM_FLAG\n",
                encoding="utf-8",
            )

            def write(category, name, content):
                document = root / "docs/zh/api/SIMD-API/c_api/defs" / category / name
                document.parent.mkdir(parents=True, exist_ok=True)
                document.write_text(content, encoding="utf-8")
                return document

            enum_doc = write("enum", "asc_mode.md", """# asc_mode

头文件路径为：`c_api/defs/enum.h`。

## 枚举具体定义

```cpp
enum asc_mode { DEFAULT = 0 };
```

## 枚举值详解

| 枚举值 | 说明 |
| --- | --- |
| DEFAULT | 默认模式。 |
""")
            alias_doc = write("enum", "asc_mode_alias.md", """# asc_mode_alias

头文件路径为：`c_api/defs/enum.h`。

## 类型别名具体定义

```cpp
using asc_mode_alias = asc_mode;
```
""")
            union_doc = write("union", "asc_config.md", """# asc_config

头文件路径为：`c_api/defs/union.h`。

## 联合体具体定义

```cpp
union asc_config { uint32_t value; };
```

## 字段详解

| 字段名 | 字段含义 |
| --- | --- |
| value | 配置值。 |
""")
            constant_doc = write("constant", "asc_size.md", """# ASC_SIZE

头文件路径为：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_SIZE = 16;
```
""")
            type_doc = write("type", "data_type_definition.md", """# 数据类型定义

## 数据类型

| 位宽类别 | 矢量数据寄存器类型 |
| --- | --- |
| b32 | vector_int32_t、vector_float |
""")
            macro_doc = write("macro", "asc_wrap.md", """# ASC_WRAP

头文件路径为：`c_api/defs/macro.h`。

## 宏定义

```cpp
#define ASC_WRAP(value) (value)
```
""")
            object_macro_doc = write("macro", "asc_flag.md", """# ASC_FLAG

头文件路径为：`c_api/defs/macro.h`。

## 宏定义

```cpp
#define ASC_FLAG PLATFORM_FLAG
```
""")

            enum = module._source_definition_fields(root, enum_doc, enum_doc.read_text(encoding="utf-8"))
            alias = module._source_definition_fields(root, alias_doc, alias_doc.read_text(encoding="utf-8"))
            union = module._source_definition_fields(root, union_doc, union_doc.read_text(encoding="utf-8"))
            constant = module._source_definition_fields(root, constant_doc, constant_doc.read_text(encoding="utf-8"))
            catalog = module._source_definition_fields(root, type_doc, type_doc.read_text(encoding="utf-8"))
            macro = module._source_definition_fields(root, macro_doc, macro_doc.read_text(encoding="utf-8"))
            object_macro = module._source_definition_fields(root, object_macro_doc, object_macro_doc.read_text(encoding="utf-8"))

            self.assertNotIn("api_kind", enum)
            self.assertEqual(enum["header"], ["/include/c_api/defs/enum.h"])
            self.assertEqual(enum["declaration"]["members"][0]["name"], "DEFAULT")
            self.assertNotIn("api_kind", alias)
            self.assertEqual(alias["declaration"]["target"], "asc_mode")
            self.assertNotIn("api_kind", union)
            self.assertEqual(constant["declaration"]["value"], "16")
            self.assertNotIn("api_kind", catalog)
            self.assertEqual(catalog["type_catalog"]["groups"][0]["types"], ["vector_int32_t", "vector_float"])
            self.assertEqual(macro["signature"], [{"macro_id": "ASC_WRAP__value"}])
            self.assertEqual(len(macro["doxygen"]), 1)
            self.assertNotIn("api_kind", object_macro)
            self.assertEqual(object_macro["declaration"]["macros"][0]["macro_kind"], "object")
            worker = module.build_worker_document(
                root,
                "docs/zh/api/SIMD-API/c_api/defs/macro/asc_wrap.md",
                ("api_surfaces",),
            )
            self.assertFalse(any(item["field"] == "signature" for item in worker["issues"]))

    def test_definition_directory_skips_index_pages(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            enum_dir = root / "docs/zh/api/SIMD-API/c_api/defs/enum"
            enum_dir.mkdir(parents=True)
            (enum_dir / "enum.md").write_text("# 枚举\n\n- [asc_mode](asc_mode.md)\n", encoding="utf-8")
            (enum_dir / "asc_mode.md").write_text(
                "# asc_mode\n\n## 枚举类具体定义\n\n```cpp\nenum class asc_mode { DEFAULT };\n```\n\n## 枚举值详解\n",
                encoding="utf-8",
            )
            selected = module.api_documents(root, "docs/zh/api/SIMD-API/c_api/defs", None)
            self.assertEqual(selected, ["docs/zh/api/SIMD-API/c_api/defs/enum/asc_mode.md"])
            worker = module.build_worker_document(
                root,
                "docs/zh/api/SIMD-API/c_api/defs/enum/asc_mode.md",
                ("api_surfaces",),
            )
            self.assertEqual(worker["fields"]["okftype"], "reference.type.cpp")

    def test_non_callable_page_uses_index_role(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document = root / "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document.parent.mkdir(parents=True)
            document.write_text("# 分类页\n\n- [接口](asc_foo.md)\n", encoding="utf-8")
            worker = module.build_worker_document(
                root,
                "docs/zh/api/SIMD-API/c_api/foo/foo.md",
                ("api_surfaces",),
            )
            self.assertEqual(worker["fields"]["okftype"], "reference.index.cpp")

    def test_definition_yaml_omits_legacy_api_kind_without_signature(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = "docs/zh/api/SIMD-API/c_api/defs/enum/asc_mode.md"
            document = root / path
            document.parent.mkdir(parents=True)
            document.write_text("# asc_mode\n", encoding="utf-8")
            namespaces, values = module.taxonomy(ROOT / "../okf-tag-generate/taxonomy/tag-taxonomy.json")
            fields = {
                "okftype": "reference.type.cpp",
                "title": "asc_mode",
                "resource": f"asc-devkit/{path}",
                "description": "枚举模式。",
                "tag": [{namespace: []} for namespace in namespaces],
                "signature": [],
                "dependencies": [],
                "examples": [],
                "api_kind": "enum",
                "header": ["/include/c_api/defs/enum.h"],
                "declaration": {
                    "symbol": "asc_mode",
                    "cpp": "enum class asc_mode { DEFAULT };",
                    "members": [{"name": "DEFAULT"}],
                },
                "type_catalog": None,
                "timestamp": "2026-09-20",
            }
            allowed_empty = {f"tag.{namespace}" for namespace in namespaces}
            rendered = module.render_yaml(fields, namespaces, values, root, path, "# asc_mode\n", {}, allowed_empty)
            self.assertNotIn("api_kind:", rendered)
            self.assertIn("header: [/include/c_api/defs/enum.h]", rendered)
            self.assertIn("declaration:", rendered)
            self.assertNotIn("signature:", rendered)

    def test_render_omits_empty_tag_namespaces(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = "docs/zh/api/SIMD-API/c_api/vector_compute/asc_add.md"
            namespaces, values = module.taxonomy(ROOT / "taxonomy/releases/core-v9-frozen.json")
            fields = {
                "okftype": "reference.api.cpp",
                "title": "asc_add",
                "resource": f"asc-devkit/{path}",
                "description": "矢量加法。",
                "tag": [
                    {"api_surfaces": ["c_api"]},
                    {"roles": ["compute"]},
                    {"families": []},
                    {"operations": ["add"]},
                    {"objects": []},
                    {"execution_models": ["memory_vector"]},
                    {"source_locations": []},
                    {"destination_locations": []},
                    {"traits": ["elementwise"]},
                ],
                "signature": [],
                "dependencies": [],
                "examples": [],
                "headers": None,
                "declaration": None,
                "type_catalog": None,
                "timestamp": "2026-09-24",
            }
            body = "# asc_add\n\n按元素相加的内存矢量加法运算。\n"
            evidence = {
                "api_surfaces:c_api": [{"source_path": path, "excerpt": path}],
                "roles:compute": [{"source_path": path, "excerpt": "加法运算"}],
                "operations:add": [{"source_path": path, "excerpt": "按元素相加"}],
                "execution_models:memory_vector": [{"source_path": path, "excerpt": "内存矢量加法运算"}],
                "traits:elementwise": [{"source_path": path, "excerpt": "按元素相加"}],
            }
            allowed_empty = {
                "tag.families", "tag.objects", "tag.source_locations", "tag.destination_locations",
            }
            rendered = module.render_yaml(fields, namespaces, values, root, path, body, evidence, allowed_empty)
            self.assertIn("api_surfaces: [c_api]", rendered)
            self.assertIn("roles: [compute]", rendered)
            self.assertIn("operations: [add]", rendered)
            self.assertIn("execution_models: [memory_vector]", rendered)
            self.assertIn("traits: [elementwise]", rendered)
            for namespace in ("families", "objects", "source_locations", "destination_locations"):
                self.assertNotIn(f"- {namespace}:", rendered, namespace)

    def test_prepare_worker_and_field_degradation(self):
        with tempfile.TemporaryDirectory() as temp:
            devkit = Path(temp) / "devkit"
            document = devkit / "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document.parent.mkdir(parents=True)
            document.write_text(
                "---\n"
                "okftype: reference.api.cpp\n"
                "title: foo\n"
                "resource: asc-devkit/docs/zh/api/SIMD-API/c_api/foo/foo.md\n"
                "description: 已确认\n"
                "usage_dependencies:\n"
                "  status: unconfirmed\n"
                "signature: []\n"
                "dependencies: []\n"
                "examples: []\n"
                "timestamp: '2026-09-18'\n"
                "---\n\n# foo\n\n正文。\n",
                encoding="utf-8",
            )
            run = Path(temp) / "run"
            subprocess.run(
                ["python3", str(CONTROLLER), "prepare", "--devkit-root", str(devkit),
                 "--document", "docs/zh/api/SIMD-API/c_api/foo/foo.md", "--run-dir", str(run)],
                check=True,
            )
            output = run / "batches/batch-01/worker-result.json"
            subprocess.run(
                ["python3", str(CONTROLLER), "worker", "--task", str(run / "batches/batch-01/task.json"),
                 "--output", str(output)],
                check=True,
            )
            payload = json.loads(output.read_text(encoding="utf-8"))
            self.assertEqual(payload["documents"][0]["path"], "docs/zh/api/SIMD-API/c_api/foo/foo.md")
            self.assertEqual(len(payload["documents"][0]["issues"]), 10)
            self.assertNotIn("usage_dependencies", payload["documents"][0]["fields"])

    def test_dependency_evidence_is_required(self):
        from importlib.util import spec_from_file_location, module_from_spec

        spec = spec_from_file_location("okf_controller", CONTROLLER)
        module = module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(module)
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            first = root / "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            second = root / "docs/zh/api/SIMD-API/c_api/foo/bar.md"
            first.parent.mkdir(parents=True)
            first.write_text("---\n---\n正文。\n", encoding="utf-8")
            second.write_text("---\n---\n配置关系正文。\n", encoding="utf-8")
            with self.assertRaises(module.ValidationError):
                module.normalize_dependencies(
                    root,
                    "docs/zh/api/SIMD-API/c_api/foo/foo.md",
                    [{"path": "docs/zh/api/SIMD-API/c_api/foo/bar.md", "kind": "requires"}],
                )

    def test_worker_does_not_extract_missing_description(self):
        with tempfile.TemporaryDirectory() as temp:
            devkit = Path(temp) / "devkit"
            document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document = devkit / document_path
            document.parent.mkdir(parents=True)
            document.write_text(
                "# foo\n\n## 功能说明\n\n执行可确认的向量操作。\n",
                encoding="utf-8",
            )
            run = Path(temp) / "run"
            subprocess.run(
                ["python3", str(CONTROLLER), "prepare", "--devkit-root", str(devkit),
                 "--document", document_path, "--run-dir", str(run)],
                check=True,
            )
            output = run / "batches/batch-01/worker-result.json"
            subprocess.run(
                ["python3", str(CONTROLLER), "worker", "--task", str(run / "batches/batch-01/task.json"),
                 "--output", str(output)],
                check=True,
            )
            worker = json.loads(output.read_text(encoding="utf-8"))["documents"][0]
            self.assertEqual(worker["fields"]["description"], "")
            self.assertTrue(any(item["field"] == "description" for item in worker["issues"]))

    def test_worker_does_not_reuse_legacy_signature_without_source_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            devkit = Path(temp) / "devkit"
            document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document = devkit / document_path
            document.parent.mkdir(parents=True)
            document.write_text(
                "---\n"
                "signature:\n"
                "  - prototype_id: asc_fixed__u32\n"
                "  - override_id: asc_template__v<dtype>\n"
                "---\n"
                "# foo\n",
                encoding="utf-8",
            )
            run = Path(temp) / "run"
            subprocess.run(
                ["python3", str(CONTROLLER), "prepare", "--devkit-root", str(devkit),
                 "--document", document_path, "--run-dir", str(run)],
                check=True,
            )
            output = run / "batches/batch-01/worker-result.json"
            subprocess.run(
                ["python3", str(CONTROLLER), "worker", "--task", str(run / "batches/batch-01/task.json"),
                 "--output", str(output)],
                check=True,
            )
            fields = json.loads(output.read_text(encoding="utf-8"))["documents"][0]["fields"]
            self.assertNotIn("signature", fields)

    def test_validate_signature_rejects_identifier_kind_mismatch(self):
        from importlib.util import spec_from_file_location, module_from_spec

        spec = spec_from_file_location("okf_controller_signature", CONTROLLER)
        module = module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(module)
        with self.assertRaises(module.ValidationError):
            module.validate_signature([{"prototype_id": "asc_fixed__u32"}], "api.md")
        with self.assertRaises(module.ValidationError):
            module.validate_signature([{"override_id": "asc_template__v<dtype>"}], "api.md")
        self.assertEqual(module.signature_id_key("asc_template__v<dtype>"), "prototype_id")
        self.assertEqual(module.signature_id_key("asc_cpp_template__v<T>"), "override_id")

    def test_dtype_is_signature_level_flow_array_and_inline_signature_yaml_is_removed(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = "docs/zh/api/SIMD-API/c_api/foo/asc_foo.md"
            document = root / path
            document.parent.mkdir(parents=True)
            body = """# asc_foo

## 函数原型

```yaml
api_signature:
  prototype_id: asc_foo__v<dtype>
  dtype:
    values: [half, float]
```

```c
__simd_callee__ inline vector_<dtype> asc_foo(vector_<dtype> src)
```

### dtype支持数据类型

`dtype`取值为：`half`、`float`。

```yaml
example: true
```
"""
            document.write_text(body, encoding="utf-8")
            self.assertEqual(
                module.source_signature_dtypes(body),
                {"asc_foo__v<dtype>": ["half", "float"]},
            )
            self.assertNotIn("api_signature:", module.strip_inline_api_signature_blocks(body))
            self.assertIn("example: true", module.strip_inline_api_signature_blocks(body))

            namespaces, values = module.taxonomy(ROOT / "../okf-tag-generate/taxonomy/tag-taxonomy.json")
            fields = {
                "okftype": "reference.api.cpp",
                "title": "asc_foo",
                "resource": f"asc-devkit/{path}",
                "description": "执行测试计算。",
                "tag": [{namespace: []} for namespace in namespaces],
                "signature": [{
                    "prototype_id": "asc_foo__v<dtype>",
                    "archs": [],
                    "header": [],
                    "dtype": ["half", "float"],
                }],
                "dependencies": [],
                "examples": [],
                "header": None,
                "declaration": None,
                "type_catalog": None,
                "timestamp": "2026-09-20",
            }
            allowed_empty = {f"tag.{namespace}" for namespace in namespaces}
            rendered = module.updated_document(
                body, fields, namespaces, values, root, path, {}, allowed_empty
            )
            self.assertIn("    dtype: [half, float]", rendered)
            self.assertNotIn("\ndtype:", rendered)
            self.assertNotIn("api_signature:", rendered)
            self.assertIn("example: true", rendered)

    def test_validate_signature_rejects_non_array_dtype(self):
        module = self._controller_module()
        with self.assertRaises(module.ValidationError):
            module.validate_signature(
                [{"prototype_id": "asc_foo__v<dtype>", "dtype": {"values": ["half"]}}],
                "api.md",
            )

    def test_source_dtype_values_stay_with_their_prototype(self):
        module = self._controller_module()
        markdown = """## 函数原型

### 返回形式

```c
__simd_callee__ inline vector_<dtype> asc_mode(vector_<dtype> src)
```

#### dtype支持数据类型

`dtype`取值为：`half`、`float`。

### 引用形式

```c
__simd_callee__ inline void asc_mode(vector_<dtype>& dst, vector_<dtype> src)
```

#### dtype支持数据类型

`dtype`取值为：`int16_t`、`uint16_t`。
"""
        self.assertEqual(
            module.source_signature_dtypes(markdown),
            {
                "asc_mode__v<dtype>": ["half", "float"],
                "asc_mode__Rv<dtype>_v<dtype>": ["int16_t", "uint16_t"],
            },
        )
        normalized = module.normalize_signature_dtypes(
            [
                {"prototype_id": "asc_mode__v<dtype>"},
                {"prototype_id": "asc_mode__Rv<dtype>_v<dtype>"},
            ],
            module.source_signature_dtypes(markdown),
            None,
        )
        self.assertEqual(normalized[0]["dtype"], ["half", "float"])
        self.assertEqual(normalized[1]["dtype"], ["int16_t", "uint16_t"])

    def test_prepare_records_default_write_contracts(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp) / "devkit"
            document = root / "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document.parent.mkdir(parents=True)
            document.write_text("# foo\n", encoding="utf-8")
            run = Path(temp) / "run"
            subprocess.run(
                ["python3", str(CONTROLLER), "prepare", "--devkit-root", str(root),
                 "--document", "docs/zh/api/SIMD-API/c_api/foo/foo.md", "--run-dir", str(run)],
                check=True,
            )
            manifest = json.loads((run / "manifest.json").read_text(encoding="utf-8"))
            self.assertEqual(manifest["write_contracts"], list(module.DEFAULT_WRITE_CONTRACTS))

    def test_profile_order_is_explicit_and_applicability_is_not_generated(self):
        module = self._controller_module()
        model = module.load_model()
        self.assertNotIn("applicability", model["field_order"])
        classification = module.classify_document(
            "docs/zh/api/SIMD-API/c_api/foo/foo.md",
            "## 函数原型\n\n```c\nvoid foo();\n```\n",
        )
        self.assertIsNotNone(classification)
        plan = module.compile_execution_plan(classification)
        self.assertEqual(
            plan.fields,
            (
                "okftype", "title", "resource", "description", "tag", "signature", "archs", "header",
                "dependencies", "examples", "timestamp",
            ),
        )
        self.assertNotIn("applicability", plan.fields)

    def test_render_yaml_follows_registry_order_and_never_emits_applicability(self):
        module = self._controller_module()
        namespaces, values = module.taxonomy(
            ROOT / "../okf-tag-generate/taxonomy/tag-taxonomy.json"
        )
        document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
        fields = {
            "okftype": "reference.api.cpp",
            "title": "foo",
            "resource": f"asc-devkit/{document_path}",
            "description": "这是一个用于测试字段顺序的接口描述。",
            "tag": [
                {namespace: [next(iter(values[namespace]))]}
                for namespace in namespaces
            ],
            "signature": [],
            "dependencies": [],
            "examples": [],
            "timestamp": "2026-09-23",
        }
        body = "# foo\n\n正文证据。\n"
        evidence = {
            f"{namespace}:{next(iter(values[namespace]))}": [{
                "source_path": document_path,
                "excerpt": document_path if namespace == "api_surfaces" else "正文证据。",
            }]
            for namespace in namespaces
        }
        rendered = module.render_yaml(fields, namespaces, values, Path("/tmp"), document_path, body, evidence, set())
        keys = [line.split(":", 1)[0] for line in rendered.splitlines() if line and not line[0].isspace()]
        self.assertEqual(
            keys,
            ["okftype", "title", "resource", "description", "tag", "signature", "dependencies", "examples", "timestamp"],
        )
        self.assertNotIn("applicability", rendered)

        index_fields = {
            "okftype": "reference.index.cpp",
            "title": "foo index",
            "resource": f"asc-devkit/{document_path}",
            "description": "这是一个用于测试索引页字段规则的描述。",
            "tag": [
                {namespace: ["c_api"] if namespace == "api_surfaces" else []}
                for namespace in namespaces
            ],
            "dependencies": [],
            "timestamp": "2026-09-23",
        }
        index_evidence = {
            "api_surfaces:c_api": [{"source_path": document_path, "excerpt": document_path}]
        }
        index_rendered = module.render_yaml(
            index_fields, namespaces, values, Path("/tmp"), document_path,
            body, index_evidence, set(),
        )
        self.assertIn("api_surfaces: [c_api]", index_rendered)

    def test_base_and_example_fields_follow_the_okf_contract(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            document = root / document_path
            sample = root / "examples/demo/foo.asc"
            document.parent.mkdir(parents=True)
            sample.parent.mkdir(parents=True)
            document.write_text("# foo\n", encoding="utf-8")
            sample.write_text("asc_foo();\n", encoding="utf-8")

            fields = {
                "okftype": "reference.api.cpp",
                "title": "foo",
                "resource": f"asc-devkit/{document_path}",
                "description": "说明。",
                "timestamp": "2026-09-20",
            }
            module.validate_base_fields(fields, document_path)
            self.assertEqual(
                module.normalize_examples(root, document_path, [{
                    "path": "/examples/demo",
                    "file": "foo.asc",
                    "archs": ["3510"],
                    "override_id": "asc_foo__u32",
                }]),
                [{
                    "path": "/examples/demo",
                    "file": "foo.asc",
                    "archs": ["3510"],
                    "override_id": "asc_foo__u32",
                }],
            )
            fields["resource"] = "asc-devkit/docs/other.md"
            with self.assertRaises(module.ValidationError):
                module.validate_base_fields(fields, document_path)
            with self.assertRaises(module.ValidationError):
                module.normalize_examples(root, document_path, [{
                    "path": "/examples/demo",
                    "override_id": "asc_foo__v<dtype>",
                }])

    def test_validate_renders_dependencies_after_verifying_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            devkit = Path(temp) / "devkit"
            document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
            dependency_path = "docs/zh/api/SIMD-API/c_api/foo/bar.md"
            document = devkit / document_path
            dependency = devkit / dependency_path
            document.parent.mkdir(parents=True)
            document.write_text("# foo\n\n关联文档。\n", encoding="utf-8")
            dependency.write_text("# bar\n", encoding="utf-8")
            run = Path(temp) / "run"
            batch = run / "batches" / "batch-01"
            batch.mkdir(parents=True)
            taxonomy = ROOT / "../okf-tag-generate/taxonomy/tag-taxonomy.json"
            (run / "manifest.json").write_text(
                json.dumps({
                    "schema_version": 1,
                    "run_id": "run",
                    "devkit_root": str(devkit),
                    "documents": [document_path],
                    "batches": [],
                    "taxonomy": str(taxonomy),
                }),
                encoding="utf-8",
            )
            namespaces = json.loads(taxonomy.read_text(encoding="utf-8"))["required_namespaces"]
            worker_result = {
                "schema_version": 1,
                "run_id": "run",
                "batch_id": "batch-01",
                "worker_id": "test",
                "documents": [{
                    "path": document_path,
                    "sha256": hashlib.sha256(document.read_bytes()).hexdigest(),
                    "semantic_verified": True,
                    "fields": {
                        "okftype": "reference.api.cpp",
                        "title": "foo",
                        "resource": f"asc-devkit/{document_path}",
                        "description": "关联文档。",
                        "tag": [
                            {"api_surfaces": ["c_api"]},
                            {"roles": ["compute"]},
                            {"families": ["arithmetic"]},
                            {"operations": ["add"]},
                            {"objects": ["vector"]},
                            {"execution_models": ["register_vector"]},
                            {"source_locations": ["vector_register"]},
                            {"destination_locations": ["vector_register"]},
                            {"traits": ["elementwise"]},
                        ],
                        "signature": [],
                        "dependencies": [{
                            "path": dependency_path,
                            "kind": "related",
                            "evidence": {"source_path": document_path, "excerpt": "关联文档。"},
                        }],
                        "examples": [],
                        "timestamp": "2026-09-18",
                    },
                    "evidence": {
                        "api_surfaces:c_api": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "roles:compute": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "families:arithmetic": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "operations:add": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "objects:vector": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "execution_models:register_vector": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "source_locations:vector_register": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "destination_locations:vector_register": [{"source_path": document_path, "excerpt": "关联文档。"}],
                        "traits:elementwise": [{"source_path": document_path, "excerpt": "关联文档。"}],
                    },
                    "issues": [],
                    "doxygen": [],
                    "example_markers": [],
                }],
            }
            (batch / "worker-result.json").write_text(
                json.dumps(worker_result, ensure_ascii=False), encoding="utf-8"
            )
            subprocess.run(
                ["python3", str(CONTROLLER), "validate", "--run-dir", str(run)], check=True
            )
            plan = json.loads((run / "write-plan.json").read_text(encoding="utf-8"))
            content = plan["patches"][0]["content"]
            self.assertIn(f"- path: {dependency_path}", content)
            self.assertNotIn("evidence:", content)

    def test_error_report_groups_issues_by_document(self):
        from importlib.util import spec_from_file_location, module_from_spec

        spec = spec_from_file_location("okf_controller_report", CONTROLLER)
        module = module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(module)
        report = {
            "run_id": "run",
            "issues": [
                {"document_path": "docs/a.md", "field": "description", "message": "缺失", "issue_type": "document_defect", "evidence": [], "decision": {"resolution": "blank_field", "note": "置空"}},
                {"document_path": "docs/a.md", "field": "signature", "message": "冲突", "issue_type": "source_defect", "evidence": [], "decision": {"resolution": "report_source_defect", "note": "报告"}},
            ],
        }
        output = module.report_markdown(report)
        self.assertEqual(output.count("## docs/a.md"), 1)
        self.assertIn("description", output)
        self.assertIn("signature", output)

    def test_validation_failure_persists_report_and_blocked_state(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            run = Path(temp) / "run"
            run.mkdir()
            (run / "manifest.json").write_text(
                json.dumps({"run_id": "run-report", "documents": []}), encoding="utf-8"
            )
            module.write_validation_failure_report(run, module.ValidationError("missing issue resolution"))
            report_json = run / "error-report.json"
            report_md = run / "error-report.md"
            validation = run / "validation.json"
            self.assertTrue(report_json.is_file())
            self.assertTrue(report_md.is_file())
            self.assertTrue(validation.is_file())
            self.assertEqual(json.loads(validation.read_text(encoding="utf-8"))["status"], "blocked")
            self.assertIn("missing issue resolution", report_md.read_text(encoding="utf-8"))


    def _build_run(self, temp, doxygen, markers):
        devkit = Path(temp) / "devkit"
        document_path = "docs/zh/api/SIMD-API/c_api/foo/foo.md"
        document = devkit / document_path
        document.parent.mkdir(parents=True)
        document.write_text("# foo\n\n关联文档。\n", encoding="utf-8")
        header = devkit / "include/test_api.h"
        header.parent.mkdir(parents=True)
        header.write_text(
            "__aicore__ inline void asc_foo(__ubuf__ void* dst, uint32_t size);\n"
            "__aicore__ inline void asc_bar(__ubuf__ void* dst, uint32_t size);\n",
            encoding="utf-8",
        )
        sample = devkit / "examples/demo/demo.asc"
        sample.parent.mkdir(parents=True)
        sample.write_text("    asc_foo(dst, 64);\n    asc_bar(dst, 64);\n", encoding="utf-8")
        run = Path(temp) / "run"
        batch = run / "batches/batch-01"
        batch.mkdir(parents=True)
        taxonomy = ROOT / "../okf-tag-generate/taxonomy/tag-taxonomy.json"
        (run / "manifest.json").write_text(
            json.dumps({
                "schema_version": 1,
                "run_id": "run",
                "devkit_root": str(devkit),
                "documents": [document_path],
                "batches": [],
                "taxonomy": str(taxonomy),
            }),
            encoding="utf-8",
        )
        tag = [
            {"api_surfaces": ["c_api"]},
            {"roles": ["compute"]},
            {"families": ["arithmetic"]},
            {"operations": ["add"]},
            {"objects": ["vector"]},
            {"execution_models": ["register_vector"]},
            {"source_locations": ["vector_register"]},
            {"destination_locations": ["vector_register"]},
            {"traits": ["elementwise"]},
        ]
        evidence = {
            f"{list(item.keys())[0]}:{list(item.values())[0][0]}": [
                {"source_path": document_path, "excerpt": "关联文档。"}
            ]
            for item in tag
        }
        worker_result = {
            "schema_version": 1,
            "run_id": "run",
            "batch_id": "batch-01",
            "worker_id": "test",
            "documents": [{
                "path": document_path,
                "sha256": hashlib.sha256(document.read_bytes()).hexdigest(),
                "semantic_verified": True,
                "fields": {
                    "okftype": "reference.api.cpp",
                    "title": "foo",
                    "resource": f"asc-devkit/{document_path}",
                    "description": "关联文档。",
                    "tag": tag,
                    "signature": [],
                    "dependencies": [],
                    "examples": [],
                    "timestamp": "2026-09-18",
                },
                "evidence": evidence,
                "issues": [],
                "doxygen": doxygen,
                "example_markers": markers,
            }],
        }
        (batch / "worker-result.json").write_text(
            json.dumps(worker_result, ensure_ascii=False), encoding="utf-8"
        )
        return devkit, run, document, header, sample

    def test_multiple_auxiliary_patches_compose_per_file(self):
        with tempfile.TemporaryDirectory() as temp:
            doxygen = [
                {
                    "source_document": "docs/zh/api/SIMD-API/c_api/foo/foo.md",
                    "header": "include/test_api.h",
                    "function": "asc_foo(__ubuf__ void*,uint32_t)",
                    "parameters": "asc_foo(__ubuf__ void* dst, uint32_t size)",
                    "override_id": "asc_foo__PUv_u32@2201",
                    "archs": ["3510"],
                    "template_parameters": [],
                    "doxygen": {
                        "brief": {"text": "功能甲", "evidence": ["关联文档。"]},
                        "params": [
                            {"name": "dst", "direction": "out", "description": "目的地址", "evidence": ["关联文档。"]},
                        ],
                    },
                },
                {
                    "source_document": "docs/zh/api/SIMD-API/c_api/foo/foo.md",
                    "header": "include/test_api.h",
                    "function": "asc_bar(__ubuf__ void*,uint32_t)",
                    "parameters": "asc_bar(__ubuf__ void* dst, uint32_t size)",
                    "archs": ["3510"],
                    "template_parameters": [],
                    "doxygen": {
                        "brief": {"text": "功能乙", "evidence": ["关联文档。"]},
                    },
                },
            ]
            markers = [
                {
                    "path": "/examples/demo",
                    "confidence": "authoritative",
                    "calls": [
                        {
                            "file": "/examples/demo/demo.asc",
                            "call_start_offset": 4,
                            "call_end_offset": 11,
                            "canonical_id": "asc_foo()",
                            "override_id": "asc_foo__z@2201",
                        },
                        {
                            "file": "/examples/demo/demo.asc",
                            "call_start_offset": 28,
                            "call_end_offset": 35,
                            "canonical_id": "asc_bar()",
                        },
                    ],
                },
            ]
            devkit, run, document, header, sample = self._build_run(temp, doxygen, markers)
            subprocess.run(["python3", str(CONTROLLER), "validate", "--run-dir", str(run)], check=True)
            plan = json.loads((run / "write-plan.json").read_text(encoding="utf-8"))
            self.assertEqual(len(plan["files"]), 3)
            subprocess.run(["python3", str(CONTROLLER), "write", "--run-dir", str(run)], check=True)
            header_text = header.read_text(encoding="utf-8")
            self.assertEqual(header_text.count("/**"), 2)
            self.assertIn("@brief 功能甲", header_text)
            self.assertIn("@brief 功能乙", header_text)
            self.assertIn("@override_id asc_foo__PUv_u32@2201", header_text)
            self.assertIn("@param [out] dst 目的地址", header_text)
            sample_text = sample.read_text(encoding="utf-8")
            self.assertEqual(sample_text.count("//! [asc_foo__z@2201]"), 2)
            self.assertEqual(sample_text.count("//! [asc_bar__z]"), 2)
            self.assertEqual(sample_text.count("    asc_foo(dst, 64);"), 1)
            final = json.loads((run / "write-result.json").read_text(encoding="utf-8"))
            self.assertEqual(final["written"], 3)

    def test_duplicate_auxiliary_anchor_is_rejected(self):
        from importlib.util import spec_from_file_location, module_from_spec

        with tempfile.TemporaryDirectory() as temp:
            doxygen = [
                {
                    "path": "include/test_api.h",
                    "anchor": "__aicore__ inline void asc_foo(__ubuf__ void* dst, uint32_t size);",
                    "text": "/**\n * @brief 功能甲\n */",
                },
                {
                    "path": "include/test_api.h",
                    "anchor": "__aicore__ inline void asc_foo(__ubuf__ void* dst, uint32_t size);",
                    "text": "/**\n * @brief 重复\n */",
                },
            ]
            devkit, run, *_ = self._build_run(temp, doxygen, [])
            spec = spec_from_file_location("okf_controller_dup", CONTROLLER)
            module = module_from_spec(spec)
            assert spec.loader is not None
            spec.loader.exec_module(module)
            with self.assertRaises(subprocess.CalledProcessError):
                subprocess.run(["python3", str(CONTROLLER), "validate", "--run-dir", str(run)], check=True)


    def test_blank_tag_namespace_preserves_other_namespaces(self):
        from importlib.util import spec_from_file_location, module_from_spec

        spec = spec_from_file_location("okf_controller_blank", CONTROLLER)
        module = module_from_spec(spec)
        assert spec.loader is not None
        spec.loader.exec_module(module)
        fields = {
            "tag": [
                {"api_surfaces": ["c_api"]},
                {"roles": ["data_movement"]},
                {"families": []},
            ]
        }
        module.blank_field(fields, "tag.families")
        self.assertEqual(
            fields["tag"],
            [{"api_surfaces": ["c_api"]}, {"roles": ["data_movement"]}, {"families": []}],
        )
        module.blank_field(fields, "tag.api_surfaces")
        self.assertEqual(
            fields["tag"],
            [{"api_surfaces": []}, {"roles": ["data_movement"]}, {"families": []}],
        )


if __name__ == "__main__":
    unittest.main()
