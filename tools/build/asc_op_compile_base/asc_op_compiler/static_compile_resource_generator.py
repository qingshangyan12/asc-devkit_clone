#!/usr/bin/python3
# coding=utf-8
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import fcntl
import json
import os
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
import re
import shutil
import subprocess
import tempfile
from typing import Optional, Sequence, Tuple

try:
    from .static_compile_resource_id import read_resource_id_section
except ImportError:
    from static_compile_resource_id import read_resource_id_section


_STATIC_VALUE_TOKEN = "@@STATIC_VALUE_tiling_data@@"
_RESOURCE_ID_PATTERN = re.compile(r"^[0-9a-f]{64}$")
_OUTPUT_NAME_PATTERN = re.compile(r"^[A-Za-z0-9_.-]+$")
_LINK_OPTION = ("-m", "aicorelinux", "-r", "-Ttext=0", "-q", "-x")


class ManifestGenerationError(RuntimeError):
    pass


class ManifestIdentityError(ManifestGenerationError):
    pass


class ManifestResourceError(ManifestGenerationError):
    pass


class ManifestCommandError(ManifestGenerationError):
    pass


class ManifestPublishError(ManifestGenerationError):
    pass


@dataclass(frozen=True)
class StaticCompileCommand:
    tiling_key: str
    compiled_symbol: str
    obj_type: str
    core_type: str
    source_path: str
    tiling_path: str
    stage: int
    argv: Tuple[str, ...]
    output_path: str


@dataclass(frozen=True)
class KernelCompileRecord:
    kernel_name: str
    tiling_key: str
    compile_tiling_key: str
    parameter_index: int
    byte_size: int
    tiling_template_path: str
    commands: Tuple[StaticCompileCommand, ...]
    link_option: Tuple[str, ...] = _LINK_OPTION


@dataclass(frozen=True)
class ManifestGenerationRequest:
    soc_version: str
    dynamic_object_path: str
    resource_id: str
    metadata_json_path: str
    source_file_path: str
    origin_func_name: str
    output_root: str
    kernels: Tuple[KernelCompileRecord, ...]
    chip_version: str
    sk_cap_bitmap: Optional[int] = None
    sk_kernel_attribute: str = ""
    basic_kernel_attribute: str = ""
    sk_bind_compile_options: Tuple[str, ...] = ()


@dataclass(frozen=True)
class ManifestGenerationResult:
    manifest_path: str
    resource_id: str
    kernel_count: int
    resource_count: int


def _read_regular_file(path, description):
    if not os.path.isfile(path) or os.path.islink(path):
        raise ManifestResourceError(f"{description} is not a regular file: {path}")
    try:
        return Path(path).read_bytes()
    except OSError as error:
        raise ManifestResourceError(f"failed to read {description}: {path}") from error


def _build_context_commands(
    compile_info,
    compile_option_tuple,
    command_builder,
    chip_version,
    tiling_template_path,
    obj_type,
):
    if obj_type not in {"basic", "sk"}:
        raise ManifestCommandError(f"unsupported object type: {obj_type}")
    try:
        command_specs = command_builder(
            compile_info,
            compile_option_tuple,
            tiling_template_path,
            chip_version,
        )
    except ValueError as error:
        raise ManifestCommandError(str(error)) from error
    commands = []
    for spec in command_specs:
        argv = tuple(spec.argv)
        if not all(isinstance(value, str) and value for value in argv):
            raise ManifestCommandError(
                "static compile argv must contain non-empty strings"
            )
        commands.append(
            StaticCompileCommand(
                tiling_key=spec.tiling_key,
                compiled_symbol=spec.compiled_symbol,
                obj_type=obj_type,
                core_type=spec.core_type,
                source_path=spec.source_path,
                tiling_path=spec.tiling_path,
                stage=0,
                argv=argv,
                output_path=spec.output_path,
            )
        )
    return tuple(commands)


