"""校验采集到的顶层运行字段；不访问文件或设备。"""
import copy
import re
from dataclasses import dataclass
from .errors import SpecializationError

RUN_INFO_FIELDS = (
    'kernel_name', 'tiling_args_index', 'tiling_data', 'tiling_key', 'block_dim',
    'workspace_bytes', 'clear_atomic', 'schedule_mode', 'local_memory_size',
)

OPTIONAL_RUN_INFO_FIELDS = ('aicpu_block_dim', 'tiling_cond')

@dataclass(frozen=True)
class StaticTilingResult:
    tiling_data: bytes
    tiling_key: int
    tiling_args_index: int
    block_dim: int
    workspaces: list[int]
    clear_atomic: bool
    schedule_mode: int
    local_memory_size: int
    aicpu_block_dim: object
    tiling_cond: object
    kernel_name: str
    raw_run_info: dict

def _integer(value, name, bits, signed=False):
    minimum = -(2**(bits-1)) if signed else 0
    maximum = 2**(bits-1)-1 if signed else 2**bits-1
    if type(value) is not int or not minimum <= value <= maximum:
        raise SpecializationError('tiling', f'{name} must be {"int" if signed else "uint"}{bits}')
    return value

def _decimal_integer(value, name, bits):
    # ACLNN dump 会将 key/block 序列化为十进制字符串；同时保留整数输入兼容性。
    if isinstance(value, str) and re.fullmatch(r'[0-9]+', value):
        value = int(value, 10)
    return _integer(value, name, bits)

def _object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise SpecializationError('tiling', f'duplicate JSON field {key}')
        result[key] = value
    return result

def get_static_op_tiling(run_info):
    try:
        raw = run_info
        if not isinstance(raw, dict):
            raise SpecializationError('tiling', 'run_info must be an object')
        values = {}
        values['block_dim'] = _decimal_integer(raw['block_dim'], 'block_dim', 32)
        for name in ('schedule_mode', 'local_memory_size'):
            values[name] = _integer(raw[name], name, 32)
        values['tiling_args_index'] = _integer(
            raw['tiling_args_index'], 'tiling_args_index', 64)
        values['tiling_key'] = _decimal_integer(raw['tiling_key'], 'tiling_key', 64)
        for name in OPTIONAL_RUN_INFO_FIELDS:
            values[name] = copy.deepcopy(raw.get(name))
        if values['block_dim'] == 0:
            raise SpecializationError('tiling', 'block_dim must be positive for execution')
        if type(raw['clear_atomic']) is not bool:
            raise SpecializationError('tiling', 'clear_atomic must be bool')
        workspace_bytes = raw['workspace_bytes']
        if (not isinstance(workspace_bytes, str) or
                re.fullmatch(r'(?:[0-9]+(?: +[0-9]+)* *)?', workspace_bytes) is None):
            raise SpecializationError('tiling', 'workspace_bytes must be space-separated decimal uint64 sizes')
        # 采集器会在每个 size 后写入尾随空格；保留顺序和零值。
        values['workspaces'] = [_integer(int(x), 'workspace', 64) for x in workspace_bytes.split()]
        hex_data = raw['tiling_data']
        if not isinstance(hex_data, str) or re.fullmatch(r'(?:[0-9a-fA-F]{2})*', hex_data) is None:
            raise SpecializationError('tiling', 'tiling_data must be an even-length hexadecimal string')
        data = bytes.fromhex(hex_data)
        entry = raw['kernel_name']
        if not isinstance(entry, str) or not entry or any(ord(c) < 32 or ord(c) == 127 for c in entry):
            raise SpecializationError('tiling', 'kernel_name must be a nonempty string without controls')
        return StaticTilingResult(tiling_data=data, clear_atomic=raw['clear_atomic'], kernel_name=entry,
                                 raw_run_info=copy.deepcopy(raw), **values)
    except (OSError, UnicodeError, ValueError, KeyError, TypeError) as exc:
        raise SpecializationError('tiling', str(exc)) from exc
