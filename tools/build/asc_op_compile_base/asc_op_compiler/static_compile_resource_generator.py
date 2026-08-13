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
from dataclasses import dataclass, replace
from pathlib import Path
import re
import shutil
import tempfile
from typing import Optional, Sequence, Tuple


_TILING_DATA_TEMPLATE = "@@STATIC_VALUE_tiling_data@@"
_LINK_OPTION = ("-m", "aicorelinux", "-r", "-Ttext=0", "-q", "-x")


class ManifestGenerationError(RuntimeError):
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
    object_type: str
    core_type: str
    source_path: str
    constant_info_files: Tuple[str, ...]
    argv: Tuple[str, ...]
    output_path: str


@dataclass(frozen=True)
class ConstantInfoRecord:
    name: str
    parameter_index: int
    byte_size: int
    template_path: str
    template: str


@dataclass(frozen=True)
class KernelCompileRecord:
    kernel_name: str
    tiling_key: str
    constant_infos: Tuple[ConstantInfoRecord, ...]
    commands: Tuple[StaticCompileCommand, ...]
    link_option: Tuple[str, ...] = _LINK_OPTION


@dataclass(frozen=True)
class ManifestGenerationRequest:
    soc_version: str
    resource_id: str
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
    try:
        return Path(path).read_bytes()
    except OSError as error:
        raise ManifestResourceError(f"failed to read {description}: {path}") from error


def _build_context_commands(
    compile_info,
    compile_option_tuple,
    command_builder,
    chip_version,
    constant_info_files,
    object_type,
):
    return tuple(
        StaticCompileCommand(
            tiling_key=spec.tiling_key,
            compiled_symbol=spec.compiled_symbol,
            object_type=object_type,
            core_type=spec.core_type,
            source_path=spec.source_path,
            constant_info_files=tuple(spec.constant_info_files),
            argv=tuple(spec.argv),
            output_path=spec.output_path,
        )
        for spec in command_builder(
            compile_info,
            compile_option_tuple,
            constant_info_files,
            chip_version,
        )
    )


def build_kernel_compile_records(
    *,
    basic_compile_info,
    basic_compile_option_tuple,
    command_builder,
    chip_version: str,
    metadata_json_path: str,
    constant_infos: Sequence[ConstantInfoRecord],
    constant_info_size_by_tiling_key: Optional[dict] = None,
    sk_compile_info=None,
    sk_compile_option_tuple=None,
    link_option: Sequence[str] = _LINK_OPTION,
):
    compile_contexts = (basic_compile_info, sk_compile_info)
    if any(
        getattr(context, "tiling_key_group_map", None)
        for context in compile_contexts
        if context is not None
    ):
        raise ManifestCommandError("grouped tiling keys are not supported")
    constant_infos = tuple(constant_infos)
    constant_info_files = tuple(
        dict.fromkeys(item.template_path for item in constant_infos)
    )
    metadata = json.loads(Path(metadata_json_path).read_text(encoding="utf-8"))
    kernel_list = metadata["kernelList"]

    all_commands = list(
        _build_context_commands(
            basic_compile_info,
            basic_compile_option_tuple,
            command_builder,
            chip_version,
            constant_info_files,
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
                constant_info_files,
                "sk",
            )
        )
    commands_by_key = {}
    for command in all_commands:
        commands_by_key.setdefault(command.tiling_key, []).append(command)
    known_keys = set(commands_by_key)
    exact_sizes = {
        name: {str(key): value for key, value in sizes.items()}
        for name, sizes in (constant_info_size_by_tiling_key or {}).items()
    }
    records = []
    for item in kernel_list:
        tiling_key_value = item.get("tilingKey")
        if tiling_key_value is None:
            tiling_key = next(
                key
                for key in sorted(known_keys, key=len, reverse=True)
                if item["kernelName"].endswith("_" + key)
            )
        else:
            tiling_key = str(tiling_key_value)
        commands = commands_by_key[tiling_key]
        resolved_constant_infos = []
        for constant_info in constant_infos:
            sizes = exact_sizes.get(constant_info.name, {})
            byte_size = sizes[tiling_key] if sizes else constant_info.byte_size
            resolved_constant_infos.append(replace(constant_info, byte_size=byte_size))
        records.append(
            KernelCompileRecord(
                kernel_name=item["kernelName"],
                tiling_key=tiling_key,
                constant_infos=tuple(resolved_constant_infos),
                commands=tuple(commands),
                link_option=tuple(link_option),
            )
        )
    return tuple(records)


def static_tiling_template_text(dynamic_header):
    if not dynamic_header:
        return (
            "#ifndef __ASCENDC_STATIC_TILING_DATA_H__\n"
            "#define __ASCENDC_STATIC_TILING_DATA_H__\n"
            "#ifdef CONST_TILING\n"
            "static constexpr unsigned char __ascendc_static_tiling_data[] = "
            f"{_TILING_DATA_TEMPLATE};\n"
            "#endif\n"
            "#endif\n"
        )
    final_endif = dynamic_header.rfind("#endif")

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
        f"static constexpr uint8_t __ascendc_static_tiling_data[] = {_TILING_DATA_TEMPLATE};",
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
    return result


