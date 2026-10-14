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
    """从dumpjson采集节点顶层提取运行信息，校验后返回其深拷贝字典。

    必需字段缺失或值不合法时失败；可选字段仅在存在时复制。
    这里只解析已有 tiling 结果，不重新执行 tiling，也不读取独立文件。
    """
    missing = [name for name in RUN_INFO_FIELDS if name not in node]
    if missing:
        raise ValueError('missing collected top-level runtime fields: ' + ', '.join(missing))
    result = {name: copy.deepcopy(node[name]) for name in RUN_INFO_FIELDS}
    result.update({name: copy.deepcopy(node[name]) for name in OPTIONAL_RUN_INFO_FIELDS if name in node})
    get_static_op_tiling(result)
    return result


def _validate_collection(node, record):
    """校验两份采集节点的描述和必需运行字段是否一致。

    描述字段先通过 normalize 统一别名；必需运行字段按原始值比较，
    可选运行字段不参与一致性比较。成功无返回值，不一致则抛出异常。
    """
    for key in ('op_type', 'inputs', 'outputs', 'attrs', 'deterministic',
                'deterministic_level', 'impl_mode', 'platform_info'):
        if normalize(node.get(key)) != normalize(record.get(key)):
            raise ValueError('collected resource association differs for ' + key)
    left, right = _runtime_fields(node), _runtime_fields(record)
    if any(left[name] != right[name] for name in RUN_INFO_FIELDS):
        raise ValueError('collected runtime fields differ between node descriptions')


def normalize(value):
    """递归统一描述符字段别名及 float/float32 写法，供上下文比较使用。

    返回规范化后的值，不修改输入；保留列表顺序和动态 Tensor 组嵌套结构。
    同一字段的多个别名对应不同值时抛出异常，避免静默覆盖。
    """
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
    """校验单 shape 编译描述与原始采集节点的 tiling 上下文是否匹配。

    比较输入输出、属性、确定性和实现模式，并检查目标 SoC 及显式指定的核数。
    collected 表示已关联另一份采集记录，此时允许采集节点缺少 soc_version；
    已提供的 soc_version 仍须匹配 soc。成功无返回值，校验失败则抛出异常。
    """
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
    """按同一采集 PID 和完全相同的节点文件名关联动态资源记录。

    origin 为采集 JSON 路径，node 为其内容。对于 opcompile/selected 目录，
    校验存在的原始 opcompile 记录及 debug 记录，返回匹配的 debug 字典，
    供调用方读取 bin_info。不识别目录或缺少 debug 文件时返回原 node。
    不跨采集批次或按算子类型搜索；关联记录不一致时抛出异常。
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
    """将单 shape OPC 输入描述适配为 JIT worker 所需的编译上下文。

    输入为编译描述文件路径 opc_json 和目标芯片 soc。通过 origin_path 读取
    采集节点顶层运行信息，再关联同 PID、同文件名的 debug 记录获取 bin_info。
    拒绝非空测试资源覆盖附件，并校验采集上下文及算子类型。

    返回 shape_config、op_type、dynamic_elf、dynamic_json、run_info 及
    selected_from_collection；run_info 是内存字典，不是独立 run_info.json。
    相对 bin_info 按 origin 所在目录解析，动态 JSON 使用 ELF 的同名 .json；
    本函数不读取动态资源内容，也不执行编译。
    """
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
