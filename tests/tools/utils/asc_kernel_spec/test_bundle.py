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
import shutil
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
    def test_include_paths_follow_install_mapping_without_changing_order(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            common = root / "build/common"
            common.mkdir(parents=True)
            external = root / "external"
            external.mkdir()
            document = {
                "source_file": "src/op/op_kernel/kernel.cpp",
                "options": {
                    "common_compile": [
                        "-I" + str(common),
                        "-I",
                        str(external),
                        "-isystem",
                        str(common / "nested"),
                        "-DVALUE=1",
                    ]
                },
                "kernels": [
                    {
                        "objects": [
                            {
                                "commands": [
                                    {
                                        "type": "compile",
                                        "cmd": ["cc", "-iquote", str(common)],
                                    },
                                    {"type": "link", "cmd": ["ld", "-I" + str(common)]},
                                ]
                            }
                        ]
                    }
                ],
            }
            build_bundle.relocate_include_paths(document, [(str(common), "src/common")])
            self.assertEqual(
                document["options"]["common_compile"],
                [
                    "-I${source_file_path}/../../common",
                    "-I",
                    str(external),
                    "-isystem",
                    "${source_file_path}/../../common/nested",
                    "-DVALUE=1",
                ],
            )
            commands = document["kernels"][0]["objects"][0]["commands"]
            self.assertEqual(
                commands[0]["cmd"],
                ["cc", "-iquote", "${source_file_path}/../../common"],
            )
            self.assertEqual(commands[1]["cmd"], ["ld", "-I" + str(common)])

    def test_include_mapping_uses_most_specific_root_and_rejects_ambiguity(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            nested = root / "nested"
            nested.mkdir()

            def document():
                return {
                    "source_file": "src/kernel.cpp",
                    "options": {"common_compile": ["-I" + str(nested)]},
                }

            mappings = [(str(root), "src"), (str(nested), "headers")]
            value = document()
            build_bundle.relocate_include_paths(value, mappings)
            self.assertEqual(
                value["options"]["common_compile"], ["-I${source_file_path}/../headers"]
            )
            with self.assertRaisesRegex(ValueError, "Ambiguous include"):
                build_bundle.relocate_include_paths(
                    document(), mappings + [(str(nested), "other")]
                )

    def test_static_without_install_mapping_preserves_options(self):
        document = {
            "source_file": "kernel.cpp",
            "options": {
                "common_compile": ["-I/build/missing", "-Irelative", "-I${env:HEADERS}"]
            },
        }
        original = json.loads(json.dumps(document))
        build_bundle.relocate_include_paths(document, [])
        self.assertEqual(document, original)

    @unittest.skipUnless(shutil.which("c++"), "requires the host C++ compiler")
    def test_staged_manifest_compiles_after_build_directory_is_removed(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            build = root / "build"
            source, headers = build / "op", build / "headers"
            source.mkdir(parents=True)
            headers.mkdir()
            (source / "kernel.cpp").write_text(
                '#include "value.h"\nstatic_assert(VALUE == 7);\n'
            )
            (headers / "value.h").write_text("#define VALUE 7\n")
            manifest = root / "kernel_manifest.json"
            manifest.write_text(
                json.dumps(
                    {
                        "source_file": "kernel.cpp",
                        "resource_id": "b" * 64,
                        "options": {"common_compile": ["-I", str(headers)]},
                    }
                )
            )
            mappings = [(str(source), "src/op"), (str(headers), "src/common")]
            paths = build_bundle.validate_sources(source, [manifest], mappings)
            staged = root / "staged"
            build_bundle.stage_unique_resources([manifest], staged, paths, mappings)
            package = root / "moved_package"
            shutil.copytree(source, package / "src/op")
            shutil.copytree(headers, package / "src/common")
            shutil.rmtree(build)
            document = json.loads(
                (staged / ("b" * 64) / "kernel_manifest.json").read_text()
            )
            installed_source = package / document["source_file"]
            options = [
                value.replace("${source_file_path}", str(installed_source.parent))
                for value in document["options"]["common_compile"]
            ]
            subprocess.run(
                ["c++", "-std=c++17", "-fsyntax-only", *options, str(installed_source)],
                check=True,
                capture_output=True,
            )
            self.assertEqual(
                json.loads(manifest.read_text())["source_file"], "kernel.cpp"
            )

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

    def test_unmapped_relative_source_is_preserved(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            manifest = root / "kernel_manifest.json"
            # The user may arrange this path only after installing the archive.
            document = {
                "source_file": "user/layout/kernel.cpp",
                "resource_id": "a" * 64,
            }
            manifest.write_text(json.dumps(document))
            resolved = build_bundle.validate_sources(root, [manifest])
            self.assertEqual(resolved, {})
            build_bundle.stage_unique_resources([manifest], root / "staged", resolved)
            staged = root / "staged" / ("a" * 64) / "kernel_manifest.json"
            self.assertEqual(json.loads(staged.read_text()), document)
            self.assertEqual(json.loads(manifest.read_text()), document)

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

    def test_source_move_invalidates_cached_bundle(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            sources = root / "sources"
            resources = root / "resources"
            (sources / "a").mkdir(parents=True)
            resources.mkdir()
            (sources / "a/kernel.cpp").write_text("// source")
            manifest = resources / "kernel_manifest.json"
            document = {"source_file": "kernel.cpp", "resource_id": "a" * 64}
            manifest.write_text(json.dumps(document))
            exporter = root / "exporter"
            exporter.write_text(
                "#!" + sys.executable + "\nimport pathlib,sys\n"
                "args = sys.argv\n"
                'source = pathlib.Path(args[args.index("--input-dir") + 1])\n'
                'output = pathlib.Path(args[args.index("--output") + 1])\n'
                'output.write_bytes(next(source.rglob("kernel_manifest.json")).read_bytes())\n'
            )
            exporter.chmod(0o755)
            output = root / "bundle.o"
            argv = [
                sys.executable,
                build_bundle.__file__,
                "--input-dir",
                str(resources),
                "--source-dir",
                str(sources),
                "--source-map",
                str(sources),
                "share/package/src",
                "--output",
                str(output),
                "--exporter",
                str(exporter),
                "--cxx",
                "/bin/true",
                "--make",
                "/bin/true",
            ]
            for directory in ("a", "b"):
                if directory == "b":
                    (sources / "a").rename(sources / "b")
                rebuilt = subprocess.run(
                    argv, check=True, capture_output=True, text=True
                )
                self.assertIn("JIT bundle published", rebuilt.stdout)
                self.assertEqual(
                    json.loads(output.read_text())["source_file"],
                    f"share/package/src/{directory}/kernel.cpp",
                )
                # A new dynamic build recreates identical manifests; keep the published bundle.
                original = manifest.read_bytes()
                manifest.unlink()
                manifest.write_bytes(original)
                cached = subprocess.run(
                    argv, check=True, capture_output=True, text=True
                )
                self.assertIn("JIT bundle unchanged", cached.stdout)
            self.assertEqual(json.loads(manifest.read_text()), document)

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
