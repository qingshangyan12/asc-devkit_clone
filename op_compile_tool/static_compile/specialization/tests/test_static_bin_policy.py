"""静态采集记录跳过策略及显式 OPC 模式。"""

import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from op_compiler_parse_args import OptionParser
from static_compile import static_compilation as compilation
from static_compile import static_compile_backend as backend


class StaticBinPolicyTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / '123_opcompile'
        self.source.mkdir()

    def write_node(self, name, bin_type):
        node = {
            'op_type': 'ReviewOp', 'inputs': [], 'outputs': [], 'attrs': [],
            'deterministic': 'false', 'bin_type': bin_type,
        }
        path = self.source / (name + '.json')
        path.write_text(json.dumps(node))
        return path

    def test_cli_default_and_explicit_values(self):
        parser = OptionParser()
        parser.define_options()
        self.assertEqual(parser.full_options['--skip-static-bin-type'].get_value(), 'true')
        for value in ('false', 'true'):
            self.assertTrue(parser.parse_other_options([('--skip-static-bin-type', value)]))
            self.assertEqual(parser.full_options['--skip-static-bin-type'].get_value(), value)
        self.assertFalse(parser.parse_other_options([('--skip-static-bin-type', 'yes')]))

    def test_default_filters_only_static_nodes_before_expansion(self):
        self.write_node('static', 0)
        dynamic = self.write_node('dynamic', 1)
        legacy = self.write_node('legacy', None)
        extent = self.root / 'extent'
        self.assertTrue(compilation._extent_shape_json(str(self.source), str(extent), 'true', False, None))
        origins = {json.loads(path.read_text())['origin_path'] for path in extent.glob('*.json')}
        self.assertEqual(origins, {str(dynamic), str(legacy)})

    def test_false_keeps_both_binary_types(self):
        static = self.write_node('static', 0)
        dynamic = self.write_node('dynamic', 1)
        extent = self.root / 'extent'
        self.assertTrue(compilation._extent_shape_json(
            str(self.source), str(extent), 'true', False, None, skip_static_bin_type=False
        ))
        origins = {json.loads(path.read_text())['origin_path'] for path in extent.glob('*.json')}
        self.assertEqual(origins, {str(static), str(dynamic)})

    def test_legacy_opcompile_reads_type_from_same_node_debug(self):
        path = self.write_node('static', 0)
        node = json.loads(path.read_text())
        debug = self.root / '123_debug'
        debug.mkdir()
        (debug / path.name).write_text(json.dumps(node))
        node.pop('bin_type')
        path.write_text(json.dumps(node))
        extent = self.root / 'extent'
        self.assertFalse(compilation._extent_shape_json(str(self.source), str(extent), 'true', False, None))
        self.assertFalse(list(extent.glob('*.json')))

    def test_all_static_nodes_finish_without_compilation_or_packaging(self):
        self.write_node('static', 0)
        parser = OptionParser()
        parser.define_options()
        values = {
            '--op_params_dir': str(self.source), '--soc_version': 'Ascend910B4',
            '--package_name': 'test.run', '--output_path': str(self.root),
            '--follow_op_params_jit': 'true',
        }
        for name, value in values.items():
            parser.full_options[name].set_value(value)
        with patch.object(compilation, '_compile_static_kernels') as compile_jobs:
            with patch.object(compilation, 'gen_binary_info_config') as generate_index:
                self.assertTrue(compilation.static_kernel_compile(parser))
        compile_jobs.assert_not_called()
        generate_index.assert_not_called()
        self.assertFalse(list(self.root.glob('*.run')))

    def test_false_selects_opc_without_loading_jit_inputs(self):
        from static_compile.static_compilation import _trans_dump_file_into_opc_format

        for bin_type in (0, 1):
            with self.subTest(bin_type=bin_type):
                run = self.root / str(bin_type)
                run.mkdir()
                node = self.write_node('node', bin_type)
                _trans_dump_file_into_opc_format(str(self.source), node.name, str(run))
                job = run / node.name

                def opc(opc_json, **kwargs):
                    target = Path(kwargs['compile_res_dir'])
                    (target / 'node.o').write_bytes(b'OPC')
                    (target / 'node.json').write_text('{}')
                    return 0

                with patch.object(backend, 'prepare_specialization_inputs') as prepare:
                    with patch.object(backend, '_run_worker') as worker:
                        with patch.object(compilation, '_opc_compile_single', side_effect=opc) as compile_opc:
                            result = backend.compile_static_kernel_job(
                                str(job), str(run / 'out'), str(run / 'meta'), 'Ascend910B4',
                                'debug', str(run / 'logs'), False, skip_static_bin_type=False
                            )
                self.assertEqual(result, 0)
                prepare.assert_not_called()
                worker.assert_not_called()
                compile_opc.assert_called_once()
                report = json.loads((run / 'logs/node_backend_result.json').read_text())
                self.assertEqual(report['backend'], 'opc')
                self.assertFalse(report['jit_attempted'])
                self.assertIn('--skip-static-bin-type=false', report['jit_skip_reason'])
