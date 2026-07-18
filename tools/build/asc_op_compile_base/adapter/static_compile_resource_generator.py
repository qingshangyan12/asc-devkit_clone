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

import copy
import hashlib
import json
import os
import re
import shutil
import tempfile
from pathlib import PurePosixPath


def parse_compile_option_macros(compile_options):
    """Return stable, typed macro definitions from compiler -D options."""
    macros = []
    seen = {}
    for option in compile_options:
        if not isinstance(option, str) or not option.startswith("-D") or len(option) == 2:
            continue
        definition = option[2:]
        name, separator, value = definition.partition("=")
        if not name:
            continue
        value = value if separator else "1"
        previous = seen.get(name)
        if previous is not None:
            if previous != value:
                raise ValueError(
                    f"compile macro {name} has conflicting values {previous} and {value}"
                )
            continue
        if name.startswith("DTYPE_") or name.startswith("ORIG_DTYPE_"):
            kind = "dtype"
        elif name.startswith("FORMAT_") or name.startswith("ORIG_FORMAT_"):
            kind = "format"
        else:
            kind = "compile"
        seen[name] = value
        macros.append({"name": name, "value": value, "kind": kind})
    return macros


def collect_kernel_symbols(metadata, linked_symbols=None):
    """Collect actual exported symbols without making tiling keys registry keys."""
    symbols = []
    seen = set()

    def add(value):
        if isinstance(value, str) and value and value not in seen:
            seen.add(value)
            symbols.append(value)

    for symbol in linked_symbols or []:
        add(symbol)
    if not isinstance(metadata, dict):
        return symbols
    add(metadata.get("kernelName"))
    for item in metadata.get("kernelList", []):
        if isinstance(item, dict):
            add(item.get("kernelName"))

    def collect_function_names(value):
        if isinstance(value, dict):
            add(value.get("func_name"))
            for child in value.values():
                collect_function_names(child)
        elif isinstance(value, list):
            for child in value:
                collect_function_names(child)

    collect_function_names(metadata.get("sub_operator_kernel_name", {}))
    return symbols


def collect_local_include_files(source_path):
    """Collect the quoted-include closure that lives under the source directory."""
    source_path = os.path.realpath(source_path)
    source_root = os.path.dirname(source_path)
    visited = {source_path}
    result = []

    def visit(path):
        try:
            with open(path, "r", encoding="utf-8") as file_obj:
                content = file_obj.read()
        except UnicodeDecodeError:
            return
        for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"', content, re.MULTILINE):
            candidate = os.path.realpath(os.path.join(os.path.dirname(path), include))
            try:
                inside_root = os.path.commonpath([source_root, candidate]) == source_root
            except ValueError:
                inside_root = False
            if not inside_root or not os.path.isfile(candidate) or candidate in visited:
                continue
            visited.add(candidate)
            logical_path = os.path.relpath(candidate, source_root).replace(os.sep, "/")
            result.append((candidate, logical_path))
            visit(candidate)

    visit(source_path)
    return result


def resolve_symbol_tiling_sizes(symbols, tiling_keys, tiling_key_data_size, fallback_size):
    """Resolve per-symbol ABI byte sizes while keeping lookup symbol-based."""
    if not isinstance(fallback_size, int) or fallback_size <= 0:
        raise ValueError("fallback tiling size must be a positive integer")
    keys = sorted((str(key) for key in tiling_keys), key=len, reverse=True)
    result = {}
    for symbol in symbols:
        size = fallback_size
        for key in keys:
            if symbol.endswith("_" + key) or ("_" + key + "_") in symbol:
                candidate = tiling_key_data_size.get(key)
                if isinstance(candidate, int) and candidate > 0:
                    size = candidate
                    break
        result[symbol] = size
    return result


