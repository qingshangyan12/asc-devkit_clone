"""集中适配目标平台及 OPC 的芯片规则；导入本模块不会初始化编译平台。"""

from threading import RLock


CORE_COUNT_FIELDS = (
    ('_op_aicore_num', 'cube_core_cnt', 'cubeCoreCnt'),
    ('_op_vectorcore_num', 'vector_core_cnt', 'vectorCoreCnt'),
)

_CHIP_VERSION_LOCK = RLock()


def initialize_platform(soc_version):
    """在隔离 worker 中初始化两套平台库，返回目标芯片的短名称。"""
    from tbe.common.platform import platform_info as tbe_platform
    from asc_op_compile_base.common.platform import platform_info as asc_platform

    tbe_platform.set_current_compile_soc_info(soc_version)
    asc_platform.set_current_compile_soc_info(soc_version)
    return asc_platform.get_soc_spec('SHORT_SOC_VERSION')


def validate_collected_platform(shape, recorded, soc):
    """纯数据检查采集目标及显式核数，不查询或初始化平台库。"""
    if recorded.get('soc_version') is not None and recorded['soc_version'] != soc:
        raise ValueError('tiling context SoC differs')
    extras = {item['key']: item.get('value') for item in shape.get('extra_settings', [])}
    platform = recorded.get('platform_info', {})
    for key, field, _ in CORE_COUNT_FIELDS:
        actual = extras.get(key)
        if actual is not None and str(actual) != str(platform.get(field)):
            raise ValueError('tiling context differs for ' + field)


def validate_resource_platform(metadata, info, shape, soc, short_soc):
    """校验资源声明的完整或短 SoC 名称与请求目标一致。"""
    for owner in (metadata, info, shape, metadata.get('platformInfo', {})):
        if not isinstance(owner, dict):
            raise ValueError('invalid resource platformInfo')
        for field in ('soc', 'socVersion', 'soc_version'):
            if field in owner and str(owner[field]).lower() not in {soc.lower(), (short_soc or soc).lower()}:
                raise ValueError('resource SoC does not match requested target')


def get_chip_version(short_soc):
    """返回 c220/c310 等编译架构标识，复用 OPC 查询并在结束时恢复内部状态。"""
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


def apply_core_platform_metadata(metadata, core_type):
    """按 worker 已初始化的目标平台更新纯核 magic 和 core_type，复用 OPC 规则。"""
    from asc_op_compile_base.common.platform.platform_info import get_soc_spec
    from asc_op_compile_base.asc_op_compiler.cce_runtime import _add_kernel_magic, _add_core_type

    metadata.pop('core_type', None)
    metadata.update(_add_kernel_magic(get_soc_spec('SHORT_SOC_VERSION'), core_type))
    core_info = {'core_type': {'VectorCore': 'AIV', 'AiCore': 'AIC'}[core_type]}
    return _add_core_type(metadata, core_info, None)
