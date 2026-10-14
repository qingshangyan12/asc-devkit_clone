"""Strict static tiling fixture loader; no device invocation or inferred defaults."""
import copy
import json
import re
from dataclasses import dataclass
from pathlib import Path
from .errors import SpecializationError

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
    aicpu_block_dim: int
    tiling_cond: int
    kernel_name: str
    raw_run_info: dict

def _integer(value, name, bits, signed=False):
    minimum = -(2**(bits-1)) if signed else 0
    maximum = 2**(bits-1)-1 if signed else 2**bits-1
    if type(value) is not int or not minimum <= value <= maximum:
        raise SpecializationError('tiling', f'{name} must be {"int" if signed else "uint"}{bits}')
    return value

def _object(pairs):
    result = {}
    for key, value in pairs:
        if key in result:
            raise SpecializationError('tiling', f'duplicate JSON field {key}')
        result[key] = value
    return result

def get_static_op_tiling(run_info_path):
    try:
        raw = json.loads(Path(run_info_path).read_text(encoding='utf-8'), object_pairs_hook=_object,
                         parse_constant=lambda x: (_ for _ in ()).throw(ValueError(f'invalid JSON constant {x}')))
        if not isinstance(raw, dict):
            raise SpecializationError('tiling', 'run_info must be an object')
        values = {}
        for name in ('block_dim', 'schedule_mode', 'local_memory_size', 'aicpu_block_dim'):
            values[name] = _integer(raw[name], name, 32)
        values['tiling_args_index'] = _integer(
            raw['tiling_args_index'], 'tiling_args_index', 64)
        values['tiling_key'] = _integer(raw['tiling_key'], 'tiling_key', 64)
        values['tiling_cond'] = _integer(raw['tiling_cond'], 'tiling_cond', 32, True)
        if values['block_dim'] == 0:
            raise SpecializationError('tiling', 'block_dim must be positive for execution')
        if type(raw['clear_atomic']) is not bool:
            raise SpecializationError('tiling', 'clear_atomic must be bool')
        if not isinstance(raw['workspaces'], list):
            raise SpecializationError('tiling', 'workspaces must be a list')
        values['workspaces'] = [_integer(x, 'workspace', 64) for x in raw['workspaces']]
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
