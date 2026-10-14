"""shape 扩展必须保留 JIT 和 OPC 共同消费的 SK 模式。"""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from static_compile import static_compilation as compilation
from static_compile import static_compile_backend as backend


def setting(key, value):
    return {'key': key, 'value': value, 'type': 'addition'}


class ShapeSkSettingsTests(unittest.TestCase):
    def expand(self, root, settings=None, *, enable_sk=True, options=''):
        node = {'op_type': 'ReviewOp', 'bin_filename': 'kernel',
                'deterministic': 'false', 'inputs': [], 'outputs': [], 'attrs': []}
        if settings is not None:
            node['extra_settings'] = copy.deepcopy(settings)
        extent = root/'extent'
        extent.mkdir()
        compilation._extent_shape_json_content(str(extent), 'node.json', node,
            str(root/'source.json'), True, enable_sk, options)
        path = extent/'node_d0.json'
        return path, json.loads(path.read_text())

    def test_missing_sk_declaration_keeps_legacy_aclgraph_entry(self):
        for original in (None, [], [setting('_op_aicore_num', '16')]):
            with self.subTest(original=original), tempfile.TemporaryDirectory() as folder:
                _, node = self.expand(Path(folder), original, options='debug-sync-all=1')
                settings = node['extra_settings']
                self.assertTrue(any(item['key'] == 'super_kernel_sub_combine'
                                    and item['value'] is True for item in settings))
                self.assertTrue(any(item['key'] == 'super_kernel_sub_info'
                                    and item['value']['enable_super_kernel'] is True
                                    for item in settings))
                self.assertIsNone(backend._jit_skip_reason(node, 'opc', True))

    def test_explicit_mode_and_other_settings_are_not_overwritten(self):
        for mode in (True, False, None, 'true', 1):
            for location in ('start', 'middle', 'end'):
                settings = [setting('custom_setting', 'keep'),
                    setting('super_kernel_sub_combine', mode),
                    setting('super_kernel_sub_info', {'enable_super_kernel': True,
                        'super_kernel_sub_loc': location, 'super_kernel_options': 'split-mode=2'})]
                with self.subTest(mode=mode, location=location), tempfile.TemporaryDirectory() as folder:
                    _, node = self.expand(Path(folder), settings, options='debug-sync-all=1')
                    self.assertEqual(node['extra_settings'], settings)

    def test_explicit_info_without_combine_keeps_legacy_aclgraph_mode(self):
        settings = [setting('super_kernel_sub_info', {'enable_super_kernel': True,
            'super_kernel_sub_loc': 'middle', 'super_kernel_options': 'early-start=2'})]
        with tempfile.TemporaryDirectory() as folder:
            _, node = self.expand(Path(folder), settings)
        self.assertEqual(node['extra_settings'][:1], settings)
        self.assertTrue(any(item['key'] == 'super_kernel_sub_combine'
                            and item['value'] is True for item in node['extra_settings']))
        self.assertIsNone(backend._jit_skip_reason(node, 'opc', True))

    def test_explicit_combine_false_is_preserved_and_true_is_completed(self):
        for mode in (False, True, 'true'):
            settings = [setting('super_kernel_sub_combine', mode)]
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as folder:
                _, node = self.expand(Path(folder), settings)
            if mode is False:
                self.assertEqual(node['extra_settings'], settings)
                self.assertIsNotNone(backend._jit_skip_reason(node, 'opc', True))
            else:
                self.assertEqual(node['extra_settings'][:1], settings)
                self.assertTrue(any(item['key'] == 'super_kernel_sub_info'
                                    for item in node['extra_settings']))

    def test_explicit_disabled_sk_and_empty_options_are_preserved(self):
        settings = [setting('super_kernel_sub_combine', True),
            setting('super_kernel_sub_info', {'enable_super_kernel': False,
                'super_kernel_sub_loc': 'middle', 'super_kernel_options': ''})]
        with tempfile.TemporaryDirectory() as folder:
            _, node = self.expand(Path(folder), settings, options='early-start=1')
        self.assertEqual(node['extra_settings'], settings)
        self.assertIsNotNone(backend._jit_skip_reason(node, 'opc', True))

    def test_sk_disabled_does_not_add_or_change_settings(self):
        for settings in (None, [], [setting('custom_setting', 7)]):
            with self.subTest(settings=settings), tempfile.TemporaryDirectory() as folder:
                _, node = self.expand(Path(folder), settings, enable_sk=False)
            self.assertEqual(node.get('extra_settings'), settings)

    def test_duplicate_sk_declarations_are_not_collapsed(self):
        settings = [setting('super_kernel_sub_combine', False),
                    setting('super_kernel_sub_combine', True)]
        with tempfile.TemporaryDirectory() as folder:
            _, node = self.expand(Path(folder), settings)
        self.assertEqual(node['extra_settings'], settings)
        self.assertIn('ambiguous', backend._jit_skip_reason(node, 'opc', True))

    def test_expansion_opc_conversion_and_dispatch_preserve_the_mode(self):
        info = setting('super_kernel_sub_info', {'enable_super_kernel': True,
            'super_kernel_sub_loc': 'middle', 'super_kernel_options': ''})
        cases = [('legacy', None, True),
                 ('aclgraph', [info, setting('super_kernel_sub_combine', True)], True),
                 ('ge', [info, setting('super_kernel_sub_combine', False)], False),
                 ('implicit_aclgraph', [info], True)]
        for name, settings, attempted in cases:
            with self.subTest(case=name), tempfile.TemporaryDirectory() as folder:
                root = Path(folder)
                extent, expanded = self.expand(root, settings)
                output = root/'opcdump'
                output.mkdir()
                compilation._trans_dump_file_into_opc_format(
                    str(extent.parent), extent.name, str(output), 'Ascend910B4')
                opc_json = output/extent.name
                forwarded = json.loads(opc_json.read_text())['op_list'][0]
                self.assertEqual(forwarded.get('extra_settings'), expanded.get('extra_settings'))

                def opc_fallback(path, **kwargs):
                    forwarded_shape = json.loads(Path(path).read_text())['op_list'][0]
                    self.assertEqual(forwarded_shape.get('extra_settings'),
                                     expanded.get('extra_settings'))
                    stage = Path(kwargs['compile_res_dir'])
                    (stage/'kernel_d0.o').write_bytes(b'OPC')
                    (stage/'kernel_d0.json').write_text('{}')
                    return 0

                with patch.object(backend, 'preflight_shape_config'), \
                     patch.object(backend, '_run_worker', return_value=1) as worker, \
                     patch.object(compilation, '_opc_compile_single', side_effect=opc_fallback) as opc:
                    self.assertEqual(backend.compile_static_kernel_job(str(opc_json),
                        str(root/'result'), str(root/'meta'), 'Ascend910B4', 'info',
                        str(root/'logs'), True), 0)
                self.assertEqual(worker.call_count, int(attempted))
                opc.assert_called_once()
                report = json.loads((root/'logs/node_d0_backend_result.json').read_text())
                self.assertEqual(report['jit_attempted'], attempted)
                self.assertEqual(report['backend'], 'opc')
