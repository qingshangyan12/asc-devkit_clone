"""从同节点 debug JSON 读取采集的运行信息和资源。"""

import copy
import json
import re
from pathlib import Path
from .static_op_tiling import RUN_INFO_FIELDS, OPTIONAL_RUN_INFO_FIELDS, _object, get_static_op_tiling


def _reject_json_constant(value):
    raise ValueError('invalid JSON constant ' + value)


def _read_json(path):
    return json.loads(
        Path(path).read_text(encoding='utf-8'), object_pairs_hook=_object, parse_constant=_reject_json_constant
    )


def _runtime_fields(node):
    """从dumpjson采集节点顶层提取运行信息，返回其深拷贝字典。

    必需字段缺失时失败；可选字段仅在存在时复制。
    字段值由调用方通过 get_static_op_tiling 统一解析和校验。
    这里只提取已有 tiling 结果，不重新执行 tiling，也不读取独立文件。
    """
    missing = [name for name in RUN_INFO_FIELDS if name not in node]
    if missing:
        raise ValueError('missing collected top-level runtime fields: ' + ', '.join(missing))
    result = {name: copy.deepcopy(node[name]) for name in RUN_INFO_FIELDS}
    for name in OPTIONAL_RUN_INFO_FIELDS:
        if name in node:
            result[name] = copy.deepcopy(node[name])
    return result


def normalize(value):
    """递归统一描述符字段别名及 float/float32 写法，供上下文比较使用。

    返回规范化后的值，不修改输入；保留列表顺序和动态 Tensor 组嵌套结构。
    同一字段的多个别名对应不同值时抛出异常，避免静默覆盖。
    """
    if isinstance(value, list):
        return [normalize(x) for x in value]
    if not isinstance(value, dict):
        return value
    aliases = {
        'origin_shape': 'ori_shape',
        'origin_format': 'ori_format',
        'param_type': 'paramType',
        'const_data': 'const_value',
    }
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


def validate_context(shape, recorded, soc):
    """校验单 shape 编译任务能否复用 debug 节点的 tiling 结果。

    比较输入输出、属性、确定性和实现模式，并检查目标 SoC 及显式指定的核数。
    debug 已由同次采集路径关联；其中存在 soc_version 时必须匹配目标。
    成功无返回值，校验失败则抛出异常，由外层回退 OPC 重新计算 tiling。
    """
    for key in ('inputs', 'outputs', 'attrs', 'deterministic', 'deterministic_level', 'impl_mode'):
        if normalize(shape.get(key)) != normalize(recorded.get(key)):
            raise ValueError('tiling context differs for ' + key)
    if recorded.get('soc_version') is not None and recorded['soc_version'] != soc:
        raise ValueError('tiling context SoC differs')
    extras = {x['key']: x.get('value') for x in shape.get('extra_settings', [])}
    platform = recorded.get('platform_info', {})
    for key, field in (('_op_aicore_num', 'cube_core_cnt'), ('_op_vectorcore_num', 'vector_core_cnt')):
        actual = extras.get(key)
        if actual is not None and str(actual) != str(platform.get(field)):
            raise ValueError('tiling context differs for ' + field)


def _debug_json_path(origin):
    """按 origin 的采集 PID 和完整节点名定位唯一 debug JSON。

    origin 可以指向 opcompile、opcompile_selected 或 debug 目录；这里只用
    路径关联，不读取 selected/opcompile 内容，不跨批次或按算子类型搜索。
    无法可靠定位或 debug 不存在时失败，不从其他描述补齐采集字段。
    """
    match = re.fullmatch(r'(\d+)_(?:opcompile(?:_selected)?|debug)', origin.parent.name)
    if not match:
        raise ValueError('cannot locate same-node debug JSON from origin_path')
    debug = origin.parent.parent / (match[1] + '_debug') / origin.name
    if not debug.is_file():
        raise ValueError('missing same-node debug JSON: ' + str(debug))
    return debug


def adapt_shape_config(opc_json, soc):
    """将单 shape OPC 输入描述适配为 JIT worker 所需的编译上下文。

    输入为编译描述文件路径 opc_json 和目标芯片 soc。当前任务提供 shape、
    产物名及编译选项；通过 origin_path 定位同 PID、同文件名的 debug JSON，
    统一读取顶层运行信息及 bin_info，校验任务上下文。

    返回 shape_config、op_type、dynamic_elf、dynamic_json、run_info 及
    selected_from_collection；run_info 是内存字典，不是独立 run_info.json。
    相对 bin_info 按 debug 所在目录解析，动态 JSON 使用 ELF 的同名 .json；
    本函数不读取动态资源内容，也不执行编译。
    """
    path = Path(opc_json).resolve()
    doc = _read_json(path)
    if len(doc.get('op_list', [])) != 1:
        raise ValueError('specialization requires exactly one shape')
    shape = copy.deepcopy(doc['op_list'][0])
    origin = Path(shape.get('origin_path', path)).resolve()
    debug_path = _debug_json_path(origin)
    resource_doc = _read_json(debug_path)
    run_info = _runtime_fields(resource_doc)
    validate_context(shape, resource_doc, soc)
    if resource_doc.get('op_type') != doc['op_type']:
        raise ValueError('job operator differs from collected node')
    dynamic = resource_doc.get('bin_info')
    if not isinstance(dynamic, str) or not dynamic:
        raise ValueError('missing dynamic ELF location (bin_info)')
    elf = Path(dynamic)
    if not elf.is_absolute():
        elf = debug_path.parent / elf
    selected_from_collection = resource_doc.get('bin_type') == 1
    return dict(
        selected_from_collection=selected_from_collection,
        op_type=doc['op_type'],
        shape_config=shape,
        dynamic_elf=str(elf),
        dynamic_json=str(elf.with_suffix('.json')),
        run_info=run_info,
    )


def preflight_shape_config(opc_json, soc):
    """启动 worker 前排除采集字段、资源文件、入口匹配及 optional 布局冲突。

    复用正式输入适配及运行字段校验；这里只读取 JSON 和检查文件存在性，
    不解析 ELF、不构造 request、不调用 aclrtc。worker 仍会重新读取并校验。
    """
    from .argument_binding import resolve_entry, validate_optional_arguments

    inputs = adapt_shape_config(opc_json, soc)
    for field, label in (('dynamic_elf', 'dynamic ELF'), ('dynamic_json', 'dynamic JSON')):
        if not Path(inputs[field]).is_file():
            raise ValueError('missing ' + label + ': ' + inputs[field])
    metadata = _read_json(inputs['dynamic_json'])
    if not isinstance(metadata, dict):
        raise ValueError('dynamic JSON must be an object')
    tiling = get_static_op_tiling(inputs['run_info'])
    resolve_entry(metadata, tiling.kernel_name)
    validate_optional_arguments(metadata, inputs['shape_config'])
