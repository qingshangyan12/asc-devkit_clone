import struct
import unittest
from types import SimpleNamespace
from unittest.mock import patch
from static_compile.specialization.resource_id_reader import read_resource_id


class ResourceIdTests(unittest.TestCase):
    def read(self, sections):
        with patch(
            'static_compile.specialization.resource_id_reader.ElfMetadata',
            return_value=SimpleNamespace(sections=sections),
        ):
            return read_resource_id('unused')

    def test_real_producer_tlv_and_spec_section(self):
        value = b'a' * 64
        self.assertEqual(self.read({'.ascend.meta': struct.pack('<HH', 6, 64) + value}), value.decode())
        self.assertEqual(self.read({'.ascend.resource_id': value}), value.decode())

    def test_missing_truncated_and_conflicting(self):
        cases = [
            {},
            {'.ascend.meta': b'\x06'},
            {'.ascend.meta': struct.pack('<HH', 6, 64) + b'a'},
            {'.ascend.resource_id': b'a' * 64, '.ascend.meta': struct.pack('<HH', 6, 64) + b'b' * 64},
        ]
        for sections in cases:
            with self.subTest(sections=sections), self.assertRaises(ValueError):
                self.read(sections)


if __name__ == '__main__':
    unittest.main()