def static_tiling_template_text(dynamic_header):
    """Add a constant-tiling override while preserving dynamic DFX/OOM code."""
    if not isinstance(dynamic_header, str) or not dynamic_header:
        raise ValueError("dynamic tiling header must be non-empty text")
    class_names = [
        name
        for name in re.findall(r"InitTilingData<([A-Za-z_][A-Za-z0-9_:]*)>", dynamic_header)
        if name not in {"T", "tiling_struct", "tiling_type"}
    ]
    if not class_names:
        raise ValueError("cannot determine top-level tiling type from dynamic header")
    class_name = class_names[0]
    marker = "@@STATIC_VALUE_tiling_data@@"
    slash = "\\"
    override = "\n" + "\n".join(
        [
            "#ifdef CONST_TILING",
            "#ifdef GET_TILING_DATA",
            "#undef GET_TILING_DATA",
            "#endif",
            f"#define GET_TILING_DATA(tiling_data, tiling_arg) {slash}",
            f"    REGISTER_TILINGDATA_SIZE({class_name}, __COUNTER__); {slash}",
            f"    {marker} {slash}",
            f"    static constexpr {class_name} tiling_data = {slash}",
            f"        convert_from_bytes<{class_name}>(__ascendc_arr_##tiling_data);",
            "#endif",
            "",
        ]
    )
    final_endif = dynamic_header.rfind("#endif")
    if final_endif < 0:
        raise ValueError("dynamic tiling header has no closing #endif")
    return dynamic_header[:final_endif] + override + dynamic_header[final_endif:]


def portable_wrapper_text(wrapper_text, original_source_path):
    if not isinstance(wrapper_text, str):
        raise ValueError("wrapper source must be text")
    if not isinstance(original_source_path, str) or not original_source_path:
        raise ValueError("original source path must be non-empty")
    absolute_include = f'#include "{original_source_path}"'
    if absolute_include not in wrapper_text:
        return wrapper_text
    return wrapper_text.replace(
        absolute_include,
        f'#include "{os.path.basename(original_source_path)}"',
        1,
    )


def super_kernel_build_extension(
    *,
    normal_relocatable,
    normal_link_dependency,
    sk_symbol,
    sk_argv,
    sk_inputs,
    bind_argv,
    bind_input,
    output_prefix,
):
    """Build the optional SK graph appended after the normal base compile."""
    sk_output = output_prefix + ".sk.o"
    split_outputs = [output_prefix + f".sk_split{index}.o" for index in range(1, 4)]
    archive_output = output_prefix + ".sk.a"
    bind_output = output_prefix + ".sk_bind.o"
    merged_output = output_prefix + ".sk_merged.o"
    final_output = output_prefix + ".static_with_sk.o"
    steps = [
        {
            "id": "compile_static_sk",
            "kind": "compile",
            "backend": "bisheng",
            "argv": list(sk_argv),
            "depends_on": [normal_link_dependency],
            "inputs": list(sk_inputs),
            "outputs": [sk_output],
        }
    ]
    for index, split_output in enumerate(split_outputs, 1):
        steps.append(
            {
                "id": f"split_static_sk_{index}",
                "kind": "objcopy",
                "argv": [
                    f"--redefine-sym={sk_symbol}={sk_symbol}_split{index}",
                    "${artifact." + sk_output + "}",
                    "${artifact." + split_output + "}",
                ],
                "depends_on": ["compile_static_sk"],
                "inputs": [sk_output],
                "outputs": [split_output],
            }
        )
    steps.extend(
        [
            {
                "id": "archive_static_sk",
                "kind": "archive",
                "argv": ["rcs", "${artifact." + archive_output + "}"]
                + ["${artifact." + path + "}" for path in [sk_output] + split_outputs],
                "depends_on": [f"split_static_sk_{index}" for index in range(1, 4)],
                "inputs": [sk_output] + split_outputs,
                "outputs": [archive_output],
            },
            {
                "id": "compile_static_sk_bind",
                "kind": "compile",
                "backend": "bisheng",
                "argv": list(bind_argv),
                "depends_on": [normal_link_dependency],
                "inputs": [bind_input],
                "outputs": [bind_output],
            },
            {
                "id": "link_static_sk_objects",
                "kind": "link",
                "argv": [
                    "-r", "-o", "${artifact." + merged_output + "}",
                    "${artifact." + normal_relocatable + "}",
                    "${artifact." + bind_output + "}",
                    "${artifact." + archive_output + "}",
                ],
                "depends_on": [
                    "archive_static_sk",
                    "compile_static_sk_bind",
                    "compile_static_normal",
                ],
                "inputs": [normal_relocatable, bind_output, archive_output],
                "outputs": [merged_output],
            },
            {
                "id": "link_static_sk_final",
                "kind": "link",
                "argv": [
                    "-m", "aicorelinux", "-Ttext=0",
                    "${artifact." + merged_output + "}", "-static", "-o",
                    "${artifact." + final_output + "}", "-q",
                ],
                "depends_on": ["link_static_sk_objects"],
                "inputs": [merged_output],
                "outputs": [final_output],
            },
        ]
    )
    return {"super_kernel": {"steps": steps}}


