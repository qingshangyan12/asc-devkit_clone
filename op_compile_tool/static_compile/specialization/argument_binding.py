"""Bind existing compiler argument layout, not a guessed tensor count."""
from dataclasses import dataclass
import hashlib
import math
from pathlib import Path
from .elf_metadata import ElfMetadata, ElfMetadataError
from .specialization_request import KernelArgument

# Consumer allocation guard, not a kernel ABI or aclrtc interface limit.
MAX_REQUEST_ARGUMENTS = 4096


def dynamic_parameter_mode(metadata, effective=None):
    """Keep descriptor-bearing groups folded, as declared by the producer."""
    info = metadata.get('supportInfo', {})
    owners = (metadata, info, effective or {})
    modes = [owner['dynamicParamMode'] for owner in owners if 'dynamicParamMode' in owner]
    groups = any(isinstance(x, list) for category in ('inputs', 'outputs')
                 for x in info.get(category, []))
    if modes and (any(mode != modes[0] for mode in modes) or
                  modes[0] != 'folded_with_desc'):
        raise ValueError('unsupported or conflicting dynamic parameter mode')
    if groups and not modes:
        raise ValueError('dynamic Tensor groups require folded_with_desc metadata')
    return modes[0] if modes else None


def _group_descriptor(group, index):
    """A nonempty homogeneous group describes one logical parameter."""
    if not group or any(not isinstance(member, dict) for member in group):
        raise ValueError('invalid dynamic Tensor group')
    first = group[0]
    name = first.get('name')
    if not isinstance(name, str) or not name:
        raise ValueError('dynamic Tensor group name is missing')
    for member in group:
        if (member.get('name') != name or member.get('index') != index or
                member.get('paramType') != 'dynamic'):
            raise ValueError('dynamic Tensor group position or parameter kind mismatch')
    return first


_ATTRIBUTE_TYPES = {'int': int, 'bool': bool, 'float': float, 'str': str,
                    'list_int': list, 'list_bool': list, 'list_float': list, 'list_str': list}


def _attribute_type(value):
    # OPC's generic descriptor conversion also rewrites float attributes.
    return 'float' if value == 'float32' else value


def _validate_attribute_value(value, dtype):
    if type(value) is not _ATTRIBUTE_TYPES[dtype]:
        raise ValueError('attribute value type mismatch')
    if dtype.startswith('list_'):
        for element in value:
            _validate_attribute_value(element, dtype[5:])
    elif dtype == 'float' and not math.isfinite(value):
        raise ValueError('attribute float value must be finite')


