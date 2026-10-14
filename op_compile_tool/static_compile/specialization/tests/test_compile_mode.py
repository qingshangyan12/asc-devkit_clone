"""所有编译模式都进入统一的单 shape 后端分派器。"""

import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock, patch

from static_compile import static_compilation as compilation


class CompileModeTests(unittest.TestCase):
    def test_dispatch_passes_every_mode_to_common_backend(self):
        for mode in ('opc', 'aoe', 'tune'):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                params = SimpleNamespace(
                    shape_info=temp,
                    compile_res_dir=str(root / 'out'),
                    kernel_meta_dir=str(root / 'meta'),
                    compile_log_dir=str(root / 'logs'),
                    opc_dump_dir=str(root / 'dump'),
                    soc_version='Ascend910B4',
                    job=1,
                    log_level='info',
                    enable_sk=False,
                )
                opc = Mock(return_value=0)
                jit = Mock(return_value=0)
                pool = Mock()
                pool.imap_unordered.side_effect = lambda fn, jobs: map(fn, jobs)
                with (
                    patch.object(compilation, 'Pool') as factory,
                    patch.object(compilation, '_trans_dump_dir_into_opc_format', return_value=['input.json']),
                    patch.object(compilation, '_summary_from_compile_result_logs'),
                    patch.object(compilation, '_opc_compile_single', opc),
                    patch.object(compilation, 'compile_static_kernel_job', jit),
                ):
                    factory.return_value.__enter__.return_value = pool
                    self.assertTrue(compilation._compile_static_kernels(params, compile_mode=mode))
                jit.assert_called_once_with(
                    'input.json',
                    compile_res_dir=params.compile_res_dir,
                    kernel_meta_dir=params.kernel_meta_dir,
                    soc_version=params.soc_version,
                    log_level=params.log_level,
                    compile_log_dir=params.compile_log_dir,
                    enable_sk=False,
                    compile_mode=mode,
                )
                opc.assert_not_called()

    def test_deprecated_mode_warns_and_reaches_shape_expansion(self):
        class StopAfterValidation(Exception):
            pass

        for mode in ('aoe', 'tune'):
            with self.subTest(mode=mode):
                values = {
                    '--op_params_dir': '/data/hxj/shapes',
                    '--soc_version': 'Ascend910B4',
                    '--package_name': 'test.run',
                    '--compile_mode': mode,
                    '--output_path': '/data/hxj/output',
                    '--follow_op_params_jit': False,
                    '--job': 1,
                    '--log': 'info',
                    '--enable_super_kernel': False,
                    '--super_kernel_options': '',
                }
                parameters = SimpleNamespace(
                    full_options={key: Mock(get_value=Mock(return_value=value)) for key, value in values.items()}
                )
                with (
                    patch.object(compilation, '_extent_shape_json', side_effect=StopAfterValidation),
                    patch('builtins.print') as output,
                    patch.object(compilation.logger, 'warn') as warning,
                ):
                    with self.assertRaises(StopAfterValidation):
                        compilation.static_kernel_compile(parameters)
                self.assertIn('automatically switching to opc mode', output.call_args.args[0])
                self.assertIn('JIT will not be attempted', warning.call_args.args[0])
