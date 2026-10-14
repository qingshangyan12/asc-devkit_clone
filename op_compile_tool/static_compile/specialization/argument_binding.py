"""校验动态资源与请求上下文，解析入口元数据，并绑定采集到的 tiling 槽位。"""

from dataclasses import dataclass
import hashlib
import math
from pathlib import Path
from .elf_metadata import ElfMetadata
from .specialization_request import KernelArgument

# 这是消费者侧的分配保护，不是 kernel ABI 或 aclrtc 接口限制。
MAX_REQUEST_ARGUMENTS = 4096


def dynamic_parameter_mode(metadata, effective=None):
    """校验生产端声明的动态 Tensor 参数组模式并返回模式，不修改输入。"""
    info = metadata.get('supportInfo', {})
    owners = (metadata, info, effective or {})
    modes = []
    for owner in owners:
        if 'dynamicParamMode' in owner:
            modes.append(owner['dynamicParamMode'])
    groups = _has_dynamic_groups(info)
    if modes and (any(mode != modes[0] for mode in modes) or modes[0] != 'folded_with_desc'):
        raise ValueError('unsupported or conflicting dynamic parameter mode')
    if groups and not modes:
        raise ValueError('dynamic Tensor groups require folded_with_desc metadata')
    return modes[0] if modes else None


def _has_dynamic_groups(info):
    for category in ('inputs', 'outputs'):
        for descriptor in info.get(category, []):
            if isinstance(descriptor, list):
                return True
    return False


def _group_descriptor(group, index):
    """非空且同质的参数组表示一个逻辑参数。"""
    if not group or any(not isinstance(member, dict) for member in group):
        raise ValueError('invalid dynamic Tensor group')
    first = group[0]
    name = first.get('name')
    if not isinstance(name, str) or not name:
        raise ValueError('dynamic Tensor group name is missing')
    for member in group:
        if member.get('name') != name or member.get('index') != index or member.get('paramType') != 'dynamic':
            raise ValueError('dynamic Tensor group position or parameter kind mismatch')
    return first


_ATTRIBUTE_TYPES = {
    'int': int,
    'bool': bool,
    'float': float,
    'str': str,
    'list_int': list,
    'list_bool': list,
    'list_float': list,
    'list_str': list,
}


def _attribute_type(value):
    # 资源和采集描述可能使用具体类型名，统一为 OPC 属性类型再比较值。
    if value == 'int64':
        return 'int'
    return 'float' if value == 'float32' else value


def _matches_attribute_type(value, dtype):
    if not isinstance(value, _ATTRIBUTE_TYPES[dtype]):
        return False

    # bool 是 int 的子类，但 JSON 布尔属性不能作为整数属性。
    return dtype != 'int' or not isinstance(value, bool)


def _validate_attribute_value(value, dtype):
    if not _matches_attribute_type(value, dtype):
        raise ValueError('attribute value type mismatch')
    if dtype.startswith('list_'):
        for element in value:
            _validate_attribute_value(element, dtype[5:])
    elif dtype == 'float' and not math.isfinite(value):
        raise ValueError('attribute float value must be finite')


def validate_optional_arguments(metadata, shape):
    """拒绝 descriptor 已明确保留、但静态调用方会省略的空 optional 槽位。

    父调度可在不解析 ELF 的情况下调用；未知 descriptor 不用于推断 ABI，
    仍由 worker 完整校验。资源与当前 shape 都为 null 的槽位不构成冲突。
    """
    info = metadata.get('supportInfo')
    if not isinstance(info, dict):
        return
    for category in ('inputs', 'outputs'):
        actual, declared = shape.get(category), info.get(category)
        if not isinstance(actual, list) or not isinstance(declared, list):
            continue
        for tensor, descriptor in zip(actual, declared):
            if tensor is None and descriptor is not None:
                raise ValueError('resource ABI retains an optional pointer omitted by the static runtime')


def _collect_operator_identities(owners):
    identities = []
    for owner in owners:
        for field in ('op_type', 'opType', 'sub_operator_op_type'):
            if field in owner:
                identities.append(owner[field])
    return identities


