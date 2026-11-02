# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

"""Portable search and environment probes, using isolated data snapshots."""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
sys.path.insert(0, str(SCRIPTS))
import build_index  # noqa: E402
import detect_context  # noqa: E402
import ensure_index  # noqa: E402
import search  # noqa: E402


def fixture(root):
    doc = root / 'docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_demo.md'
    doc.parent.mkdir(parents=True)
    doc.write_text('''---
okftype: reference.api
description: 对输入执行简单的加法示例计算，用于验证隔离数据源的检索。
tag:
  - roles: [compute]
  - operations: [add]
dependencies: []
version: ">=9.0.0"
version_changes: []
---
# asc_demo
## 函数原型
```cpp
__aicore__ inline float asc_demo(float x);
```
## 产品支持情况
<!-- npu="950" id1 -->
- Ascend 950：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3：支持
<!-- end id2 -->
''')
    header = root / 'include/c_api/vector_compute/compute/vector_arith.h'
    header.parent.mkdir(parents=True)
    header.write_text('/**\n * @override_id asc_demo__f32\n * @archs 2201 3510\n * @param [in] x Value.\n */\n__aicore__ inline float asc_demo(float x);\n')
    source = root / 'examples/02_simd_c_api/demo/demo.asc'
    source.parent.mkdir(parents=True)
    source.write_text('//! [asc_demo__f32]\nasc_demo(1.0f);\n//! [asc_demo__f32]\n')
    (root / 'version.cmake').write_text('set(VERSION "9.2.0")\n')
    return doc


class ExternalSearchTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / 'data'
        self.doc = fixture(self.root)
        self.index = Path(self.temp.name) / 'indices/c_api'
        build_index.build(self.root, build_index.SCOPES['c_api'], self.index, '9.2.0', '3510', source_commit='a' * 40)

    def args(self, *values):
        # The public wrapper extends the same query parser; use CLI for final output tests.
        import query
        parser = query.parser()
        args = parser.parse_args(list(values))
        args.root = self.root
        args.index_dir = self.index
        args.platform = '950'
        args.repository = 'fixture'
        args.limit = 20
        args.keywords = []
        return args

    def test_source_provenance_and_locations(self):
        result = search.search(self.args('--api', 'asc_demo', '--version', '9.2.0'))
        self.assertEqual(result['source_commit'], 'a' * 40)
        self.assertEqual(result['count'], 1)
        hit = result['results'][0]
        self.assertEqual(hit['local_path'], str(self.doc))
        self.assertTrue(Path(hit['header_locations'][0]).is_file())
        self.assertTrue(Path(hit['example_locations'][0]['local_path']).is_file())
        other = Path(self.temp.name) / 'relocated'
        import shutil
        shutil.copytree(self.root, other)
        out = Path(self.temp.name) / 'second'
        build_index.build(other, build_index.SCOPES['c_api'], out, '9.2.0', '3510', source_commit='a' * 40)
        self.assertEqual(json.loads((out/'index.json').read_text()), json.loads((self.index/'index.json').read_text()))

    def test_keywords_intersect_dimensions_and_platform(self):
        args = self.args('--operation', 'add')
        args.keywords = ['加法', '隔离']
        self.assertEqual(search.search(args)['count'], 1)
        args.keywords = ['加法', '不存在']
        self.assertEqual(search.search(args)['count'], 0)
        args = self.args('--api', 'asc_demo')
        args.platform = 'a3'
        with self.assertRaisesRegex(ValueError, 'architecture'):
            search.search(args)
        args = self.args('--product', 'A3')
        with self.assertRaisesRegex(ValueError, 'conflicts'):
            search.search(args)

    def test_stale_query_fails_without_rebuild(self):
        self.doc.write_text(self.doc.read_text() + '\nNew constraint.\n')
        before = (self.index / 'index.json').read_bytes()
        with mock.patch.object(build_index, 'build', side_effect=AssertionError('no rebuild')):
            with self.assertRaisesRegex(ValueError, 'stale'):
                search.search(self.args('--api', 'asc_demo'))
        self.assertEqual(before, (self.index/'index.json').read_bytes())

    def test_snapshot_identity_is_checked_by_preparation(self):
        result = ensure_index.ensure(self.root, 'c_api', '9.2.0', '3510',
                                     index_root=self.index.parent, source_commit='b'*40)
        self.assertTrue(result['actions'])
        self.assertEqual(json.loads((self.index/'index.json').read_text())['meta']['source_commit'], 'b'*40)

    def test_external_dependency_checked_against_data_root(self):
        # Previously this check accidentally read the engine checkout.
        text = self.doc.read_text().replace('dependencies: []', 'dependencies:\n  - path: docs/zh/api/SIMD-API/c_api/other.md\n    kind: related')
        self.doc.write_text(text)
        with self.assertRaisesRegex(build_index.BuildError, 'dependency target not found'):
            build_index.build(self.root, build_index.SCOPES['c_api'], self.index, '9.2.0', '3510', source_commit='a'*40)


class ContextDetectionTests(unittest.TestCase):
    def test_explicit_context_does_not_probe(self):
        with mock.patch.object(detect_context.subprocess, 'run', side_effect=AssertionError('no probe')):
            result = detect_context.detect('a3', '9.2.0')
        self.assertEqual(result['arch'], '2201')
        self.assertEqual(result['unknown'], [])

    def test_version_metadata_and_missing_device(self):
        with tempfile.TemporaryDirectory() as temporary:
            package = Path(temporary) / 'cann-99.0.0'
            meta = package / 'share/info/asc-devkit/version.info'
            meta.parent.mkdir(parents=True)
            meta.write_text('Version=9.2.0\n')
            with mock.patch.object(detect_context.shutil, 'which', return_value=None):
                result = detect_context.detect(package_root=package)
            self.assertEqual(result['cann_version'], '9.2.0')
            self.assertEqual(result['unknown'], ['platform'])

    def test_shared_enum_and_mixed_devices_are_not_guessed(self):
        for output in ('ASCEND910B DAV_2201', 'Ascend910B3 Ascend910_9391'):
            with self.subTest(output=output), mock.patch.object(detect_context.shutil, 'which', return_value='npu-smi'), mock.patch.object(detect_context.subprocess, 'run', return_value=argparse.Namespace(returncode=0, stdout=output)):
                result = detect_context.detect(cann_version='9.2.0')
                self.assertIsNone(result['platform'])
                self.assertIn('platform', result['unknown'])


if __name__ == '__main__':
    unittest.main()
