"""Shared source ranges: migration, exact ID association and code preservation."""

from dataclasses import asdict
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from example_snippet_markers import apply_insertions_to_text, candidate_insertions
from extract_api_yaml_metadata import (
    EXAMPLE_SNIPPET_MARKER_RE, SignatureHeaderMapping, _example_calls,
    _example_snippet_markers, _snippet_marker_for_offset,
)
from prototype_id import signature_identifier


def without_markers(text):
    return "".join(line for line in text.splitlines(keepends=True)
                   if not EXAMPLE_SNIPPET_MARKER_RE.match(line.rstrip("\r\n")))


class RegionTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.source = self.root / "sample.asc"

    def calls(self, symbols):
        signatures = {
            name: (SignatureHeaderMapping(name, (), "hint_scope_single", "test", True,
                                          canonical_id=f"{name}(int32_t)"),)
            for name in symbols
        }
        calls, _ = _example_calls(self.root, self.root, signatures)
        return [asdict(call) for call in calls]

    def plan(self, calls, range_calls=None):
        record = {"path": "sample", "calls": calls}
        if range_calls is not None:
            record["range_calls"] = range_calls
        return candidate_insertions([record], self.root, "test")

    def test_merge_across_functions_and_repeat_calls(self):
        add_id = signature_identifier("asc_add(int32_t)")[1]
        text = (f"void first() {{\n//! [{add_id}]\nasc_add(1);\n//! [{add_id}]\n}}\n"
                "void second() {\nasc_add(2);\nasc_copy(3);\n}\n")
        self.source.write_text(text)
        calls = self.calls(("asc_add", "asc_copy"))
        plan, skips = self.plan(calls)
        self.assertEqual([], skips)
        self.assertEqual(1, len(plan))
        updated = apply_insertions_to_text(text, plan)
        self.assertEqual(without_markers(text), without_markers(updated))
        markers, issues = _example_snippet_markers(updated, "sample.asc")
        self.assertEqual((), issues)
        self.assertEqual(2, len(markers))
        self.assertEqual(1, len({(marker.start_offset, marker.end_offset) for marker in markers}))
        self.source.write_text(updated)
        rescanned = self.calls(("asc_add", "asc_copy"))
        for call in rescanned:
            self.assertEqual(signature_identifier(call["canonical_id"])[1], call["snippet"])
        repeat, skips = self.plan(rescanned)
        self.assertEqual(updated, apply_insertions_to_text(updated, repeat))

    def test_unresolved_calls_extend_range_without_invented_ids(self):
        text = "void f() {\nasc_unknown(x);\nasc_add(1);\n}\nvoid g() {\nasc_unknown(y);\n}\n"
        self.source.write_text(text)
        calls = self.calls(("asc_add",))
        ranges, _ = _example_calls(self.root, self.root, {"asc_unknown": ()})
        plan, _ = self.plan(calls, calls + [asdict(call) for call in ranges])
        updated = apply_insertions_to_text(text, plan)
        self.assertEqual(3, plan[0].covered_calls)
        self.assertNotIn("asc_unknown", plan[0].marker_id)
        self.assertEqual(without_markers(text), without_markers(updated))
        markers, _ = _example_snippet_markers(updated, "sample.asc")
        self.assertTrue(markers[0].start_offset <= updated.index("asc_unknown(x)"))
        self.assertTrue(markers[0].end_offset > updated.index("asc_unknown(y)"))

    def test_selected_api_preserves_other_ids_and_repairs_its_own(self):
        text = ("void f() {\n//! [asc_add__stale]\nasc_add(1);\n//! [asc_add__stale]\n"
                "//! [asc_other__z]\nasc_other();\n//! [asc_other__z]\n}\n")
        self.source.write_text(text)
        plan, _ = self.plan(self.calls(("asc_add",)))
        updated = apply_insertions_to_text(text, plan)
        self.assertNotIn("asc_add__stale", updated)
        self.assertIn("asc_other__z", updated)
        self.assertEqual(without_markers(text), without_markers(updated))

    def test_group_selects_exact_overload_and_ignores_unlisted_api(self):
        text = ("//! [asc_add__i32, asc_add__f32]\nasc_add(1);\nasc_other();\n"
                "//! [asc_add__f32, asc_add__i32]\n")
        markers, issues = _example_snippet_markers(text, "sample.asc")
        self.assertEqual((), issues)
        marker = _snippet_marker_for_offset(markers, text.index("asc_add(1)"), "asc_add", "asc_add__i32")
        self.assertEqual("asc_add__i32", marker.id)
        self.assertIsNone(_snippet_marker_for_offset(markers, text.index("asc_other()"), "asc_other"))

    def test_broken_existing_marker_blocks_write_plan(self):
        self.source.write_text("//! [asc_add__i32]\nasc_add(1);\n")
        plan, skips = self.plan(self.calls(("asc_add",)))
        self.assertEqual([], plan)
        self.assertIn("unclosed", skips[0].reason)


if __name__ == "__main__":
    unittest.main()