def _sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as file_obj:
        while True:
            block = file_obj.read(1024 * 1024)
            if not block:
                break
            digest.update(block)
    return digest.hexdigest()


def _relative_path(value, context):
    if not isinstance(value, str) or not value:
        raise ValueError(f"{context} must be a non-empty string")
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or "." in path.parts:
        raise ValueError(f"{context} must stay inside the resource directory: {value}")
    return path.as_posix()


def _validate_static_target(target, context):
    allowed = {
        "name",
        "value_type",
        "required",
        "parameter_index",
        "byte_size",
    }
    unknown = set(target) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if not isinstance(target.get("name"), str) or not target["name"]:
        raise ValueError(f"{context}.name must be a non-empty string")
    if target.get("value_type") != "bytes":
        raise ValueError(f"{context}.value_type must be bytes")
    if not isinstance(target.get("required"), bool):
        raise ValueError(f"{context}.required must be bool")
    parameter_index = target.get("parameter_index")
    byte_size = target.get("byte_size")
    if not isinstance(parameter_index, int) or parameter_index < 0:
        raise ValueError(f"{context}.parameter_index must be a nonnegative integer")
    if not isinstance(byte_size, int) or byte_size <= 0:
        raise ValueError(f"{context}.byte_size must be a positive integer")


def _validate_kernel(kernel, context):
    allowed = {
        "kernel_name",
        "kernel_type",
        "architecture",
        "macros",
        "static_targets",
        "actions",
        "build",
    }
    unknown = set(kernel) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    for field in ("kernel_name", "kernel_type", "architecture"):
        if not isinstance(kernel.get(field), str) or not kernel[field]:
            raise ValueError(f"{context}.{field} must be a non-empty string")
    macros = kernel.get("macros")
    if not isinstance(macros, list):
        raise ValueError(f"{context}.macros must be a list")
    macro_names = set()
    for index, macro in enumerate(macros):
        macro_context = f"{context}.macros[{index}]"
        if not isinstance(macro, dict) or set(macro) != {"name", "value", "kind"}:
            raise ValueError(f"{macro_context} must contain name, value, and kind")
        if not all(isinstance(macro[field], str) and macro[field] for field in macro):
            raise ValueError(f"{macro_context} fields must be non-empty strings")
        if macro["name"] in macro_names:
            raise ValueError(f"{context} has duplicate macro {macro['name']}")
        macro_names.add(macro["name"])
    targets = kernel.get("static_targets")
    if not isinstance(targets, list):
        raise ValueError(f"{context}.static_targets must be a list")
    target_names = set()
    for index, target in enumerate(targets):
        if not isinstance(target, dict):
            raise ValueError(f"{context}.static_targets[{index}] must be an object")
        _validate_static_target(target, f"{context}.static_targets[{index}]")
        if target["name"] in target_names:
            raise ValueError(f"{context} has duplicate static target {target['name']}")
        target_names.add(target["name"])
    if not isinstance(kernel.get("actions"), list):
        raise ValueError(f"{context}.actions must be a list")
    build = kernel.get("build")
    if not isinstance(build, dict) or not isinstance(build.get("base"), dict):
        raise ValueError(f"{context}.build.base must be an object")
    if not isinstance(build["base"].get("steps"), list):
        raise ValueError(f"{context}.build.base.steps must be a list")


