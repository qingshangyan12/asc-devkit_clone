"""单 shape 特化 worker；此处不允许调用 OPC。"""

import argparse
import json
import os
from pathlib import Path
import traceback
from dataclasses import dataclass
from .shape_config_adapter import adapt_shape_config
from .resource_id_reader import read_resource_id
from .static_op_tiling import get_static_op_tiling
from .argument_binding import resolve_kernel_metadata, bind_arguments, validate_resource_context, resolve_entry
from .specialization_options import build_specialization_options
from .specialization_request import SpecializationInput, OwnedSpecializationRequest
from .static_kernel_metadata import build_static_kernel_metadata


@dataclass(frozen=True)
class StaticKernelSpecializationJob:
    opc_json: str
    output_dir: str
    soc_version: str
    enable_sk: bool = False
    log_level: str = 'info'


def specialize_static_kernel(job):
    # 与 OPC 保持一致：在此隔离 worker 中初始化两个平台库。
    from tbe.common.platform import platform_info as tbe_platform
    from asc_op_compile_base.common.platform import platform_info as asc_platform

    tbe_platform.set_current_compile_soc_info(job.soc_version)
    asc_platform.set_current_compile_soc_info(job.soc_version)
    short_soc = asc_platform.get_soc_spec('SHORT_SOC_VERSION')
    inputs = adapt_shape_config(job.opc_json, job.soc_version)
    tiling = get_static_op_tiling(inputs['run_info'])
    resource = read_resource_id(inputs['dynamic_elf'])
    metadata = json.loads(Path(inputs['dynamic_json']).read_text())
    selected = resolve_entry(metadata, tiling.kernel_name, tiling.tiling_key)
    validate_resource_context(
        metadata,
        inputs['shape_config'],
        inputs['op_type'],
        job.soc_version,
        selected_kernel=selected,
        selected_from_collection=inputs['selected_from_collection'],
        short_soc=short_soc,
    )
    resolved = resolve_kernel_metadata(inputs['dynamic_elf'], metadata, tiling)
    arguments = bind_arguments(resolved, tiling)
    options = build_specialization_options(
        inputs['shape_config'], job.enable_sk, tiling, resolved=resolved, op_type=inputs['op_type']
    )
    basename = inputs['shape_config']['bin_filename']
    output = Path(job.output_dir) / (basename + '.o')
    output.parent.mkdir(parents=True, exist_ok=True)
    request = OwnedSpecializationRequest(
        SpecializationInput(resource, tiling.kernel_name, arguments, options.basic, options.super_kernel, str(output))
    )
    cann = Path(os.environ.get('ASCEND_HOME_PATH', ''))
    if not cann.is_absolute():
        raise ValueError('ASCEND_HOME_PATH must identify the installed toolkit')
    # aclrtcKernelSpecialization 成功返回 0；invoke 已将非零 aclError 转为异常。
    # 显式保存状态码，说明只有特化成功后才可以继续生成配套静态 JSON。
    aclrtc_status = request.invoke(str(cann / 'lib64/libacl_rtc.so'))
    if aclrtc_status != 0:  # 防御性检查，覆盖未来 invoke 契约变化。
        raise RuntimeError(f'aclrtcKernelSpecialization failed with status {aclrtc_status}')
    result = build_static_kernel_metadata(
        metadata,
        inputs['shape_config'],
        tiling,
        str(output),
        basename,
        tiling.kernel_name,
        options,
        resolved,
        inputs['op_type'],
        job.log_level,
        short_soc=short_soc,
    )
    json_path = output.with_suffix('.json')
    json_path.write_text(json.dumps(result, sort_keys=True, indent=2))
    return str(output), str(json_path)


def main():
    parser = argparse.ArgumentParser()
    for key in ('input', 'stage', 'soc', 'result'):
        parser.add_argument('--' + key, required=True)
    parser.add_argument('--enable-sk', action='store_true')
    parser.add_argument('--log-level', default='info')
    args = parser.parse_args()
    try:
        pair = specialize_static_kernel(
            StaticKernelSpecializationJob(args.input, args.stage, args.soc, args.enable_sk, args.log_level)
        )
        report = {'success': True, 'artifacts': pair}
        rc = 0
    except Exception as error:
        traceback.print_exc()
        report = {
            'success': False,
            'stage': getattr(error, 'stage', 'specialization'),
            'error_type': type(error).__name__,
            'message': str(error),
        }
        rc = 1
    Path(args.result).write_text(json.dumps(report, indent=2))
    return rc


if __name__ == '__main__':
    raise SystemExit(main())
