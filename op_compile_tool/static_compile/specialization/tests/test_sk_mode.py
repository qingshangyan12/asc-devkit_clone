"""公共分派和特化 worker 共同遵循 ACLGraph SK 契约。"""
import unittest
from types import SimpleNamespace
from unittest.mock import patch

from static_compile.specialization import specialization_options as options


class SkModeTests(unittest.TestCase):
    def shape(self, combine=True, info=None):
        return {'deterministic': 'false', 'extra_settings': [
            {'key': 'super_kernel_sub_combine', 'value': combine, 'type': 'addition'},
            {'key': 'super_kernel_sub_info', 'type': 'addition', 'value':
             info if info is not None else {'enable_super_kernel': True,
                                            'super_kernel_sub_loc': 'middle'}}]}

    def test_only_explicit_boolean_combine_selects_aclgraph(self):
        self.assertTrue(hasattr(options, 'is_aclgraph_sk'))
        self.assertTrue(options.is_aclgraph_sk(self.shape()))
        for combine in (False, None, 'true', 1):
            with self.subTest(combine=combine):
                self.assertFalse(options.is_aclgraph_sk(self.shape(combine)))
        self.assertFalse(options.is_aclgraph_sk({}))

    def test_combine_without_explicit_sub_info_does_not_select_aclgraph(self):
        shape = {'deterministic': 'false', 'extra_settings': [
            {'key': 'super_kernel_sub_combine', 'value': True, 'type': 'addition'}
        ]}
        self.assertFalse(options.is_aclgraph_sk(shape))

    def test_aclgraph_requires_explicit_enable_and_location(self):
        for info in ({}, {'enable_super_kernel': True},
                     {'super_kernel_sub_loc': 'middle'},
                     {'enable_super_kernel': True, 'super_kernel_sub_loc': 'middle',
                      'super_kernel_options': ''}):
            with self.subTest(info=info):
                shape = self.shape(info=info)
                self.assertEqual(
                    options.is_aclgraph_sk(shape),
                    info.get('enable_super_kernel') is True
                    and info.get('super_kernel_sub_loc') == 'middle'
                )

    def test_worker_rejects_duplicate_or_non_addition_settings(self):
        duplicate = self.shape()
        duplicate['extra_settings'].append(
            {'key': 'super_kernel_sub_combine', 'value': False})
        wrong_type = self.shape()
        wrong_type['extra_settings'][0]['type'] = 'compile'
        for shape in (duplicate, wrong_type):
            with self.subTest(shape=shape), patch.object(options, '_generate_sk_options', return_value=((), ())):
                with self.assertRaises(ValueError):
                    options.build_specialization_options(shape, True, SimpleNamespace(block_dim=1))

    def test_worker_rejects_conflicting_enable_and_location(self):
        for info in ({'enable_super_kernel': False},
                     {'enable_super_kernel': 'true'},
                     {'super_kernel_sub_loc': 'start'}):
            with self.subTest(info=info), patch.object(options, '_generate_sk_options', return_value=((), ())):
                with self.assertRaises(ValueError):
                    options.build_specialization_options(self.shape(info=info), True,
                                                         SimpleNamespace(block_dim=1))

    def test_unknown_aclgraph_options_still_reach_original_parser(self):
        shape = self.shape()
        shape['extra_settings'][1]['value']['super_kernel_options'] = 'func-align=512'
        with patch.object(options, '_generate_sk_options', return_value=((), ())) as generator:
            options.build_specialization_options(shape, True, SimpleNamespace(block_dim=1))
        generator.assert_called_once()


if __name__ == '__main__':
    unittest.main()