def validate_resource_context(metadata, shape, op_type, soc, selected_kernel=None, *, selected_from_collection=False, short_soc=None):
    """Bind an existing dynamic JSON descriptor to a concrete compile request.

    All operators use the resource tensor/group and attribute descriptors.
    Targets follow the installed OPC platform database. Some compiler JSONs
    omit SoC identity; the caller must independently bind their ELF to the
    target. Explicit SoC declarations, when present, are checked here.
    """
    if not isinstance(soc, str) or not soc:
        raise ValueError('missing requested SoC')
    if not isinstance(metadata, dict) or not isinstance(shape, dict):
        raise ValueError('resource and shape context must be dictionaries')
    info = metadata.get('supportInfo')
    if not isinstance(info, dict) or info.get('opMode') != 'dynamic':
        raise ValueError('resource must have dynamic supportInfo')
    dynamic_parameter_mode(metadata, selected_kernel)
    identities = []
    for owner in (metadata, info, shape):
        for field in ('op_type', 'opType', 'sub_operator_op_type'):
            if field in owner:
                identities.append(owner[field])
    keys = info.get('simplifiedKey', [])
    if not isinstance(keys, list) or any(not isinstance(key, str) or '/' not in key for key in keys):
        raise ValueError('invalid resource simplified keys')
    identities.extend(key.split('/', 1)[0] for key in keys)
    if any(identity != op_type for identity in identities):
        raise ValueError('resource operator identity mismatch')
    resource_identities = [owner[field] for owner in (metadata, info)
                           for field in ('op_type', 'opType', 'sub_operator_op_type')
                           if field in owner]
    resource_identities.extend(key.split('/', 1)[0] for key in keys)
    # A verified same-node dynamic selection also identifies unnamed resources.
    # The request's op_type alone is not independent resource evidence.
    if not resource_identities and not selected_from_collection:
        raise ValueError('resource operator identity is missing')
    tensor_schemas = []
    for category in ('inputs', 'outputs'):
        descriptors = info.get(category)
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
        tensor_schemas.append(tuple(schema))
    attributes = info.get('attrs')
    if not isinstance(attributes, list) or any(
            not isinstance(a, dict) or not isinstance(a.get('name'), str) or not a['name']
            or _attribute_type(a.get('dtype')) not in _ATTRIBUTE_TYPES for a in attributes):
        raise ValueError('unsupported resource attribute schema')
    if len({a['name'] for a in attributes}) != len(attributes):
        raise ValueError('duplicate resource attribute name')
    attr_schema = tuple((a['name'], _attribute_type(a['dtype'])) for a in attributes)
    for owner in (metadata, info, shape, metadata.get('platformInfo', {})):
        if not isinstance(owner, dict):
            raise ValueError('invalid resource platformInfo')
        for field in ('soc', 'socVersion', 'soc_version'):
            if field in owner and str(owner[field]).lower() not in {soc.lower(), (short_soc or soc).lower()}:
                raise ValueError('resource SoC does not match requested target')

    def dtype(value):
        return {'float': 'float32'}.get(value, value)

    def check_shape(expected, actual, label):
        if not isinstance(expected, list) or not isinstance(actual, list):
            raise ValueError(label + ' shape is missing or invalid')
        if any(type(dim) is not int or dim < 0 for dim in actual):
            raise ValueError(label + ' request must have concrete shape')
        if expected == [-2]:
            return
        if len(expected) != len(actual) or any(type(dim) is not int or dim < -1 for dim in expected):
            raise ValueError(label + ' shape rank or symbolic dimension is invalid')
        if any(dim != -1 and dim != other for dim, other in zip(expected, actual)):
            raise ValueError(label + ' concrete shape mismatch')

    def check_tensor(resource, tensor, name, param_type, index, label):
        if resource is None:
            if tensor is not None:
                raise ValueError(label + ' resource omits a requested tensor')
            return
        if not isinstance(resource, dict) or resource.get('name') != name or resource.get('index') != index or resource.get('paramType') != param_type:
            raise ValueError(label + ' resource position or optionality mismatch')
        if tensor is None:
            if param_type != 'optional':
                raise ValueError(label + ' required tensor is absent')
            return
        if not isinstance(tensor, dict):
            raise ValueError(label + ' tensor descriptor is invalid')
        for field, wanted in (('name', name), ('index', index), ('param_type', param_type), ('paramType', param_type)):
            if field in tensor and tensor[field] != wanted:
                raise ValueError(label + ' position or optionality mismatch')
        if not isinstance(resource.get('dtype'), str) or dtype(resource['dtype']) != dtype(tensor.get('dtype')):
            raise ValueError(label + ' dtype mismatch')
        if not isinstance(resource.get('format'), str) or resource['format'] != tensor.get('format'):
            raise ValueError(label + ' format mismatch')
        check_shape(resource.get('shape'), tensor.get('shape'), label)

    for category, schema in zip(('inputs', 'outputs'), tensor_schemas):
        expected, actual = info.get(category), shape.get(category)
        if not isinstance(expected, list) or not isinstance(actual, list) or len(expected) != len(schema) or len(actual) != len(schema):
            raise ValueError(category + ' descriptor count mismatch')
        for index, (resource, tensor, (name, param_type)) in enumerate(zip(expected, actual, schema)):
            label = f'{category}[{index}]'
            if param_type == 'dynamic':
                _group_descriptor(resource, index)
                if not isinstance(tensor, list) or not tensor:
                    raise ValueError(label + ' dynamic Tensor group must be nonempty')
                if len(resource) not in (1, len(tensor)):
                    raise ValueError(label + ' dynamic Tensor group count mismatch')
                for member_index, member in enumerate(tensor):
                    descriptor = resource[0] if len(resource) == 1 else resource[member_index]
                    check_tensor(descriptor, member, name, param_type, index,
                                 f'{label}[{member_index}]')
            else:
                check_tensor(resource, tensor, name, param_type, index, label)

    # Static ACLNN callers omit absent optional pointers. A dynamic placeholder
    # resource therefore cannot be installed under a static key for that shape.
    for category in ('inputs', 'outputs'):
        if any(a is None and b is not None for a, b in zip(shape[category], info[category])):
            raise ValueError('resource ABI retains an optional pointer omitted by the static runtime')

    resource_attrs, actual_attrs = info.get('attrs'), shape.get('attrs')
    if not isinstance(resource_attrs, list) or not isinstance(actual_attrs, list) or len(resource_attrs) != len(attr_schema) or len(actual_attrs) != len(attr_schema):
        raise ValueError('attribute count mismatch')
    for index, (resource, actual, (name, attr_type)) in enumerate(zip(resource_attrs, actual_attrs, attr_schema)):
        if not isinstance(resource, dict) or not isinstance(actual, dict):
            raise ValueError('invalid attribute descriptor')
        if resource.get('name') != name or actual.get('name') != name or _attribute_type(resource.get('dtype')) != attr_type or _attribute_type(actual.get('dtype')) != attr_type:
            raise ValueError('attribute name or dtype mismatch')
        if actual.get('index', index) != index or resource.get('index', index) != index:
            raise ValueError('attribute index mismatch')
        if resource.get('value_range') or resource.get('value_list'):
            raise ValueError('attribute range/list resources are unsupported')
        _validate_attribute_value(actual.get('value'), attr_type)
        # The same-node dynamic bin_info witnesses selection for these concrete attrs.
        # Dynamic metadata may contain compilation placeholders (e.g. num_experts=0).
        # Without that independently checked association, retain exact value matching.
        if not selected_from_collection and resource.get('value') is not None and (type(resource['value']) is not type(actual['value']) or resource['value'] != actual['value']):
            raise ValueError('attribute value mismatch')
    requested_determinism = shape.get('deterministic', 'false')
    requested_determinism = {True: 'true', False: 'false'}.get(requested_determinism, requested_determinism)
    if requested_determinism not in ('true', 'false'):
        raise ValueError('invalid requested determinism')
    effective = {**metadata, **(selected_kernel or {})}
    declared = [owner['deterministic'] for owner in (effective, info) if 'deterministic' in owner]
    if not declared or any(value not in ('true', 'false', 'ignore') or value not in ('ignore', requested_determinism) for value in declared):
        raise ValueError('resource determinism mismatch or missing')
    requested_level = shape.get('deterministic_level')
    resource_level = metadata.get('platformInfo', {}).get('deterministicLevel')
    if requested_level is not None and resource_level is not None and requested_level != resource_level:
        raise ValueError('resource deterministic level mismatch')
    # OPC's default implementation mode is high_performance.
    if (shape.get('impl_mode') or 'high_performance') != (info.get('implMode') or 'high_performance'):
        raise ValueError('resource implementation mode mismatch')


