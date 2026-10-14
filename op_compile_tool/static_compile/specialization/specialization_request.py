"""与公开 acl_rt_compile.h 匹配的 LP64 aclrtcKernelSpecRequest 封装。"""

import ctypes
from dataclasses import dataclass
from pathlib import Path
from .errors import SpecializationError

MAX_STRING_BYTES = 1024 * 1024
MAX_TEXT_BYTES = 4 * MAX_STRING_BYTES
MAX_OPTIONS = 4096


@dataclass(frozen=True)
class KernelArgument:
    byte_size: int
    constant_data: bytes | None = None


@dataclass(frozen=True)
class SpecializationInput:
    resource_id: str
    kernel_entry: str
    arguments: list[KernelArgument]
    basic_options: list[str]
    sk_options: list[str]
    output_path: str


class AclrtcKernelSpecRequest(ctypes.Structure):
    _fields_ = [
        ('resourceId', ctypes.c_char_p),
        ('kernelEntry', ctypes.c_char_p),
        ('argsCount', ctypes.c_uint64),
        ('argsAddr', ctypes.POINTER(ctypes.c_void_p)),
        ('argsBytes', ctypes.POINTER(ctypes.c_uint64)),
        ('options', ctypes.POINTER(ctypes.c_char_p)),
        ('optionCount', ctypes.c_uint64),
        ('skOptions', ctypes.POINTER(ctypes.c_char_p)),
        ('skOptionCount', ctypes.c_uint64),
        ('reserved', ctypes.c_uint8 * 256),
    ]


def _text(value, name):
    if not isinstance(value, str) or not value:
        raise SpecializationError('request', f'{name} must be a nonempty string')
    if any(ord(c) < 32 or ord(c) == 127 for c in value):
        raise SpecializationError('request', f'{name} contains NUL or control characters')
    try:
        data = value.encode('utf-8')
    except UnicodeError as exc:
        raise SpecializationError('request', f'{name} is not UTF-8 encodable') from exc
    if len(data) > MAX_STRING_BYTES:
        raise SpecializationError('request', f'{name} exceeds 1 MiB')
    return data


def _overrides_output(value):
    """依次识别输出覆盖选项，保留原有 startswith 调用和短路顺序。"""
    return value.startswith('-o') or value.startswith('--output') or value.startswith('--serialize-diagnostics')


def _options(values, name):
    if not isinstance(values, (list, tuple)):
        raise SpecializationError('request', f'{name} must be a list or tuple')
    result = []
    for value in values:
        data = _text(value, name)
        # options 是 argv 项，不能是 shell 文本或 response-file 间接引用。
        if '@' in value or _overrides_output(value) or value in ('-MF', '-MJ'):
            raise SpecializationError('request', f'{name} contains a response-file or output override')
        result.append(data)
    return result


class OwnedSpecializationRequest:
    def __init__(self, value: SpecializationInput):
        if ctypes.sizeof(ctypes.c_void_p) != 8 or ctypes.sizeof(AclrtcKernelSpecRequest) != 328:
            raise SpecializationError('request', 'specialization requires the LP64 328-byte ABI')
        offsets = {
            'resourceId': 0,
            'kernelEntry': 8,
            'argsCount': 16,
            'argsAddr': 24,
            'argsBytes': 32,
            'options': 40,
            'optionCount': 48,
            'skOptions': 56,
            'skOptionCount': 64,
            'reserved': 72,
        }
        if any(getattr(AclrtcKernelSpecRequest, name).offset != offset for name, offset in offsets.items()):
            raise SpecializationError('request', 'specialization C field offsets differ from LP64 ABI')
        resource = _text(value.resource_id, 'resource_id')
        entry = _text(value.kernel_entry, 'kernel_entry')
        output = _text(value.output_path, 'output_path')
        basic = _options(value.basic_options, 'basic_options')
        sk = _options(value.sk_options, 'sk_options')
        if len(basic) + len(sk) > MAX_OPTIONS:
            raise SpecializationError('request', 'option count exceeds 4096')
        self._strings = [resource, entry, output, *basic, *sk]
        if sum(len(x) for x in self._strings) > MAX_TEXT_BYTES:
            raise SpecializationError('request', 'request text exceeds 4 MiB')
        if not isinstance(value.arguments, (list, tuple)):
            raise SpecializationError('request', 'arguments must be the full list or tuple')
        self._buffers = []
        addresses = []
        sizes = []
        for arg in value.arguments:
            if not isinstance(arg, KernelArgument):
                raise SpecializationError('request', 'argument byte_size must be uint64')
            if (
                not isinstance(arg.byte_size, int)
                or isinstance(arg.byte_size, bool)
                or not 0 <= arg.byte_size <= 2**64 - 1
            ):
                raise SpecializationError('request', 'argument byte_size must be uint64')
            if arg.constant_data is None:
                addresses.append(None)
            else:
                if not isinstance(arg.constant_data, bytes) or len(arg.constant_data) != arg.byte_size:
                    raise SpecializationError('request', 'constant data length must exactly match byte_size')
                buffer = ctypes.create_string_buffer(arg.constant_data, max(1, arg.byte_size))
                self._buffers.append(buffer)
                addresses.append(ctypes.addressof(buffer))
            sizes.append(arg.byte_size)
        self._addresses = (ctypes.c_void_p * len(addresses))(*addresses)
        self._sizes = (ctypes.c_uint64 * len(sizes))(*sizes)
        self._basic = (ctypes.c_char_p * len(basic))(*basic)
        self._sk = (ctypes.c_char_p * len(sk))(*sk)
        self._output = output
        self._diagnostic = f'resource={value.resource_id[:12]}, entry={value.kernel_entry}, output={value.output_path}'
        self.request = AclrtcKernelSpecRequest()
        request = self.request
        request.resourceId, request.kernelEntry = resource, entry
        request.argsCount = len(sizes)
        request.argsAddr = self._addresses if sizes else None
        request.argsBytes = self._sizes if sizes else None
        request.options, request.optionCount = self._basic if basic else None, len(basic)
        request.skOptions, request.skOptionCount = self._sk if sk else None, len(sk)

    def invoke(self, library_path):
        """调用 aclrtcKernelSpecialization；成功返回 0，失败转换为异常。"""
        path = Path(library_path)
        if not path.is_absolute():
            raise SpecializationError('aclrtc', 'library_path must be explicit and absolute')
        try:
            self._library = ctypes.CDLL(str(path))
            function = self._library.aclrtcKernelSpecialization
            function.argtypes = [ctypes.POINTER(AclrtcKernelSpecRequest), ctypes.c_char_p]
            function.restype = ctypes.c_int
            result = function(ctypes.byref(self.request), self._output)
        except (OSError, AttributeError, ctypes.ArgumentError) as exc:
            raise SpecializationError('aclrtc', f'{exc}; {self._diagnostic}') from exc
        if result != 0:
            raise SpecializationError('aclrtc', f'aclrtcKernelSpecialization returned {result}; {self._diagnostic}')
        return result
