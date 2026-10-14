"""Resolve per-node files without adding ABI fields to the resource producer."""
import copy
import json
import re
from pathlib import Path


def normalize(value):
    if isinstance(value, list):
        return [normalize(x) for x in value]
    if not isinstance(value, dict):
        return value
    aliases = {'origin_shape':'ori_shape', 'origin_format':'ori_format', 'param_type':'paramType'}
    result = {aliases.get(k,k): normalize(v) for k,v in value.items()}
    if result.get('dtype') == 'float':
        result['dtype'] = 'float32'
    return result


def validate_context(shape, recorded, soc, collected=False):
    for key in ('inputs','outputs','attrs','deterministic','impl_mode'):
        if normalize(shape.get(key)) != normalize(recorded.get(key)):
            raise ValueError('tiling context differs for ' + key)
    if (recorded.get('soc_version') is None and not collected) or (
            recorded.get('soc_version') is not None and recorded['soc_version'] != soc):
        raise ValueError('tiling context SoC differs')
    extras = {x['key']:x.get('value') for x in shape.get('extra_settings',[])}
    platform = recorded.get('platform_info',{})
    for key,field in (('_op_aicore_num','cube_core_cnt'),('_op_vectorcore_num','vector_core_cnt')):
        actual = extras.get(key)
        if actual is not None and str(actual) != str(platform.get(field)):
            raise ValueError('tiling context differs for ' + field)


def collected_resource(origin, node):
    """Join only the same collection PID and exact node filename.

    Existing ACLNN dumps put bin_info in <pid>_debug while opcompile_selected
    contains the compile description. Never search other runs or by op type.
    """
    match = re.fullmatch(r'(\d+)_opcompile(?:_selected)?', origin.parent.name)
    if not match:
        return node
    debug = origin.parent.parent / (match[1] + '_debug') / origin.name
    if not debug.is_file():
        return node
    record = json.loads(debug.read_text())
    for key in ('op_type', 'inputs', 'outputs', 'attrs', 'deterministic',
                'deterministic_level', 'impl_mode', 'platform_info'):
        if normalize(node.get(key)) != normalize(record.get(key)):
            raise ValueError('collected resource association differs for ' + key)
    return record


def adapt_shape_config(opc_json, soc):
    path = Path(opc_json).resolve()
    doc = json.loads(path.read_text())
    if len(doc.get('op_list',[])) != 1:
        raise ValueError('specialization requires exactly one shape')
    shape = copy.deepcopy(doc['op_list'][0])
    origin = Path(shape.get('origin_path', path)).resolve()
    # Paths may be attached to the existing node JSON, or a node-named directory.
    origin_doc = json.loads(origin.read_text()) if origin != path else shape
    location = origin.parent / (origin.stem + '.specialization')
    attached = origin_doc.get('specialization', {})
    if not isinstance(attached,dict):
        raise ValueError('specialization must contain file locations')
    def file(name, default):
        value = Path(attached.get(name,default))
        return str(value if value.is_absolute() else origin.parent/value)
    run_info = file('run_info', location/'run_info.json')
    context = file('context', location/'shape_context.json')
    if Path(context).is_file():
        # Optional validation fixture, not a required upstream dump addition.
        recorded = json.loads(Path(context).read_text())
        if not recorded.get('dynamic_sha256'):
            raise ValueError('fixture context must bind the dynamic ELF SHA256')
    else:
        # The existing node descriptor owns these files; reject shape expansion
        # which no longer matches the node that produced the tiling.
        if 'context' in attached:
            raise ValueError('explicit tiling context file is missing: ' + context)
        recorded = copy.deepcopy(origin_doc)
        # Target identity must come from the producer, not the current CLI.
    resource_doc = collected_resource(origin, origin_doc) if not attached.get('dynamic_elf') else origin_doc
    # Legacy live dumps omit exact SoC: use the CLI target with a verified
    # same-collection association. Standalone fixtures still require SoC.
    validate_context(shape, recorded, soc, collected=resource_doc is not origin_doc)
    dynamic = attached.get('dynamic_elf', resource_doc.get('bin_info'))
    if not isinstance(dynamic,str):
        raise ValueError('missing dynamic ELF location (bin_info)')
    elf = Path(dynamic)
    if not elf.is_absolute():
        elf = origin.parent/elf
    metadata = file('dynamic_json',elf.with_suffix('.json'))
    source = attached.get('kernel_source')
    if source:
        source = file('kernel_source',source)
    return dict(op_type=doc['op_type'], shape_config=shape, dynamic_elf=str(elf),
                dynamic_json=metadata, run_info=run_info,
                kernel_source=source, context=recorded)