def resolve_entry(metadata, entry, key):
    kernels = metadata.get('kernelList', [metadata])
    matches = [item for item in kernels if item.get('kernelName') == entry]
    if len(matches) != 1:
        raise ValueError('entry must uniquely match dynamic kernel metadata')
    item = matches[0]
    mapped = item.get('tilingKey')
    if mapped is None:
        # Existing compile option restricts a resource to ONE key and ONE entry.
        # This is an explicit singleton association; do not parse name suffixes.
        keys = metadata.get('supportInfo',{}).get('tilingKey',[])
        if len(kernels) == 1 and isinstance(keys,list) and len(keys) == 1:
            mapped = keys[0]
    if mapped is None:
        # Current AscendC dynamic JSON producer uses base + '_' + logical key.
        # Require both the declared base and the exact kernelList member; never
        # rewrite the caller's entry or override an explicit conflicting key.
        base = metadata.get('kernelName')
        if isinstance(base, str) and base and type(key) is int and entry == base + '_' + str(key):
            mapped = key
    if isinstance(mapped,bool) or mapped is None or str(mapped) != str(key):
        raise ValueError('entry/key mapping is missing or inconsistent')
    return item


@dataclass(frozen=True)
class ResolvedKernelMetadata:
    entry: str
    load_symbol: str
    tiling_index: int
    read_bytes: int
    argument_count: int
    argument_names: tuple
    metadata: dict
    load_symbols: tuple = ()


def get_kernel_argument_layout(metadata, effective):
    """Reuse CANN's pointer-argument ABI generator, retaining folded groups.

    Runs in the isolated specialization worker. No manifest, ABI sidecar, or
    array-length guess is needed. Missing optional descriptors stay missing.
    """
    from types import SimpleNamespace
    from asc_op_compile_base.asc_op_compiler.compile_op import _gen_kernel_func_declare_head
    from asc_op_compile_base.asc_op_compiler.ascendc_compile_dfx import DFXSectionGenerator
    from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage
    info = metadata['supportInfo']
    dynamic_parameter_mode(metadata, effective)

    def compiler_descriptors(category, suffix):
        result = []
        for index, descriptor in enumerate(info[category]):
            if isinstance(descriptor, list):
                # folded_with_desc uses ONE descriptor pointer per group.
                descriptor = _group_descriptor(descriptor, index)
            result.append(dict(descriptor, param_name=descriptor['name'] + suffix)
                          if descriptor is not None else None)
        return result

    op = SimpleNamespace(
        inputs=compiler_descriptors('inputs', '_in__'),
        outputs=compiler_descriptors('outputs', '_out_'),
        mc2_ctx=[], param_type_list=[], output_shape_depend_on_compute=[])
    tiling = SimpleNamespace(static_shape_flag=False, static_workspace_size=0)
    keys = ('ascendc_short_soc_version', 'ascendc_enable_super_kernel')
    saved = {k: global_var_storage.get_variable(k) for k in keys}
    dfx = DFXSectionGenerator()
    saved_dfx = dict(dfx.__dict__)
    try:
        from asc_op_compile_base.common.platform.platform_info import get_soc_spec
        short_soc = get_soc_spec('SHORT_SOC_VERSION')
        if not short_soc:
            raise ValueError('compile platform has no SHORT_SOC_VERSION')
        global_var_storage.set_variable(keys[0], short_soc)
        global_var_storage.set_variable(keys[1], False)
        dfx.dfx_info_reset(op)
        _, workspace, declaration, _ = _gen_kernel_func_declare_head(
            effective.get('coreType')=='MIX', effective.get('intercoreSync')==1, op, tiling)
        names = tuple(name.strip() for name in declaration.split(','))
        if names[-2:] != ('workspace','tiling') or names[workspace] != 'workspace':
            raise ValueError('unsupported compiler argument layout')
        return names, workspace + 1
    finally:
        dfx.__dict__.clear()
        dfx.__dict__.update(saved_dfx)
        for key, value in saved.items():
            global_var_storage.set_variable(key, value)


