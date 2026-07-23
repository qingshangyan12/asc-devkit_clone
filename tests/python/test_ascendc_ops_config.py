#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


UTIL_DIR = Path(__file__).resolve().parents[2] / "cmake" / "asc" / "fwk_modules" / "util"
sys.path.insert(0, str(UTIL_DIR))
SPEC = importlib.util.spec_from_file_location(
    "ascendc_ops_config_under_test", UTIL_DIR / "ascendc_ops_config.py"
)
ASCENDC_OPS_CONFIG = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(ASCENDC_OPS_CONFIG)


class AscendcOpsConfigTest(unittest.TestCase):
    def test_static_compile_manifest_is_not_mutated_or_aggregated(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            root = Path(temp_dir) / "ascend910b" / "add_custom"
            manifest = root / "static_compile" / "kernel_id" / "manifest.json"
            metadata = root / "kernel.json"
            manifest.parent.mkdir(parents=True)
            manifest.write_text('{"schema_version":"4.0"}', encoding="utf-8")
            metadata.write_text('{"op_type":"AddCustom"}', encoding="utf-8")

            generated_inputs = []

            def capture(json_files, *_args):
                generated_inputs.extend(json_files)

            with mock.patch.object(
                ASCENDC_OPS_CONFIG, "check_single_op_is_void", return_value=True
            ), mock.patch.object(
                ASCENDC_OPS_CONFIG, "generate_operator_cfg_file", side_effect=capture
            ):
                self.assertTrue(
                    ASCENDC_OPS_CONFIG.gen_all_config(
                        str(Path(temp_dir)), "ascend910b", str(Path(temp_dir) / "config"), True
                    )
                )

            self.assertEqual(
                json.loads(manifest.read_text(encoding="utf-8")),
                {"schema_version": "4.0"},
            )
            self.assertIn("filePath", json.loads(metadata.read_text(encoding="utf-8")))
            self.assertNotIn(str(manifest), generated_inputs)
            self.assertIn(str(metadata), generated_inputs)

    def test_only_exact_static_compile_manifest_layout_is_excluded(self):
        self.assertTrue(
            ASCENDC_OPS_CONFIG.is_static_compile_manifest(
                "/root/static_compile/kernel_id/manifest.json"
            )
        )
        self.assertFalse(
            ASCENDC_OPS_CONFIG.is_static_compile_manifest(
                "/root/static_compile/kernel_id/resources/manifest.json"
            )
        )
        self.assertFalse(
            ASCENDC_OPS_CONFIG.is_static_compile_manifest("/root/kernel_id/manifest.json")
        )


if __name__ == "__main__":
    unittest.main()
