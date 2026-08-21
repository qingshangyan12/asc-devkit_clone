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
from typing import Optional, Tuple


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
    compile_tiling_key: str
    compiled_symbol: str
    compile_symbol: str
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
    compile_tiling_key: str
    constant_infos: Tuple[ConstantInfoRecord, ...]
    commands: Tuple[StaticCompileCommand, ...]
    link_options: Tuple[str, ...] = _LINK_OPTION


@dataclass(frozen=True)
class KernelCompileRecordBuildRequest:
    basic_compile_info: object
    metadata_json_path: str
    constant_infos: Tuple[ConstantInfoRecord, ...]
    constant_info_size_by_tiling_key: Optional[dict] = None
    sk_compile_info: Optional[object] = None
    link_options: Tuple[str, ...] = _LINK_OPTION


@dataclass(frozen=True)
class SkBindCommandSpec:
    source_path: str
    argv: Tuple[str, ...]
    output_path: str


@dataclass(frozen=True)
class ManifestGenerationRequest:
    soc_version: str
    resource_id: str
    source_file_path: str
    origin_func_name: str
    output_dir: str
    manifest_name: str
    kernels: Tuple[KernelCompileRecord, ...]
    sk_cap_bitmap: Optional[int] = None
    sk_kernel_attribute: str = ""
    basic_kernel_attribute: str = ""
    sk_bind_command: Optional[SkBindCommandSpec] = None


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


def build_tiling_key_group_index(*compile_contexts):
    master_to_slaves = {}
    slave_to_master = {}
    for context in compile_contexts:
        if context is None:
            continue
        group_map = getattr(context, "tiling_key_group_map", {}) or {}
        for master, slaves in group_map.items():
            master = str(master)
            known_slaves = list(master_to_slaves.setdefault(master, ()))
            for slave in slaves:
                slave = str(slave)
                if slave == master:
                    raise ManifestCommandError(
                        f"tiling key {master} cannot be both master and slave"
                    )
                previous_master = slave_to_master.get(slave)
                if previous_master is not None and previous_master != master:
                    raise ManifestCommandError(
                        f"tiling key {slave} belongs to multiple masters"
                    )
                slave_to_master[slave] = master
                if slave not in known_slaves:
                    known_slaves.append(slave)
            master_to_slaves[master] = tuple(known_slaves)
    for master in master_to_slaves:
        if master in slave_to_master:
            raise ManifestCommandError(
                f"tiling key {master} cannot be both master and slave"
            )
    return master_to_slaves, slave_to_master


def _logical_symbol(compile_symbol, compile_tiling_key, logical_tiling_key):
    if logical_tiling_key == compile_tiling_key:
        return compile_symbol
    for core_suffix in ("", "_mix_aic", "_mix_aiv"):
        master_suffix = f"_{compile_tiling_key}{core_suffix}"
        if compile_symbol.endswith(master_suffix):
            return (
                compile_symbol[: -len(master_suffix)]
                + f"_{logical_tiling_key}{core_suffix}"
            )
    raise ManifestCommandError(
        f"compile symbol {compile_symbol} does not end with master tiling key "
        f"{compile_tiling_key}"
    )


