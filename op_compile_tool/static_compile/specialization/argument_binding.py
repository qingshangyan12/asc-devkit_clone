"""Bind existing compiler argument layout, not a guessed tensor count."""
from dataclasses import dataclass
import hashlib
from pathlib import Path
import re
from .elf_metadata import ElfMetadata, ElfMetadataError
from .specialization_request import KernelArgument

# Consumer allocation guard, not a kernel ABI or aclrtc interface limit.
MAX_REQUEST_ARGUMENTS = 4096


def validate_resource_context(metadata, shape, op_type, soc, selected_kernel=None):
    """Bind an existing dynamic JSON descriptor to a concrete compile request.

    Explicitly identified resources use their own ordinary tensor schema.
    Legacy resources without operator identity retain the two validated schemas.
    Ascend910B4 is the first backend's supported target. Some compiler JSONs
    omit SoC identity; the caller must independently bind their ELF to the
    target. Explicit SoC declarations, when present, are checked here.
    """
    schemas = {
        'DynamicQuant': ((('x', 'required'), ('smooth_scales', 'optional'),
                          ('group_index', 'optional')),
                         (('y', 'required'), ('scale', 'required')),
                         (('dst_type', 'int'),)),
        'MoeGatingTopKSoftmaxV2': ((('x', 'required'), ('finished', 'optional')),
                                 (('y', 'required'), ('expert_idx', 'required'),
                                  ('softmax_result', 'optional')),
                                 (('k', 'int'), ('renorm', 'int'),
                                  ('output_softmax_result_flag', 'bool'))),
    }
    if soc != 'Ascend910B4':
        raise ValueError('unsupported resource operator or SoC')
    if not isinstance(metadata, dict) or not isinstance(shape, dict):
        raise ValueError('resource and shape context must be dictionaries')
    info = metadata.get('supportInfo')
    if not isinstance(info, dict) or info.get('opMode') != 'dynamic':
        raise ValueError('resource must have dynamic supportInfo')
    identities = []
    for owner in (metadata, info, shape):
        for field in ('op_type', 'opType', 'sub_operator_op_type'):
            if field in owner:
                identities.append(owner[field])
    keys = info.get('simplifiedKey', [])
    if not isinstance(keys, list) or any(not isinstance(key, str) or '/' not in key for key in keys):
        raise ValueError('invalid resource simplified keys')
    identities.extend(key.split('/', 1)[0] for key in keys)
    # simplified_key_mode=1 omits names; in that format the exact descriptor
    # and attribute schemas below identify the supported operator.
    if any(identity != op_type for identity in identities):
        raise ValueError('resource operator identity mismatch')
    if op_type not in schemas or (metadata.get('op_type') == op_type and
            all(isinstance(x, dict) for category in ('inputs', 'outputs')
                for x in info.get(category, []))):
        # Non-legacy resources must explicitly identify their operator. The
        # request's own op_type is not independent resource evidence.
        resource_identities = [owner[field] for owner in (metadata, info)
                               for field in ('op_type', 'opType', 'sub_operator_op_type')
                               if field in owner]
        resource_identities.extend(key.split('/', 1)[0] for key in keys)
        if not resource_identities:
            raise ValueError('resource operator identity is missing')
        tensor_schemas = []
        for category in ('inputs', 'outputs'):
            descriptors = info.get(category)
            if not isinstance(descriptors, list):
                raise ValueError('resource descriptors are missing')
            schema = []
            for tensor in descriptors:
                if not isinstance(tensor, dict) or not isinstance(tensor.get('name'), str):
                    raise ValueError('custom resource needs explicit tensor descriptors')
                param_type = tensor.get('paramType')
                if param_type not in ('required', 'optional'):
                    raise ValueError('unsupported resource parameter kind')
                schema.append((tensor['name'], param_type))
            if len({name for name, _ in schema}) != len(schema):
                raise ValueError('duplicate resource tensor name')
            tensor_schemas.append(tuple(schema))
        attributes = info.get('attrs')
        if not isinstance(attributes, list) or any(
                not isinstance(a, dict) or not isinstance(a.get('name'), str)
                or a.get('dtype') not in ('int', 'bool') for a in attributes):
            raise ValueError('unsupported resource attribute schema')
        schemas[op_type] = (*tensor_schemas, tuple((a['name'], a['dtype']) for a in attributes))
    for owner in (metadata, info, shape, metadata.get('platformInfo', {})):
        if not isinstance(owner, dict):
            raise ValueError('invalid resource platformInfo')
        for field in ('soc', 'socVersion', 'soc_version'):
            if field in owner and str(owner[field]).lower() not in ('ascend910b', 'ascend910b4'):
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

    for category, schema in zip(('inputs', 'outputs'), schemas[op_type][:2]):
        expected, actual = info.get(category), shape.get(category)
        if not isinstance(expected, list) or not isinstance(actual, list) or len(expected) != len(schema) or len(actual) != len(schema):
            raise ValueError(category + ' descriptor count mismatch')
        for index, (resource, tensor, (name, param_type)) in enumerate(zip(expected, actual, schema)):
            label = f'{category}[{index}]'
            if resource is None:
                if param_type != 'optional' or tensor is not None:
                    raise ValueError(label + ' resource omits a requested tensor')
                continue
            if not isinstance(resource, dict) or resource.get('name') != name or resource.get('index') != index or resource.get('paramType') != param_type:
                raise ValueError(label + ' resource position or optionality mismatch')
            if tensor is None:
                if param_type != 'optional':
                    raise ValueError(label + ' required tensor is absent')
                continue
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

    # Static ACLNN callers omit absent optional pointers. A dynamic placeholder
    # resource therefore cannot be installed under a static key for that shape.
    for category in ('inputs', 'outputs'):
        if any(a is None and b is not None for a, b in zip(shape[category], info[category])):
            raise ValueError('resource ABI retains an optional pointer omitted by the static runtime')

    resource_attrs, actual_attrs = info.get('attrs'), shape.get('attrs')
    attr_schema = schemas[op_type][2]
    if not isinstance(resource_attrs, list) or not isinstance(actual_attrs, list) or len(resource_attrs) != len(attr_schema) or len(actual_attrs) != len(attr_schema):
        raise ValueError('attribute count mismatch')
    for index, (resource, actual, (name, attr_type)) in enumerate(zip(resource_attrs, actual_attrs, attr_schema)):
        if not isinstance(resource, dict) or not isinstance(actual, dict):
            raise ValueError('invalid attribute descriptor')
        if resource.get('name') != name or actual.get('name') != name or resource.get('dtype') != attr_type or actual.get('dtype') != attr_type:
            raise ValueError('attribute name or dtype mismatch')
        if actual.get('index', index) != index or resource.get('index', index) != index:
            raise ValueError('attribute index mismatch')
        if resource.get('value_range') or resource.get('value_list'):
            raise ValueError('attribute range/list resources are unsupported')
        if type(actual.get('value')) is not (int if attr_type == 'int' else bool):
            raise ValueError('attribute value type mismatch')
        if resource.get('value') is not None and (type(resource['value']) is not type(actual['value']) or resource['value'] != actual['value']):
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
    if shape.get('impl_mode', '') != info.get('implMode', ''):
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


