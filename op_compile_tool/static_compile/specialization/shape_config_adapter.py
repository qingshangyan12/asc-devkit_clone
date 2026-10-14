"""Resolve per-node files without adding ABI fields to the resource producer."""
import copy
import json
import re
from pathlib import Path
from .static_op_tiling import RUN_INFO_FIELDS, OPTIONAL_RUN_INFO_FIELDS, _object, get_static_op_tiling


def _read_json(path):
    return json.loads(Path(path).read_text(encoding='utf-8'), object_pairs_hook=_object,
                      parse_constant=lambda value: (_ for _ in ()).throw(
                          ValueError('invalid JSON constant ' + value)))


def _runtime_fields(node):
    missing = [name for name in RUN_INFO_FIELDS if name not in node]
    if missing:
        raise ValueError('missing collected top-level runtime fields: ' + ', '.join(missing))
    result = {name: copy.deepcopy(node[name]) for name in RUN_INFO_FIELDS}
    result.update({name: copy.deepcopy(node[name]) for name in OPTIONAL_RUN_INFO_FIELDS if name in node})
    get_static_op_tiling(result)
    return result


def _validate_collection(node, record):
    for key in ('op_type', 'inputs', 'outputs', 'attrs', 'deterministic',
                'deterministic_level', 'impl_mode', 'platform_info'):
        if normalize(node.get(key)) != normalize(record.get(key)):
            raise ValueError('collected resource association differs for ' + key)
    left, right = _runtime_fields(node), _runtime_fields(record)
    if any(left[name] != right[name] for name in RUN_INFO_FIELDS):
        raise ValueError('collected runtime fields differ between node descriptions')


def normalize(value):
    if isinstance(value, list):
        return [normalize(x) for x in value]
    if not isinstance(value, dict):
        return value
    aliases = {'origin_shape':'ori_shape', 'origin_format':'ori_format',
               'param_type':'paramType', 'const_data':'const_value'}
    result = {}
    for key, item in value.items():
        target = aliases.get(key, key)
        normalized = normalize(item)
        if target in result and result[target] != normalized:
            raise ValueError('conflicting descriptor aliases for ' + target)
        result[target] = normalized
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
    original = origin.parent.parent / (match[1] + '_opcompile') / origin.name
    if original != origin and original.is_file():
        _validate_collection(node, _read_json(original))
    debug = origin.parent.parent / (match[1] + '_debug') / origin.name
    if not debug.is_file():
        return node
    record = _read_json(debug)
    _validate_collection(node, record)
    return record


def adapt_shape_config(opc_json, soc):
    path = Path(opc_json).resolve()
    doc = _read_json(path)
    if len(doc.get('op_list',[])) != 1:
        raise ValueError('specialization requires exactly one shape')
    shape = copy.deepcopy(doc['op_list'][0])
    origin = Path(shape.get('origin_path', path)).resolve()
    origin_doc = _read_json(origin) if origin != path else shape
    run_info = _runtime_fields(origin_doc)
    if origin_doc.get('specialization'):
        raise ValueError('test resource overrides are not supported; use collected bin_info')
    resource_doc = collected_resource(origin, origin_doc)
    validate_context(shape, origin_doc, soc, collected=resource_doc is not origin_doc)
    if origin_doc.get('op_type') != doc['op_type']:
        raise ValueError('job operator differs from collected node')
    dynamic = resource_doc.get('bin_info')
    if not isinstance(dynamic, str) or not dynamic:
        raise ValueError('missing dynamic ELF location (bin_info)')
    elf = Path(dynamic)
    if not elf.is_absolute():
        elf = origin.parent / elf
    selected_from_collection = (resource_doc is not origin_doc and resource_doc.get('bin_type') == 1)
    return dict(selected_from_collection=selected_from_collection, op_type=doc['op_type'],
                shape_config=shape, dynamic_elf=str(elf), dynamic_json=str(elf.with_suffix('.json')),
                run_info=run_info)
