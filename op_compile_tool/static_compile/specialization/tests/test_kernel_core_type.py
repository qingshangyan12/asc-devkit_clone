"""每个 key 的 kernel 类型会覆盖动态二进制的聚合 MIX 类型。"""

import copy
import hashlib
from pathlib import Path
import tempfile
from types import SimpleNamespace as NS
import unittest
from unittest.mock import Mock, patch

from static_compile.specialization.argument_binding import resolve_kernel_metadata, resolve_kernel_core_type
from static_compile.specialization.static_kernel_metadata import build_static_kernel_metadata

PREFIX = 'static_compile.specialization.'


class KernelCoreTypeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        from asc_op_compile_base.common.platform import platform_info

        platform_info.set_current_compile_soc_info('Ascend910B4')

    def resolve(self, selected, functions, *, intercore_sync=0, dfx=None):
        item = {'kernelName': 'Op_7', 'tilingKey': 7, **selected}
        metadata = {
            'coreType': 'MIX',
            'core_type': '',
            'intercoreSync': intercore_sync,
            'magic': 'RT_DEV_BINARY_MAGIC_ELF',
            'taskRation': 'tilingKey',
            'kernelName': 'Op',
            'kernelList': [item],
            'parameters': [None, None],
            'opParaSize': 80,
            'sha256': hashlib.sha256(b'unit-elf').hexdigest(),
            'supportInfo': {'inputs': [], 'outputs': [], 'int64Mode': False},
        }
        original = copy.deepcopy(metadata)
        tiling = NS(
            kernel_name='Op_7',
            tiling_key=7,
            tiling_args_index=2,
            tiling_data=bytes(72),
            block_dim=4,
            workspaces=[],
            local_memory_size=0,
            schedule_mode=0,
            clear_atomic=False,
        )
        elf = NS(
            functions=set(functions),
            validate_sk_bind=Mock(),
            arguments=lambda symbol: dfx if dfx is not None else ({'kind': 0}, {'kind': 4}, {'kind': 7, 'size': 80}),
        )
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / 'dynamic.o'
            path.write_bytes(b'unit-elf')
            with patch(PREFIX + 'argument_binding.ElfMetadata', return_value=elf):
                resolved = resolve_kernel_metadata(path, metadata, tiling)
        self.assertEqual(metadata, original, 'resolving a key must not mutate the source JSON')
        return metadata, resolved, tiling, elf

    def test_only_core_keys_in_mix_container_use_unsuffixed_entry(self):
        for ratio, core, marker in [('0:1', 'VectorCore', 'AIV'), ('1:0', 'AiCore', 'AIC')]:
            with self.subTest(ratio=ratio):
                _, resolved, _, _ = self.resolve(
                    {'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': ratio}, {'Op_7'}
                )
                self.assertEqual(resolved.load_symbols, ('Op_7',))
                self.assertEqual(resolved.metadata['coreType'], core)
                self.assertEqual(resolved.metadata['core_type'], marker)
                self.assertEqual(
                    resolved.metadata['magic'],
                    'RT_DEV_BINARY_MAGIC_ELF_' + marker + ('EC' if marker == 'AIV' else 'UBE'),
                )
                self.assertEqual(resolved.metadata['intercoreSync'], 0)
                self.assertNotIn('taskRation', resolved.metadata)

    def test_explicit_pure_core_type_overrides_container(self):
        for core, ratio in [('VectorCore', '0:1'), ('AiCore', '1:0')]:
            with self.subTest(core=core):
                _, resolved, _, _ = self.resolve({'kernelType': core, 'taskRation': ratio}, {'Op_7'})
                self.assertEqual(resolved.metadata['coreType'], core)
                self.assertEqual(resolved.load_symbols, ('Op_7',))

    def test_explicit_pure_core_does_not_inherit_container_ratio_placeholder(self):
        for core in ('VectorCore', 'AiCore'):
            with self.subTest(core=core):
                _, resolved, _, _ = self.resolve({'kernelType': core}, {'Op_7'})
                self.assertEqual(resolved.metadata['coreType'], core)
                self.assertNotIn('taskRation', resolved.metadata)

    def test_pure_key_retaining_dynamic_ffts_abi_requires_opc(self):
        for sync in (1, None):
            with self.subTest(intercore_sync=sync), self.assertRaisesRegex(ValueError, 'FFTS.*ABI.*OPC'):
                self.resolve(
                    {'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': '0:1'}, {'Op_7'}, intercore_sync=sync
                )

    def test_selected_sync_cannot_hide_container_ffts_abi(self):
        with self.assertRaisesRegex(ValueError, 'FFTS.*ABI.*OPC'):
            self.resolve({'kernelType': 'VectorCore', 'intercoreSync': 0}, {'Op_7'}, intercore_sync=1)

    def test_pure_key_dfx_cannot_retain_ffts_argument(self):
        with self.assertRaisesRegex(ValueError, 'DFX.*FFTS'):
            self.resolve(
                {'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': '0:1'},
                {'Op_7'},
                dfx=({'kind': 11}, {'kind': 0}, {'kind': 7, 'size': 80}),
            )

    def test_true_mix_keys_keep_each_required_physical_entry(self):
        for ratio, cores in [('0:1', ('aiv',)), ('1:0', ('aic',)), ('1:1', ('aic', 'aiv')), ('1:2', ('aic', 'aiv'))]:
            functions = {'Op_7_mix_' + core for core in cores}
            with self.subTest(ratio=ratio):
                _, resolved, _, _ = self.resolve(
                    {'kernelType': 'MIX_AIC', 'crossCoreSync': 1, 'taskRation': ratio}, functions
                )
                self.assertEqual(resolved.metadata['coreType'], 'MIX')
                self.assertEqual(set(resolved.load_symbols), functions)
                self.assertEqual(resolved.metadata['intercoreSync'], 1)

    def test_legacy_key_without_kernel_type_keeps_existing_mix_semantics(self):
        _, resolved, _, _ = self.resolve({'taskRation': '0:1'}, {'Op_7_mix_aiv'})
        self.assertEqual(resolved.metadata['coreType'], 'MIX')
        self.assertEqual(resolved.load_symbols, ('Op_7_mix_aiv',))

    def test_legacy_pure_resource_preserves_original_metadata(self):
        metadata = {
            'coreType': 'AiCore',
            'core_type': 'AIC',
            'intercoreSync': 0,
            'magic': 'RT_DEV_BINARY_MAGIC_ELF',
            'kernelName': 'Op',
        }
        selected = {'kernelName': 'Op_7'}
        self.assertEqual(resolve_kernel_core_type(metadata, selected), {**metadata, **selected})

    def test_mix_key_cannot_inherit_a_pure_container_runtime_description(self):
        with self.assertRaisesRegex(ValueError, 'MIX.*container'):
            resolve_kernel_core_type(
                {'coreType': 'AiCore', 'core_type': 'AIC', 'magic': 'RT_DEV_BINARY_MAGIC_ELF_AICUBE'},
                {'kernelType': 'MIX_AIC', 'crossCoreSync': 1, 'taskRation': '1:2'},
            )

    def test_per_key_core_and_physical_symbols_must_agree(self):
        cases = [
            ({'kernelType': 'MIX_AIC', 'crossCoreSync': 1, 'taskRation': '0:1'}, {'Op_7'}),
            ({'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': '0:1'}, {'Op_7_mix_aiv'}),
            ({'kernelType': 'VectorCore'}, {'Op_7', 'Op_7_mix_aiv'}),
        ]
        for selected, symbols in cases:
            with self.subTest(selected=selected, symbols=symbols), self.assertRaises(ValueError):
                self.resolve(selected, symbols)

    def test_inconsistent_or_unknown_key_types_fail_closed(self):
        cases = [
            {'kernelType': 'unknown'},
            {'kernelType': 'MIX_AICORE', 'taskRation': '1:1'},
            {'kernelType': 'VectorCore', 'taskRation': '1:2'},
            {'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': '1:1'},
            {'kernelType': 'MIX_AIC', 'crossCoreSync': 2, 'taskRation': '0:1'},
            {'kernelType': 'MIX_AIC', 'crossCoreSync': False, 'taskRation': '0:1'},
            {'kernelType': 'VectorCore', 'crossCoreSync': 1, 'taskRation': '0:1'},
            {'kernelType': 'MIX_AIC', 'crossCoreSync': '0', 'taskRation': '0:1'},
            {'kernelType': 'VectorCore', 'coreType': 'AiCore', 'taskRation': '0:1'},
        ]
        for selected in cases:
            with self.subTest(selected=selected), self.assertRaises(ValueError):
                self.resolve(selected, {'Op_7'})

    def test_pure_static_json_and_sk_metadata_use_selected_core(self):
        for sk in (False, True):
            for ratio, core, kind in [('0:1', 'VectorCore', 'AIV'), ('1:0', 'AiCore', 'AIC')]:
                with self.subTest(sk=sk, core=core):
                    metadata, resolved, tiling, elf = self.resolve(
                        {'kernelType': 'MIX_AIC', 'crossCoreSync': 0, 'taskRation': ratio}, {'Op_7'}
                    )
                    elf.functions.update({'Op_7_static_sk', *(f'Op_7_static_sk_split{i}' for i in range(1, 4))})
                    with tempfile.TemporaryDirectory() as folder:
                        path = Path(folder) / 'static.o'
                        path.write_bytes(b'unit-static-elf')
                        with (
                            patch(PREFIX + 'static_kernel_metadata.ElfMetadata', return_value=elf),
                            patch(PREFIX + 'static_kernel_metadata._static_key', return_value='key'),
                        ):
                            result = build_static_kernel_metadata(
                                metadata,
                                {'deterministic': 'false', 'inputs': [{'name': 'x'}], 'outputs': []},
                                tiling,
                                path,
                                'static',
                                'Op_7',
                                NS(enable_super_kernel=sk),
                                resolved,
                                'Op',
                            )
                    self.assertEqual(result['coreType'], core)
                    self.assertEqual(result['kernelName'], 'Op_7')
                    self.assertEqual(result['intercoreSync'], 0)
                    self.assertNotIn('taskRation', result)
                    if sk:
                        self.assertEqual(result['sub_operator_kernel_type'], f'KERNEL_TYPE_{kind}_ONLY')
                        elf.validate_sk_bind.assert_called_once_with(('Op_7',), 0)
