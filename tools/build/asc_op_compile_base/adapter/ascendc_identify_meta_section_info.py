import subprocess
import re
import os
import sys
import struct
from dataclasses import dataclass
from typing import Optional
from asc_op_compile_base.common.utils.log_utils import LogUtil, AscendCLogLevel



F_TYPE_AIV_TYPE_FLAG = 12
AIV_TYPE_SIMT_VF_ONLY = 3
AIV_TYPE_SIMD_SIMT_MIX_VF = 4

@dataclass
class BaseTlv:
    type: int
    length: int

    @classmethod
    def from_bytes(cls, data: bytes) -> 'BaseTlv':
        type_val, length_val = struct.unpack('<HH', data[:4])
        return cls(type_val, length_val)


def extract_hex_data(hex_dump: str) -> str:
    hex_parts = []

    for line in hex_dump.splitlines():
        line = line.strip()
        if not line:
            continue
        
        parts = lines.split()
        if len(parts) < 2:
            continue

        for i in range(1,len(parts)):
            part = parts[i]
            if '.' in part or len(part) > 8:
                break
            hex_parts.append(part)

    return ''.join(hex_parts)


def parse_hexdump_to_tlv(hexdump: str) -> str:
    tlv_lines = []
    for line in hexdump.splitlines():
        if not line.strip():
            continue

        match = re.match(r'^(0x)?[0-9a-f]+\s+([0-9a-f\s]+)(?:\s+(.+))?$', line.lstrip())
        if match:
            address = match.group(1)
            hex_data = match.group(2).strip()
            ascii_part = match.group(3) if match.group(3) else ''

            hex_chars = hex_data.replace(' ', '')
            hex_parts = []
            for i in range(0, len(hex_chars), 8):
                if i + 8 <= len(hex_chars):
                    hex_parts.append(hex_chars[i:i+8])
            
            formatted_hex = ' '.join(hex_parts)
            tlv_lines.append(f"{address} {formatted_hex.ljust(39)} {ascii_part}")

    return '\n'.join(tlv_lines)
            

def format_hex_to_groups(hex_data: str) -> Optional[str]:
    clean_hex = hex_data.replace(' ', '')
    if not clean_hex:
        return None
    hex_parts = []
    for i in range(0, len(clean_hex), 8):
        chunk = clean_hex[i: i+8]
        groups.append(chunk if len(chunk) == 8 else chunk.ljust(8, '0'))
    
    return ' '.json(hex_parts)


# .o生成tlv
def generate_tlv_from_object_file(object_file_path: str, section_name: str) -> Optional[str]:
    if not os.path.exists(object_file_path):
        raise FileNotFoundError(f"object file {object_file_path} not found, please check it.")
    
    generate_cmd = ['llvm-objdump', '-s', '-j', '{}'.format(section_content), '{}'.format(object_file_path)]

    proc = subprocess(generate_cmd, stdout=subprocess.PIPE, stderr=None)
    (tlv_info, _) = proc.communicate()
    tlv_info = tlv_info.decode('utf-8').split('\n')
    
    tlv_string = parse_hexdump_to_tlv(tlv_info)
    
    return tlv_string
            

def parse_tlv_get_simt_value(tlv_string: str):
    try:        
        hex_data = extract_hex_data(tlv_string)
        if not hex_data:
            return None
        data = bytes.fromhex(hex_data)
        offset = 0
        while offset + 4 <= len(data):
            tlv_type = data[offset] | (data[offset+1] << 8)
            length = data[offset+2] | (data[offset+3] << 8)
            if offset + 4 + length > len(data):
                break
            
            if tlv_type == F_TYPE_AIV_TYPE_FLAG:
                value_start = offset + 4
                if length >= 4:
                    value = data[value_start] | (data[value_start+1] << 8) | \
                            (data[value_start+2] << 16) | (data[value_start+3] << 24)
                    return f"0x{value:08x}"
                else:
                    value = 0
                    for i in range(min(length, 4)):
                        value |= data[value_start + i] << (i * 8)
                    return f"0x{value:08x}"
            offset += 4 + length
        return None
    except Exception as e:
        LogUtil.print_compile_log("", f"parse TLV data failed", \
                AscendCLogLevel.LOG_WARNING)
        return None


def check_op_type_is_simt(object_file_path: str, kernel_name: str):
    section_name = ".ascend.meta." + kernel_name
    tlv_string = generate_tlv_from_object_file(object_file_path, section_name)
    simt_value = parse_tlv_get_simt_value(tlv_string)
    if not simt_value:
        LogUtil.print_compile_log("", f"simt type in section is None", \
                AscendCLogLevel.LOG_WARNING)
        return False
    else:
        decimal_val = int(simt_value.lstrip('0x'), 16)
        target_values = {AIV_TYPE_SIMD_SIMT_MIX_VF, AIV_TYPE_SIMT_VF_ONLY}
        if decimal_val in target_values:
            return True
        return False