"""统一资源校验入口应同时约束 shape 上下文和 ELF/ABI。"""

import hashlib
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

from static_compile.specialization.argument_binding import resolve_kernel_metadata


class KernelValidationTests(unittest.TestCase):
    def setUp(self):
        folder = tempfile.TemporaryDirectory()
        self.addCleanup(folder.cleanup)
        self.path = Path(folder.name) / 'dynamic.o'
        self.path.write_bytes(b'ELF payload')
        selected = {'kernelName': 'entry'}
        metadata = {
            'op_type': 'Example',
            'kernelList': [selected],
            'sha256': hashlib.sha256(self.path.read_bytes()).hexdigest(),
            'coreType': 'VectorCore',
            'opParaSize': 12,
            'supportInfo': {
                'opMode': 'dynamic',
                'inputs': [],
                'outputs': [],
                'attrs': [],
                'deterministic': 'ignore',
                'dynamicParamMode': 'folded_with_desc',
            },
        }
        self.inputs = {
            'metadata': metadata,
            'shape_config': {'inputs': [], 'outputs': [], 'attrs': [], 'deterministic': 'false'},
            'op_type': 'Example',
            'dynamic_elf': str(self.path),
            'tiling': SimpleNamespace(kernel_name='entry', tiling_args_index=1, tiling_data=b'abcd'),
            'selected_kernel': selected,
            'parameter_mode': 'folded_with_desc',
            'selected_from_collection': True,
        }
        self.elf = SimpleNamespace(
            functions={'entry'},
            arguments=lambda _: ({'kind': 4}, {'kind': 7, 'size': 12}),
        )

    def test_valid_prepared_inputs_return_verified_layout_and_mode(self):
        with patch('static_compile.specialization.argument_binding.ElfMetadata', return_value=self.elf):
            resolved = resolve_kernel_metadata(self.inputs)
        self.assertEqual(resolved.load_symbols, ('entry',))
        self.assertEqual(resolved.tiling_index, 1)
        self.assertEqual(resolved.read_bytes, 4)
        self.assertEqual(resolved.parameter_mode, 'folded_with_desc')

    def test_resource_context_mismatch_is_rejected_before_elf_parsing(self):
        self.inputs['shape_config']['inputs'] = [{'shape': [1]}]
        with patch('static_compile.specialization.argument_binding.ElfMetadata') as parse_elf:
            with self.assertRaisesRegex(ValueError, 'descriptor count mismatch'):
                resolve_kernel_metadata(self.inputs)
        parse_elf.assert_not_called()

    def test_valid_context_still_rejects_elf_json_hash_mismatch(self):
        self.inputs['metadata']['sha256'] = 'wrong'
        with patch('static_compile.specialization.argument_binding.ElfMetadata') as parse_elf:
            with self.assertRaisesRegex(ValueError, 'ELF/JSON hash mismatch'):
                resolve_kernel_metadata(self.inputs)
        parse_elf.assert_not_called()
