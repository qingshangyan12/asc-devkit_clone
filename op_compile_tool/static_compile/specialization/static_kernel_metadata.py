"""根据实际输出符号和现有 OPC 规则合成静态元数据。"""

import copy
import hashlib
from pathlib import Path
import sys
from threading import RLock
from .elf_metadata import ElfMetadata
from .argument_binding import dynamic_parameter_mode, MIX_CORES, resolve_load_symbols

_CHIP_VERSION_LOCK = RLock()


def _get_chip_version(short_soc):
    """显式指定目标芯片，复用 OPC 架构规则，调用后恢复编译器内部状态。

    编译器查询仍依赖全局变量，因此仅在本适配函数内临时设置；正常返回和
    异常退出都恢复旧值，不重置其他 SK 状态。锁保护本函数的并发及重入。
    """
    if not isinstance(short_soc, str) or not short_soc:
        raise ValueError('MIX SK metadata requires an explicit short_soc')
    from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage as storage
    from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import CommonUtility

    with _CHIP_VERSION_LOCK:
        previous = storage.get_variable('ascendc_short_soc_version')
        try:
            storage.set_variable('ascendc_short_soc_version', short_soc)
            return CommonUtility.get_chip_version()
        finally:
            storage.set_variable('ascendc_short_soc_version', previous)


def _static_key(shape):
    # 已安装的 OPC 以绝对名称导入同级模块；这里复用相同算法。
    import opc_tool

    folder = str(Path(opc_tool.__file__).parent)
    if folder not in sys.path:
        sys.path.insert(0, folder)
    from single_op_post_compile import SingleOpPostCompile
    from constant import OpcOptions, OpcCompileMode

    helper = SingleOpPostCompile(
        {OpcOptions.OP_COMPILE_MODE: OpcCompileMode.SINGLE_OP_CONFIG_FILE_MODE}, False, False, '0'
    )
    info = {k: copy.deepcopy(shape.get(k)) for k in ('inputs', 'outputs', 'attrs', 'impl_mode')}
    info['status_check'] = shape.get('status_check', 'true')
    return helper._SingleOpPostCompile__generate_static_key({}, info)


def _validate_runtime_parameters(dynamic_metadata, source, result, shape_config):
    """确认运行参数布局与逻辑输入输出槽位一致。"""
    if dynamic_metadata.get('supportInfo', {}).get('int64Mode') is not False:
        raise ValueError('int64 mode is unsupported')
    if source.get('coreType') not in ('VectorCore', 'AiCore', 'MIX'):
        raise ValueError('unsupported core type')
    if result.get('taskRation') == 'tilingKey':
        raise ValueError('unresolved per-entry task ratio')
    if not isinstance(result.get('parameters'), list):
        raise ValueError('missing runtime parameter metadata')
    # OPC 的 _init_param_value 使用逻辑输入、输出和 workspace。
    # NULL optional descriptor 仍保留槽位；FFTS 和 tiling 不属于这些参数项。
    if any(not isinstance(shape_config.get(k), list) for k in ('inputs', 'outputs')):
        raise ValueError('missing static parameter descriptors')
    expected_parameters = len(shape_config['inputs']) + len(shape_config['outputs']) + 1
    if len(result['parameters']) != expected_parameters:
        raise ValueError('runtime parameter initialization count differs from descriptors')
    # 非简单的输出/workspace 初始化需要明确的生成规则。
    # 不能从 tiling 标志推断 atomic 填充值，也不能静默丢弃这些信息。
    if any(value is not None for value in result['parameters']):
        raise ValueError('nontrivial initialization metadata requires OPC')


def _build_support_info(shape_config, result, log_level):
    """沿用 OPC 静态上下文，并保留 optional 槽位。"""
    support = {k: copy.deepcopy(shape_config.get(k, [])) for k in ('inputs', 'outputs', 'attrs')}
    support.update(
        opMode='static',
        implMode=shape_config.get('impl_mode', ''),
        int64Mode=False,
        staticKey=_static_key(shape_config),
    )
    if log_level == 'debug':
        support['op_debug_config'] = 'dump_cce'
    # 保留 optional 占位符，因为特化动态资源仍保留 ABI 槽位。
    for key in ('optionalInputMode', 'optionalOutputMode'):
        if key in result:
            support[key] = result[key]
    return support


def _build_platform_info(dynamic_metadata, shape_config):
    settings = {v['key']: v.get('value') for v in shape_config.get('extra_settings', [])}
    platform = copy.deepcopy(dynamic_metadata.get('platformInfo', {}))
    for key, field in (('_op_aicore_num', 'cubeCoreCnt'), ('_op_vectorcore_num', 'vectorCoreCnt')):
        if key in settings:
            platform[field] = int(settings[key])
    return platform


def _super_kernel_arches(source, short_soc):
    if source['coreType'] == 'MIX':
        prefix = 'dav-' + _get_chip_version(short_soc) + '-'
        ratio = source['taskRation']
        arches = tuple(prefix + {'aic': 'cube', 'aiv': 'vec'}[core] for core in MIX_CORES[ratio])
        kernel_type = {
            '0:1': 'KERNEL_TYPE_MIX_AIV_1_0',
            '1:0': 'KERNEL_TYPE_MIX_AIC_1_0',
            '1:1': 'KERNEL_TYPE_MIX_AIC_1_1',
            '1:2': 'KERNEL_TYPE_MIX_AIC_1_2',
        }[ratio]
    else:
        arches = ('AiCore',)
        kernel_type = {'VectorCore': 'KERNEL_TYPE_AIV_ONLY', 'AiCore': 'KERNEL_TYPE_AIC_ONLY'}[source['coreType']]
    return arches, kernel_type