def _validate_operator_identity(metadata, info, shape, op_type, selected_from_collection):
    identities = _collect_operator_identities((metadata, info, shape))
    keys = info.get('simplifiedKey', [])
    if not isinstance(keys, list):
        raise ValueError('invalid resource simplified keys')
    for key in keys:
        if not isinstance(key, str) or '/' not in key:
            raise ValueError('invalid resource simplified keys')
    key_identities = []
    for key in keys:
        key_identities.append(key.split('/', 1)[0])
    identities.extend(key_identities)
    if any(identity != op_type for identity in identities):
        raise ValueError('resource operator identity mismatch')
    resource_identities = _collect_operator_identities((metadata, info))
    resource_identities.extend(key_identities)
    # 同节点动态选择可以标识未命名资源；request 身份本身不构成资源证据。
    if not resource_identities and not selected_from_collection:
        raise ValueError('resource operator identity is missing')


def _tensor_schema(descriptors):
    if not isinstance(descriptors, list):
        raise ValueError('resource descriptors are missing')
    schema = []
    for index, tensor in enumerate(descriptors):
        if tensor is None:
            schema.append((None, None))
            continue
        if isinstance(tensor, list):
            schema.append((_group_descriptor(tensor, index)['name'], 'dynamic'))
            continue
        if not isinstance(tensor, dict) or not isinstance(tensor.get('name'), str) or not tensor['name']:
            raise ValueError('resource needs explicit tensor descriptors')
        param_type = tensor.get('paramType')
        if param_type not in ('required', 'optional'):
            raise ValueError('unsupported resource parameter kind')
        schema.append((tensor['name'], param_type))
    names = [name for name, _ in schema if name is not None]
    if len(set(names)) != len(names):
        raise ValueError('duplicate resource tensor name')
    return tuple(schema)


def _attribute_schema(attributes):
    if not isinstance(attributes, list):
        raise ValueError('unsupported resource attribute schema')
    for attribute in attributes:
        valid_name = isinstance(attribute, dict) and isinstance(attribute.get('name'), str) and bool(attribute['name'])
        if not valid_name or _attribute_type(attribute.get('dtype')) not in _ATTRIBUTE_TYPES:
            raise ValueError('unsupported resource attribute schema')
    names = {attribute['name'] for attribute in attributes}
    if len(names) != len(attributes):
        raise ValueError('duplicate resource attribute name')
    schema = []
    for attribute in attributes:
        schema.append((attribute['name'], _attribute_type(attribute['dtype'])))
    return tuple(schema)


def _tensor_dtype(value):
    return {'float': 'float32'}.get(value, value)


def _validate_tensor_shape(expected, actual, label):
    if not isinstance(expected, list) or not isinstance(actual, list):
        raise ValueError(label + ' shape is missing or invalid')
    for dim in actual:
        if not isinstance(dim, int) or isinstance(dim, bool) or dim < 0:
            raise ValueError(label + ' request must have concrete shape')
    if expected == [-2]:
        return
    if len(expected) != len(actual):
        raise ValueError(label + ' shape rank or symbolic dimension is invalid')
    for dim in expected:
        if not isinstance(dim, int) or isinstance(dim, bool) or dim < -1:
            raise ValueError(label + ' shape rank or symbolic dimension is invalid')
    for dim, other in zip(expected, actual):
        if dim != -1 and dim != other:
            raise ValueError(label + ' concrete shape mismatch')


def _validate_tensor(resource, tensor, name, param_type, index, label):
    if resource is None:
        if tensor is not None:
            raise ValueError(label + ' resource omits a requested tensor')
        return
    # 资源名称和参数类型已由 schema 验证；这里只补充位置检查。
    if resource.get('index') != index:
        raise ValueError(label + ' resource position or optionality mismatch')
    if tensor is None:
        if param_type != 'optional':
            raise ValueError(label + ' required tensor is absent')
        return
    if not isinstance(tensor, dict):
        raise ValueError(label + ' tensor descriptor is invalid')
    fields = (('name', name), ('index', index), ('param_type', param_type), ('paramType', param_type))
    for field, wanted in fields:
        if field in tensor and tensor[field] != wanted:
            raise ValueError(label + ' position or optionality mismatch')
    if not isinstance(resource.get('dtype'), str) or (
        _tensor_dtype(resource['dtype']) != _tensor_dtype(tensor.get('dtype'))
    ):
        raise ValueError(label + ' dtype mismatch')
    if not isinstance(resource.get('format'), str) or resource['format'] != tensor.get('format'):
        raise ValueError(label + ' format mismatch')
    _validate_tensor_shape(resource.get('shape'), tensor.get('shape'), label)


