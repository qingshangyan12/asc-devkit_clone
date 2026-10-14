"""校验普通特化编译选项，并复用 CANN 解析器生成 ACLGraph SK 选项及标志。"""

from dataclasses import dataclass
from threading import RLock
from types import SimpleNamespace
from .specialization_request import encode_compile_options


@dataclass(frozen=True)
class SpecializationOptions:
    basic: tuple
    super_kernel: tuple
    enable_super_kernel: bool
    metadata_flags: tuple = ()


# 这些标志属于本次编译调用，不能沿用之前 worker 任务的状态。
_SK_FLAGS = {
    'sub_operator_early_start_set_flag': 'ascendc_sub_super_kernel_early_start_set_flag',
    'sub_operator_early_start_wait_flag': 'ascendc_sub_super_kernel_early_start_wait_flag',
    'sub_operator_call_dcci_before_kernel_start': 'ascendc_sub_super_kernel_call_dcci_before_kernel_start',
    'sub_operator_call_dcci_after_kernel_end': 'ascendc_sub_super_kernel_call_dcci_after_kernel_end',
    'sub_operator_call_dcci_disable_on_kernel': 'ascendc_sub_super_kernel_call_dcci_disable_on_kernel',
    'sub_op_with_sync_all': 'ascendc_op_with_syncall',
}
_HELPER_LOCK = RLock()


def _read_extra_settings(shape):
    """统一解析配置，禁止重复 key 掩盖真实的 SK 模式。"""
    entries = shape.get('extra_settings', [])
    if not isinstance(entries, list):
        raise ValueError('extra_settings must be a list')
    settings = {}
    for entry in entries:
        if not isinstance(entry, dict) or not isinstance(entry.get('key'), str):
            raise ValueError('extra_settings entries require a string key')
        key = entry['key']
        if key in settings:
            raise ValueError('duplicate OPC extra setting: ' + key)
        if key in ('super_kernel_sub_info', 'super_kernel_sub_combine') and entry.get('type', 'addition') != 'addition':
            raise ValueError('SK settings must be context additions')
        settings[key] = entry.get('value')
    return settings


def is_aclgraph_sk(shape):
    """供父调度和 worker 共用：仅接受明确的 ACLGraph combine SK 配置。

    不根据 relocatable 参数或 options 名称猜测模式；配置格式错误交由
    调用方记录并走 OPC，缺少 combine 或配置矛盾时不尝试 ACLGraph JIT。
    """
    settings = _read_extra_settings(shape)
    if settings.get('super_kernel_sub_combine') is not True:
        return False
    # 位置和启用状态必须由上游明确交付，不能用默认值把不完整配置判为 ACLGraph。
    sk = settings.get('super_kernel_sub_info')
    if not isinstance(sk, dict):
        return False
    return sk.get('enable_super_kernel') is True and sk.get('super_kernel_sub_loc') == 'middle'


def _generate_sk_options(sk_info, tiling, resolved, op_type):
    """返回 ACLGraph SK argv 及静态 JSON 标志，调用后恢复编译器全局状态。"""
    from asc_op_compile_base.common.context.op_context import OpContext
    from asc_op_compile_base.asc_op_compiler.super_kernel_option_parse import parse_super_kernel_options
    from asc_op_compile_base.asc_op_compiler.super_kernel_sub_op_compile import gen_sub_super_kernel_compile_options
    from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage

    metadata = resolved.metadata if resolved is not None else {}
    # JIT 的 DCCI 配置按算子类型匹配；此处仅为 SK 选项生成器提供匹配目标，
    # 不改变静态二进制文件名或 ELF 入口名称。
    kernel_name = op_type
    if not isinstance(kernel_name, str) or not kernel_name:
        raise ValueError('SK options require the current task operator type')
    text = sk_info.get('super_kernel_options') or ''
    if not isinstance(text, str):
        raise ValueError('super_kernel_options must be a string')
    with _HELPER_LOCK, OpContext('static') as context:
        context.add_addition('super_kernel_sub_combine', True)
        context.add_addition('super_kernel_sub_info', sk_info)
        parsed = parse_super_kernel_options(text)
        early = parsed.get('early-start') == '1' and parsed.get('debug-sync-all') != '1'
        capabilities = []
        for key in ('super_kernel_early_start_set_flag', 'super_kernel_early_start_wait_flag'):
            value = metadata.get(key)
            if early and not isinstance(value, bool):
                raise ValueError('early-start requires verified resource capability: ' + key)
            capabilities.append(value if isinstance(value, bool) else False)
        info = SimpleNamespace(
            kernel_name=kernel_name,
            op_type=op_type,
            super_kernel_info={'sp_options': parsed},
            super_kernel_early_start_set_flag=capabilities[0],
            super_kernel_early_start_wait_flag=capabilities[1],
        )
        compile_options = SimpleNamespace(compile_options=[])
        static_tiling = SimpleNamespace(block_num=tiling.block_dim, static_shape_flag=True)
        saved = {key: storage.get_variable(key) for key in _SK_FLAGS.values()}
        try:
            for key in saved:
                storage.set_variable(key, False)
            sync_all = metadata.get('sub_op_with_sync_all', False)
            if not isinstance(sync_all, bool):
                raise ValueError('invalid resource sync-all metadata')
            storage.set_variable('ascendc_op_with_syncall', sync_all)
            gen_sub_super_kernel_compile_options(compile_options, static_tiling, info)
            flags = []
            for key, value in _SK_FLAGS.items():
                flags.append((key, storage.get_variable(value)))
            flags = tuple(flags)
        finally:
            for key, value in saved.items():
                storage.set_variable(key, value)
    argv = ['--enable-super-kernel']
    argv.extend(compile_options.compile_options)
    argv = tuple(argv)
    encode_compile_options(argv, 'sk_options')
    return argv, flags


def build_specialization_options(shape, enable_sk, tiling, *, resolved=None, op_type=None):
    """组装普通选项及可选的 ACLGraph SK 选项，返回 SpecializationOptions。"""
    settings = _read_extra_settings(shape)
    supported = {'super_kernel_sub_info', 'super_kernel_sub_combine', '_op_aicore_num', '_op_vectorcore_num'}
    if set(settings) - supported:
        raise ValueError('unsupported OPC extra settings: ' + repr(sorted(set(settings) - supported)))
    sk = settings.get('super_kernel_sub_info', {}) or {}
    if not isinstance(sk, dict):
        raise ValueError('super_kernel_sub_info must be an object')
    if shape.get('deterministic') not in ('true', 'false'):
        raise ValueError('deterministic must be explicit')
    basic = shape.get('additional_compile_options', ())
    encode_compile_options(basic, 'basic_options')
    if '--enable-super-kernel' in basic:
        raise ValueError('SK enable must be supplied through the SK configuration')
    if not enable_sk:
        return SpecializationOptions(tuple(basic), (), False)
    # 本后端面向现有 ACLGraph 资源包装入口。GE 包装入口以及源码生成的
    # 对齐/拆分变化需要独立的资源支持。
    if not is_aclgraph_sk(shape):
        raise ValueError('SK specialization requires the ACLGraph combine resource path')
    argv, flags = _generate_sk_options(sk, tiling, resolved, op_type)
    return SpecializationOptions(tuple(basic), argv, True, flags)