def _apply_super_kernel_metadata(result, source, elf, load_symbols, options, output_basename, op_type, short_soc):
    sk_symbols = tuple(symbol + '_static_sk' for symbol in load_symbols)
    required = set()
    for symbol in sk_symbols:
        required.add(symbol)
        for index in range(1, 4):
            required.add(symbol + '_split' + str(index))
    if not required <= elf.functions:
        raise ValueError('ELF does not supply requested SK_BIND and split functions')
    flags = dict(getattr(options, 'metadata_flags', ()))
    capability = (
        int(bool(flags.get('sub_operator_early_start_wait_flag')))
        | (int(bool(flags.get('sub_operator_early_start_set_flag'))) << 1)
        | (int(bool(flags.get('sub_operator_call_dcci_disable_on_kernel'))) << 2)
    )
    elf.validate_sk_bind(load_symbols, capability)
    # 每个物理核提供四个 SK 变体，共用 fat 对象。
    arches, kernel_type = _super_kernel_arches(source, short_soc)
    kernel_names = {}
    for arch, symbol in zip(arches, sk_symbols):
        kernel_names[arch] = {'func_name': symbol, 'obj_files': output_basename + '.o'}
    result.update(
        supportSuperKernel=1,
        split_mode=4,
        sub_operator_op_type=op_type,
        sub_operator_kernel_type=kernel_type,
        sub_operator_kernel_name=kernel_names,
        sub_operator_early_start_set_flag=False,
        sub_operator_early_start_wait_flag=False,
        sub_operator_call_dcci_before_kernel_start=False,
        sub_operator_call_dcci_after_kernel_end=False,
        sub_operator_call_dcci_disable_on_kernel=False,
        sub_op_with_sync_all=False,
    )
    # 使用生成 request argv 的同一个原始 helper 结果。
    result.update(dict(getattr(options, 'metadata_flags', ())))


def build_static_kernel_metadata(
    dynamic_metadata,
    shape_config,
    tiling,
    output_elf,
    output_basename,
    kernel_entry,
    options,
    resolved=None,
    op_type=None,
    log_level='info',
    *,
    short_soc=None,
):
    if resolved is None:
        raise ValueError('verified kernel layout is required')
    elf = ElfMetadata(output_elf)
    load_symbols = getattr(resolved, 'load_symbols', ()) or (resolved.load_symbol,)
    if not set(load_symbols) <= elf.functions:
        raise ValueError('specialized ELF is missing the verified load symbol')
    source = resolved.metadata
    if source.get('coreType') == 'MIX':
        expected = resolve_load_symbols(kernel_entry, source, elf.functions)
        if set(expected) != set(load_symbols):
            raise ValueError('specialized MIX load symbols differ from verified resource')
    keep = (
        'coreType',
        'magic',
        'core_type',
        'intercoreSync',
        'taskRation',
        'memoryStamping',
        'parameters',
        'optionalInputMode',
        'optionalOutputMode',
        'feature_list',
    )
    result = {}
    for key in keep:
        if key in source:
            result[key] = copy.deepcopy(source[key])
    _validate_runtime_parameters(dynamic_metadata, source, result, shape_config)
    if not tiling.clear_atomic:
        result['parameters'] = [None] * len(result['parameters'])
    result.update(
        binFileName=output_basename,
        binFileSuffix='.o',
        sha256=hashlib.sha256(Path(output_elf).read_bytes()).hexdigest(),
        kernelName=kernel_entry if source['coreType'] == 'MIX' else resolved.load_symbol,
        blockDim=tiling.block_dim,
        opParaSize=resolved.read_bytes + 8,
        workspace={
            'num': len(tiling.workspaces),
            'size': list(tiling.workspaces),
            'type': [0] * len(tiling.workspaces),
        },
        localMemorySize=tiling.local_memory_size,
    )
    if tiling.schedule_mode:
        result['schedule_mode'] = tiling.schedule_mode
    # 本版本采集 aicpu_block_dim 和 tiling_cond，但暂不消费。
    support = _build_support_info(shape_config, result, log_level)
    mode = dynamic_parameter_mode(dynamic_metadata, source)
    if mode:
        result['dynamicParamMode'] = mode
        support['dynamicParamMode'] = mode
    result['supportInfo'] = support
    result['platformInfo'] = _build_platform_info(dynamic_metadata, shape_config)
    if source.get('deterministic') in ('true', 'false'):
        if source['deterministic'] != shape_config['deterministic']:
            raise ValueError('resource deterministic variant differs from request')
        result['deterministic'] = source['deterministic']
    if options.enable_super_kernel:
        _apply_super_kernel_metadata(result, source, elf, load_symbols, options, output_basename, op_type, short_soc)
    else:
        result['supportSuperKernel'] = 0
    return result
