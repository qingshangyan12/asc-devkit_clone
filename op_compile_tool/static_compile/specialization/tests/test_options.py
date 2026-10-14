"""验证已安装的 CANN 解析器和生成器，不使用模拟转换。"""

from types import SimpleNamespace
import unittest
from static_compile.specialization.specialization_options import build_specialization_options


class OptionsTests(unittest.TestCase):
    def build(self, text='', *, basic=(), metadata=None, sk=True):
        shape = {
            'bin_filename': 'static_kernel_DynamicQuant_test',
            'deterministic': 'false',
            'additional_compile_options': basic,
            'extra_settings': [
                {'key': 'super_kernel_sub_combine', 'value': True},
                {
                    'key': 'super_kernel_sub_info',
                    'value': {
                        'enable_super_kernel': True,
                        'super_kernel_options': text,
                        'super_kernel_sub_loc': 'middle',
                    },
                },
            ],
        }
        return build_specialization_options(
            shape,
            sk,
            SimpleNamespace(block_dim=13),
            resolved=SimpleNamespace(metadata=metadata or {}),
            op_type='DynamicQuant',
        )

    def test_dcci_regex_and_same_metadata_flags(self):
        result = self.build(
            'dcci_before_kernel_start=static_kernel_DynamicQuant_.*:'
            'dcci_disable_on_kernel=static_kernel_DynamicQuant_.*'
        )
        self.assertIn('-D__ASCENDC_SUPER_KERNEL_DISABLE_DCCI__', result.super_kernel)
        self.assertTrue(dict(result.metadata_flags)['sub_operator_call_dcci_before_kernel_start'])
        self.assertTrue(dict(result.metadata_flags)['sub_operator_call_dcci_disable_on_kernel'])
        self.assertEqual(result.super_kernel.count('--enable-super-kernel'), 1)

    def test_debug_macro_follows_current_configuration(self):
        on = self.build('debug_per_op_max_core_num=1')
        off = self.build('debug_per_op_max_core_num=0')
        self.assertIn('-D__ASCENDC_SUPER_KERNEL_DEBUG__', on.super_kernel)
        self.assertNotIn('-D__ASCENDC_SUPER_KERNEL_DEBUG__', off.super_kernel)
        self.assertFalse(any(option.startswith('-U') for option in on.super_kernel))
        self.assertFalse(any(option.startswith('-U') for option in off.super_kernel))
        self.assertIn('-D__SUPER_KERNEL_STATIC_BLOCK_NUM__=13', off.super_kernel)

    def test_aclgraph_ignored_options_follow_original_parser(self):
        self.assertEqual(self.build('func-align=1024').super_kernel, self.build().super_kernel)

    def test_early_start_uses_capabilities_and_debug_sync_disables(self):
        metadata = {'super_kernel_early_start_set_flag': True, 'super_kernel_early_start_wait_flag': False}
        on = self.build('early_start=1', metadata=metadata)
        off = self.build('early_start=1:debug_sync_all=1', metadata=metadata)
        self.assertIn('-D__ASCENDC_ENABLE_SET_NEXT_TASK_START', on.super_kernel)
        self.assertTrue(dict(on.metadata_flags)['sub_operator_early_start_set_flag'])
        self.assertFalse(dict(off.metadata_flags)['sub_operator_early_start_set_flag'])
        with self.assertRaisesRegex(ValueError, 'early.start.*capabil'):
            self.build('early_start=1')

    def test_additional_argv_scope(self):
        result = self.build(basic=['-O2', '-DUSER_OPTION=1'])
        self.assertEqual(result.basic, ('-O2', '-DUSER_OPTION=1'))
        self.assertNotIn('-DUSER_OPTION=1', result.super_kernel)
        self.assertEqual(self.build(basic=['-O2'], sk=False).super_kernel, ())

    def test_parser_errors_and_global_state_restoration(self):
        from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage

        key = 'ascendc_sub_super_kernel_call_dcci_before_kernel_start'
        previous = storage.get_variable(key)
        storage.set_variable(key, True)
        try:
            self.assertFalse(dict(self.build().metadata_flags)['sub_operator_call_dcci_before_kernel_start'])
            with self.assertRaises(Exception):
                self.build('early_start=2')
            with self.assertRaises(Exception):
                self.build('early_start=0:early-start=1')
            with self.assertRaises(Exception):
                self.build('dcci_before_kernel_start=[')
            self.assertTrue(storage.get_variable(key))
        finally:
            storage.set_variable(key, previous)


if __name__ == '__main__':
    unittest.main()
