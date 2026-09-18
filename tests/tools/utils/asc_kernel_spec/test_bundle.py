#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import json
import subprocess
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(
    0, str(Path(__file__).resolve().parents[4] / "cmake/asc/fwk_modules/util")
)
import ascendc_build_kernel_spec as build_bundle
import kernel_spec_options


class SourceUniqueness(unittest.TestCase):
    def test_kernel_spec_modes(self):
        with tempfile.TemporaryDirectory() as tmp:
            config = Path(tmp) / "options.ini"
            for mode in ("None", "Normal", "SK"):
                with self.subTest(mode=mode):
                    remaining = kernel_spec_options.split_and_write_options(
                        [f"--kernel-spec={mode}", f"--kernel-spec-dir={tmp}", "-O2"],
                        str(config),
                        "AddCustom",
                        "ascend910b",
                    )
                    self.assertEqual(remaining, ["-O2"])
                    self.assertIn(f"--kernel-spec={mode}", config.read_text())
            with self.assertRaisesRegex(ValueError, "None, Normal or SK"):
                kernel_spec_options.split_and_write_options(
                    ["--kernel-spec=All", f"--kernel-spec-dir={tmp}"],
                    str(config),
                    "AddCustom",
                    "ascend910b",
                )

    def test_resource_scope_and_shared_dtype_source(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            manifests = []
            for dtype in ("float", "int"):
                manifest = root / (dtype + "_manifest.json")
                manifest.write_text(
                    json.dumps({"source_file": "kernel.cpp", "resource_id": "a" * 64})
                )
                manifests.append(manifest)
            for resource in ("first", "second"):
                source = root / resource
                source.mkdir()
                (source / "kernel.cpp").write_text("// kernel")
                build_bundle.validate_sources(source, manifests)

    def test_duplicate_even_if_not_referenced(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for name in ("one", "two"):
                (root / name).mkdir()
                (root / name / "other.cpp").write_text("// same bytes")
            build_bundle.validate_sources(root, [])

    def test_multiple_roots_form_one_resource(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            first, second = root / "first", root / "second"
            first.mkdir()
            second.mkdir()
            (first / "kernel.cpp").write_text("// first")
            manifest = root / "a_manifest.json"
            manifest.write_text(
                json.dumps({"source_file": "kernel.cpp", "resource_id": "a" * 64})
            )
            build_bundle.validate_sources([first, second], [manifest])
            (second / "kernel.cpp").write_text("// second")
            with self.assertRaisesRegex(ValueError, "Ambiguous source_file"):
                build_bundle.validate_sources([first, second], [manifest])

    def test_selected_files_form_one_resource(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            paths = [root / "a/kernel.cpp", root / "b/kernel.cpp"]
            for path in paths:
                path.parent.mkdir()
                path.write_text("// kernel")
            build_bundle.validate_sources([paths[0]], [])
            with self.assertRaisesRegex(ValueError, "Ambiguous source_file"):
                manifest = root / "source_manifest.json"
                manifest.write_text(json.dumps({"source_file": "kernel.cpp"}))
                build_bundle.validate_sources(paths, [manifest])

    def test_same_id_retains_one_complete_resource_and_other_ids(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            manifests = []
            for name, resource_id in [
                ("chip_a", "a" * 64),
                ("chip_b", "a" * 64),
                ("chip_c", "b" * 64),
            ]:
                directory = root / name
                (directory / "resources").mkdir(parents=True)
                (directory / "resources/stub.cpp").write_text(name)
                (directory / "resources/type.h").write_text(name)
                manifest = directory / "kernel_manifest.json"
                manifest.write_text(
                    json.dumps(
                        {
                            "resource_id": resource_id,
                            "resource_path": "resources",
                            "soc_version": name,
                        }
                    )
                )
                manifests.append(manifest)
            output = root / "collected"
            self.assertEqual(build_bundle.stage_unique_resources(manifests, output), 2)
            for resource_id, name in [("a" * 64, "chip_a"), ("b" * 64, "chip_c")]:
                selected = output / resource_id
                self.assertEqual(
                    json.loads((selected / "kernel_manifest.json").read_text())[
                        "soc_version"
                    ],
                    name,
                )
                self.assertEqual((selected / "resources/stub.cpp").read_text(), name)
                self.assertEqual((selected / "resources/type.h").read_text(), name)

    def test_packaged_source_path_preserves_original_and_resolves_duplicates(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for directory in ("a", "b"):
                (root / directory).mkdir()
                (root / directory / "kernel.cpp").write_text("// source")
            manifest = root / "kernel_manifest.json"
            document = {"source_file": "kernel.cpp", "resource_id": "a" * 64}
            manifest.write_text(json.dumps(document))
            maps = [(root / "a", "impl/a"), (root / "b", "impl/b")]
            with self.assertRaisesRegex(
                ValueError, "set source_file to a package-relative path"
            ):
                build_bundle.validate_sources(root, [manifest], maps)
            document["source_file"] = "impl/b/kernel.cpp"
            manifest.write_text(json.dumps(document))
            resolved = build_bundle.validate_sources(root, [manifest], maps)
            self.assertEqual(resolved[manifest], "impl/b/kernel.cpp")
            document["source_file"] = "kernel.cpp"
            manifest.write_text(json.dumps(document))
            resolved = build_bundle.validate_sources(root / "a", [manifest], maps)
            build_bundle.stage_unique_resources([manifest], root / "staged", resolved)
            self.assertEqual(
                json.loads(manifest.read_text())["source_file"], "kernel.cpp"
            )
            self.assertEqual(
                json.loads(
                    (root / "staged" / ("a" * 64) / "kernel_manifest.json").read_text()
                )["source_file"],
                "impl/a/kernel.cpp",
            )
            for bad in (
                "/tmp/kernel.cpp",
                "../kernel.cpp",
                "a/../kernel.cpp",
                "a//kernel.cpp",
            ):
                with self.assertRaises(ValueError):
                    build_bundle.safe_source_path(bad)

    def test_missing_source(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            manifest = root / "a_manifest.json"
            manifest.write_text(json.dumps({"source_file": "missing.cpp"}))
            with self.assertRaisesRegex(ValueError, "missing.cpp"):
                build_bundle.validate_sources(root, [manifest])

    def test_missing_root(self):
        with self.assertRaisesRegex(ValueError, "Source directory"):
            build_bundle.validate_sources(
                Path("/nonexistent/source-validation-root"), []
            )

    def test_duplicate_rejected_before_cached_bundle_is_reused(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            resources = root / "resources"
            sources = root / "sources"
            resources.mkdir()
            sources.mkdir()
            (resources / "a_manifest.json").write_text(
                json.dumps({"source_file": "kernel.cpp", "resource_id": "a" * 64})
            )
            (sources / "kernel.cpp").write_text("// kernel")
            exporter = root / "exporter"
            exporter.write_text(
                "#!" + sys.executable + "\nimport pathlib,sys\n"
                'pathlib.Path(sys.argv[sys.argv.index("--output")+1]).write_bytes(b"bundle")\n'
            )
            exporter.chmod(0o755)
            output = root / "binary" / "lib.so"
            argv = [
                sys.executable,
                build_bundle.__file__,
                "--input-dir",
                str(resources),
                "--source-dir",
                str(sources),
                "--output",
                str(output),
                "--exporter",
                str(exporter),
                "--cxx",
                "/bin/true",
                "--make",
                "/bin/true",
            ]
            subprocess.run(argv, check=True, capture_output=True)
            cached = subprocess.run(argv, check=True, capture_output=True, text=True)
            self.assertIn("JIT bundle unchanged", cached.stdout)
            self.assertEqual([p.name for p in output.parent.iterdir()], ["lib.so"])
            self.assertEqual(
                len(list((root / ".kernel_spec_cache").rglob("*.stamp.json"))), 1
            )
            stamp = next((root / ".kernel_spec_cache").rglob("*.stamp.json"))
            stamp.write_text("invalid json")
            recovered = subprocess.run(argv, check=True, capture_output=True, text=True)
            self.assertIn("JIT bundle published", recovered.stdout)
            self.assertEqual([p.name for p in output.parent.iterdir()], ["lib.so"])
            (sources / "nested").mkdir()
            (sources / "nested/kernel.cpp").write_text("// duplicate")
            result = subprocess.run(argv, capture_output=True, text=True)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("Ambiguous source_file", result.stderr)
            self.assertEqual(output.read_bytes(), b"bundle")


if __name__ == "__main__":
    unittest.main()