def get_kernel_argument_layout(metadata, effective):
    """Reuse CANN's ordinary pointer-argument ABI generator.

    Runs in the isolated specialization worker. No manifest, ABI sidecar, or
    array-length guess is needed. Missing optional descriptors stay missing.
    """
    from types import SimpleNamespace
    from asc_op_compile_base.asc_op_compiler.compile_op import _gen_kernel_func_declare_head
    from asc_op_compile_base.asc_op_compiler.ascendc_compile_dfx import DFXSectionGenerator
    from asc_op_compile_base.asc_op_compiler.global_storage import global_var_storage
    info = metadata['supportInfo']
    op = SimpleNamespace(
        inputs=[dict(x, param_name=x['name']+'_in__') if x else None for x in info['inputs']],
        outputs=[dict(x, param_name=x['name']+'_out_') if x else None for x in info['outputs']],
        mc2_ctx=[], param_type_list=[], output_shape_depend_on_compute=[])
    tiling = SimpleNamespace(static_shape_flag=False, static_workspace_size=0)
    keys = ('ascendc_short_soc_version', 'ascendc_enable_super_kernel')
    saved = {k: global_var_storage.get_variable(k) for k in keys}
    dfx = DFXSectionGenerator()
    saved_dfx = dict(dfx.__dict__)
    try:
        global_var_storage.set_variable(keys[0], 'Ascend910B')
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


def resolve_kernel_metadata(dynamic_elf, metadata, tiling, kernel_source=None):
    item = resolve_entry(metadata,tiling.kernel_name,tiling.tiling_key)
    if metadata.get('sha256') != hashlib.sha256(Path(dynamic_elf).read_bytes()).hexdigest():
        raise ValueError('dynamic ELF/JSON hash mismatch')
    effective = {**metadata, **item}
    if effective.get('oom') or effective.get('dynamicParamMode') or effective.get('debugOptions'):
        raise ValueError('unsupported OOM/descriptor/debug argument ABI')
    elf = ElfMetadata(dynamic_elf)
    candidates = [name for name in (tiling.kernel_name, tiling.kernel_name+'_mix_aiv',
                                    tiling.kernel_name+'_mix_aic') if name in elf.functions]
    if len(candidates) != 1:
        raise ValueError('first backend requires a single physical load symbol')
    symbol = candidates[0]
    names, index = get_kernel_argument_layout(metadata, effective)
    if kernel_source:
        source = Path(kernel_source).read_text()
        observed = tuple(re.findall(r'GM_ADDR\s+(\w+)\s*=\s*\(GM_ADDR\)param\[__asc_index\+\+\];',source))
        if observed != names or source.count('uint32_t __asc_index = 0;') != 1:
            raise ValueError('generated wrapper differs from compiler argument layout')
    allocation = effective.get('opParaSize')
    count = len(names)
    try:
        arguments = elf.arguments(symbol)
    except ElfMetadataError as error:
        # Missing DFX is normal for SK resources; malformed present DFX fails.
        if '.ascend.meta.' + symbol in elf.sections and 'missing DFX argument metadata' not in str(error):
            raise
    else:
        positions = [i for i,a in enumerate(arguments) if a['kind']==7]
        if positions != [index] or len(arguments) != count:
            raise ValueError('DFX and compiler argument layout disagree')
        if arguments[index]['size'] != allocation:
            raise ValueError('DFX and JSON tiling registration sizes disagree')
    # Existing ordinary registration rule: allocation = tiling struct + key uint64.
    if type(allocation) is not int or allocation-8 != len(tiling.tiling_data):
        raise ValueError('tiling bytes do not match ordinary registered structure size')
    return ResolvedKernelMetadata(tiling.kernel_name,symbol,index,allocation-8,count,names,effective)


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
