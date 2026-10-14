"""绑定字节数以采集数据为准，不从资源注册容量或 DFX 推导。"""

import hashlib
from pathlib import Path
from types import SimpleNamespace
import tempfile
import unittest
from unittest.mock import Mock, patch
from static_compile.specialization.argument_binding import resolve_kernel_metadata, resolve_entry


class PerKeyAllocationTests(unittest.TestCase):
    def test_collected_size_is_independent_of_registration_capacity(self):
        with tempfile.TemporaryDirectory() as directory:
            p = Path(directory) / 'kernel.o'
            p.write_bytes(b'fixture')
            metadata = {
                'kernelName': 'kernel',
                'tilingKey': 9,
                'sha256': hashlib.sha256(b'fixture').hexdigest(),
                'opParaSize': 600,
                'supportInfo': {'inputs': [], 'outputs': []},
            }
            tiling = SimpleNamespace(kernel_name='kernel', tiling_key=9, tiling_args_index=1, tiling_data=bytes(72))
            elf = SimpleNamespace(
                functions={'kernel'},
                sections={},
                arguments=Mock(side_effect=AssertionError('DFX must not be read for tiling binding')),
            )
            # 本组只验证 dump 数据长度；真实 schema 校验由 test_kernel_validation 覆盖。
            with (
                patch('static_compile.specialization.argument_binding.ElfMetadata', return_value=elf),
                patch('static_compile.specialization.argument_binding.validate_resource_context'),
            ):
                inputs = dict(
                    dynamic_elf=p, metadata=metadata, tiling=tiling,
                    selected_kernel=resolve_entry(metadata, tiling.kernel_name), parameter_mode=None,
                    shape_config={}, op_type='Example', selected_from_collection=False,
                )
                for capacity in (None, 0, 64, 600, 'invalid'):
                    metadata['opParaSize'] = capacity
                    for size in (4, 72, 592):
                        tiling.tiling_data = bytes(size)
                        with self.subTest(capacity=capacity, size=size):
                            result = resolve_kernel_metadata(inputs)
                            self.assertEqual(result.read_bytes, size)
                metadata.pop('opParaSize')
                result = resolve_kernel_metadata(inputs)
                self.assertEqual(result.read_bytes, len(tiling.tiling_data))
                elf.arguments.assert_not_called()
