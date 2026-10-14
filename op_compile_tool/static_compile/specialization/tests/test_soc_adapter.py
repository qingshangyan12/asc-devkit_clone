"""验证集中式芯片适配及编译器平台状态恢复。"""

import unittest
from unittest.mock import Mock, call, patch

from static_compile.specialization import soc_adapter


class PlatformInitializationTests(unittest.TestCase):
    def test_initializes_both_platforms_before_querying_short_soc(self):
        from tbe.common.platform import platform_info as tbe_platform
        from asc_op_compile_base.common.platform import platform_info as asc_platform

        calls = Mock()
        with (
            patch.object(tbe_platform, 'set_current_compile_soc_info') as initialize_tbe,
            patch.object(asc_platform, 'set_current_compile_soc_info') as initialize_asc,
            patch.object(asc_platform, 'get_soc_spec', return_value='Ascend910B') as get_spec,
        ):
            calls.attach_mock(initialize_tbe, 'initialize_tbe')
            calls.attach_mock(initialize_asc, 'initialize_asc')
            calls.attach_mock(get_spec, 'get_spec')
            self.assertEqual(soc_adapter.initialize_platform('Ascend910B4'), 'Ascend910B')
        self.assertEqual(
            calls.mock_calls,
            [
                call.initialize_tbe('Ascend910B4'),
                call.initialize_asc('Ascend910B4'),
                call.get_spec('SHORT_SOC_VERSION'),
            ],
        )


class ChipVersionTests(unittest.TestCase):
    def query(self, short_soc):
        return soc_adapter.get_chip_version(short_soc)

    def test_target_soc_overrides_stale_state_and_restores_it(self):
        from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage

        key = 'ascendc_short_soc_version'
        previous = storage.get_variable(key)
        try:
            storage.set_variable(key, 'Ascend310P')
            for short_soc, expected in (
                ('Ascend910B', 'c220'),
                ('Ascend950', 'c310'),
                ('MC62', '510r2'),
                ('Ascend910B', 'c220'),
            ):
                with self.subTest(short_soc=short_soc):
                    self.assertEqual(self.query(short_soc), expected)
                    self.assertEqual(storage.get_variable(key), 'Ascend310P')
        finally:
            storage.set_variable(key, previous)

    def test_compiler_exception_restores_original_value(self):
        from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage
        from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import CommonUtility

        key = 'ascendc_short_soc_version'
        previous = storage.get_variable(key)
        try:
            for value in (None, 'Ascend910B'):
                with self.subTest(previous=value):
                    storage.set_variable(key, value)

                    def fail():
                        self.assertEqual(storage.get_variable(key), 'Ascend950')
                        raise RuntimeError('architecture query failed')

                    with patch.object(CommonUtility, 'get_chip_version', side_effect=fail):
                        with self.assertRaisesRegex(RuntimeError, 'architecture query failed'):
                            self.query('Ascend950')
                    self.assertEqual(storage.get_variable(key), value)
        finally:
            storage.set_variable(key, previous)

    def test_missing_explicit_soc_is_rejected(self):
        for value in (None, '', False, 123):
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, 'short_soc'):
                self.query(value)
