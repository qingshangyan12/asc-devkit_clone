"""Collection-to-backend contract: no separate run_info file is consulted."""
import copy
import json
from pathlib import Path
import tempfile
import unittest

from static_compile.specialization.shape_config_adapter import adapt_shape_config
from static_compile.specialization.static_op_tiling import RUN_INFO_FIELDS, OPTIONAL_RUN_INFO_FIELDS, get_static_op_tiling
from static_compile.specialization.errors import SpecializationError


class CollectedTilingTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.runtime = dict(kernel_name='kernel', tiling_args_index=4, tiling_data='00ab00',
                            tiling_key=1, block_dim=1, workspace_bytes='0 ', clear_atomic=False,
                            schedule_mode=0, local_memory_size=0, aicpu_block_dim=0, tiling_cond=-1)
        self.node = dict(self.runtime, op_type='CustomQuant', inputs=[], outputs=[], attrs=[],
                         deterministic='false', impl_mode='')
        self.paths = {}
        for kind in ('opcompile', 'opcompile_selected', 'debug'):
            folder = self.root / ('123_' + kind)
            folder.mkdir()
            self.paths[kind] = folder/'node.json'
            data = dict(self.node)
            if kind == 'debug':
                data['bin_info'] = str(self.root/'actual.o')
            self.write(kind, data)
        self.job = self.root/'job.json'
        self.job.write_text(json.dumps({'op_type':'CustomQuant','op_list':[
            dict(self.node, origin_path=str(self.paths['opcompile_selected']))]}))

    def write(self, kind, value):
        self.paths[kind].write_text(json.dumps(value))

    def adapt(self):
        return adapt_shape_config(self.job, 'Ascend910B4')

    def test_no_files_and_owned_mapping(self):
        value = self.adapt()
        self.assertEqual(value['run_info'], self.runtime)
        self.assertEqual(get_static_op_tiling(value['run_info']).tiling_data, b'\x00\xab\x00')
        self.assertFalse(list(self.root.rglob('run_info.json')))
        self.assertNotIn('inputs', value['run_info'])

    def test_missing_or_nested_fields_do_not_use_legacy_attachment(self):
        for field in RUN_INFO_FIELDS:
            with self.subTest(field=field):
                node = dict(self.node)
                node.pop(field)
                node['run_info'] = dict(self.runtime)
                old = self.root/'run_info.json'
                old.write_text(json.dumps(self.runtime))
                node['specialization'] = {'run_info':str(old)}
                self.write('opcompile_selected', node)
                with self.assertRaisesRegex(ValueError, 'top-level'):
                    self.adapt()

    def test_conflicts_in_either_collection_are_rejected(self):
        for kind in ('debug','opcompile'):
            original = json.loads(self.paths[kind].read_text())
            for value in ('00ac00', 'ffff'):
                with self.subTest(kind=kind, value=value):
                    self.write(kind, dict(original, tiling_data=value))
                    with self.assertRaisesRegex(ValueError, 'runtime fields differ'):
                        self.adapt()
            self.write(kind, original)

    def test_malformed_peer_and_expanded_shape_rejected(self):
        original = json.loads(self.paths['debug'].read_text())
        self.write('debug', dict(original, block_dim=True))
        with self.assertRaises(SpecializationError):
            self.adapt()
        self.write('debug', original)
        job = json.loads(self.job.read_text())
        job['op_list'][0]['attrs'] = [{'name':'changed','value':1}]
        self.job.write_text(json.dumps(job))
        with self.assertRaisesRegex(ValueError, 'context differs'):
            self.adapt()

    def test_duplicate_field_in_collection_is_rejected(self):
        path = self.paths['opcompile_selected']
        path.write_text(path.read_text()[:-1]+',"tiling_key":2}')
        with self.assertRaisesRegex(SpecializationError, 'duplicate'):
            self.adapt()

    def test_opc_conversion_drops_runtime_but_preserves_origin(self):
        from static_compile.static_compilation import _trans_dump_file_into_opc_format
        output = self.root/'opc';output.mkdir()
        folder = self.root/'extent';folder.mkdir()
        node = dict(self.node, origin_path=str(self.paths['opcompile_selected']))
        (folder/'node.json').write_text(json.dumps(node))
        _trans_dump_file_into_opc_format(str(folder), 'node.json', str(output), 'Ascend910B4')
        shape = json.loads((output/'node.json').read_text())['op_list'][0]
        self.assertEqual(shape['origin_path'], str(self.paths['opcompile_selected']))
        self.assertFalse(set((*RUN_INFO_FIELDS, *OPTIONAL_RUN_INFO_FIELDS)) & set(shape))
        self.assertEqual(adapt_shape_config(output/'node.json', 'Ascend910B4')['run_info'], self.runtime)

    def test_workspace_bytes_preserves_order_and_zeros(self):
        for text, expected in [('', []), ('0 ', [0]), ('1024 0 2048 ', [1024, 0, 2048]),
                               ('1024 2048', [1024, 2048]),
                               ('18446744073709551615 ', [2**64-1])]:
            with self.subTest(text=text):
                result = get_static_op_tiling(dict(self.runtime, workspace_bytes=text))
                self.assertEqual(result.workspaces, expected)
                self.assertEqual(result.raw_run_info['workspace_bytes'], text)

    def test_decimal_key_and_block_strings_are_parsed_without_mutating_source(self):
        for key, block in [('101031', '40'), ('0', '1'),
                           (str(2**64-1), str(2**32-1)), ('001', '040')]:
            with self.subTest(key=key, block=block):
                raw = dict(self.runtime, tiling_key=key, block_dim=block)
                before = copy.deepcopy(raw)
                result = get_static_op_tiling(raw)
                self.assertEqual(result.tiling_key, int(key))
                self.assertEqual(result.block_dim, int(block))
                self.assertEqual(raw, before)
                self.assertEqual(result.raw_run_info, before)

    def test_decimal_strings_flow_through_collected_node_adapter(self):
        for kind, path in self.paths.items():
            self.write(kind, dict(json.loads(path.read_text()), tiling_key='101031', block_dim='40'))
        result = get_static_op_tiling(self.adapt()['run_info'])
        self.assertEqual((result.tiling_key, result.block_dim), (101031, 40))

    def test_decimal_key_and_block_reject_invalid_values(self):
        for field, bits in [('tiling_key', 64), ('block_dim', 32)]:
            for value in ('', '-1', '+1', ' 1', '1 ', '1\n', '0x10', '1.0', '1e2',
                          '１２', str(2**bits), 2**bits, -1, True, False, 1.0, None):
                with self.subTest(field=field, value=value), self.assertRaises(SpecializationError):
                    get_static_op_tiling(dict(self.runtime, **{field: value}))
        with self.assertRaisesRegex(SpecializationError, 'positive'):
            get_static_op_tiling(dict(self.runtime, block_dim='0'))

    def test_workspace_bytes_rejects_invalid_sizes_and_types(self):
        for value in ([], [1], None, 12, True, '-1 ', '+1 ', '1.5 ', '0x10 ',
                      '18446744073709551616 ', '1,2', '1\t2', '1\n2', '１２ '):
            with self.subTest(value=value), self.assertRaises(SpecializationError):
                get_static_op_tiling(dict(self.runtime, workspace_bytes=value))

    def test_old_workspace_array_does_not_replace_required_string(self):
        node = dict(self.node)
        node.pop('workspace_bytes')
        node['workspaces'] = [0]
        self.write('opcompile_selected', node)
        with self.assertRaisesRegex(ValueError, 'workspace_bytes'):
            self.adapt()

    def test_conflicting_workspace_bytes_between_nodes_rejected(self):
        original = json.loads(self.paths['debug'].read_text())
        self.write('debug', dict(original, workspace_bytes='1024 '))
        with self.assertRaisesRegex(ValueError, 'runtime fields differ'):
            self.adapt()

    def test_selection_witness_requires_dynamic_peer_without_overrides(self):
        self.assertFalse(self.adapt()['selected_from_collection'])
        record = json.loads(self.paths['debug'].read_text())
        self.write('debug', dict(record, bin_type=1))
        self.assertTrue(self.adapt()['selected_from_collection'])
        node = dict(self.node, specialization={'dynamic_elf':str(self.root/'override.o')})
        self.write('opcompile_selected', node)
        with self.assertRaisesRegex(ValueError, 'override'):
            self.adapt()

    def test_sidecar_files_are_not_runtime_inputs(self):
        location=self.paths['opcompile_selected'].parent/'node.specialization'
        location.mkdir();(location/'shape_context.json').write_text('{broken')
        self.assertEqual(self.adapt()['dynamic_elf'],str(self.root/'actual.o'))

    def test_unused_fields_are_not_required_or_validated(self):
        for kind,path in self.paths.items():
            node=json.loads(path.read_text())
            node.pop('aicpu_block_dim');node.pop('tiling_cond')
            self.write(kind,node)
        self.assertEqual(get_static_op_tiling(self.adapt()['run_info']).tiling_data,b'\x00\xab\x00')
        self.write('debug',dict(json.loads(self.paths['debug'].read_text()),tiling_cond={'unused':True}))
        self.adapt()
