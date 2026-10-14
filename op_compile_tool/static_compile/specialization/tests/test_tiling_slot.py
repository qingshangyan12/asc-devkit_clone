"""采集到的 launch 偏移是权威值；若存在 DFX，必须与其交叉印证。"""
import hashlib
from pathlib import Path
import tempfile
from types import SimpleNamespace as NS
import unittest
from unittest.mock import patch
from static_compile.specialization.argument_binding import resolve_kernel_metadata, bind_arguments
from static_compile.specialization.elf_metadata import ElfMetadata, ElfMetadataError


class TilingSlotTests(unittest.TestCase):
    def resolve(self, index=6, dfx=None, malformed=False, mix=False):
        symbols = ('entry_mix_aic', 'entry_mix_aiv') if mix else ('entry',)
        elf = object.__new__(ElfMetadata)
        elf.functions = set(symbols)
        elf.sections = {'.ascend.meta.' + s: b'\x04' for s in symbols} if malformed else {}
        elf.endian = '<'
        if dfx is not None:
            elf.arguments = lambda s: dfx[s] if isinstance(dfx, dict) else dfx
        metadata = dict(kernelName='entry', tilingKey=1, opParaSize=12,
                        sha256=hashlib.sha256(b'ELF').hexdigest(),
                        coreType='MIX' if mix else 'VectorCore', taskRation='1:2',
                        supportInfo=dict(inputs=[], outputs=[]))
        tiling = NS(kernel_name='entry', tiling_key=1, tiling_args_index=index, tiling_data=b'abcd')
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp)/'kernel.o'; path.write_bytes(b'ELF')
            with patch('static_compile.specialization.argument_binding.ElfMetadata', return_value=elf):
                resolved = resolve_kernel_metadata(path, metadata, tiling)
        return bind_arguments(resolved, tiling)

    def test_missing_dfx_trusts_launch_slot_without_descriptor_count(self):
        args = self.resolve(index=6)
        self.assertEqual(len(args), 7)
        # 不需要从输入 descriptor 重建 MC2/FFTS/shape 槽位。

    def test_present_dfx_checks_exact_tiling_position(self):
        dfx = [{'kind': 0}] * 6 + [{'kind': 7, 'size': 12}]
        self.assertEqual(len(self.resolve(dfx=dfx)), 7)
        with self.assertRaisesRegex(ValueError, 'tiling.*index|DFX'):
            self.resolve(index=5, dfx=dfx)
        for invalid in ([], [{'kind': 0}], dfx + [{'kind': 7, 'size': 12}]):
            with self.subTest(dfx=invalid), self.assertRaises(ValueError):
                self.resolve(dfx=invalid)

    def test_malformed_dfx_never_treated_as_absent(self):
        with self.assertRaises(ElfMetadataError):
            self.resolve(malformed=True)

    def test_each_mix_entity_must_match(self):
        good = [{'kind': 0}] * 6 + [{'kind': 7, 'size': 12}]
        with self.assertRaises(ValueError):
            self.resolve(mix=True, dfx={'entry_mix_aic': good, 'entry_mix_aiv': good[1:]})
        self.assertEqual(len(self.resolve(mix=True)), 7)

    def test_invalid_launch_indices_rejected_without_dfx(self):
        for index in (-1, 4096, True, '6'):
            with self.subTest(index=index), self.assertRaises(ValueError):
                self.resolve(index=index)
