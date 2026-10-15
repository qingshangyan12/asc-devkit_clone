"""Evidence-backed overload summaries: writing, preservation and rejection paths."""

from contextlib import redirect_stdout
from copy import deepcopy
import io
import json
from pathlib import Path
import re
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import annotate_sources
from prototype_id import signature_identifier


class BriefTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.source = "docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_abs.md"
        self.header = "include/c_api/reg_compute/compute/reg_arith.h"
        self.evidence = "根据mask对源操作数按元素取绝对值。"
        document = self.root / self.source
        document.parent.mkdir(parents=True)
        document.write_text('---\ndescription: "只存在于头中的证据"\n---\n# asc_abs\n\n' + self.evidence + '\n')
        self.path = self.root / self.header
        self.path.parent.mkdir(parents=True)
        self.original = (
            "/**\n * @param [out] dst Existing English parameter description.\n */\n"
            "__simd_callee__ inline void asc_abs(vector_float& dst, vector_float src, vector_bool mask);\n"
            "__simd_callee__ inline vector_float asc_abs(vector_float src, vector_bool mask);\n"
        )
        self.path.write_text(self.original)
        self.records = []
        self.briefs = []
        for function, parameters, brief in (
            ("asc_abs(vector_float&,vector_float,vector_bool)",
             "asc_abs(vector_float& dst,vector_float src,vector_bool mask)",
             "Computes element-wise absolute values of src under mask and stores the results in dst."),
            ("asc_abs(vector_float,vector_bool)",
             "asc_abs(vector_float src,vector_bool mask)",
             "Returns element-wise absolute values of src under mask."),
        ):
            _, identifier = signature_identifier(function)
            self.records.append({
                "source_document": self.source, "header": self.header,
                "function": function, "parameters": parameters,
                "override_id": identifier, "archs": ["3510"],
                "template_parameters": [], "doxygen": {},
            })
            self.briefs.append({
                "source_document": self.source, "override_id": identifier,
                "brief": {"text": brief, "evidence": [self.evidence]},
            })
        self.brief_path = self.root / "briefs.json"
        self.brief_path.write_text(json.dumps(self.briefs, ensure_ascii=False))

    def run_include(self, *, write=False, brief_file=True):
        cli = ["include", "--devkit-root", str(self.root), "--run-dir", str(self.root / "run")]
        if brief_file:
            cli += ["--brief-records", str(self.brief_path)]
        if write:
            cli.append("--write")
        args = annotate_sources.parser().parse_args(cli)
        with mock.patch.object(annotate_sources, "collect_signatures", return_value=(
            {}, [], {self.header: deepcopy(self.records)},
        )), redirect_stdout(io.StringIO()):
            annotate_sources.run_include(args)

    def test_dry_run_and_write_are_overload_specific_and_idempotent(self):
        self.run_include()
        self.assertEqual(self.original, self.path.read_text())
        self.run_include(write=True)
        updated = self.path.read_text()
        self.assertEqual(2, updated.count("@brief"))
        for item in self.briefs:
            self.assertIn(item["brief"]["text"], updated)
        self.assertIn("Existing English parameter description.", updated)
        self.assertRegex(updated, r"@ingroup [^\n]+\n \* @brief [^\n]+\n \*\n \* @override_id")
        strip = lambda text: re.sub(r"\s+", "", re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL))
        self.assertEqual(strip(self.original), strip(updated))
        self.run_include(write=True)
        self.assertEqual(updated, self.path.read_text())

    def test_existing_brief_is_preserved_including_backslash_style(self):
        for command in ("@brief", "\\brief"):
            with self.subTest(command=command):
                self.path.write_text(self.original.replace(
                    " * @param", f" * {command} Existing summary.\n *\n * @param", 1
                ))
                self.run_include(write=True)
                self.assertIn(command + " Existing summary.", self.path.read_text())
                self.assertNotIn(self.briefs[0]["brief"]["text"], self.path.read_text())

    def test_missing_brief_skips_declarations_and_reports_issue(self):
        self.run_include(write=True, brief_file=False)
        self.assertEqual(self.original, self.path.read_text())
        report = json.loads((self.root / "run/include-report.json").read_text())
        self.assertEqual(2, len(report["headers"][0]["skipped"]))
        self.assertEqual(2, report["unresolved_count"])

    def test_default_run_directory_brief_file_is_consumed(self):
        run = self.root / "run"
        run.mkdir()
        (run / "include-briefs.json").write_text(self.brief_path.read_text())
        self.run_include(write=True, brief_file=False)
        self.assertEqual(2, self.path.read_text().count("@brief"))

    def test_invalid_evidence_never_writes_headers(self):
        for evidence in ("编造的正文", "只存在于头中的证据"):
            with self.subTest(evidence=evidence):
                payload = deepcopy(self.briefs)
                payload[1]["brief"]["evidence"] = [evidence]
                self.brief_path.write_text(json.dumps(payload))
                with self.assertRaisesRegex(ValueError, "source body excerpt"):
                    self.run_include(write=True)
                self.assertEqual(self.original, self.path.read_text())

    def test_invalid_english_or_comment_injection_is_rejected(self):
        for text in ("计算绝对值。", "Computes values.\rMore text.", "Computes values. */", "Computes @archs 2201."):
            with self.subTest(text=text):
                payload = deepcopy(self.briefs)
                payload[0]["brief"]["text"] = text
                self.brief_path.write_text(json.dumps(payload))
                with self.assertRaisesRegex(ValueError, "English single-line"):
                    self.run_include(write=True)
                self.assertEqual(self.original, self.path.read_text())

    def test_unknown_and_duplicate_targets_are_rejected(self):
        payloads = [deepcopy(self.briefs), self.briefs + [self.briefs[0]]]
        payloads[0][0]["override_id"] = "asc_missing__z"
        for payload in payloads:
            with self.subTest(payload=payload):
                self.brief_path.write_text(json.dumps(payload))
                with self.assertRaisesRegex(ValueError, "unknown or duplicate"):
                    self.run_include(write=True)
                self.assertEqual(self.original, self.path.read_text())


if __name__ == "__main__":
    unittest.main()
