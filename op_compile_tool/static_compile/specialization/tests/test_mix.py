"""逻辑 MIX 入口必须包含其比例声明的每个物理核。"""
import hashlib
import struct
from pathlib import Path
from types import SimpleNamespace as NS
import tempfile
import unittest
from unittest.mock import patch, Mock
from static_compile.specialization.argument_binding import resolve_kernel_metadata
from static_compile.specialization import static_kernel_metadata
from static_compile.specialization.static_kernel_metadata import build_static_kernel_metadata

PREFIX = 'static_compile.specialization.'

class MixTests(unittest.TestCase):
    def resolve(self, ratio='1:2', missing=None, bad_dfx=None):
        functions={'entry_mix_aic','entry_mix_aiv'}
        if ratio=='0:1': functions={'entry_mix_aiv'}
        if ratio=='1:0': functions={'entry_mix_aic'}
        if missing: functions.discard(missing)
        metadata={'coreType':'MIX','taskRation':ratio,'kernelName':'entry',
                  'tilingKey':7,'sha256':hashlib.sha256(b'fixture').hexdigest(),
                  'opParaSize':80,'parameters':[None],
                  'supportInfo':{'inputs':[],'outputs':[],'int64Mode':False}}
        def arguments(symbol):
            return [{'kind':0},{'kind':4},{'kind':7,'size':72 if symbol==bad_dfx else 80}]
        elf=NS(functions=functions,sections={},arguments=arguments)
        tiling=NS(kernel_name='entry',tiling_key=7,tiling_args_index=2,tiling_data=bytes(72),block_dim=20,
                  workspaces=[],local_memory_size=0,schedule_mode=0,clear_atomic=False)
        with tempfile.TemporaryDirectory() as temp:
            path=Path(temp)/'kernel.o';path.write_bytes(b'fixture')
            with patch(PREFIX+'argument_binding.ElfMetadata',return_value=elf):
                resolved=resolve_kernel_metadata(path,metadata,tiling)
        return resolved,tiling,elf

    def test_both_core_ratios_and_single_core_mix(self):
        for ratio,n in [('1:1',2),('1:2',2),('0:1',1),('1:0',1)]:
            with self.subTest(ratio=ratio):
                result,_,_=self.resolve(ratio)
                self.assertEqual(len(result.load_symbols),n)

    def test_missing_core_or_conflicting_dfx_rejected(self):
        with self.assertRaisesRegex(ValueError,'physical|MIX'):
            self.resolve(missing='entry_mix_aic')
        with self.assertRaisesRegex(ValueError,'tiling|allocation'):
            self.resolve(bad_dfx='entry_mix_aiv')
        with self.assertRaisesRegex(ValueError,'ratio'):
            self.resolve(ratio='2:3')

    def test_output_requires_both_cores_and_preserves_logical_entry(self):
        resolved,tiling,elf=self.resolve()
        with tempfile.TemporaryDirectory() as temp:
            path=Path(temp)/'out.o';path.write_bytes(b'fixture')
            with patch(PREFIX+'static_kernel_metadata.ElfMetadata',return_value=elf), patch(PREFIX+'static_kernel_metadata._static_key',return_value='key'):
                def build():
                    return build_static_kernel_metadata(resolved.metadata,{'inputs':[],'outputs':[]},tiling,path,'out','entry',NS(enable_super_kernel=False),resolved)
                result=build()
                self.assertEqual(result['kernelName'],'entry')
                self.assertEqual(result['taskRation'],'1:2')
                elf.functions.remove('entry_mix_aiv')
                with self.assertRaisesRegex(ValueError,'load symbol'):build()

    def test_sk_maps_both_architectures_and_checks_each_split(self):
        resolved,tiling,elf=self.resolve()
        for symbol in resolved.load_symbols:
            elf.functions.update({symbol+'_static_sk', *(symbol+'_static_sk_split'+str(i) for i in range(1,4))})
        elf.sections['.ascend.meta']=struct.pack('<HH',5,52)+bytes(52)
        elf.validate_sk_bind = Mock()
        with tempfile.TemporaryDirectory() as temp:
            path=Path(temp)/'out.o';path.write_bytes(b'fixture')
            with patch(PREFIX+'static_kernel_metadata.ElfMetadata',return_value=elf), patch(PREFIX+'static_kernel_metadata._static_key',return_value='key'):
                def build():
                    return build_static_kernel_metadata(resolved.metadata,{'inputs':[],'outputs':[]},tiling,path,'out','entry',NS(enable_super_kernel=True),resolved,short_soc='Ascend950')
                result=build()
                elf.validate_sk_bind.assert_called_once_with(resolved.load_symbols, 0)
                self.assertEqual(result['sub_operator_kernel_type'],'KERNEL_TYPE_MIX_AIC_1_2')
                self.assertEqual(set(result['sub_operator_kernel_name']),{'dav-c310-cube','dav-c310-vec'})
                elf.functions.remove('entry_mix_aic_static_sk_split2')
                with self.assertRaisesRegex(ValueError,'split functions'):build()


class ChipVersionTests(unittest.TestCase):
    def query(self, short_soc):
        helper = getattr(static_kernel_metadata, '_get_chip_version', None)
        self.assertTrue(callable(helper), 'missing explicit-SoC architecture adapter')
        return helper(short_soc)

    def test_target_soc_overrides_stale_state_and_restores_it(self):
        from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage
        key = 'ascendc_short_soc_version'
        previous = storage.get_variable(key)
        try:
            storage.set_variable(key, 'Ascend310P')
            for short_soc, expected in (('Ascend910B', 'c220'), ('Ascend950', 'c310'),
                                        ('MC62', '510r2'), ('Ascend910B', 'c220')):
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