def _portable_wrapper_text(source_content, original_source_path):
    wrapper_text = source_content.decode("utf-8")
    absolute_include = f'#include "{original_source_path}"'
    return wrapper_text.replace(
        absolute_include,
        f'#include "{os.path.basename(original_source_path)}"',
        1,
    )


def _add_resource(resources, logical_path, content):
    resources[logical_path] = content
    return logical_path


def _core_suffix(core_type):
    return "aic" if core_type == "cube" else "aiv"


def _output_placeholder(name):
    return "${output}/" + name


def _resource_placeholder(resource_path, logical_path):
    return "${resource}/" + resource_path + "/" + logical_path


def _replace_path_token(value, source, replacement):
    if value == source:
        return replacement
    for prefix in ("-I", "-iquote", "-include", "-isystem"):
        path_value = value[len(prefix) :] if value.startswith(prefix) else value
        if value.startswith(prefix) and path_value == source:
            return prefix + replacement
    return value


def _normalize_source_include_options(
    argv, source_file_path, replacement, insertion_before=None
):
    source_dir = os.path.dirname(source_file_path)
    normalized = []
    source_include_added = False
    index = 0
    while index < len(argv):
        value = argv[index]
        if value == "-I" and index + 1 < len(argv):
            include_path = argv[index + 1]
            if include_path == source_dir:
                if replacement is not None and not source_include_added:
                    normalized.extend(["-I", replacement])
                    source_include_added = True
                index += 2
                continue
            if os.path.isabs(include_path) and not os.path.exists(include_path):
                index += 2
                continue
            normalized.extend([value, include_path])
            index += 2
            continue
        if value.startswith("-I") and value != "-I":
            include_path = value[2:]
            if include_path == source_dir:
                if replacement is not None and not source_include_added:
                    normalized.extend(["-I", replacement])
                    source_include_added = True
                index += 1
                continue
            if os.path.isabs(include_path) and not os.path.exists(include_path):
                index += 1
                continue
        normalized.append(value)
        index += 1
    if replacement is not None and not source_include_added:
        insertion_index = next(
            (
                index
                for index, value in enumerate(normalized)
                if insertion_before is not None and value == insertion_before
            ),
            len(normalized),
        )
        normalized[insertion_index:insertion_index] = ["-I", replacement]
    return normalized


def _normalize_compile_command(
    record,
    source_file_path,
    resource_path,
    wrapper_logical,
    constant_info_logicals,
    output_name,
    origin_func_name=None,
    target_symbol=None,
    tiling_key=None,
):
    argv = _normalize_source_include_options(
        list(record.argv),
        source_file_path,
        "${source_file_path}",
        insertion_before=record.source_path,
    )
    compiler_index = next(
        (
            index
            for index, value in enumerate(argv)
            if os.path.basename(value) == "bisheng"
        )
    )
    argv = argv[compiler_index:]
    argv[0] = "${env:ASCEND_HOME_PATH}/bin/bisheng"
    cann_root = os.environ.get("ASCEND_HOME_PATH", "")
    source_reference = _resource_placeholder(resource_path, wrapper_logical)
    constant_info_references = {
        path: _resource_placeholder(resource_path, constant_info_logicals[path])
        for path in record.constant_info_files
    }
    output_reference = _output_placeholder(output_name)
    normalized = []
    for value in argv:
        if value == record.source_path:
            value = source_reference
        else:
            value = _replace_path_token(value, record.source_path, source_reference)
        value = _replace_path_token(value, record.output_path, output_reference)
        for constant_info_file, reference in constant_info_references.items():
            value = _replace_path_token(value, constant_info_file, reference)
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
        for index, value in enumerate(normalized):
            if value.startswith(auto_gen_prefix):
                normalized[index] = auto_gen_prefix + target_symbol
            elif value.startswith(origin_prefix):
                normalized[index] = (
                    origin_prefix + f"{origin_func_name}_{tiling_key}_tilingkey"
                )
    source_index = normalized.index(source_reference)
    if "-DCONST_TILING" not in normalized:
        normalized.insert(source_index, "-DCONST_TILING")
    return normalized


def _static_bind_command(request, resource_path, source_logical, output_name):
    source_reference = _resource_placeholder(resource_path, source_logical)
    output_reference = _output_placeholder(output_name)
    options = [
        option
        for option in request.sk_bind_compile_options
        if option != "-DCONST_TILING"
        and not option.startswith("-D__SUPER_KERNEL_DYNAMIC_BLOCK_NUM__")
    ]
    options = _normalize_source_include_options(options, request.source_file_path, None)
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
    cann_root = os.environ.get("ASCEND_HOME_PATH", "")
    normalized = []
    for value in argv:
        if cann_root and cann_root in value:
            value = value.replace(cann_root, "${env:ASCEND_HOME_PATH}")
        normalized.append(value)
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