def build_kernel_compile_records(
    *,
    basic_compile_info,
    basic_compile_option_tuple,
    command_builder,
    chip_version: str,
    metadata_json_path: str,
    parameter_index: int,
    tiling_template_path: str,
    tiling_key_data_size: dict,
    default_tiling_size: int,
    tiling_key_group_map: Optional[dict] = None,
    sk_compile_info=None,
    sk_compile_option_tuple=None,
    link_option: Sequence[str] = _LINK_OPTION,
):
    if (sk_compile_info is None) != (sk_compile_option_tuple is None):
        raise ManifestCommandError(
            "SK compile info and options must be provided together"
        )
    if not callable(command_builder):
        raise TypeError("command_builder must be callable")
    if not chip_version:
        raise ManifestCommandError("chip version must be non-empty")
    try:
        metadata = json.loads(Path(metadata_json_path).read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise ManifestIdentityError(
            "failed to read dynamic compile metadata"
        ) from error
    kernel_list = metadata.get("kernelList")
    if not isinstance(kernel_list, list) or not kernel_list:
        raise ManifestIdentityError("dynamic compile metadata has no kernelList")

    all_commands = list(
        _build_context_commands(
            basic_compile_info,
            basic_compile_option_tuple,
            command_builder,
            chip_version,
            tiling_template_path,
            "basic",
        )
    )
    if sk_compile_info is not None:
        all_commands.extend(
            _build_context_commands(
                sk_compile_info,
                sk_compile_option_tuple,
                command_builder,
                chip_version,
                tiling_template_path,
                "sk",
            )
        )
    commands_by_key = {}
    for command in all_commands:
        commands_by_key.setdefault(command.tiling_key, []).append(command)
    group_map = {
        str(master): tuple(str(slave) for slave in slaves)
        for master, slaves in (tiling_key_group_map or {}).items()
    }
    slave_to_master = {
        slave: master for master, slaves in group_map.items() for slave in slaves
    }
    known_keys = set(commands_by_key) | set(slave_to_master)
    exact_sizes = {str(key): value for key, value in tiling_key_data_size.items()}
    compile_keys = set(commands_by_key)
    records = []
    for item in kernel_list:
        if not isinstance(item, dict) or not item.get("kernelName"):
            raise ManifestIdentityError("kernelList contains an invalid entry")
        tiling_key_value = item.get("tilingKey")
        if tiling_key_value is None:
            matches = sorted(
                (key for key in known_keys if item["kernelName"].endswith("_" + key)),
                key=len,
                reverse=True,
            )
            if not matches or (len(matches) > 1 and len(matches[0]) == len(matches[1])):
                raise ManifestIdentityError(
                    f"cannot determine tiling key for {item['kernelName']}"
                )
            tiling_key = matches[0]
        else:
            tiling_key = str(tiling_key_value)
        compile_key = slave_to_master.get(tiling_key, tiling_key)
        commands = commands_by_key.get(compile_key)
        if not commands:
            raise ManifestCommandError(
                f"no static compile command for tiling key {tiling_key}"
            )
        byte_size = exact_sizes.get(compile_key)
        if not isinstance(byte_size, int) or byte_size < 0:
            use_default_size = len(compile_keys) == 1 or (
                not exact_sizes and default_tiling_size == 0
            )
            if use_default_size and isinstance(default_tiling_size, int):
                byte_size = default_tiling_size
            if not isinstance(byte_size, int) or byte_size < 0:
                raise ManifestCommandError(
                    f"exact tiling byte size is unavailable for key {tiling_key}"
                )
        records.append(
            KernelCompileRecord(
                kernel_name=item["kernelName"],
                tiling_key=tiling_key,
                compile_tiling_key=compile_key,
                parameter_index=parameter_index,
                byte_size=byte_size,
                tiling_template_path=tiling_template_path,
                commands=tuple(commands),
                link_option=tuple(link_option),
            )
        )
    return tuple(records)


def static_tiling_template_text(dynamic_header):
    if not isinstance(dynamic_header, str):
        raise ManifestResourceError("dynamic tiling header must be text")
    if not dynamic_header:
        return (
            "#ifndef __ASCENDC_STATIC_TILING_DATA_H__\n"
            "#define __ASCENDC_STATIC_TILING_DATA_H__\n"
            "#ifdef CONST_TILING\n"
            "static constexpr unsigned char __ascendc_static_tiling_data[] = "
            f"{_STATIC_VALUE_TOKEN};\n"
            "#endif\n"
            "#endif\n"
        )
    if _STATIC_VALUE_TOKEN in dynamic_header:
        raise ManifestResourceError(
            "dynamic tiling header already contains static token"
        )
    final_endif = dynamic_header.rfind("#endif")
    if final_endif < 0:
        raise ManifestResourceError("dynamic tiling header has no closing #endif")

    class_names = [
        name
        for name in re.findall(
            r"InitTilingData<([A-Za-z_][A-Za-z0-9_:]*)>", dynamic_header
        )
        if name not in {"T", "tiling_struct", "tiling_type", "type", "Type"}
    ]
    class_name = class_names[0] if class_names else None
    slash = "\\"
    lines = [
        "#ifdef CONST_TILING",
        f"static constexpr uint8_t __ascendc_static_tiling_data[] = {_STATIC_VALUE_TOKEN};",
    ]
    for macro in (
        "GET_TILING_DATA",
        "GET_TILING_DATA_WITH_STRUCT",
        "GET_TILING_DATA_MEMBER",
    ):
        lines.extend([f"#ifdef {macro}", f"#undef {macro}", "#endif"])
    if class_name is None:
        lines.append("#define GET_TILING_DATA(tiling_data, tiling_arg)")
    else:
        lines.extend(
            [
                f"#define GET_TILING_DATA(tiling_data, tiling_arg) {slash}",
                f"    REGISTER_TILINGDATA_SIZE({class_name}, __COUNTER__); {slash}",
                f"    static constexpr {class_name} tiling_data = {slash}",
                f"        convert_from_bytes<{class_name}>(__ascendc_static_tiling_data);",
            ]
        )
    lines.extend(
        [
            f"#define GET_TILING_DATA_WITH_STRUCT(tiling_struct, tiling_data, tiling_arg) {slash}",
            f"    REGISTER_TILINGDATA_SIZE(tiling_struct, __COUNTER__); {slash}",
            f"    static constexpr tiling_struct tiling_data = {slash}",
            "        convert_from_bytes<tiling_struct>(__ascendc_static_tiling_data);",
            f"#define GET_TILING_DATA_MEMBER(tiling_type, member, var, tiling) {slash}",
            f"    REGISTER_TILINGDATA_SIZE(tiling_type, __COUNTER__); {slash}",
            f"    static constexpr tiling_type __ascendc_static_full_##var = {slash}",
            f"        convert_from_bytes<tiling_type>(__ascendc_static_tiling_data); {slash}",
            "    auto& var = __ascendc_static_full_##var.member;",
            "#endif",
            "",
        ]
    )
    result = dynamic_header[:final_endif] + "\n" + "\n".join(lines)
    result += dynamic_header[final_endif:]
    if result.count(_STATIC_VALUE_TOKEN) != 1:
        raise ManifestResourceError("static tiling template must contain one token")
    if _has_absolute_source_directive(result):
        raise ManifestResourceError("static tiling template contains an absolute path")
    return result


def _has_absolute_source_directive(content):
    return bool(
        re.search(r"^\s*#\s*include\s*[<\"]/", content, re.MULTILINE)
        or re.search(r"^\s*#\s*line\s+\d+\s+\"/", content, re.MULTILINE)
    )


def _portable_wrapper_text(source_content, original_source_path):
    try:
        wrapper_text = source_content.decode("utf-8")
    except UnicodeDecodeError as error:
        raise ManifestResourceError("generated wrapper is not UTF-8 text") from error
    absolute_include = f'#include "{original_source_path}"'
    if absolute_include not in wrapper_text:
        raise ManifestResourceError(
            f"generated wrapper does not include original source: {original_source_path}"
        )
    result = wrapper_text.replace(
        absolute_include,
        f'#include "{os.path.basename(original_source_path)}"',
        1,
    )
    if _STATIC_VALUE_TOKEN in result:
        raise ManifestResourceError("static input token must not appear in wrapper")
    if _has_absolute_source_directive(result):
        raise ManifestResourceError("static wrapper contains an absolute path")
    return result


def _logical_path(value, context):
    if not isinstance(value, str) or not value or "\\" in value:
        raise ManifestResourceError(f"{context} is not a portable relative path")
    path = PurePosixPath(value)
    if path.is_absolute() or any(part in {"", ".", ".."} for part in path.parts):
        raise ManifestResourceError(f"{context} is not a portable relative path")
    return path.as_posix()


def _find_tool(*names):
    cann_root = os.environ.get("ASCEND_HOME_PATH")
    for name in names:
        if cann_root:
            candidate = os.path.join(cann_root, "bin", name)
            if os.path.isfile(candidate) and os.access(candidate, os.X_OK):
                return candidate
        candidate = shutil.which(name)
        if candidate:
            return candidate
    raise ManifestIdentityError(f"required tool is unavailable: {', '.join(names)}")


def _read_exported_symbols(object_path):
    readelf = _find_tool("llvm-readelf", "readelf")
    try:
        result = subprocess.run(
            [readelf, "-W", "-s", object_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
            text=True,
        )
    except OSError as error:
        raise ManifestIdentityError(
            "failed to inspect dynamic object symbols"
        ) from error
    if result.returncode != 0:
        raise ManifestIdentityError(
            f"failed to inspect dynamic object symbols: {result.stdout.strip()}"
        )
    symbols = set()
    for line in result.stdout.splitlines():
        fields = line.split()
        if len(fields) >= 8 and fields[3] == "FUNC" and fields[4] in {"GLOBAL", "WEAK"}:
            symbols.add(fields[-1])
    return symbols


def _same_physical_path(value, canonical_path):
    if not isinstance(value, str) or not value or value.startswith("-"):
        return False
    try:
        return os.path.realpath(value) == os.path.realpath(canonical_path)
    except (OSError, ValueError):
        return False


def _add_resource(resources, logical_path, content=None, source_path=None):
    logical_path = _logical_path(logical_path, "resource path")
    if (content is None) == (source_path is None):
        raise ValueError("resource requires exactly one content source")
    if source_path is not None:
        content = _read_regular_file(source_path, "resource")
    previous = resources.get(logical_path)
    if previous is not None and previous != content:
        raise ManifestResourceError(f"conflicting resource path: {logical_path}")
    resources[logical_path] = content
    return logical_path


def _core_suffix(core_type):
    return "aic" if core_type == "cube" else "aiv"


def _output_placeholder(name):
    if not _OUTPUT_NAME_PATTERN.fullmatch(name):
        raise ManifestCommandError(f"output name is not portable: {name}")
    return "${output:" + name + "}"


def _replace_path_token(value, source, replacement):
    if value == source or _same_physical_path(value, source):
        return replacement
    for prefix in ("-I", "-iquote", "-include", "-isystem"):
        path_value = value[len(prefix) :] if value.startswith(prefix) else value
        if value.startswith(prefix) and (
            path_value == source or _same_physical_path(path_value, source)
        ):
            return prefix + replacement
    return value


def _normalize_compile_command(
    record,
    wrapper_logical,
    tiling_logical,
    output_name,
    origin_func_name=None,
    target_symbol=None,
    compile_tiling_key=None,
):
    argv = list(record.argv)
    compiler_index = next(
        (
            index
            for index, value in enumerate(argv)
            if os.path.basename(value) == "bisheng"
        ),
        None,
    )
    if compiler_index is None:
        raise ManifestCommandError("compile command does not invoke Bisheng")
    argv = argv[compiler_index:]
    argv[0] = "${env:ASCEND_HOME_PATH}/bin/bisheng"
    cann_root = os.path.realpath(os.environ.get("ASCEND_HOME_PATH", ""))
    source_reference = "${resource:" + wrapper_logical + "}"
    tiling_reference = "${resource:" + tiling_logical + "}"
    output_reference = _output_placeholder(output_name)
    normalized = []
    for value in argv:
        if _same_physical_path(value, record.source_path):
            value = source_reference
        else:
            value = _replace_path_token(value, record.source_path, source_reference)
        value = _replace_path_token(value, record.output_path, output_reference)
        value = _replace_path_token(value, record.tiling_path, tiling_reference)
        if cann_root and cann_root in value:
            value = value.replace(cann_root, "${env:ASCEND_HOME_PATH}")
        normalized.append(value)

    for index, value in enumerate(normalized):
        if value == record.source_path:
            normalized[index] = source_reference
        if value.startswith("-include") and record.source_path in value:
            normalized[index] = value.replace(record.source_path, source_reference)

    if target_symbol is not None:
        auto_gen_prefix = f"-Dauto_gen_{origin_func_name}_kernel="
        origin_prefix = f"-D{origin_func_name}="
        found_auto_gen = False
        found_origin = False
        for index, value in enumerate(normalized):
            if value.startswith(auto_gen_prefix):
                normalized[index] = auto_gen_prefix + target_symbol
                found_auto_gen = True
            elif value.startswith(origin_prefix):
                normalized[index] = (
                    origin_prefix + f"{origin_func_name}_{compile_tiling_key}_tilingkey"
                )
                found_origin = True
        if not found_auto_gen or not found_origin:
            raise ManifestCommandError("SK command is missing required kernel macros")
    if source_reference not in normalized:
        raise ManifestCommandError("compile source path was not found in static argv")
    source_index = normalized.index(source_reference)
    if "-DCONST_TILING" not in normalized:
        normalized.insert(source_index, "-DCONST_TILING")
    if output_reference not in normalized:
        raise ManifestCommandError("compile output path was not found in static argv")
    if tiling_reference not in normalized:
        raise ManifestCommandError("static argv does not include the tiling header")
    _reject_absolute_arguments(normalized)
    return normalized


def _static_bind_command(request, source_logical, output_name):
    source_reference = "${resource:" + source_logical + "}"
    output_reference = _output_placeholder(output_name)
    options = [
        option
        for option in request.sk_bind_compile_options
        if option != "-DCONST_TILING"
        and not option.startswith("-D__SUPER_KERNEL_DYNAMIC_BLOCK_NUM__")
    ]
    argv = ["${env:ASCEND_HOME_PATH}/bin/bisheng", "-c", "-O3", *options]
    argv.extend(
        [
            "-xcce",
            source_reference,
            f"--cce-aicore-arch=dav-{request.chip_version}-cube",
            "--cce-aicore-only",
            "-std=c++17",
            "-DTILING_KEY_VAR=0",
            "-o",
            output_reference,
        ]
    )
    cann_root = os.path.realpath(os.environ.get("ASCEND_HOME_PATH", ""))
    normalized = []
    for value in argv:
        if cann_root and cann_root in value:
            value = value.replace(cann_root, "${env:ASCEND_HOME_PATH}")
        normalized.append(value)
    _reject_absolute_arguments(normalized)
    return normalized


def _static_bind_source(pairs, basic_attribute, sk_attribute, cap_bitmap):
    lines = ['#include "kernel_operator.h"']
    for basic_symbol, sk_symbol in pairs:
        lines.extend(
            [
                f'extern "C" {basic_attribute} void {basic_symbol}();',
                f'extern "C" {sk_attribute} void {sk_symbol}();',
                f'extern "C" {sk_attribute} void {sk_symbol}_split1();',
                f'extern "C" {sk_attribute} void {sk_symbol}_split2();',
                f'extern "C" {sk_attribute} void {sk_symbol}_split3();',
                f"SK_BIND({basic_symbol}, {cap_bitmap}, {sk_symbol}, "
                f"{sk_symbol}_split1, {sk_symbol}_split2, {sk_symbol}_split3);",
            ]
        )
    return ("\n".join(lines) + "\n").encode("utf-8")


def _reject_absolute_arguments(argv):
    for value in argv:
        scrubbed = re.sub(r"\$\{(?:env|resource|output|option):[^}]+\}", "X", value)
        joined_path = any(
            scrubbed.startswith(prefix) and os.path.isabs(scrubbed[len(prefix) :])
            for prefix in ("-I", "-iquote", "-include", "-isystem", "-L", "-B")
        )
        if (
            os.path.isabs(scrubbed)
            or joined_path
            or re.search(r"(?:^|[=,:])/[A-Za-z0-9_.-]", scrubbed)
        ):
            raise ManifestCommandError(f"command contains an absolute path: {value}")


def _factor_common_compile(commands):
    if not commands:
        raise ManifestCommandError("Manifest contains no wrapper compile commands")
    prefixes = []
    originals = []
    for command in commands:
        argv = command["cmd"]
        source_index = next(
            (
                index
                for index, value in enumerate(argv)
                if value.startswith("${resource:src/")
            ),
            None,
        )
        if source_index is None:
            raise ManifestCommandError("compile command has no static wrapper source")
        prefixes.append(argv[1:source_index])
        originals.append(list(argv))
    common = list(prefixes[0])
    for prefix in prefixes[1:]:
        length = 0
        while (
            length < min(len(common), len(prefix)) and common[length] == prefix[length]
        ):
            length += 1
        common = common[:length]
    if not common:
        raise ManifestCommandError("wrapper commands have no common compile prefix")
    for command in commands:
        command["cmd"][1 : 1 + len(common)] = ["${option:common_compile}"]
    for command, original in zip(commands, originals):
        expanded = list(command["cmd"])
        option_index = expanded.index("${option:common_compile}")
        expanded[option_index : option_index + 1] = common
        if expanded != original:
            raise ManifestCommandError("common_compile factoring changed argv ordering")
    return common


def _validate_request(request):
    if not isinstance(request, ManifestGenerationRequest):
        raise TypeError("request must be a ManifestGenerationRequest")
    if not request.soc_version:
        raise ManifestIdentityError("soc_version must be non-empty")
    if not request.chip_version:
        raise ManifestIdentityError("chip_version must be non-empty")
    if not _RESOURCE_ID_PATTERN.fullmatch(request.resource_id):
        raise ManifestIdentityError("resource_id must be 64 lowercase hex characters")
    _read_regular_file(request.dynamic_object_path, "dynamic object")
    _read_regular_file(request.metadata_json_path, "metadata JSON")
    _read_regular_file(request.source_file_path, "operator source")
    if not request.kernels:
        raise ManifestCommandError("Manifest request contains no kernels")
    actual_resource_id = read_resource_id_section(request.dynamic_object_path)
    if actual_resource_id != request.resource_id:
        raise ManifestIdentityError(
            "Manifest resource_id does not match dynamic object"
        )
    try:
        metadata = json.loads(
            Path(request.metadata_json_path).read_text(encoding="utf-8")
        )
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as error:
        raise ManifestIdentityError(
            "failed to read dynamic compile metadata"
        ) from error
    metadata_names = [
        item.get("kernelName")
        for item in metadata.get("kernelList", [])
        if isinstance(item, dict)
    ]
    request_names = [kernel.kernel_name for kernel in request.kernels]
    if request_names != metadata_names:
        raise ManifestIdentityError("Manifest kernels do not match metadata kernelList")
    exported_symbols = _read_exported_symbols(request.dynamic_object_path)
    has_sk_commands = False
    for kernel in request.kernels:
        if not isinstance(kernel.parameter_index, int) or kernel.parameter_index < 0:
            raise ManifestCommandError("tiling parameter_index must be non-negative")
        if not isinstance(kernel.byte_size, int) or kernel.byte_size < 0:
            raise ManifestCommandError("tiling byte_size must be non-negative")
        if not kernel.commands:
            raise ManifestCommandError(f"kernel {kernel.kernel_name} has no commands")
        for command in kernel.commands:
            has_sk_commands = has_sk_commands or command.obj_type == "sk"
            if command.compiled_symbol not in exported_symbols:
                raise ManifestIdentityError(
                    f"compiled symbol is absent from dynamic object: {command.compiled_symbol}"
                )
    if has_sk_commands and (
        not isinstance(request.sk_cap_bitmap, int) or request.sk_cap_bitmap < 0
    ):
        raise ManifestCommandError("SK commands require a non-negative cap bitmap")


def _write_resources(stage_dir, resources):
    resource_root = os.path.join(stage_dir, "resources")
    for logical_path, content in sorted(resources.items()):
        destination = os.path.join(resource_root, *PurePosixPath(logical_path).parts)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        Path(destination).write_bytes(content)


def _fsync_tree(root):
    for directory, _, files in os.walk(root, topdown=False):
        for name in files:
            with open(os.path.join(directory, name), "rb") as file_obj:
                os.fsync(file_obj.fileno())
        descriptor = os.open(directory, os.O_RDONLY)
        try:
            os.fsync(descriptor)
        finally:
            os.close(descriptor)


def _fsync_directory(path):
    descriptor = os.open(path, os.O_RDONLY)
    try:
        os.fsync(descriptor)
    finally:
        os.close(descriptor)


def _publish_directory(stage_dir, target_dir):
    parent = os.path.dirname(target_dir)
    backup_dir = tempfile.mkdtemp(prefix=".manifest.backup.", dir=parent)
    os.rmdir(backup_dir)
    moved_previous = False
    published = False
    try:
        if os.path.lexists(target_dir):
            os.replace(target_dir, backup_dir)
            moved_previous = True
        os.replace(stage_dir, target_dir)
        published = True
        _fsync_directory(parent)
    except Exception as error:
        if published and os.path.lexists(target_dir):
            os.replace(target_dir, stage_dir)
        if moved_previous and not os.path.lexists(target_dir):
            os.replace(backup_dir, target_dir)
        raise ManifestPublishError("failed to publish Manifest directory") from error
    if moved_previous:
        shutil.rmtree(backup_dir)


def _manifest_resource_references(manifest):
    references = set()
    for values in manifest.get("option", {}).values():
        for argument in values:
            references.update(re.findall(r"\$\{resource:([^}]+)\}", argument))
    for kernel in manifest["kernels"]:
        references.add(kernel["static_inputs"][0]["file"])
        for obj in kernel["objects"]:
            for command in obj["commands"]:
                for argument in command["cmd"]:
                    references.update(re.findall(r"\$\{resource:([^}]+)\}", argument))
    return references


def _validate_generated_manifest(manifest, resources):
    expected = {
        "schema_version",
        "soc_version",
        "resource_id",
        "source_file",
        "resource_path",
        "option",
        "kernels",
    }
    if set(manifest) != expected:
        raise ManifestCommandError("generated Manifest has invalid top-level fields")
    for reference in _manifest_resource_references(manifest):
        if reference not in resources and not any(
            path.startswith(reference.rstrip("/") + "/") for path in resources
        ):
            raise ManifestResourceError(
                f"Manifest references a missing resource: {reference}"
            )
    for kernel in manifest["kernels"]:
        static_input = kernel["static_inputs"][0]
        template = resources[static_input["file"]].decode("utf-8")
        if template.count(_STATIC_VALUE_TOKEN) != 1:
            raise ManifestResourceError(
                "tiling template must contain exactly one token"
            )
        template_reference = "${resource:" + static_input["file"] + "}"
        all_outputs = set()
        for obj in kernel["objects"]:
            if any(
                not isinstance(output, str)
                or re.fullmatch(r"\$\{output:[A-Za-z0-9_.-]+\}", output) is None
                for output in obj["outputs"]
            ):
                raise ManifestCommandError(
                    "object outputs must use output placeholders"
                )
            if all_outputs.intersection(obj["outputs"]):
                raise ManifestCommandError("kernel objects contain duplicate outputs")
            all_outputs.update(obj["outputs"])
            produced = set()
            previous_stage = -1
            for command in obj["commands"]:
                argv = command["cmd"]
                stage = command["stage"]
                if not isinstance(stage, int) or stage < previous_stage:
                    raise ManifestCommandError("command stages are not ordered")
                previous_stage = stage
                output_references = [
                    reference
                    for argument in argv
                    for reference in re.findall(r"\$\{output:[^}]+\}", argument)
                ]
                if command["type"] == "compile":
                    if stage != 0 or len(output_references) != 1:
                        raise ManifestCommandError(
                            "compile command must produce one stage-0 output"
                        )
                    produced.add(output_references[0])
                elif command["type"] == "objcopy":
                    if (
                        stage != 1
                        or len(output_references) != 2
                        or output_references[0] not in produced
                    ):
                        raise ManifestCommandError(
                            "objcopy command must consume a prior output at stage 1"
                        )
                    produced.add(output_references[1])
                else:
                    raise ManifestCommandError(
                        "Manifest object contains an unsupported command"
                    )
                if command["type"] != "compile" or "-DCONST_TILING" not in argv:
                    continue
                includes_template = "-include" + template_reference in argv or any(
                    argv[index : index + 2] == ["-include", template_reference]
                    for index in range(len(argv) - 1)
                )
                if not includes_template:
                    raise ManifestCommandError(
                        "wrapper compile command does not include its tiling template"
                    )
                if not any(value.startswith("${resource:src/") for value in argv):
                    raise ManifestCommandError(
                        "wrapper compile command has no static wrapper source"
                    )
            if produced != set(obj["outputs"]):
                raise ManifestCommandError(
                    "object outputs do not match command-produced outputs"
                )
    token = _STATIC_VALUE_TOKEN.encode("utf-8")
    token_paths = [path for path, content in resources.items() if token in content]
    expected_token_paths = {
        kernel["static_inputs"][0]["file"] for kernel in manifest["kernels"]
    }
    if set(token_paths) != expected_token_paths:
        raise ManifestResourceError(
            "static input token may only appear in referenced tiling templates"
        )
    if "intermediates" in json.dumps(manifest):
        raise ManifestCommandError("Manifest must not use intermediates")


def generate_manifest(request: ManifestGenerationRequest) -> ManifestGenerationResult:
    _validate_request(request)
    output_root = os.path.abspath(request.output_root)
    os.makedirs(output_root, exist_ok=True)
    lock_path = os.path.join(output_root, ".manifest.lock")
    target_dir = os.path.join(output_root, "manifest")
    stage_dir = ""
    with open(lock_path, "a+", encoding="utf-8") as lock_file:
        fcntl.flock(lock_file.fileno(), fcntl.LOCK_EX)
        try:
            stage_dir = tempfile.mkdtemp(prefix=".manifest.stage.", dir=output_root)
            resources = {}
            _add_resource(
                resources,
                "src/" + os.path.basename(request.source_file_path),
                source_path=request.source_file_path,
            )

            all_records = [
                command for kernel in request.kernels for command in kernel.commands
            ]
            wrapper_paths = {}
            for command in all_records:
                key = command.source_path
                if key not in wrapper_paths:
                    logical = "src/" + os.path.basename(command.source_path)
                    source_content = _read_regular_file(
                        command.source_path, "generated wrapper"
                    )
                    wrapper = _portable_wrapper_text(
                        source_content, request.source_file_path
                    ).encode("utf-8")
                    wrapper_paths[key] = _add_resource(
                        resources, logical, content=wrapper
                    )

            tiling_paths = {}
            for kernel in request.kernels:
                path = os.path.realpath(kernel.tiling_template_path)
                if path not in tiling_paths:
                    try:
                        dynamic_header = _read_regular_file(
                            path, "dynamic tiling header"
                        ).decode("utf-8")
                    except UnicodeDecodeError as error:
                        raise ManifestResourceError(
                            "dynamic tiling header is not UTF-8 text"
                        ) from error
                    logical = "include/" + os.path.basename(path)
                    template = static_tiling_template_text(dynamic_header).encode(
                        "utf-8"
                    )
                    tiling_paths[path] = _add_resource(
                        resources, logical, content=template
                    )

            manifest_kernels = []
            wrapper_commands = []
            for kernel in request.kernels:
                basic_commands = []
                outputs = []
                ordered_commands = sorted(
                    (
                        record
                        for record in kernel.commands
                        if record.obj_type == "basic"
                    ),
                    key=lambda record: (0 if record.core_type == "cube" else 1),
                )
                if not ordered_commands:
                    raise ManifestCommandError(
                        f"kernel {kernel.kernel_name} has no basic command"
                    )
                tiling_logical = tiling_paths[
                    os.path.realpath(kernel.tiling_template_path)
                ]
                for record in ordered_commands:
                    output_name = (
                        f"{kernel.kernel_name}.{_core_suffix(record.core_type)}.o"
                    )
                    cmd = _normalize_compile_command(
                        record,
                        wrapper_paths[record.source_path],
                        tiling_logical,
                        output_name,
                    )
                    command = {"type": "compile", "stage": record.stage, "cmd": cmd}
                    basic_commands.append(command)
                    wrapper_commands.append(command)
                    outputs.append(_output_placeholder(output_name))
                objects = [
                    {
                        "obj_name": "basic",
                        "obj_type": "basic",
                        "commands": basic_commands,
                        "outputs": outputs,
                    }
                ]

                sk_records = sorted(
                    (record for record in kernel.commands if record.obj_type == "sk"),
                    key=lambda record: (0 if record.core_type == "cube" else 1),
                )
                if sk_records:
                    basic_by_core = {
                        record.core_type: record for record in ordered_commands
                    }
                    if set(basic_by_core) != {
                        record.core_type for record in sk_records
                    }:
                        raise ManifestCommandError(
                            f"kernel {kernel.kernel_name} has unmatched basic/SK cores"
                        )
                    sk_commands = []
                    sk_outputs = []
                    split_specs = []
                    bind_pairs = []
                    multiple_cores = len(sk_records) > 1
                    for record in sk_records:
                        basic_record = basic_by_core[record.core_type]
                        static_sk_symbol = basic_record.compiled_symbol + "_static_sk"
                        core_suffix = _core_suffix(record.core_type)
                        sk_output_name = (
                            f"{kernel.kernel_name}.{core_suffix}.sk.o"
                            if multiple_cores
                            else f"{kernel.kernel_name}.sk.o"
                        )
                        cmd = _normalize_compile_command(
                            record,
                            wrapper_paths[record.source_path],
                            tiling_logical,
                            sk_output_name,
                            origin_func_name=request.origin_func_name,
                            target_symbol=static_sk_symbol,
                            compile_tiling_key=kernel.compile_tiling_key,
                        )
                        command = {"type": "compile", "stage": 0, "cmd": cmd}
                        sk_commands.append(command)
                        wrapper_commands.append(command)
                        sk_output = _output_placeholder(sk_output_name)
                        sk_outputs.append(sk_output)
                        bind_pairs.append(
                            (basic_record.compiled_symbol, static_sk_symbol)
                        )
                        for split_index in range(1, 4):
                            split_name = (
                                f"{kernel.kernel_name}.{core_suffix}.sk_split{split_index}.o"
                                if multiple_cores
                                else f"{kernel.kernel_name}.sk_split{split_index}.o"
                            )
                            split_specs.append(
                                (
                                    static_sk_symbol,
                                    sk_output,
                                    split_index,
                                    split_name,
                                )
                            )

                    bind_name = f"sk_bind_{kernel.kernel_name}.cpp"
                    bind_logical = _add_resource(
                        resources,
                        "src/" + bind_name,
                        content=_static_bind_source(
                            bind_pairs,
                            request.basic_kernel_attribute
                            or request.sk_kernel_attribute,
                            request.sk_kernel_attribute,
                            request.sk_cap_bitmap,
                        ),
                    )
                    bind_output_name = f"{kernel.kernel_name}.sk_bind.o"
                    sk_commands.append(
                        {
                            "type": "compile",
                            "stage": 0,
                            "cmd": _static_bind_command(
                                request,
                                bind_logical,
                                bind_output_name,
                            ),
                        }
                    )
                    for (
                        static_sk_symbol,
                        sk_output,
                        split_index,
                        split_name,
                    ) in split_specs:
                        sk_commands.append(
                            {
                                "type": "objcopy",
                                "stage": 1,
                                "cmd": [
                                    "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
                                    f"--redefine-sym={static_sk_symbol}="
                                    f"{static_sk_symbol}_split{split_index}",
                                    sk_output,
                                    _output_placeholder(split_name),
                                ],
                            }
                        )
                    sk_outputs.extend(
                        _output_placeholder(split_name)
                        for _, _, _, split_name in split_specs
                    )
                    sk_outputs.append(_output_placeholder(bind_output_name))
                    objects.append(
                        {
                            "obj_name": "sk",
                            "obj_type": "sk",
                            "commands": sk_commands,
                            "outputs": sk_outputs,
                        }
                    )
                manifest_kernels.append(
                    {
                        "kernel_name": kernel.kernel_name,
                        "static_inputs": [
                            {
                                "name": "tiling_data",
                                "parameter_index": kernel.parameter_index,
                                "byte_size": kernel.byte_size,
                                "file": tiling_logical,
                                "token": _STATIC_VALUE_TOKEN,
                            }
                        ],
                        "link_option": list(kernel.link_option),
                        "objects": objects,
                    }
                )

            common_compile = _factor_common_compile(wrapper_commands)
            manifest = {
                "schema_version": "1.0",
                "soc_version": request.soc_version,
                "resource_id": request.resource_id,
                "source_file": os.path.basename(request.source_file_path),
                "resource_path": "resources",
                "option": {"common_compile": common_compile},
                "kernels": manifest_kernels,
            }
            _validate_generated_manifest(manifest, resources)
            _write_resources(stage_dir, resources)
            manifest_path = os.path.join(stage_dir, "manifest.json")
            with open(manifest_path, "w", encoding="utf-8") as file_obj:
                json.dump(manifest, file_obj, indent=2, ensure_ascii=False)
                file_obj.write("\n")
            _fsync_tree(stage_dir)
            _publish_directory(stage_dir, target_dir)
            stage_dir = ""
            return ManifestGenerationResult(
                manifest_path=os.path.join(target_dir, "manifest.json"),
                resource_id=request.resource_id,
                kernel_count=len(request.kernels),
                resource_count=len(resources),
            )
        finally:
            if stage_dir and os.path.isdir(stage_dir):
                shutil.rmtree(stage_dir)
            fcntl.flock(lock_file.fileno(), fcntl.LOCK_UN)