class KernelCompileRecordBuilder:
    def __init__(self, request: KernelCompileRecordBuildRequest):
        self._request = request
        self._constant_infos = tuple(request.constant_infos)
        self._constant_info_files = tuple(
            dict.fromkeys(item.template_path for item in self._constant_infos)
        )

    def build(self) -> Tuple[KernelCompileRecord, ...]:
        request = self._request
        metadata = json.loads(
            Path(request.metadata_json_path).read_text(encoding="utf-8")
        )
        kernel_list = metadata["kernelList"]
        _, slave_to_master = build_tiling_key_group_index(
            request.basic_compile_info, request.sk_compile_info
        )

        all_commands = list(
            self._build_context_commands(request.basic_compile_info, "basic")
        )
        if request.sk_compile_info is not None:
            all_commands.extend(
                self._build_context_commands(request.sk_compile_info, "sk")
            )
        commands_by_key = {}
        for command in all_commands:
            commands_by_key.setdefault(command.tiling_key, []).append(command)
        known_keys = set(commands_by_key)
        logical_keys = known_keys | set(slave_to_master)
        exact_sizes = {
            name: {str(key): value for key, value in sizes.items()}
            for name, sizes in (request.constant_info_size_by_tiling_key or {}).items()
        }
        records = []
        for item in kernel_list:
            tiling_key_value = item.get("tilingKey")
            if tiling_key_value is None:
                try:
                    tiling_key = next(
                        key
                        for key in sorted(logical_keys, key=len, reverse=True)
                        if item["kernelName"].endswith("_" + key)
                    )
                except StopIteration as error:
                    raise ManifestCommandError(
                        f"cannot determine tiling key for kernel {item['kernelName']}"
                    ) from error
            else:
                tiling_key = str(tiling_key_value)
            compile_tiling_key = slave_to_master.get(tiling_key, tiling_key)
            try:
                physical_commands = commands_by_key[compile_tiling_key]
            except KeyError as error:
                raise ManifestCommandError(
                    f"master tiling key {compile_tiling_key} has no physical "
                    "compile command"
                ) from error
            commands = tuple(
                replace(
                    command,
                    tiling_key=tiling_key,
                    compile_tiling_key=compile_tiling_key,
                    compiled_symbol=_logical_symbol(
                        command.compile_symbol,
                        compile_tiling_key,
                        tiling_key,
                    ),
                )
                for command in physical_commands
            )
            resolved_constant_infos = []
            for constant_info in self._constant_infos:
                sizes = exact_sizes.get(constant_info.name, {})
                if tiling_key in sizes:
                    byte_size = sizes[tiling_key]
                elif compile_tiling_key in sizes:
                    byte_size = sizes[compile_tiling_key]
                else:
                    byte_size = constant_info.byte_size
                resolved_constant_infos.append(
                    replace(constant_info, byte_size=byte_size)
                )
            records.append(
                KernelCompileRecord(
                    kernel_name=item["kernelName"],
                    tiling_key=tiling_key,
                    compile_tiling_key=compile_tiling_key,
                    constant_infos=tuple(resolved_constant_infos),
                    commands=commands,
                    link_options=tuple(request.link_options),
                )
            )
        return tuple(records)

    def _build_context_commands(self, compile_info, object_type):
        recorded_commands = tuple(
            getattr(compile_info, "recorded_compile_commands", ())
        )
        if not recorded_commands:
            raise ManifestCommandError(
                f"{object_type} Manifest generation requires recorded "
                "dynamic compile commands"
            )
        return tuple(
            StaticCompileCommand(
                tiling_key=str(spec.tiling_key),
                compile_tiling_key=str(spec.tiling_key),
                compiled_symbol=spec.compiled_symbol,
                compile_symbol=spec.compiled_symbol,
                object_type=object_type,
                core_type=spec.core_type,
                source_path=spec.source_path,
                constant_info_files=self._constant_info_files,
                argv=tuple(spec.argv),
                output_path=spec.output_path,
            )
            for spec in recorded_commands
        )


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
        "GET_TILING_DATA_PTR_WITH_STRUCT",
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
            f"#define GET_TILING_DATA_PTR_WITH_STRUCT(tiling_struct, dst_ptr, tiling_ptr) {slash}",
            f"    REGISTER_TILINGDATA_SIZE(tiling_struct, __COUNTER__); {slash}",
            f"    static constexpr tiling_struct __ascendc_static_full_##dst_ptr = {slash}",
            f"        convert_from_bytes<tiling_struct>(__ascendc_static_tiling_data); {slash}",
            "    const tiling_struct *dst_ptr = &__ascendc_static_full_##dst_ptr;",
            "#endif",
            "",
        ]
    )
    result = dynamic_header[:final_endif] + "\n" + "\n".join(lines)
    result += dynamic_header[final_endif:]
    return result