# OPC's per-key taskRation determines the participating physical cores.
MIX_CORES = {'0:1': ('aiv',), '1:0': ('aic',),
             '1:1': ('aic', 'aiv'), '1:2': ('aic', 'aiv')}


def resolve_load_symbols(entry, effective, functions):
    candidates = {name for name in (entry, entry + '_mix_aic', entry + '_mix_aiv')
                  if name in functions}
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


def resolve_kernel_metadata(dynamic_elf, metadata, tiling):
    item = resolve_entry(metadata,tiling.kernel_name,tiling.tiling_key)
    if metadata.get('sha256') != hashlib.sha256(Path(dynamic_elf).read_bytes()).hexdigest():
        raise ValueError('dynamic ELF/JSON hash mismatch')
    effective = {**metadata, **item}
    dynamic_parameter_mode(metadata, effective)
    if effective.get('oom') or effective.get('debugOptions'):
        raise ValueError('unsupported OOM/descriptor/debug argument ABI')
    elf = ElfMetadata(dynamic_elf)
    symbols = resolve_load_symbols(tiling.kernel_name, effective, elf.functions)
    names, index = get_kernel_argument_layout(metadata, effective)
    capacity = effective.get('opParaSize')
    count = len(names)
    allocations = set()
    for symbol in symbols:
        try:
            arguments = elf.arguments(symbol)
        except ElfMetadataError as error:
            # Missing DFX is normal for SK resources; malformed present DFX fails.
            if '.ascend.meta.' + symbol in elf.sections and 'missing DFX argument metadata' not in str(error):
                raise
        else:
            positions = [i for i, arg in enumerate(arguments) if arg['kind'] == 7]
            if positions != [index] or len(arguments) != count:
                raise ValueError('DFX and compiler argument layout disagree: ' + symbol)
            selected_allocation = arguments[index]['size']
            if (type(selected_allocation) is not int or type(capacity) is not int
                    or not 8 <= selected_allocation <= capacity):
                raise ValueError('DFX tiling allocation exceeds JSON registration capacity')
            allocations.add(selected_allocation)
    if len(allocations) > 1:
        raise ValueError('MIX physical entries disagree on tiling allocation')
    allocation = next(iter(allocations), capacity)
    # Existing ordinary registration rule: allocation = tiling struct + key uint64.
    if type(allocation) is not int or allocation-8 != len(tiling.tiling_data):
        raise ValueError('tiling bytes do not match ordinary registered structure size')
    return ResolvedKernelMetadata(tiling.kernel_name,symbols[0],index,allocation-8,count,names,effective,symbols)


def bind_arguments(resolved, tiling):
    """Bind the upstream tiling slot; retain full runtime metadata separately."""
    index = tiling.tiling_args_index
    if type(index) is not int or not 0 <= index < MAX_REQUEST_ARGUMENTS:
        raise ValueError('tiling parameter index exceeds request allocation limit (4096 slots)')
    if index != resolved.tiling_index:
        raise ValueError('upstream tiling parameter index disagrees with resource layout')
    # Ordinary registration consistency only. This is not a manifest pointer
    # read-size check; pointer byte_size validation remains deferred.
    if len(tiling.tiling_data) != resolved.read_bytes:
        raise ValueError('constant buffer length differs from verified ABI')
    args = [KernelArgument(0, None) for _ in range(index + 1)]
    args[index] = KernelArgument(len(tiling.tiling_data), tiling.tiling_data)
    return tuple(args)
