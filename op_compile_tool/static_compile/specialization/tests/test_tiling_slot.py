"""采集的 tiling 槽位和数据是权威值，不再依赖 DFX 交叉校验。"""

import hashlib
from pathlib import Path
import tempfile
from types import SimpleNamespace as NS
import unittest
from unittest.mock import Mock, patch
from static_compile.specialization.argument_binding import resolve_kernel_metadata, bind_arguments, resolve_entry
from static_compile.specialization.elf_metadata import ElfMetadata, ElfMetadataError


class TilingSlotTests(unittest.TestCase):
    def resolve(self, index=6, dfx=None, malformed=False, mix=False):
        symbols = ('entry_mix_aic', 'entry_mix_aiv') if mix else ('entry',)
        elf = object.__new__(ElfMetadata)
        elf.functions = set(symbols)
        elf.sections = {'.ascend.meta.' + s: b'\x04' for s in symbols} if malformed else {}
        elf.endian = '<'
        if malformed:
            elf.arguments = Mock(side_effect=ElfMetadataError('malformed DFX'))
        else:
            elf.arguments = Mock(return_value=dfx)
        metadata = dict(
            kernelName='entry',
            tilingKey=1,
            opParaSize=12,
            sha256=hashlib.sha256(b'ELF').hexdigest(),
            coreType='MIX' if mix else 'VectorCore',
            taskRation='1:2',
            supportInfo=dict(inputs=[], outputs=[]),
        )
        tiling = NS(kernel_name='entry', tiling_key=1, tiling_args_index=index, tiling_data=b'abcd')
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / 'kernel.o'
            path.write_bytes(b'ELF')
            # 本组验证 dump tiling 的权威性；真实 schema 校验由 test_kernel_validation 覆盖。
            with (
                patch('static_compile.specialization.argument_binding.ElfMetadata', return_value=elf),
                patch('static_compile.specialization.argument_binding.validate_resource_context'),
            ):
                selected = resolve_entry(metadata, tiling.kernel_name)
                inputs = dict(
                    dynamic_elf=path, metadata=metadata, tiling=tiling, selected_kernel=selected,
                    parameter_mode=None, shape_config={}, op_type='Example', selected_from_collection=False,
                )
                resolved = resolve_kernel_metadata(inputs)
                elf.arguments.assert_not_called()
        return bind_arguments(resolved, tiling)

    def test_missing_dfx_trusts_launch_slot_without_descriptor_count(self):
        args = self.resolve(index=6)
        self.assertEqual(len(args), 7)
        # 不需要从输入 descriptor 重建 MC2/FFTS/shape 槽位。

    def test_conflicting_dfx_does_not_override_launch_slot(self):
        dfx = [{'kind': 0}] * 6 + [{'kind': 7, 'size': 12}]
        for index in (0, 5, 6):
            with self.subTest(index=index):
                args = self.resolve(index=index, dfx=dfx)
                self.assertEqual(len(args), index + 1)
                self.assertEqual(args[index].constant_data, b'abcd')
        for invalid in ([], [{'kind': 0}], dfx + [{'kind': 7, 'size': 12}]):
            with self.subTest(dfx=invalid):
                self.assertEqual(len(self.resolve(dfx=invalid)), 7)

    def test_malformed_dfx_does_not_block_collected_tiling(self):
        self.assertEqual(len(self.resolve(malformed=True)), 7)

    def test_mix_entity_dfx_differences_do_not_override_dump(self):
        good = [{'kind': 0}] * 6 + [{'kind': 7, 'size': 12}]
        args = self.resolve(mix=True, dfx={'entry_mix_aic': good, 'entry_mix_aiv': good[1:]})
        self.assertEqual(len(args), 7)
        self.assertEqual(args[6].constant_data, b'abcd')

    def test_invalid_launch_indices_rejected_without_dfx(self):
        for index in (-1, 4096, True, '6'):
            with self.subTest(index=index), self.assertRaises(ValueError):
                self.resolve(index=index)
