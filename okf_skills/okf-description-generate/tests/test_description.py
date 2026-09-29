from __future__ import annotations

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / "scripts" / "description.py"
DOCUMENT = "docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md"


class DescriptionSkillTest(unittest.TestCase):
    def run_cli(self, *arguments: str, expect: int = 0) -> subprocess.CompletedProcess[str]:
        result = subprocess.run(
            [sys.executable, str(SCRIPT), *arguments],
            check=False,
            text=True,
            capture_output=True,
        )
        self.assertEqual(result.returncode, expect, result.stdout + result.stderr)
        return result

    def make_document(self, root: Path) -> Path:
        document = root / DOCUMENT
        document.parent.mkdir(parents=True)
        document.write_text(
            "---\n"
            "okftype: reference.api\n"
            "title: asc_add\n"
            "resource: asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md\n"
            "description: \"\"\n"
            "tag: []\n"
            "timestamp: '2026-09-20'\n"
            "---\n"
            "# asc_add\n\n"
            "## 功能说明\n\n"
            "对两个输入向量执行逐元素加法计算，并将计算结果写入输出向量。\n",
            encoding="utf-8",
        )
        return document

    def prepare(self, root: Path, run: Path) -> dict:
        self.run_cli(
            "prepare",
            "--devkit-root",
            str(root),
            "--document",
            DOCUMENT,
            "--run-id",
            "asc-add-description",
            "--run-dir",
            str(run),
        )
        return json.loads((run / "run-manifest.json").read_text(encoding="utf-8"))

    def test_valid_agent_result_requires_dry_run_before_write(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "devkit"
            document = self.make_document(root)
            run = Path(directory) / "run"
            manifest = self.prepare(root, run)
            result_path = run / "description-result.json"
            result_path.write_text(
                json.dumps(
                    {
                        "schema_version": 1,
                        "purpose": "okf_api_description_result",
                        "run_id": manifest["run_id"],
                        "document_path": DOCUMENT,
                        "document_sha256": manifest["document_sha256"],
                        "status": "ready",
                        "description": "对两个输入向量执行逐元素加法计算，并将结果写入输出向量。",
                        "evidence": [{"excerpt": "对两个输入向量执行逐元素加法计算，并将计算结果写入输出向量。"}],
                        "confidence": 0.95,
                    },
                    ensure_ascii=False,
                ),
                encoding="utf-8",
            )
            before = document.read_text(encoding="utf-8")
            self.run_cli("validate", "--run-dir", str(run), "--result", str(result_path))
            self.run_cli("write", "--run-dir", str(run), "--result", str(result_path), "--write", expect=2)
            self.run_cli("write", "--run-dir", str(run), "--result", str(result_path))
            self.assertEqual(document.read_text(encoding="utf-8"), before)
            self.run_cli("write", "--run-dir", str(run), "--result", str(result_path), "--write")
            self.assertIn('description: "对两个输入向量执行逐元素加法计算，并将结果写入输出向量。"', document.read_text(encoding="utf-8"))

    def test_evidence_must_be_literal_body_text(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / "devkit"
            self.make_document(root)
            run = Path(directory) / "run"
            manifest = self.prepare(root, run)
            result_path = run / "description-result.json"
            result_path.write_text(
                json.dumps(
                    {
                        "schema_version": 1,
                        "purpose": "okf_api_description_result",
                        "run_id": manifest["run_id"],
                        "document_path": DOCUMENT,
                        "document_sha256": manifest["document_sha256"],
                        "status": "ready",
                        "description": "执行高性能向量加法，并输出最佳精度结果。",
                        "evidence": [{"excerpt": "这是不存在的正文摘录。"}],
                        "confidence": 0.8,
                    },
                    ensure_ascii=False,
                ),
                encoding="utf-8",
            )
            self.run_cli("validate", "--run-dir", str(run), "--result", str(result_path), expect=2)
            validation = json.loads((run / "validation.json").read_text(encoding="utf-8"))
            self.assertFalse(validation["valid"])
            self.assertIn("未逐字出现在目标 Markdown 正文", validation["errors"][0])


if __name__ == "__main__":
    unittest.main()
