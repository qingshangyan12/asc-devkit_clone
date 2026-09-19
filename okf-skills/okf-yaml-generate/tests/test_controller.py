import json
import hashlib
from pathlib import Path
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
CONTROLLER = ROOT / "scripts" / "controller.py"


class ControllerSmokeTest(unittest.TestCase):
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
                    "path": "include/test_api.h",
                    "anchor": "__aicore__ inline void asc_foo(__ubuf__ void* dst, uint32_t size);",
                    "text": "/**\n * @brief 功能甲\n * @see docs/zh/api/SIMD-API/c_api/foo/foo.md\n */",
                },
                {
                    "path": "include/test_api.h",
                    "anchor": "__aicore__ inline void asc_bar(__ubuf__ void* dst, uint32_t size);",
                    "text": "/**\n * @brief 功能乙\n * @see docs/zh/api/SIMD-API/c_api/foo/foo.md\n */",
                },
            ]
            markers = [
                {"path": "examples/demo/demo.asc", "anchor": "    asc_foo(dst, 64);", "text": "// okf-marker: asc_foo__vp_u32"},
                {"path": "examples/demo/demo.asc", "anchor": "    asc_bar(dst, 64);", "text": "// okf-marker: asc_bar__vp_u32"},
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
            sample_text = sample.read_text(encoding="utf-8")
            self.assertEqual(sample_text.count("// okf-marker:"), 2)
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
