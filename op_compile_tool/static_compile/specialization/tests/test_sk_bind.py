"""SK 绑定必须识别当前入口及其有序的四个变体。"""

import struct
import unittest
from static_compile.specialization.elf_metadata import ElfMetadata, ElfMetadataError


class SkBindTests(unittest.TestCase):
    @staticmethod
    def _relocations(start_offset, first_symbol):
        records = []
        for index in range(5):
            offset = start_offset + 8 * index
            info = ((first_symbol + index) << 32) | 257
            records.append(struct.pack('<QQq', offset, info, 0))
        return b''.join(records)

    def elf(self, names=None, cap=0):
        names = names or ['entry', 'entry_static_sk', *(f'entry_static_sk_split{i}' for i in range(1, 4))]
        elf = ElfMetadata.__new__(ElfMetadata)
        elf.endian = '<'
        elf.elf_type = 2
        elf.machine = 0x1029
        elf.sections = {'.ascend.meta': struct.pack('<HHIQ5Q', 5, 52, 0, cap, 0, 0, 0, 0, 0)}
        elf.section_headers = [
            (0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
            (0, 1, 0, 0, 0, 56, 0, 0, 8, 0),
            (0, 4, 0, 0, 0, 120, 3, 1, 8, 24),
        ]
        elf.section_names = ['', '.ascend.meta', '.rela.ascend.meta']
        elf.section_contents = [
            b'',
            elf.sections['.ascend.meta'],
            self._relocations(16, 1),
        ]
        elf.symbol_tables = {3: [('', 0, 0, 0, 0)] + [(n, 2, 4, 100 * i, 8) for i, n in enumerate(names)]}
        elf.functions = set(names)
        return elf

    def test_correct_binding(self):
        self.elf().validate_sk_bind(('entry',), 0)

    def test_other_entry_wrong_split_and_capabilities(self):
        for names in (
            ['other', 'entry_static_sk', *(f'entry_static_sk_split{i}' for i in range(1, 4))],
            ['entry', 'other_static_sk', *(f'entry_static_sk_split{i}' for i in range(1, 4))],
            ['entry', 'entry_static_sk', 'entry_static_sk_split2', 'entry_static_sk_split1', 'entry_static_sk_split3'],
        ):
            with self.subTest(names=names), self.assertRaises(ElfMetadataError):
                self.elf(names).validate_sk_bind(('entry',), 0)
        with self.assertRaises(ElfMetadataError):
            self.elf(cap=1).validate_sk_bind(('entry',), 0)

    def test_missing_relocation_bad_type_addend_and_truncation(self):
        for kind in ('missing', 'type', 'addend', 'truncated', 'duplicate'):
            elf = self.elf()
            data = elf.section_contents[2]
            if kind == 'missing':
                data = data[:-24]
            if kind == 'type':
                data = struct.pack('<QQq', 16, (1 << 32) | 999, 0) + data[24:]
            if kind == 'addend':
                data = struct.pack('<QQq', 16, (1 << 32) | 257, 8) + data[24:]
            if kind == 'duplicate':
                data += data[:24]
            if kind == 'truncated':
                elf.sections['.ascend.meta'] = elf.sections['.ascend.meta'][:-1]
            elf.section_contents[2] = data
            with self.subTest(kind=kind), self.assertRaises(ElfMetadataError):
                elf.validate_sk_bind(('entry',), 0)

    def test_every_mix_core_requires_binding(self):
        with self.assertRaises(ElfMetadataError):
            self.elf().validate_sk_bind(('entry', 'second_core'), 0)

    def test_two_bound_entries_and_duplicate_record(self):
        elf = self.elf()
        other = self.elf(['second', 'second_static_sk', *(f'second_static_sk_split{i}' for i in range(1, 4))])
        elf.sections['.ascend.meta'] += other.sections['.ascend.meta']
        elf.symbol_tables[3] += other.symbol_tables[3][1:]
        elf.functions.update(other.functions)
        elf.section_contents[2] += self._relocations(72, 6)
        elf.validate_sk_bind(('entry', 'second'), 0)
        elf.symbol_tables[3][6:] = elf.symbol_tables[3][1:6]
        with self.assertRaisesRegex(ElfMetadataError, 'duplicate SK_BIND'):
            elf.validate_sk_bind(('entry', 'second'), 0)
