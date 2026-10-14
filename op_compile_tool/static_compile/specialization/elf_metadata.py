"""Strict, dependency-free reader for ELF64 Ascend argument metadata.

DFX sizes are allocation sizes, not bytes read by device instructions. In the
ordinary non-OOM compiler layout, TILING size includes an extra eight bytes.
"""
import hashlib
from pathlib import Path
import struct


class ElfMetadataError(ValueError):
    """Malformed or unsupported ELF/DFX data."""


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
        if offset < 0 or size < 0 or offset > len(data) or size > len(data) - offset:
            raise ElfMetadataError(f"truncated {label}: offset={offset}, size={size}")
        return data[offset:offset + size]

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
        if version != 1 or ehsize != 64 or shentsize != 64 or not shoff:
            raise ElfMetadataError("unsupported ELF header layout")
        if phnum == 65535:
            raise ElfMetadataError("extended program-header count is unsupported")
        if phnum:
            if phentsize != 56:
                raise ElfMetadataError("invalid program header entry size")
            self._slice(data, phoff, phnum * phentsize, "program headers")
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
        for index, section in enumerate(headers):
            if section[1] not in (2, 11):
                continue
            if section[9] != 24 or section[5] % 24:
                raise ElfMetadataError("invalid symbol table entry size")
            if headers[section[6]][1] != 3:
                raise ElfMetadataError("symbol table link does not refer to strings")
            strings = contents[section[6]]
            for nameoff, info, _, shindex, _, _ in struct.iter_unpack(self.endian + "IBBHQQ", contents[index]):
                if shindex == 65535:
                    raise ElfMetadataError("extended symbol section indices unsupported")
                if 0 < shindex < 65280 and shindex >= shnum:
                    raise ElfMetadataError("symbol section index outside section table")
                name = self._string(strings, nameoff)
                if name and shindex:
                    self.symbols.add(name)
                    if info & 15 == 2:
                        self.functions.add(name)

    def arguments(self, entry):
        """Return ordered DFX argument dicts for an exact ELF entry symbol.

        Missing metadata or unknown entries fail closed. Unknown allocation
        lengths are represented by None. No entry-name suffix inference occurs.
        """
        if entry not in self.functions:
            raise ElfMetadataError(f"entry is not a defined ELF function: {entry}")
        name = ".ascend.meta." + entry
        if name not in self.sections:
            raise ElfMetadataError(f"missing metadata section: {name}")
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
                result.append({"kind": fields[0] & 255,
                               "pointer_kind": (fields[0] >> 16) & 255,
                               "size": size})
        if result is None:
            raise ElfMetadataError("missing DFX argument metadata")
        return tuple(result)
