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
                "```\n"
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
                    "headers": ["/include/c_api/detail/asc_example.h"],
                },
                signatures,
            )
            self.assertEqual(len(doxygen), 1)

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
                "```\n"
            )
            document.write_text(markdown, encoding="utf-8")

            signatures, _, _, doxygen, issues = module._source_signature_fields(
                root, document, markdown
            )

            self.assertEqual(issues, [])
            self.assertEqual(signatures[0]["archs"], ["3510"])
            self.assertEqual(doxygen[0]["archs"], ["3510"])

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

            self.assertEqual(enum["api_kind"], "enum")
            self.assertEqual(enum["headers"], ["/include/c_api/defs/enum.h"])
            self.assertEqual(enum["declaration"]["members"][0]["name"], "DEFAULT")
            self.assertEqual(alias["api_kind"], "type_alias")
            self.assertEqual(alias["declaration"]["target"], "asc_mode")
            self.assertEqual(union["api_kind"], "union")
            self.assertEqual(constant["declaration"]["value"], "16")
            self.assertEqual(catalog["api_kind"], "type_catalog")
            self.assertEqual(catalog["type_catalog"]["groups"][0]["types"], ["vector_int32_t", "vector_float"])
            self.assertEqual(macro["signature"], [{"macro_id": "ASC_WRAP__value"}])
            self.assertEqual(len(macro["doxygen"]), 1)
            self.assertEqual(object_macro["api_kind"], "macro")
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

    def test_definition_yaml_renders_typed_fields_without_signature(self):
        module = self._controller_module()
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = "docs/zh/api/SIMD-API/c_api/defs/enum/asc_mode.md"
            document = root / path
            document.parent.mkdir(parents=True)
            document.write_text("# asc_mode\n", encoding="utf-8")
            namespaces, values = module.taxonomy(ROOT / "taxonomy/core-v8-frozen.json")
            fields = {
                "okftype": "reference.api.cpp",
                "title": "asc_mode",
                "resource": f"asc-devkit/{path}",
                "description": "枚举模式。",
                "tag": [{namespace: []} for namespace in namespaces],
                "signature": [],
                "dependencies": [],
                "examples": [],
                "api_kind": "enum",
                "headers": ["/include/c_api/defs/enum.h"],
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
            self.assertIn("api_kind: enum", rendered)
            self.assertIn("headers: [/include/c_api/defs/enum.h]", rendered)
            self.assertIn("declaration:", rendered)
            self.assertNotIn("signature:", rendered)

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

    def test_worker_normalizes_signature_identifier_kind(self):
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
            signature = json.loads(output.read_text(encoding="utf-8"))["documents"][0]["fields"]["signature"]
            self.assertEqual(
                signature,
                [
                    {"override_id": "asc_fixed__u32"},
                    {"prototype_id": "asc_template__v<dtype>"},
                ],
            )

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

            namespaces, values = module.taxonomy(ROOT / "taxonomy/core-v8-frozen.json")
            fields = {
                "okftype": "reference.api.cpp",
                "title": "asc_foo",
                "resource": f"asc-devkit/{path}",
                "description": "执行测试计算。",
                "tag": [{namespace: []} for namespace in namespaces],
                "signature": [{
                    "prototype_id": "asc_foo__v<dtype>",
                    "archs": [],
                    "headers": [],
                    "dtype": ["half", "float"],
                }],
                "dependencies": [],
                "examples": [],
                "api_kind": None,
                "headers": None,
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

    def test_prepare_records_all_default_write_targets(self):
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
            self.assertEqual(manifest["write_targets"], list(module.DEFAULT_WRITE_TARGETS))

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
            taxonomy = ROOT / "taxonomy" / "core-v8-frozen.json"
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
        taxonomy = ROOT / "taxonomy" / "core-v8-frozen.json"
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
            self.assertIn("@override_id asc_foo__PUv_u32", header_text)
            self.assertIn("@param [out] dst 目的地址", header_text)
            sample_text = sample.read_text(encoding="utf-8")
            self.assertEqual(sample_text.count("//! [asc_foo__z]"), 2)
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
