#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# ----------------------------------------------------------------------------------------------------------

import json
import os
import sys
import unittest
from tempfile import TemporaryDirectory
from types import SimpleNamespace


FILE_PATH = os.path.dirname(os.path.realpath(__file__))
TOP_PATH = os.path.join(FILE_PATH, "../../../")
sys.path.insert(0, os.path.join(TOP_PATH, "tools/build/"))

from asc_op_compile_base.asc_op_compiler.compile_op import (  # noqa: E402
    JIT_COLLECTION_ROOT_ENV,
    _collect_compile_resources,
)


class TestCompileOpResourceCollection(unittest.TestCase):
    def test_collects_manifest_and_files_with_stable_relative_layout(self):
        with TemporaryDirectory() as temp_dir:
            kernel_meta = os.path.join(temp_dir, "kernel_meta")
            resources = os.path.join(kernel_meta, "resources")
            collection = os.path.join(temp_dir, "collected")
            unit_name = "AddCustom_0123456789abcdef"
            manifest_path = os.path.join(kernel_meta, unit_name + "_manifest.json")
            os.makedirs(resources)
            with open(os.path.join(resources, "kernel.cpp"), "w") as output:
                output.write("wrapper source\n")
            nested = os.path.join(resources, "include")
            os.makedirs(nested)
            with open(os.path.join(nested, "kernel.cpp"), "w") as output:
                output.write("nested source\n")
            manifest = {"base_dir": "resources"}
            with open(manifest_path, "w") as output:
                json.dump(manifest, output)

            previous = os.environ.get(JIT_COLLECTION_ROOT_ENV)
            os.environ[JIT_COLLECTION_ROOT_ENV] = collection
            try:
                _collect_compile_resources(SimpleNamespace(
                    op_name=unit_name,
                    manifest_path=manifest_path,
                    kernel_meta_dir=kernel_meta,
                ))
            finally:
                if previous is None:
                    os.environ.pop(JIT_COLLECTION_ROOT_ENV, None)
                else:
                    os.environ[JIT_COLLECTION_ROOT_ENV] = previous

            unit_root = os.path.join(collection, "manifests", unit_name)
            self.assertTrue(os.path.isfile(os.path.join(unit_root, unit_name + "_manifest.json")))
            with open(os.path.join(unit_root, "resources", "kernel.cpp")) as collected:
                self.assertEqual(collected.read(), "wrapper source\n")
            with open(os.path.join(unit_root, "resources", "include", "kernel.cpp")) as collected:
                self.assertEqual(collected.read(), "nested source\n")

    def test_collection_rejects_duplicate_unit_identity(self):
        with TemporaryDirectory() as temp_dir:
            unit_name = "AddCustom_duplicate"
            manifest_path = os.path.join(temp_dir, unit_name + "_manifest.json")
            resources = os.path.join(temp_dir, "resources")
            collection = os.path.join(temp_dir, "collected")
            os.makedirs(resources)
            with open(manifest_path, "w") as output:
                json.dump({"base_dir": "resources"}, output)
            os.makedirs(os.path.join(collection, "manifests", unit_name))

            previous = os.environ.get(JIT_COLLECTION_ROOT_ENV)
            os.environ[JIT_COLLECTION_ROOT_ENV] = collection
            try:
                with self.assertRaisesRegex(RuntimeError, "duplicate collected manifest identity"):
                    _collect_compile_resources(SimpleNamespace(
                        op_name=unit_name,
                        manifest_path=manifest_path,
                        kernel_meta_dir=temp_dir,
                    ))
            finally:
                if previous is None:
                    os.environ.pop(JIT_COLLECTION_ROOT_ENV, None)
                else:
                    os.environ[JIT_COLLECTION_ROOT_ENV] = previous


if __name__ == "__main__":
    unittest.main()
