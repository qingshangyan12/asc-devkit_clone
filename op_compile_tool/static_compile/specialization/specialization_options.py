"""Reuse CANN's ACLGraph parser and static SK option generator."""
from dataclasses import dataclass
from threading import RLock
from types import SimpleNamespace
from .specialization_request import _options


@dataclass(frozen=True)
class SpecializationOptions:
    basic: tuple
    super_kernel: tuple
    enable_super_kernel: bool
    metadata_flags: tuple = ()


# These flags belong to this compile invocation, not a previous worker job.
_SK_FLAGS = {
    'sub_operator_early_start_set_flag': 'ascendc_sub_super_kernel_early_start_set_flag',
    'sub_operator_early_start_wait_flag': 'ascendc_sub_super_kernel_early_start_wait_flag',
    'sub_operator_call_dcci_before_kernel_start': 'ascendc_sub_super_kernel_call_dcci_before_kernel_start',
    'sub_operator_call_dcci_after_kernel_end': 'ascendc_sub_super_kernel_call_dcci_after_kernel_end',
    'sub_operator_call_dcci_disable_on_kernel': 'ascendc_sub_super_kernel_call_dcci_disable_on_kernel',
    'sub_op_with_sync_all': 'ascendc_op_with_syncall',
}
# Reset both enabled and disabled states before applying the original generator.
# Otherwise a macro baked into the resource's argv could survive a request to disable it.
_SK_MACROS = (
    '__ASCENDC_ENABLE_SUPER_KERNEL__', '__SUPER_KERNEL_STATIC_BLOCK_NUM__',
    '__SUPER_KERNEL_DYNAMIC_BLOCK_NUM__', '__ASCENDC_SUPERKERNEL_AUTO_SYNC_ALL__',
    '__ASCENDC_SUPER_KERNEL_DEBUG__', '__ASCENDC_SUPERKERNEL_EARLY_START_V1',
    '__ASCENDC_SUPERKERNEL_EARLY_START_V2', '__ASCENDC_SUPERKERNEL_EARLY_START_V3',
    '__ASCENDC_ENABLE_SET_NEXT_TASK_START', '__ASCENDC_ENABLE_WAIT_PRE_TASK_END',
    '__ASCENDC_SUPER_KERNEL_DISABLE_DCCI__',
    '__ASCENDC_SUPER_KERNEL_ENABLE_GM_GET_SET_VALUE_DCCI__',
)
_HELPER_LOCK = RLock()


def _generate_sk_options(shape, sk_info, tiling, resolved, op_type):
    from asc_op_compile_base.common.context.op_context import OpContext
    from asc_op_compile_base.asc_op_compiler.super_kernel_option_parse import parse_super_kernel_options
    from asc_op_compile_base.asc_op_compiler.super_kernel_sub_op_compile import gen_sub_super_kernel_compile_options
    from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage

    metadata = resolved.metadata if resolved is not None else {}
    kernel_name = shape.get('bin_filename')
    if not isinstance(kernel_name, str) or not kernel_name:
        raise ValueError('SK options require the current task kernel name')
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
            if early and type(value) is not bool:
                raise ValueError('early-start requires verified resource capability: ' + key)
            capabilities.append(value if type(value) is bool else False)
        info = SimpleNamespace(kernel_name=kernel_name, op_type=op_type,
                               super_kernel_info={'sp_options': parsed},
                               super_kernel_early_start_set_flag=capabilities[0],
                               super_kernel_early_start_wait_flag=capabilities[1])
        compile_options = SimpleNamespace(compile_options=[])
        static_tiling = SimpleNamespace(block_num=tiling.block_dim, static_shape_flag=True)
        saved = {key:storage.get_variable(key) for key in _SK_FLAGS.values()}
        try:
            for key in saved:
                storage.set_variable(key, False)
            sync_all = metadata.get('sub_op_with_sync_all', False)
            if type(sync_all) is not bool:
                raise ValueError('invalid resource sync-all metadata')
            storage.set_variable('ascendc_op_with_syncall', sync_all)
            gen_sub_super_kernel_compile_options(compile_options, static_tiling, info)
            flags = tuple((key, storage.get_variable(value)) for key, value in _SK_FLAGS.items())
        finally:
            for key, value in saved.items():
                storage.set_variable(key, value)
    argv = ('--enable-super-kernel', *('-U'+name for name in _SK_MACROS),
            *compile_options.compile_options)
    _options(argv, 'sk_options')
    return argv, flags


def build_specialization_options(shape, enable_sk, tiling, *, resolved=None, op_type=None):
    settings = {s['key']:s.get('value') for s in shape.get('extra_settings', [])}
    supported = {'super_kernel_sub_info', 'super_kernel_sub_combine', '_op_aicore_num', '_op_vectorcore_num'}
    if set(settings) - supported:
        raise ValueError('unsupported OPC extra settings: '+repr(sorted(set(settings)-supported)))
    sk = settings.get('super_kernel_sub_info', {}) or {}
    if not isinstance(sk, dict):
        raise ValueError('super_kernel_sub_info must be an object')
    if shape.get('deterministic') not in ('true', 'false'):
        raise ValueError('deterministic must be explicit')
    basic = shape.get('additional_compile_options', ())
    _options(basic, 'basic_options')
    if '--enable-super-kernel' in basic:
        raise ValueError('SK enable must be supplied through the SK configuration')
    if not enable_sk:
        return SpecializationOptions(tuple(basic), (), False)
    # This backend targets the existing ACLGraph resource wrapper. GE wrappers
    # and source-generated alignment/split changes need their own resource support.
    if settings.get('super_kernel_sub_combine') is not True:
        raise ValueError('SK specialization requires the ACLGraph combine resource path')
    if sk.get('super_kernel_sub_loc', 'middle') != 'middle':
        raise ValueError('unsupported SK sub-operator location')
    argv, flags = _generate_sk_options(shape, sk, tiling, resolved, op_type)
    return SpecializationOptions(tuple(basic), argv, True, flags)