def _validate_dynamic_group(resource, tensor, name, param_type, index, label):
    if not isinstance(tensor, list) or not tensor:
        raise ValueError(label + ' dynamic Tensor group must be nonempty')
    if len(resource) not in (1, len(tensor)):
        raise ValueError(label + ' dynamic Tensor group count mismatch')
    for member_index, member in enumerate(tensor):
        descriptor = resource[0] if len(resource) == 1 else resource[member_index]
        _validate_tensor(descriptor, member, name, param_type, index, f'{label}[{member_index}]')


def _validate_tensor_context(info, shape, tensor_schemas):
    for category, schema in zip(('inputs', 'outputs'), tensor_schemas):
        expected, actual = info.get(category), shape.get(category)
        if not isinstance(actual, list) or len(actual) != len(schema):
            raise ValueError(category + ' descriptor count mismatch')
        for index, (resource, tensor, (name, param_type)) in enumerate(zip(expected, actual, schema)):
            label = f'{category}[{index}]'
            if param_type == 'dynamic':
                _validate_dynamic_group(resource, tensor, name, param_type, index, label)
            else:
                _validate_tensor(resource, tensor, name, param_type, index, label)


def _validate_attribute(resource, actual, name, attr_type, index, selected_from_collection):
    if not isinstance(actual, dict):
        raise ValueError('invalid attribute descriptor')
    if actual.get('name') != name or _attribute_type(actual.get('dtype')) != attr_type:
        raise ValueError('attribute name or dtype mismatch')
    if actual.get('index', index) != index or resource.get('index', index) != index:
        raise ValueError('attribute index mismatch')
    if resource.get('value_range') or resource.get('value_list'):
        raise ValueError('attribute range/list resources are unsupported')
    _validate_attribute_value(actual.get('value'), attr_type)
    # 已校验的同节点选择允许编译占位值；其余情况仍要求属性值完全匹配。
    if not selected_from_collection and resource.get('value') is not None:
        if not _matches_attribute_type(resource['value'], attr_type) or resource['value'] != actual['value']:
            raise ValueError('attribute value mismatch')


def _validate_attribute_context(info, shape, attr_schema, selected_from_collection):
    resource_attrs, actual_attrs = info.get('attrs'), shape.get('attrs')
    if not isinstance(actual_attrs, list) or len(actual_attrs) != len(attr_schema):
        raise ValueError('attribute count mismatch')
    for index, (resource, actual, (name, attr_type)) in enumerate(zip(resource_attrs, actual_attrs, attr_schema)):
        _validate_attribute(resource, actual, name, attr_type, index, selected_from_collection)


def _validate_execution_context(metadata, info, shape, selected_kernel):
    requested_determinism = shape.get('deterministic', 'false')
    requested_determinism = {True: 'true', False: 'false'}.get(requested_determinism, requested_determinism)
    if requested_determinism not in ('true', 'false'):
        raise ValueError('invalid requested determinism')
    effective = {**metadata, **(selected_kernel or {})}
    declared = []
    for owner in (effective, info):
        if 'deterministic' in owner:
            declared.append(owner['deterministic'])
    if not declared:
        raise ValueError('resource determinism mismatch or missing')
    for value in declared:
        if value not in ('true', 'false', 'ignore') or value not in ('ignore', requested_determinism):
            raise ValueError('resource determinism mismatch or missing')
    requested_level = shape.get('deterministic_level')
    resource_level = metadata.get('platformInfo', {}).get('deterministicLevel')
    if requested_level is not None and resource_level is not None and requested_level != resource_level:
        raise ValueError('resource deterministic level mismatch')
    # OPC 的默认实现模式是 high_performance。
    if (shape.get('impl_mode') or 'high_performance') != (info.get('implMode') or 'high_performance'):
        raise ValueError('resource implementation mode mismatch')


