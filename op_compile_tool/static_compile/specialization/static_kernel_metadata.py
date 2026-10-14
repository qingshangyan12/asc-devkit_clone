"""Static metadata synthesis from actual output symbols and existing OPC rules."""
import copy
import hashlib
from pathlib import Path
import struct
import sys
from .elf_metadata import ElfMetadata
from .argument_binding import dynamic_parameter_mode, MIX_CORES, resolve_load_symbols


def _static_key(shape):
    # Installed OPC imports sibling modules as absolute names. Reuse its algorithm.
    import opc_tool
    folder = str(Path(opc_tool.__file__).parent)
    if folder not in sys.path:
        sys.path.insert(0,folder)
    from single_op_post_compile import SingleOpPostCompile
    from constant import OpcOptions, OpcCompileMode
    helper = SingleOpPostCompile({OpcOptions.OP_COMPILE_MODE:OpcCompileMode.SINGLE_OP_CONFIG_FILE_MODE},
                                 False,False,'0')
    info = {k:copy.deepcopy(shape.get(k)) for k in ('inputs','outputs','attrs','impl_mode')}
    info['status_check'] = shape.get('status_check','true')
    return helper._SingleOpPostCompile__generate_static_key({},info)


def _sk_bind_present(elf):
    data = elf.sections.get('.ascend.meta',b'')
    at = 0
    while at+4 <= len(data):
        kind,size = struct.unpack_from('<HH',data,at)
        at += 4
        if at+size > len(data):
            raise ValueError('truncated SK metadata TLV')
        if kind == 5 and size == 52:
            return True
        at += size
    return False


