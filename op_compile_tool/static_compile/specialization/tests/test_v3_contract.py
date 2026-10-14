import ctypes
import hashlib
import json
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest

from static_compile.specialization.argument_binding import bind_arguments, resolve_kernel_metadata
from static_compile.specialization.shape_config_adapter import adapt_shape_config
from static_compile.specialization.static_op_tiling import get_static_op_tiling
from static_compile.specialization.errors import SpecializationError
from static_compile.specialization.specialization_request import OwnedSpecializationRequest, SpecializationInput


class V3ContractTests(unittest.TestCase):
    def test_index_required_uint64_and_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            raw = dict(
                tiling_data='abcd',
                tiling_key=1,
                block_dim=1,
                workspace_bytes='',
                clear_atomic=False,
                schedule_mode=0,
                local_memory_size=0,
                aicpu_block_dim=0,
                tiling_cond=0,
                kernel_name='kernel',
            )
            for index in ('missing', True, -1, 2**64, 0, 5):
                value = dict(raw)
                if index != 'missing':
                    value['tiling_args_index'] = index
                (root / 'run.json').write_text(json.dumps(value))
                with self.subTest(index=index):
                    valid_integer = isinstance(index, int) and not isinstance(index, bool)
                    if valid_integer and 0 <= index < 2**64:
                        result = get_static_op_tiling(value)
                        self.assertEqual(result.tiling_args_index, index)
                    else:
                        with self.assertRaises(SpecializationError):
                            get_static_op_tiling(value)

    def test_request_array_uses_upstream_slot_not_complete_parameter_count(self):
        for index in (0, 4):
            resolved = SimpleNamespace(tiling_index=index, read_bytes=4)
            tiling = SimpleNamespace(tiling_data=b'abcd', tiling_args_index=index)
            arguments = bind_arguments(resolved, tiling)
            owner = OwnedSpecializationRequest(
                SpecializationInput(
                    resource_id='test',
                    kernel_entry='kernel',
                    arguments=arguments,
                    basic_options=[],
                    sk_options=[],
                    output_path='/data/hxj/test.o',
                )
            )
            request = owner.request
            self.assertEqual(request.argsCount, index + 1)
            self.assertEqual(list(request.argsBytes[: index + 1]), [0] * index + [4])
            self.assertTrue(all(request.argsAddr[i] is None for i in range(index)))
            self.assertEqual(ctypes.string_at(request.argsAddr[index], 4), b'abcd')

    def test_binding_rejects_mismatch_and_unallocatable_index(self):
        for index, expected in ((3, 4), (2**64 - 1, 2**64 - 1), (True, True), (-1, -1)):
            with self.subTest(index=index), self.assertRaises(ValueError):
                bind_arguments(
                    SimpleNamespace(tiling_index=expected, read_bytes=4),
                    SimpleNamespace(tiling_data=b'abcd', tiling_args_index=index),
                )

    def test_selected_kernel_cannot_override_unsupported_abi_checks(self):
        with tempfile.TemporaryDirectory() as directory:
            elf = Path(directory) / 'input.o'
            elf.write_bytes(b'not used by ABI rejection')
            for field in ('oom', 'dynamicParamMode', 'debugOptions'):
                metadata = {
                    'sha256': hashlib.sha256(elf.read_bytes()).hexdigest(),
                    'kernelList': [{'kernelName': 'kernel', 'tilingKey': 1, field: True}],
                }
                message = 'dynamic parameter mode' if field == 'dynamicParamMode' else 'unsupported OOM'
                with self.subTest(field=field), self.assertRaisesRegex(ValueError, message):
                    resolve_kernel_metadata(elf, metadata, SimpleNamespace(kernel_name='kernel', tiling_key=1))

    def test_missing_explicit_context_and_unproven_soc_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            node = {'bin_info': 'dynamic.o'}
            shape = {'origin_path': str(root / 'node.json')}
            (root / 'input.json').write_text(json.dumps({'op_type': 'DynamicQuant', 'op_list': [shape]}))
            for attachment in ({'context': 'missing.json'}, {}):
                node['specialization'] = attachment
                (root / 'node.json').write_text(json.dumps(node))
                with self.subTest(attachment=attachment), self.assertRaises(ValueError):
                    adapt_shape_config(root / 'input.json', 'Ascend910B4')