def validate_resource_context(
    metadata, shape, op_type, selected_kernel=None, *, selected_from_collection=False
):
    """将动态 JSON descriptor 绑定到具体编译请求，并按原顺序校验。

    tensor/参数组和属性使用资源声明；目标平台由 worker 的平台准备阶段统一检查。
    调用前由 prepare_specialization_inputs 完成入口、optional 空槽和动态参数组模式检查。
    """
    if not isinstance(metadata, dict) or not isinstance(shape, dict):
        raise ValueError('resource and shape context must be dictionaries')
    info = metadata.get('supportInfo')
    if not isinstance(info, dict) or info.get('opMode') != 'dynamic':
        raise ValueError('resource must have dynamic supportInfo')
    _validate_operator_identity(metadata, info, shape, op_type, selected_from_collection)
    tensor_schemas = []
    for category in ('inputs', 'outputs'):
        tensor_schemas.append(_tensor_schema(info.get(category)))
    attr_schema = _attribute_schema(info.get('attrs'))
    _validate_tensor_context(info, shape, tensor_schemas)
    _validate_attribute_context(info, shape, attr_schema, selected_from_collection)
    _validate_execution_context(metadata, info, shape, selected_kernel)


def resolve_entry(metadata, entry):
    """按采集的完整逻辑入口唯一匹配资源，不要求独立的 tilingKey 字段。"""
    kernels = metadata.get('kernelList', [metadata])
    matches = [item for item in kernels if item.get('kernelName') == entry]
    if len(matches) != 1:
        raise ValueError('entry must uniquely match dynamic kernel metadata')
    return matches[0]


@dataclass(frozen=True)
class ResolvedKernelMetadata:
    load_symbol: str
    # 槽位和数据长度直接采用采集结果，不从 ELF DFX 或资源 opParaSize 推导。
    tiling_index: int
    read_bytes: int
    metadata: dict
    load_symbols: tuple = ()
    # 统一准备阶段已验证的参数组模式，供静态 JSON 直接消费。
    parameter_mode: str | None = None


# OPC 的 per-key taskRation 决定参与执行的物理核。
MIX_CORES = {'0:1': ('aiv',), '1:0': ('aic',), '1:1': ('aic', 'aiv'), '1:2': ('aic', 'aiv')}


def resolve_entry_metadata(metadata, selected):
    """合并并规范化选中入口的资源元数据，按当前 key 确定有效核型。

    编译器在 MIX 容器中用 MIX_AIC + crossCoreSync=0 表示纯核 key；
    它的普通入口没有 _mix 后缀。解析结果同时供入口校验和静态 JSON 使用。
    未声明 per-key 核型的旧资源继续采用根描述，未知或矛盾的描述交 OPC。
    """
    effective = {**metadata, **selected}
    kind = selected.get('kernelType')
    if kind is None:
        return effective
    # 显式纯核类型本身已确定比例，不能继承根容器的 "tilingKey" 占位。
    ratio = selected.get('taskRation') if kind in ('VectorCore', 'AiCore') else effective.get('taskRation')
    sync = selected.get('crossCoreSync')
    if sync is not None:
        if not isinstance(sync, int) or isinstance(sync, bool):
            raise ValueError('invalid per-key crossCoreSync')
        if sync not in (0, 1):
            raise ValueError('invalid per-key crossCoreSync')
    if kind == 'MIX_AIC':
        if ratio not in MIX_CORES:
            raise ValueError('unsupported per-key MIX task ratio')
        if sync == 0:
            core = {'0:1': 'VectorCore', '1:0': 'AiCore'}.get(ratio)
            if core is None:
                raise ValueError('non-sync key requires a single physical core')
        else:
            core = 'MIX'
    elif kind in ('VectorCore', 'AiCore'):
        core = kind
        if sync == 1 or ratio not in (None, {'VectorCore': '0:1', 'AiCore': '1:0'}[core]):
            raise ValueError('pure per-key core disagrees with sync/task ratio')
    else:
        raise ValueError('unsupported per-key kernel type: ' + str(kind))
    if 'coreType' in selected and selected['coreType'] != core:
        raise ValueError('per-key coreType disagrees with kernelType')
    effective['coreType'] = core
    if core == 'MIX':
        if metadata.get('coreType') != 'MIX':
            raise ValueError('MIX key requires a MIX binary container')
        if sync is not None:
            effective['intercoreSync'] = sync
        return effective
    # 多 key 共用的动态桩可能给纯核 key 也保留 FFTS 首参数。individual
    # 静态路径只按 coreType=MIX 补该槽，不能将此资源改成纯核 JSON 后发布。
    # crossCoreSync=0 只描述执行核型，不证明入口没有 FFTS 参数。
    if metadata.get('coreType') == 'MIX' and (
        metadata.get('intercoreSync') != 0 or effective.get('intercoreSync') != 0
    ):
        raise ValueError('pure per-key kernel retains or lacks proof of the MIX FFTS argument ABI; requires OPC')
    # 不继承 MIX 容器的运行信息；magic/core_type 在静态 JSON 生成阶段更新。
    effective.pop('taskRation', None)
    effective['intercoreSync'] = 0
    return effective


