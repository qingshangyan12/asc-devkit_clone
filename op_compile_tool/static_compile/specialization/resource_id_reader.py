"""Read the compiler-embedded resource ID, never the whole-ELF hash."""
import re
import struct
from .elf_metadata import ElfMetadata


def read_resource_id(dynamic_elf, explicit_resource_id=None):
    elf = ElfMetadata(dynamic_elf)
    candidates = []
    # CANN 9.3's real producer emits BinaryMetaSpecializationResourceId:
    # B_TYPE_SPECIALIZATION_RESOURCE_ID (6), uint16 length, 64 ASCII bytes.
    data = elf.sections.get('.ascend.meta', b'')
    offset = 0
    while offset < len(data):
        if len(data)-offset < 4:
            raise ValueError('truncated resource metadata header')
        kind, size = struct.unpack_from('<HH', data, offset)
        offset += 4
        if size > len(data)-offset:
            raise ValueError('truncated resource metadata payload')
        if kind == 6:
            candidates.append(data[offset:offset+size])
        offset += size
    # Also accept the separately named section described by the integration spec.
    if '.ascend.resource_id' in elf.sections:
        candidates.append(elf.sections['.ascend.resource_id'])
    if not candidates or len(set(candidates)) != 1:
        raise ValueError('missing or conflicting embedded resource IDs')
    value = candidates[0].decode('ascii')
    if not re.fullmatch('[0-9a-f]{64}', value):
        raise ValueError('resource ID must contain exactly 64 lowercase hex bytes')
    if explicit_resource_id is not None and value != explicit_resource_id:
        raise ValueError('explicit and ELF resource IDs differ')
    return value
