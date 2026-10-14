"""读取 ELF64 section、符号和 DFX 参数描述，并校验 Ascend SK_BIND 关系。

DFX size 表示分配大小，不是设备指令读取的字节数。在普通非 OOM 编译器
布局中，TILING 大小还包含额外的 8 字节。
"""

import hashlib
from pathlib import Path
import struct


class ElfMetadataError(ValueError):
    """ELF/DFX 数据格式错误或不受支持。"""


class MissingDFXMetadataError(ElfMetadataError):
    """选中的入口没有 DFX 参数记录；格式错误的记录单独处理。"""


class ElfMetadata:
    def __init__(self, path):
        self.path = Path(path)
        data = self.path.read_bytes()
        self.content_sha256 = hashlib.sha256(data).hexdigest()
        self.sections = {}
        self.symbols = set()
        self.functions = set()
        self._parse(data)

    @staticmethod
    def _slice(data, offset, size, label):
        invalid_range = offset < 0 or size < 0
        if not invalid_range:
            invalid_range = offset > len(data) or size > len(data) - offset
        if invalid_range:
            raise ElfMetadataError(f"truncated {label}: offset={offset}, size={size}")
        return data[offset : offset + size]

    @staticmethod
    def _string(table, offset):
        if offset >= len(table):
            raise ElfMetadataError("string offset outside string table")
        end = table.find(b"\0", offset)
        if end < 0:
            raise ElfMetadataError("unterminated ELF string")
        try:
            return table[offset:end].decode("utf-8")
        except UnicodeDecodeError as exc:
            raise ElfMetadataError("invalid ELF string encoding") from exc

    def _parse(self, data):
        header = self._slice(data, 0, 64, "ELF header")
        if header[:4] != b"\x7fELF" or header[4] != 2 or header[6] != 1:
            raise ElfMetadataError("expected ELF64 version 1")
        if header[5] not in (1, 2):
            raise ElfMetadataError("invalid ELF endianness")
        self.endian = "<" if header[5] == 1 else ">"
        values = struct.unpack(self.endian + "16sHHIQQQIHHHHHH", header)
        _, _, _, version, _, phoff, shoff, _, ehsize, phentsize, phnum, shentsize, shnum, shstr = values
        invalid_sizes = ehsize != 64 or shentsize != 64
        if version != 1 or invalid_sizes or not shoff:
            raise ElfMetadataError("unsupported ELF header layout")
        if phnum == 65535:
            raise ElfMetadataError("extended program-header count is unsupported")
        if phnum:
            if phentsize != 56:
                raise ElfMetadataError("invalid program header entry size")
            self._slice(data, phoff, phnum * phentsize, "program headers")
        self.elf_type, self.machine = values[1:3]
        first = struct.unpack(self.endian + "IIQQQQIIQQ", self._slice(data, shoff, 64, "section zero"))
        if shnum == 0:
            shnum = first[5]
        if shstr == 65535:
            shstr = first[6]
        if not shnum or shstr >= shnum:
            raise ElfMetadataError("invalid section string-table index")
        raw = self._slice(data, shoff, shnum * 64, "section headers")
        headers = list(struct.iter_unpack(self.endian + "IIQQQQIIQQ", raw))
        if headers[shstr][1] != 3:
            raise ElfMetadataError("section names do not refer to a string table")
        names = self._slice(data, headers[shstr][4], headers[shstr][5], "section names")
        contents = []
        for section in headers:
            name_offset, kind, _, _, offset, size, link, _, align, _ = section
            if align and align & (align - 1):
                raise ElfMetadataError("invalid section alignment")
            if link >= shnum and link:
                raise ElfMetadataError("section link outside section table")
            content = b"" if kind in (0, 8) else self._slice(data, offset, size, "section data")
            contents.append(content)
            name = self._string(names, name_offset)
            if name and name in self.sections:
                raise ElfMetadataError(f"duplicate section name: {name}")
            self.sections[name] = content
        self.section_headers = headers
        self.section_contents = contents
        self.section_names = [self._string(names, section[0]) for section in headers]
        self._parse_symbol_tables(headers, contents, shnum)

    def _parse_symbol_tables(self, headers, contents, shnum):
        """读取符号表并收集已定义的函数入口。"""
        self.symbol_tables = {}
        for index, section in enumerate(headers):
            if section[1] not in (2, 11):
                continue
            if section[9] != 24 or section[5] % 24:
                raise ElfMetadataError("invalid symbol table entry size")
            if headers[section[6]][1] != 3:
                raise ElfMetadataError("symbol table link does not refer to strings")
            strings = contents[section[6]]
            self.symbol_tables[index] = []
            for nameoff, info, _, shindex, value, size in struct.iter_unpack(self.endian + "IBBHQQ", contents[index]):
                if shindex == 65535:
                    raise ElfMetadataError("extended symbol section indices unsupported")
                if 0 < shindex < 65280 and shindex >= shnum:
                    raise ElfMetadataError("symbol section index outside section table")
                name = self._string(strings, nameoff)
                self.symbol_tables[index].append((name, info & 15, shindex, value, size))
                if name and shindex:
                    self.symbols.add(name)
                    if info & 15 == 2:
                        self.functions.add(name)

    def validate_sk_bind(self, load_symbols, capability=0):
        """通过精确的函数重定位校验编译器 SK_BIND v1。

        生成的 56 字节记录由 TLV(5,52)、reserved u32、capability u64、
        globalFunc 和四个有序 SK 指针组成。只接受 Ascend ABS64 RELA
        引用；不支持或被 strip 的绑定按失败处理。
        """
        if self.machine != 0x1029 or self.elf_type not in (1, 2, 3):
            raise ElfMetadataError('unsupported SK_BIND ELF architecture or type')
        if capability & ~7:
            raise ElfMetadataError('unsupported SK_BIND capability bits')
        data = self.sections.get('.ascend.meta', b'')
        if not data:
            raise ElfMetadataError('missing SK_BIND metadata')
        target = self.section_names.index('.ascend.meta')
        relocations = self._sk_bind_relocations(data, target)
        bindings = self._sk_bind_records(data, relocations)
        for symbol in load_symbols:
            sk = symbol + '_static_sk'
            expected = [sk]
            for index in range(1, 4):
                expected.append(sk + '_split' + str(index))
            if bindings.get(symbol) != (tuple(expected), capability):
                raise ElfMetadataError('SK_BIND target/split/capability mismatch: ' + symbol)

    def _sk_bind_relocations(self, data, target):
        """只接受指向目标元数据 section 的 ABS64 RELA 表。"""
        relocations = {}
        for index, header in enumerate(self.section_headers):
            if header[1] not in (4, 9) or header[7] != target:
                continue
            if header[1] != 4 or header[9] != 24:
                raise ElfMetadataError('unsupported SK_BIND relocation format')
            table = self.symbol_tables.get(header[6])
            raw = self.section_contents[index]
            if table is None or len(raw) % 24:
                raise ElfMetadataError('invalid SK_BIND relocation table')
            for offset, info, addend in struct.iter_unpack(self.endian + 'QQq', raw):
                if self.elf_type != 1:
                    offset -= self.section_headers[target][3]
                symbol_index = info >> 32
                outside = offset < 0 or offset + 8 > len(data)
                invalid_target = offset in relocations or symbol_index >= len(table)
                if outside or invalid_target:
                    raise ElfMetadataError('invalid or duplicate SK_BIND relocation')
                relocations[offset] = (info & 0xFFFFFFFF, addend, table[symbol_index])
        return relocations

    def _sk_bind_function_names(self, at, relocations):
        names = []
        pointer_offsets = range(at + 16, at + 56, 8)
        for offset in pointer_offsets:
            relocation = relocations.get(offset)
            if relocation is None:
                raise ElfMetadataError('SK_BIND pointer lacks verifiable relocation')
            relocation_type, addend, symbol = relocation
            name, symbol_type, section_index, _, _ = symbol
            valid_relocation = relocation_type == 257 and addend == 0
            defined_function = symbol_type == 2 and 0 < section_index < 65280
            if not valid_relocation or not defined_function or name not in self.functions:
                raise ElfMetadataError('SK_BIND pointer does not reference a defined function')
            names.append(name)
        # 不接受会改写记录头或 capability 的重定位。
        for offset in relocations:
            if at <= offset < at + 56 and offset not in pointer_offsets:
                raise ElfMetadataError('unexpected relocation inside SK_BIND record')
        return names

    def _sk_bind_records(self, data, relocations):
        """按原始 TLV 顺序校验每个物理入口的四个 SK 指针。"""
        bindings = {}
        at = 0
        while at < len(data):
            kind, size = struct.unpack(self.endian + 'HH', self._slice(data, at, 4, 'SK_BIND TLV'))
            payload = self._slice(data, at + 4, size, 'SK_BIND payload')
            if kind == 5:
                if size != 52:
                    raise ElfMetadataError('unsupported SK_BIND record length')
                reserved, cap = struct.unpack_from(self.endian + 'IQ', payload)
                if reserved or cap & ~7:
                    raise ElfMetadataError('unsupported SK_BIND reserved/capability fields')
                names = self._sk_bind_function_names(at, relocations)
                if names[0] in bindings:
                    raise ElfMetadataError('duplicate SK_BIND for physical entry')
                bindings[names[0]] = (tuple(names[1:]), cap)
            at += 4 + size
        return bindings

    def arguments(self, entry):
        """返回确切 ELF 入口对应的 DFX 描述元组，元素为按参数顺序排列的字典。

        元数据缺失或入口未知时按失败处理。未知分配长度用 None 表示，
        不根据入口名后缀推断。
        """
        if entry not in self.functions:
            raise ElfMetadataError(f"entry is not a defined ELF function: {entry}")
        name = ".ascend.meta." + entry
        if name not in self.sections:
            raise MissingDFXMetadataError(f"missing metadata section: {name}")
        data = self.sections[name]
        offset = 0
        result = None
        while offset < len(data):
            kind, length = struct.unpack(self.endian + "HH", self._slice(data, offset, 4, "TLV header"))
            value = self._slice(data, offset + 4, length, "TLV value")
            offset += 4 + length
            if kind != 4:
                continue
            if result is not None:
                raise ElfMetadataError("duplicate argument metadata")
            result = []
            arg_offset = 0
            while arg_offset < len(value):
                arg_kind, words = struct.unpack(">HH", self._slice(value, arg_offset, 4, "DFX header"))
                if arg_kind != 5 or words < 1:
                    raise ElfMetadataError("invalid DFX argument record")
                payload = self._slice(value, arg_offset + 4, words * 8, "DFX argument")
                fields = struct.unpack(">" + "Q" * words, payload)
                arg_offset += 4 + words * 8
                size = fields[1] if words > 1 else None
                if size == (1 << 64) - 1:
                    size = None
                result.append({"kind": fields[0] & 255, "pointer_kind": (fields[0] >> 16) & 255, "size": size})
        if result is None:
            raise MissingDFXMetadataError("missing DFX argument metadata")
        return tuple(result)