def resolve_load_symbols(entry, effective, functions):
    """根据有效核型匹配实际 ELF 入口，返回有序的物理函数名元组。"""
    candidates = {name for name in (entry, entry + '_mix_aic', entry + '_mix_aiv') if name in functions}
    if effective.get('coreType') == 'MIX':
        cores = MIX_CORES.get(effective.get('taskRation'))
        if cores is None:
            raise ValueError('unsupported or unresolved MIX task ratio')
        expected = tuple(entry + '_mix_' + core for core in cores)
        if candidates != set(expected):
            raise ValueError('MIX physical load symbols disagree with declared task ratio')
        return expected
    if candidates != {entry}:
        raise ValueError('non-MIX entry requires a unique physical load symbol')
    return (entry,)


def resolve_kernel_metadata(inputs):
    """统一校验已准备的 shape、动态 JSON 和 ELF，返回参数绑定所需的资源布局。

    inputs 来自 prepare_specialization_inputs。先确认资源支持当前 shape，
    再校验 ELF/JSON 身份和函数实体；tiling 槽位及原始数据以采集结果为准。
    """
    metadata = inputs['metadata']
    validate_resource_context(
        metadata,
        inputs['shape_config'],
        inputs['op_type'],
        selected_kernel=inputs['selected_kernel'],
        selected_from_collection=inputs['selected_from_collection'],
    )
    dynamic_elf = inputs['dynamic_elf']
    tiling = inputs['tiling']
    selected_kernel = inputs['selected_kernel']
    parameter_mode = inputs['parameter_mode']
    if metadata.get('sha256') != hashlib.sha256(Path(dynamic_elf).read_bytes()).hexdigest():
        raise ValueError('dynamic ELF/JSON hash mismatch')
    effective = resolve_entry_metadata(metadata, selected_kernel)
    if effective.get('oom') or effective.get('debugOptions'):
        raise ValueError('unsupported OOM/descriptor/debug argument ABI')
    elf = ElfMetadata(dynamic_elf)
    symbols = resolve_load_symbols(tiling.kernel_name, effective, elf.functions)
    index = tiling.tiling_args_index
    return ResolvedKernelMetadata(symbols[0], index, len(tiling.tiling_data), effective, symbols, parameter_mode)


def bind_arguments(resolved, tiling):
    """绑定上游提供的 tiling 槽位；完整运行时元数据单独保留。"""
    index = resolved.tiling_index
    if not isinstance(index, int) or isinstance(index, bool) or not 0 <= index < MAX_REQUEST_ARGUMENTS:
        raise ValueError('tiling parameter index exceeds request allocation limit (4096 slots)')
    # 上游槽位已包含 FFTS、MC2 等前置参数；直接绑定采集到的原始字节。
    args = [KernelArgument(0, None) for _ in range(index + 1)]
    args[index] = KernelArgument(len(tiling.tiling_data), tiling.tiling_data)
    return tuple(args)