def _portable_wrapper_text(source_content, original_source_path, sk_slave_symbols=()):
    wrapper_text = source_content.decode("utf-8")
    absolute_include = f'#include "{original_source_path}"'
    wrapper_text = wrapper_text.replace(
        absolute_include,
        f'#include "{os.path.basename(original_source_path)}"',
        1,
    )
    for symbol in sk_slave_symbols:
        pattern = (
            rf"(\bvoid\s+{re.escape(symbol)}\s*\(\s*)"
            r"uint64_t\s+args_offset(\s*\))"
        )
        replacement = r"\1__gm__ uint64_t *param, sk::SkSystemArgs *sargs\2"
        wrapper_text = re.sub(pattern, replacement, wrapper_text, count=1)
    return wrapper_text


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
        command["cmd"][1 : 1 + len(common)] = ["${options:common_compile}"]
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


class StaticCompileResourceGenerator:
    def __init__(self, request: ManifestGenerationRequest):
        self._request = request
        self._target_dir = os.path.abspath(request.output_dir)
        self._parent_dir = os.path.dirname(self._target_dir)
        self._kernel_name = os.path.basename(self._target_dir)
        self._resource_path = "resources"
        self._stage_dir = ""
        self._resources = {}
        self._wrapper_paths = {}
        self._constant_info_logicals = {}
        self._wrapper_commands = []

    def generate(self) -> ManifestGenerationResult:
        request = self._request
        os.makedirs(self._parent_dir, exist_ok=True)
        lock_path = os.path.join(
            self._parent_dir, f".{self._kernel_name}.manifest.lock"
        )
        with open(lock_path, "a+", encoding="utf-8") as lock_file:
            fcntl.flock(lock_file.fileno(), fcntl.LOCK_EX)
            try:
                self._stage_dir = tempfile.mkdtemp(
                    prefix=f".{self._kernel_name}.stage.", dir=self._parent_dir
                )
                self._collect_resources()
                manifest = self._build_manifest()
                _write_resources(self._stage_dir, self._resource_path, self._resources)
                manifest_path = os.path.join(self._stage_dir, request.manifest_name)
                with open(manifest_path, "w", encoding="utf-8") as file_obj:
                    json.dump(manifest, file_obj, indent=2, ensure_ascii=False)
                    file_obj.write("\n")
                _fsync_tree(self._stage_dir)
                _publish_directory(self._stage_dir, self._target_dir)
                self._stage_dir = ""
                return ManifestGenerationResult(
                    manifest_path=os.path.join(self._target_dir, request.manifest_name),
                    resource_id=request.resource_id,
                    kernel_count=len(request.kernels),
                    resource_count=len(self._resources),
                )
            finally:
                if self._stage_dir and os.path.isdir(self._stage_dir):
                    shutil.rmtree(self._stage_dir)
                fcntl.flock(lock_file.fileno(), fcntl.LOCK_UN)

    def _collect_resources(self):
        request = self._request
        all_records = [
            command for kernel in request.kernels for command in kernel.commands
        ]
        for command in all_records:
            source_path = command.source_path
            if source_path in self._wrapper_paths:
                continue
            logical = "src/" + os.path.basename(source_path)
            source_content = _read_regular_file(source_path, "generated wrapper")
            sk_slave_symbols = tuple(
                record.compiled_symbol
                for record in all_records
                if record.source_path == source_path
                and record.object_type == "sk"
                and record.tiling_key != record.compile_tiling_key
            )
            wrapper = _portable_wrapper_text(
                source_content,
                request.source_file_path,
                sk_slave_symbols,
            ).encode("utf-8")
            self._wrapper_paths[source_path] = _add_resource(
                self._resources, logical, content=wrapper
            )

        templates_by_path = {}
        for kernel in request.kernels:
            for constant_info in kernel.constant_infos:
                templates_by_path.setdefault(constant_info.template_path, set()).add(
                    constant_info.template
                )
        for path, template_markers in templates_by_path.items():
            source_text = _read_regular_file(path, "constant info template").decode(
                "utf-8"
            )
            logical = "include/" + os.path.basename(path)
            if _TILING_DATA_TEMPLATE in template_markers:
                source_text = static_tiling_template_text(source_text)
            self._constant_info_logicals[path] = _add_resource(
                self._resources, logical, content=source_text.encode("utf-8")
            )

    def _build_manifest(self):
        request = self._request
        manifest_kernels = []
        for kernel in request.kernels:
            basic_records = sorted(
                (record for record in kernel.commands if record.object_type == "basic"),
                key=lambda record: (0 if record.core_type == "cube" else 1),
            )
            objects = [self._build_basic_object(kernel, basic_records)]
            sk_records = sorted(
                (record for record in kernel.commands if record.object_type == "sk"),
                key=lambda record: (0 if record.core_type == "cube" else 1),
            )
            if sk_records:
                objects.append(self._build_sk_object(kernel, basic_records, sk_records))
            manifest_kernels.append(
                {
                    "kernel_name": kernel.kernel_name,
                    "constant_infos": [
                        {
                            "name": constant_info.name,
                            "parameter_index": constant_info.parameter_index,
                            "byte_size": constant_info.byte_size,
                            "file": _resource_placeholder(
                                self._resource_path,
                                self._constant_info_logicals[
                                    constant_info.template_path
                                ],
                            ),
                            "template": constant_info.template,
                        }
                        for constant_info in kernel.constant_infos
                    ],
                    "link_options": list(kernel.link_options),
                    "objects": objects,
                }
            )

        common_compile = _factor_common_compile(
            self._wrapper_commands, self._resource_path
        )
        return {
            "soc_version": request.soc_version,
            "resource_id": request.resource_id,
            "source_file": os.path.basename(request.source_file_path),
            "resource_path": self._resource_path,
            "options": {"common_compile": common_compile},
            "kernels": manifest_kernels,
        }

    def _build_basic_object(self, kernel, records):
        commands = []
        outputs = []
        for record in records:
            output_name = f"{kernel.kernel_name}.{_core_suffix(record.core_type)}.o"
            command = {
                "type": "compile",
                "cmd": self._normalize_compile_command(record, output_name),
            }
            commands.append(command)
            self._wrapper_commands.append(command)
            outputs.append(_output_placeholder(output_name))
        return {
            "object_name": "basic",
            "object_type": "basic",
            "commands": commands,
            "outputs": outputs,
        }

    def _build_sk_object(self, kernel, basic_records, sk_records):
        request = self._request
        basic_by_core = {record.core_type: record for record in basic_records}
        commands = []
        outputs = []
        logical_rename_specs = []
        split_specs = []
        bind_pairs = []
        multiple_cores = len(sk_records) > 1
        for record in sk_records:
            basic_record = basic_by_core[record.core_type]
            static_sk_symbol = basic_record.compiled_symbol + "_static_sk"
            compile_static_sk_symbol = basic_record.compile_symbol + "_static_sk"
            core_suffix = _core_suffix(record.core_type)
            final_output_name = (
                f"{kernel.kernel_name}.{core_suffix}.sk.o"
                if multiple_cores
                else f"{kernel.kernel_name}.sk.o"
            )
            is_slave = kernel.tiling_key != kernel.compile_tiling_key
            compile_output_name = (
                final_output_name.replace(".sk.o", ".sk_source.o")
                if is_slave
                else final_output_name
            )
            command = {
                "type": "compile",
                "cmd": self._normalize_compile_command(
                    record,
                    compile_output_name,
                    target_symbol=compile_static_sk_symbol,
                    tiling_key=kernel.compile_tiling_key,
                ),
            }
            commands.append(command)
            self._wrapper_commands.append(command)
            compile_output = _output_placeholder(compile_output_name)
            final_output = _output_placeholder(final_output_name)
            if is_slave:
                logical_rename_specs.append(
                    (
                        record.compiled_symbol,
                        static_sk_symbol,
                        compile_static_sk_symbol,
                        compile_output,
                        final_output,
                    )
                )
            outputs.append(final_output)
            bind_pairs.append((basic_record.compiled_symbol, static_sk_symbol))
            for split_index in range(1, 4):
                split_name = (
                    f"{kernel.kernel_name}.{core_suffix}.sk_split{split_index}.o"
                    if multiple_cores
                    else f"{kernel.kernel_name}.sk_split{split_index}.o"
                )
                split_specs.append(
                    (static_sk_symbol, final_output, split_index, split_name)
                )

        bind_name = f"sk_bind_{kernel.kernel_name}.cpp"
        bind_logical = _add_resource(
            self._resources,
            "src/" + bind_name,
            content=_static_bind_source(
                bind_pairs,
                request.basic_kernel_attribute or request.sk_kernel_attribute,
                request.sk_kernel_attribute,
                request.sk_cap_bitmap,
            ),
        )
        bind_output_name = f"{kernel.kernel_name}.sk_bind.o"
        commands.append(
            {
                "type": "compile",
                "cmd": self._build_bind_command(bind_logical, bind_output_name),
            }
        )
        for (
            dynamic_slave_symbol,
            static_sk_symbol,
            compile_static_sk_symbol,
            compile_output,
            final_output,
        ) in logical_rename_specs:
            commands.append(
                {
                    "type": "objcopy",
                    "cmd": [
                        "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
                        f"--redefine-sym={dynamic_slave_symbol}={static_sk_symbol}",
                        f"--strip-symbol={compile_static_sk_symbol}",
                        compile_output,
                        final_output,
                    ],
                }
            )
        for static_sk_symbol, sk_output, split_index, split_name in split_specs:
            commands.append(
                {
                    "type": "objcopy",
                    "cmd": [
                        "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
                        f"--redefine-sym={static_sk_symbol}="
                        f"{static_sk_symbol}_split{split_index}",
                        sk_output,
                        _output_placeholder(split_name),
                    ],
                }
            )
        outputs.extend(
            _output_placeholder(split_name) for _, _, _, split_name in split_specs
        )
        outputs.append(_output_placeholder(bind_output_name))
        return {
            "object_name": "sk",
            "object_type": "sk",
            "commands": commands,
            "outputs": outputs,
        }

    def _normalize_compile_command(
        self,
        record,
        output_name,
        target_symbol=None,
        tiling_key=None,
    ):
        request = self._request
        argv = _normalize_source_include_options(
            list(record.argv),
            request.source_file_path,
            "${source_file_path}",
            insertion_before=record.source_path,
        )
        argv = [
            value
            for value in argv
            if not value.startswith("-D__SUPER_KERNEL_DYNAMIC_BLOCK_NUM__")
        ]
        compiler_index = next(
            index
            for index, value in enumerate(argv)
            if os.path.basename(value) == "bisheng"
        )
        argv = argv[compiler_index:]
        argv[0] = "${env:ASCEND_HOME_PATH}/bin/bisheng"
        cann_root = os.environ.get("ASCEND_HOME_PATH", "")
        source_reference = _resource_placeholder(
            self._resource_path, self._wrapper_paths[record.source_path]
        )
        constant_info_references = {
            path: _resource_placeholder(
                self._resource_path, self._constant_info_logicals[path]
            )
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
            origin_func_name = request.origin_func_name
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

    def _build_bind_command(self, source_logical, output_name):
        request = self._request
        recorded = request.sk_bind_command
        if recorded is None:
            raise ManifestCommandError(
                "SK Manifest generation requires a recorded sk-bind command"
            )
        source_reference = _resource_placeholder(self._resource_path, source_logical)
        output_reference = _output_placeholder(output_name)
        argv = list(recorded.argv)
        compiler_index = next(
            index
            for index, value in enumerate(argv)
            if os.path.basename(value) == "bisheng"
        )
        argv = argv[compiler_index:]
        argv[0] = "${env:ASCEND_HOME_PATH}/bin/bisheng"
        argv = _normalize_source_include_options(argv, request.source_file_path, None)
        cann_root = os.environ.get("ASCEND_HOME_PATH", "")
        normalized = []
        for value in argv:
            if value == recorded.source_path:
                value = source_reference
            else:
                value = _replace_path_token(
                    value, recorded.source_path, source_reference
                )
            value = _replace_path_token(value, recorded.output_path, output_reference)
            if value == "-DCONST_TILING" or value.startswith(
                "-D__SUPER_KERNEL_DYNAMIC_BLOCK_NUM__"
            ):
                continue
            if cann_root and cann_root in value:
                value = value.replace(cann_root, "${env:ASCEND_HOME_PATH}")
            normalized.append(value)
        return normalized
