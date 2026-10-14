import json
from pathlib import Path
import struct
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch
from static_compile.specialization.static_kernel_metadata import build_static_kernel_metadata


class MetadataTests(unittest.TestCase):
    def build(self, *, sk=True, bind=True, init=None, int64=False, flags=(), missing_function=None):
        dynamic={'coreType':'VectorCore','parameters':[init,None], 'opParaSize':248,
                 'supportInfo':{'int64Mode':int64}}
        resolved=SimpleNamespace(load_symbol='entry',metadata=dynamic,read_bytes=240,
                                 argument_names=('x','workspace','tiling'))
        tiling=SimpleNamespace(block_dim=40,workspaces=[16777216],local_memory_size=0,
                               schedule_mode=0,aicpu_block_dim=0,tiling_cond=0,clear_atomic=True)
        functions={'entry','entry_static_sk',*(f'entry_static_sk_split{i}' for i in range(1,4))}
        if missing_function:
            functions.remove(missing_function)
        elf=SimpleNamespace(functions=functions,sections={'.ascend.meta':struct.pack('<HH',5,52)+bytes(52)} if bind else {})
        options=SimpleNamespace(enable_super_kernel=sk,metadata_flags=flags)
        with tempfile.TemporaryDirectory() as temp:
            path=Path(temp)/'out.o';path.write_bytes(b'ELF')
            with patch('static_compile.specialization.static_kernel_metadata.ElfMetadata',return_value=elf), patch('static_compile.specialization.static_kernel_metadata._static_key',return_value='static-key'):
                return build_static_kernel_metadata(dynamic,{'deterministic':'false'},tiling,str(path),'out','entry',3,options,resolved,'DynamicQuant')

    def test_real_capability_required_for_sk(self):
        with self.assertRaisesRegex(ValueError,'SK_BIND'):
            self.build(bind=False)
        result=self.build()
        self.assertEqual(result['supportSuperKernel'],1)
        self.assertEqual(result['split_mode'],4)
        self.assertEqual(result['opParaSize'],248)
        self.assertEqual(result['parameters'],[None,None])
        self.assertNotEqual(len(result['parameters']),len(result['sub_operator_params']))

    def test_split_mode_requires_all_four_sk_functions(self):
        for name in ('entry_static_sk', *(f'entry_static_sk_split{i}' for i in range(1,4))):
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, 'split functions'):
                self.build(missing_function=name)

    def test_basic_does_not_advertise_split_mode(self):
        result = self.build(sk=False, bind=False)
        self.assertEqual(result['supportSuperKernel'], 0)
        self.assertNotIn('split_mode', result)

    def test_option_generator_flags_reach_final_json(self):
        result=self.build(flags=(('sub_operator_call_dcci_before_kernel_start',True),
                                 ('sub_operator_early_start_wait_flag',True)))
        self.assertTrue(result['sub_operator_call_dcci_before_kernel_start'])
        self.assertTrue(result['sub_operator_early_start_wait_flag'])
        self.assertFalse(result['sub_operator_call_dcci_after_kernel_end'])

    def test_unsupported_initializers_and_int64_fail(self):
        with self.assertRaisesRegex(ValueError,'initialization'):
            self.build(init={'init_value':0})
        with self.assertRaisesRegex(ValueError,'int64'):
            self.build(int64=True)

if __name__ == '__main__':
    unittest.main()