def _factor_common_compile(commands, resource_path):
    prefixes = []
    for command in commands:
        argv = command["cmd"]
        source_index = next(
            index
            for index, value in enumerate(argv)
            if value.startswith("${resource}/" + resource_path + "/src/")
        )
        prefixes.append(argv[1:source_index])
    common = list(prefixes[0])
    for prefix in prefixes[1:]:
        length = 0
        while (
            length < min(len(common), len(prefix)) and common[length] == prefix[length]
        ):
            length += 1
        common = common[:length]
    for command in commands:
        command["cmd"][1 : 1 + len(common)] = ["${option:common_compile}"]
    return common


def _write_resources(stage_dir, resource_path, resources):
    resource_root = os.path.join(stage_dir, resource_path)
    for logical_path, content in sorted(resources.items()):
        destination = os.path.join(resource_root, *logical_path.split("/"))
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


def generate_manifest(request: ManifestGenerationRequest) -> ManifestGenerationResult:
    output_root = os.path.abspath(request.output_root)
    os.makedirs(output_root, exist_ok=True)
    lock_path = os.path.join(output_root, ".manifest.lock")
    target_dir = os.path.join(output_root, "manifest")
    stage_dir = ""
    resource_path = "resources"
    with open(lock_path, "a+", encoding="utf-8") as lock_file:
        fcntl.flock(lock_file.fileno(), fcntl.LOCK_EX)
        try:
            stage_dir = tempfile.mkdtemp(prefix=".manifest.stage.", dir=output_root)
            resources = {}

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

            constant_info_logicals = {}
            templates_by_path = {}
            for kernel in request.kernels:
                for constant_info in kernel.constant_infos:
                    path = constant_info.template_path
                    templates_by_path.setdefault(path, set()).add(
                        constant_info.template
                    )
            for path, template_markers in templates_by_path.items():
                if path not in constant_info_logicals:
                    source_text = _read_regular_file(
                        path, "constant info template"
                    ).decode("utf-8")
                    logical = "include/" + os.path.basename(path)
                    if _TILING_DATA_TEMPLATE in template_markers:
                        source_text = static_tiling_template_text(source_text)
                    constant_info_logicals[path] = _add_resource(
                        resources, logical, content=source_text.encode("utf-8")
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
                        if record.object_type == "basic"
                    ),
                    key=lambda record: (0 if record.core_type == "cube" else 1),
                )
                for record in ordered_commands:
                    output_name = (
                        f"{kernel.kernel_name}.{_core_suffix(record.core_type)}.o"
                    )
                    cmd = _normalize_compile_command(
                        record,
                        request.source_file_path,
                        resource_path,
                        wrapper_paths[record.source_path],
                        constant_info_logicals,
                        output_name,
                    )
                    command = {"type": "compile", "stage": 0, "cmd": cmd}
                    basic_commands.append(command)
                    wrapper_commands.append(command)
                    outputs.append(_output_placeholder(output_name))
                objects = [
                    {
                        "object_name": "basic",
                        "object_type": "basic",
                        "commands": basic_commands,
                        "outputs": outputs,
                    }
                ]

                sk_records = sorted(
                    (
                        record
                        for record in kernel.commands
                        if record.object_type == "sk"
                    ),
                    key=lambda record: (0 if record.core_type == "cube" else 1),
                )
                if sk_records:
                    basic_by_core = {
                        record.core_type: record for record in ordered_commands
                    }
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
                            request.source_file_path,
                            resource_path,
                            wrapper_paths[record.source_path],
                            constant_info_logicals,
                            sk_output_name,
                            origin_func_name=request.origin_func_name,
                            target_symbol=static_sk_symbol,
                            tiling_key=kernel.tiling_key,
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
                                resource_path,
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
                            "object_name": "sk",
                            "object_type": "sk",
                            "commands": sk_commands,
                            "outputs": sk_outputs,
                        }
                    )
                manifest_kernels.append(
                    {
                        "kernel_name": kernel.kernel_name,
                        "constant_infos": [
                            {
                                "name": constant_info.name,
                                "parameter_index": constant_info.parameter_index,
                                "byte_size": constant_info.byte_size,
                                "file": _resource_placeholder(
                                    resource_path,
                                    constant_info_logicals[constant_info.template_path],
                                ),
                                "template": constant_info.template,
                            }
                            for constant_info in kernel.constant_infos
                        ],
                        "link_option": list(kernel.link_option),
                        "objects": objects,
                    }
                )

            common_compile = _factor_common_compile(wrapper_commands, resource_path)
            manifest = {
                "schema_version": "1.0",
                "soc_version": request.soc_version,
                "resource_id": request.resource_id,
                "source_file": os.path.basename(request.source_file_path),
                "resource_path": resource_path,
                "option": {"common_compile": common_compile},
                "kernels": manifest_kernels,
            }
            _write_resources(stage_dir, resource_path, resources)
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