def _publish_directory(stage_dir, target_dir):
    backup_dir = target_dir + ".previous"
    if os.path.lexists(backup_dir):
        shutil.rmtree(backup_dir)
    moved_previous = False
    try:
        if os.path.lexists(target_dir):
            os.replace(target_dir, backup_dir)
            moved_previous = True
        os.replace(stage_dir, target_dir)
    except Exception:
        if moved_previous and not os.path.lexists(target_dir):
            os.replace(backup_dir, target_dir)
        raise
    if moved_previous:
        shutil.rmtree(backup_dir)


def generate_static_compile_resources(
    *,
    output_root,
    kernel_base,
    op_type,
    binary_path,
    resources,
    include_dirs,
    kernels,
):
    if not isinstance(kernel_base, str) or not kernel_base or os.path.basename(kernel_base) != kernel_base:
        raise ValueError("kernel_base must be one path component")
    if not isinstance(op_type, str) or not op_type:
        raise ValueError("op_type must be a non-empty string")
    if not os.path.isfile(binary_path):
        raise FileNotFoundError(binary_path)
    if not isinstance(resources, list) or not resources:
        raise ValueError("resources must be a non-empty list")
    if not isinstance(include_dirs, list):
        raise ValueError("include_dirs must be a list")
    if not isinstance(kernels, list) or not kernels:
        # Resource existence is checked first so publication failures remain diagnosable.
        for resource in resources:
            if not os.path.isfile(resource.get("source", "")):
                raise FileNotFoundError(resource.get("source", ""))
        raise ValueError("kernels must be a non-empty list")

    static_root = os.path.join(os.path.abspath(output_root), "static_compile")
    os.makedirs(static_root, exist_ok=True)
    stage_dir = tempfile.mkdtemp(prefix=f".{kernel_base}.", dir=static_root)
    target_dir = os.path.join(static_root, kernel_base)
    try:
        manifest_files = []
        seen_paths = set()
        for index, resource in enumerate(resources):
            if not isinstance(resource, dict) or set(resource) != {"source", "path"}:
                raise ValueError(f"resources[{index}] must contain source and path")
            source = resource["source"]
            if not os.path.isfile(source):
                raise FileNotFoundError(source)
            logical_path = _relative_path(resource["path"], f"resources[{index}].path")
            if logical_path in seen_paths:
                raise ValueError(f"duplicate resource path: {logical_path}")
            seen_paths.add(logical_path)
            destination = os.path.join(stage_dir, *PurePosixPath(logical_path).parts)
            os.makedirs(os.path.dirname(destination), exist_ok=True)
            shutil.copyfile(source, destination)
            manifest_files.append(
                {"path": logical_path, "sha256": _sha256_file(destination)}
            )

        normalized_include_dirs = []
        for index, include_dir in enumerate(include_dirs):
            logical_dir = _relative_path(include_dir, f"include_dirs[{index}]")
            normalized_include_dirs.append(logical_dir)
            os.makedirs(os.path.join(stage_dir, *PurePosixPath(logical_dir).parts), exist_ok=True)

        seen_kernels = set()
        normalized_kernels = []
        for index, kernel in enumerate(kernels):
            if not isinstance(kernel, dict):
                raise ValueError(f"kernels[{index}] must be an object")
            _validate_kernel(kernel, f"kernels[{index}]")
            if kernel["kernel_name"] in seen_kernels:
                raise ValueError(f"duplicate kernel symbol: {kernel['kernel_name']}")
            seen_kernels.add(kernel["kernel_name"])
            normalized_kernels.append(copy.deepcopy(kernel))

        manifest = {
            "contract_version": "3.0",
            "op": {
                "op_type": op_type,
                "resources": {
                    "files": sorted(manifest_files, key=lambda item: item["path"]),
                    "include_dirs": normalized_include_dirs,
                },
                "binary": {"sha256": _sha256_file(binary_path)},
                "kernels": normalized_kernels,
            },
        }
        manifest_path = os.path.join(stage_dir, "op_resource.json")
        with open(manifest_path, "w", encoding="utf-8") as file_obj:
            json.dump(manifest, file_obj, indent=2, sort_keys=True)
            file_obj.write("\n")
        _publish_directory(stage_dir, target_dir)
        stage_dir = ""
        return target_dir
    finally:
        if stage_dir and os.path.isdir(stage_dir):
            shutil.rmtree(stage_dir)