def build_static_kernel_metadata(dynamic_metadata, shape_config, tiling, output_elf,
                                 output_basename, kernel_entry, argument_count, options,
                                 resolved=None, op_type=None, log_level='info'):
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
    keep = ('coreType','magic','core_type','intercoreSync','taskRation','memoryStamping',
            'parameters','optionalInputMode','optionalOutputMode','feature_list')
    result = {k:copy.deepcopy(source[k]) for k in keep if k in source}
    if dynamic_metadata.get('supportInfo', {}).get('int64Mode') is not False:
        raise ValueError('int64 mode is unsupported')
    if source.get('coreType') not in ('VectorCore','AiCore','MIX'):
        raise ValueError('unsupported core type')
    if result.get('taskRation') == 'tilingKey':
        raise ValueError('unresolved per-entry task ratio')
    if not isinstance(result.get('parameters'),list):
        raise ValueError('missing runtime parameter metadata')
    # OPC _init_param_value uses logical inputs + outputs + workspace.
    # Null optional descriptors retain positions; FFTS and tiling are not entries.
    if any(not isinstance(shape_config.get(k), list) for k in ('inputs', 'outputs')):
        raise ValueError('missing static parameter descriptors')
    expected_parameters = len(shape_config['inputs']) + len(shape_config['outputs']) + 1
    if len(result['parameters']) != expected_parameters:
        raise ValueError('runtime parameter initialization count differs from descriptors')
    # Nontrivial output/workspace initialization needs an explicit generation rule. Do not
    # infer atomic fill values from the tiling flag or silently drop them.
    if any(value is not None for value in result['parameters']):
        raise ValueError('nontrivial initialization metadata requires OPC')
    if not tiling.clear_atomic:
        result['parameters'] = [None] * len(result['parameters'])
    result.update(binFileName=output_basename,binFileSuffix='.o',
                  sha256=hashlib.sha256(Path(output_elf).read_bytes()).hexdigest(),
                  kernelName=kernel_entry if source['coreType']=='MIX' else resolved.load_symbol,
                  blockDim=tiling.block_dim,opParaSize=resolved.read_bytes+8,
                  workspace={'num':len(tiling.workspaces),'size':list(tiling.workspaces),
                             'type':[0]*len(tiling.workspaces)},
                  localMemorySize=tiling.local_memory_size)
    if tiling.schedule_mode:
        result['schedule_mode'] = tiling.schedule_mode
    # aicpu_block_dim and tiling_cond are collected but not consumed in this version.
    support = {k:copy.deepcopy(shape_config.get(k,[])) for k in ('inputs','outputs','attrs')}
    support.update(opMode='static',implMode=shape_config.get('impl_mode',''),int64Mode=False,
                   staticKey=_static_key(shape_config))
    if log_level == 'debug':
        support['op_debug_config'] = 'dump_cce'
    # Preserve optional placeholders because specialized dynamic resources retain ABI slots.
    for key in ('optionalInputMode','optionalOutputMode'):
        if key in result:
            support[key] = result[key]
    mode = dynamic_parameter_mode(dynamic_metadata, source)
    if mode:
        result['dynamicParamMode'] = mode
        support['dynamicParamMode'] = mode
    result['supportInfo'] = support
    settings = {v['key']:v.get('value') for v in shape_config.get('extra_settings',[])}
    platform = copy.deepcopy(dynamic_metadata.get('platformInfo',{}))
    for key,field in (('_op_aicore_num','cubeCoreCnt'),('_op_vectorcore_num','vectorCoreCnt')):
        if key in settings:
            platform[field] = int(settings[key])
    result['platformInfo'] = platform
    if source.get('deterministic') in ('true','false'):
        if source['deterministic'] != shape_config['deterministic']:
            raise ValueError('resource deterministic variant differs from request')
        result['deterministic'] = source['deterministic']
    if options.enable_super_kernel:
        sk_symbols = tuple(symbol + '_static_sk' for symbol in load_symbols)
        required = {name for symbol in sk_symbols
                    for name in (symbol, *(symbol + '_split' + str(i) for i in range(1, 4)))}
        if not required <= elf.functions or not _sk_bind_present(elf):
            raise ValueError('ELF does not supply requested SK_BIND and split functions')
        if not resolved.argument_names:
            raise ValueError('missing verified SK wrapper parameter names')
        # Every declared physical core must provide all four SK variants.
        # The shared fat object supplies them; no fictitious split files are emitted.
        if source['coreType'] == 'MIX':
            from asc_op_compile_base.asc_op_compiler.ascendc_common_utility import CommonUtility
            prefix = 'dav-' + CommonUtility.get_chip_version() + '-'
            ratio = source['taskRation']
            arches = tuple(prefix + {'aic': 'cube', 'aiv': 'vec'}[core]
                           for core in MIX_CORES[ratio])
            kernel_type = {'0:1': 'KERNEL_TYPE_MIX_AIV_1_0',
                           '1:0': 'KERNEL_TYPE_MIX_AIC_1_0',
                           '1:1': 'KERNEL_TYPE_MIX_AIC_1_1',
                           '1:2': 'KERNEL_TYPE_MIX_AIC_1_2'}[ratio]
        else:
            arches = ('AiCore',)
            kernel_type = {'VectorCore': 'KERNEL_TYPE_AIV_ONLY',
                           'AiCore': 'KERNEL_TYPE_AIC_ONLY'}[source['coreType']]
        kernel_names = {arch: {'func_name': symbol, 'obj_files': output_basename + '.o'}
                        for arch, symbol in zip(arches, sk_symbols)}
        result.update(supportSuperKernel=1,split_mode=4,sub_operator_op_type=op_type,
                      sub_operator_params=list(resolved.argument_names),
                      sub_operator_kernel_type=kernel_type,
                      sub_operator_kernel_name=kernel_names,
                      sub_operator_early_start_set_flag=False,sub_operator_early_start_wait_flag=False,
                      sub_operator_call_dcci_before_kernel_start=False,
                      sub_operator_call_dcci_after_kernel_end=False,
                      sub_operator_call_dcci_disable_on_kernel=False,sub_op_with_sync_all=False)
        # Use the very same original-helper result that generated request argv.
        result.update(dict(getattr(options, 'metadata_flags', ())))
    else:
        result['supportSuperKernel'] = 0
    return result
