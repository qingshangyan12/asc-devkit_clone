"""A registration capacity is not the selected tiling-key structure size."""
import hashlib
from pathlib import Path
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import patch
from static_compile.specialization.argument_binding import resolve_kernel_metadata

class PerKeyAllocationTests(unittest.TestCase):
    def test_selected_dfx_size_within_registered_capacity(self):
        with tempfile.TemporaryDirectory() as directory:
            p=Path(directory)/'kernel.o';p.write_bytes(b'fixture')
            metadata={'kernelName':'kernel','tilingKey':9,'sha256':hashlib.sha256(b'fixture').hexdigest(),'opParaSize':600,'supportInfo':{'inputs':[],'outputs':[]}}
            tiling=SimpleNamespace(kernel_name='kernel',tiling_key=9,tiling_data=bytes(72))
            elf=SimpleNamespace(functions={'kernel'},sections={},arguments=lambda _:({'kind':4,'size':None},{'kind':7,'size':80}))
            with patch('static_compile.specialization.argument_binding.ElfMetadata',return_value=elf),patch('static_compile.specialization.argument_binding.get_kernel_argument_layout',return_value=(('workspace','tiling'),1)):
                result=resolve_kernel_metadata(p,metadata,tiling)
                self.assertEqual(result.read_bytes,72)
                metadata['opParaSize']=64
                with self.assertRaisesRegex(ValueError,'capacity'):resolve_kernel_metadata(p,metadata,tiling)
                metadata['opParaSize']=600;tiling.tiling_data=bytes(592)
                with self.assertRaisesRegex(ValueError,'structure size'):resolve_kernel_metadata(p,metadata,tiling)
