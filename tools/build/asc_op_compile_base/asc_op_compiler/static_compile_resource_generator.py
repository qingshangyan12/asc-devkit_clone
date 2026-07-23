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
import json
import os
import posixpath
import re
import shutil
import tempfile
from pathlib import PurePosixPath


_MAX_TARGET_COUNT = 256
_MAX_TARGET_BYTES = 16 * 1024 * 1024
_MAX_COMMANDS = 64
_MAX_ARGS = 256
_MAX_INT64 = (1 << 63) - 1
_RESERVED_MACHINE_MANIFEST = ".sc/manifest.json"
_JOINED_PATH_PREFIXES = (
    "--gcc-toolchain=", "-iwithprefixbefore", "-iwithprefix",
    "--sysroot=", "-idirafter", "-isysroot", "-isystem", "-imacros",
    "-include", "-iprefix", "-iquote", "-MF", "-I", "-L", "-B", "-R",
    "-T", "-o",
)
_LINKER_PATH_LIST_OPTIONS = frozenset({
    "-R", "-rpath", "--rpath", "-rpath-link", "--rpath-link",
})


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
    if symbols:
        return symbols
    if not isinstance(metadata, dict):
        return symbols
    for item in metadata.get("kernelList", []):
        if isinstance(item, dict):
            add(item.get("kernelName"))
    if not symbols:
        add(metadata.get("kernelName"))
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
            if not inside_root:
                raise ValueError(
                    f'quoted include "{include}" in {path} resolves outside source root'
                )
            if not os.path.isfile(candidate) or candidate in visited:
                continue
            visited.add(candidate)
            logical_path = os.path.relpath(candidate, source_root).replace(os.sep, "/")
            result.append((candidate, logical_path))
            visit(candidate)

    visit(source_path)
    return result


def resolve_symbol_tiling_sizes(symbols, tiling_keys, tiling_key_data_size, fallback_size):
    """Resolve per-symbol ABI byte sizes while keeping lookup symbol-based."""
    if type(fallback_size) is not int or fallback_size <= 0:
        raise ValueError("fallback tiling size must be a positive integer")
    keys = sorted((str(key) for key in tiling_keys), key=len, reverse=True)
    result = {}
    for symbol in symbols:
        size = fallback_size
        matched_key = None
        for key in keys:
            if symbol.endswith("_" + key) or ("_" + key + "_") in symbol:
                matched_key = key
                candidate = tiling_key_data_size.get(key)
                if type(candidate) is int and candidate > 0:
                    size = candidate
                break
        if len(keys) > 1 and (
            matched_key is None
            or type(tiling_key_data_size.get(matched_key)) is not int
            or tiling_key_data_size[matched_key] <= 0
        ):
            raise ValueError(
                f"exact tiling byte size is unavailable for kernel symbol {symbol}"
            )
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
    class_names = [name for name in class_names if name not in {"type", "Type"}]
    class_name = class_names[0] if class_names else None
    marker = "@@STATIC_VALUE_tiling_data@@"
    slash = "\\"
    lines = [
        "#ifdef CONST_TILING",
        f"static constexpr uint8_t __ascendc_static_tiling_data[] = {marker};",
    ]
    for macro in ("GET_TILING_DATA", "GET_TILING_DATA_WITH_STRUCT", "GET_TILING_DATA_MEMBER"):
        lines.extend([f"#ifdef {macro}", f"#undef {macro}", "#endif"])
    if class_name is None:
        lines.append("#define GET_TILING_DATA(tiling_data, tiling_arg)")
    else:
        lines.extend([
            f"#define GET_TILING_DATA(tiling_data, tiling_arg) {slash}",
            f"    REGISTER_TILINGDATA_SIZE({class_name}, __COUNTER__); {slash}",
            f"    static constexpr {class_name} tiling_data = {slash}",
            f"        convert_from_bytes<{class_name}>(__ascendc_static_tiling_data);",
        ])
    lines.extend([
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
    ])
    override = "\n" + "\n".join(lines)
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


def normalize_static_program(kind):
    try:
        return {
            "compile": "${env:ASCEND_HOME_PATH}/bin/bisheng",
            "link": "${env:ASCEND_HOME_PATH}/bin/ld.lld",
            "objcopy": "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy",
            "archive": "${env:ASCEND_HOME_PATH}/bin/llvm-ar",
        }[kind]
    except KeyError as error:
        raise ValueError(f"unsupported static command kind: {kind}") from error


def _is_uri_component(parts, index):
    if (
        index < 2
        or parts[index - 1] != ":"
        or not parts[index].startswith("//")
    ):
        return False
    return re.search(
        r"(?:^|=)[A-Za-z][A-Za-z0-9+.-]*$", parts[index - 2]
    ) is not None


def _contains_compound_absolute_path(value, allow_uri=True):
    parts = re.split(r"([:,])", value)
    return any(
        parts[index].startswith("/")
        and not (allow_uri and _is_uri_component(parts, index))
        for index in range(0, len(parts), 2)
    )


def _is_linker_path_list_value(fields, field_index):
    field = fields[field_index]
    if field_index > 0 and fields[field_index - 1] in _LINKER_PATH_LIST_OPTIONS:
        return True
    if field.startswith("-R"):
        return True
    return any(
        field.startswith(option + "=")
        for option in _LINKER_PATH_LIST_OPTIONS - {"-R"}
    )


def _contains_literal_absolute_path(value):
    if value.startswith(("/", "@/")):
        return True
    for prefix in _JOINED_PATH_PREFIXES:
        if value.startswith(prefix) and len(value) > len(prefix):
            if value[len(prefix)] == "/":
                return True
    if "=/" in value:
        return True
    if value.startswith(("-Wl,", "-Wa,")):
        fields = value.split(",")
        for field_index, field in enumerate(fields[1:], 1):
            path_parts = _split_absolute_path_argument(field)
            if path_parts is not None and path_parts[1].startswith("/"):
                return True
            allow_uri = not _is_linker_path_list_value(fields, field_index)
            if _contains_compound_absolute_path(field, allow_uri=allow_uri):
                return True
        return False
    equals_index = value.find("=")
    if equals_index >= 0:
        return _contains_compound_absolute_path(value[equals_index + 1:])
    for prefix in ("@",) + _JOINED_PATH_PREFIXES:
        if value.startswith(prefix):
            return _contains_compound_absolute_path(
                value[len(prefix):], allow_uri=prefix == "@"
            )
    return False


def _split_absolute_path_argument(value, extra_prefixes=()):
    if value.startswith("/"):
        return "", value
    if value.startswith("${env:"):
        return "", value
    for prefix in tuple(extra_prefixes) + ("@",) + _JOINED_PATH_PREFIXES:
        path = value[len(prefix):] if value.startswith(prefix) else ""
        if path.startswith(("/", "${env:")):
            return prefix, path
    equals_index = value.find("=/")
    if equals_index >= 0:
        return value[:equals_index + 1], value[equals_index + 1:]
    equals_index = value.find("=${env:")
    if equals_index >= 0:
        return value[:equals_index + 1], value[equals_index + 1:]
    return None


def _canonical_cann_argument(path, cann_root):
    canonical_path = os.path.realpath(os.path.normpath(path))
    if not cann_root:
        return None, canonical_path
    try:
        owned = os.path.commonpath([cann_root, canonical_path]) == cann_root
    except ValueError:
        owned = False
    if not owned:
        return None, canonical_path
    relative = os.path.relpath(canonical_path, cann_root).replace(os.sep, "/")
    if relative == ".":
        return "${env:ASCEND_HOME_PATH}", canonical_path
    relative = _relative_path(relative, "CANN command path")
    return "${env:ASCEND_HOME_PATH}/" + relative, canonical_path


def _normalize_cann_path_list(path_list, cann_root, allow_uri=True):
    parts = re.split(r"([:,])", path_list)
    for index in range(0, len(parts), 2):
        if (
            not parts[index].startswith("/")
            or (allow_uri and _is_uri_component(parts, index))
        ):
            continue
        portable_path, canonical_path = _canonical_cann_argument(
            parts[index], cann_root
        )
        if portable_path is None:
            return None, canonical_path
        parts[index] = portable_path
    return "".join(parts), None


def normalize_static_command(kind, argv, ascend_home, command_index):
    """Return a shell-free command whose paths are portable across CANN installs."""
    if not isinstance(argv, (list, tuple)):
        raise ValueError(f"command[{command_index}] argv must be a list")
    result = [normalize_static_program(kind)]
    cann_root = (
        os.path.realpath(os.path.normpath(os.path.abspath(ascend_home))).rstrip(os.sep)
        if ascend_home
        else ""
    )
    for argv_index, argument in enumerate(argv):
        if not isinstance(argument, str) or not argument:
            raise ValueError(
                f"command[{command_index}][{argv_index}] must be a non-empty string"
            )
        value = argument.replace("${resource.", "${resource:")
        value = value.replace("${artifact.", "${output:")
        value = value.replace("${launch.block_dim}", "${context:block_dim}")
        separate_linker_path = argv_index > 0 and argv[argv_index - 1] == "-R"
        if separate_linker_path:
            portable_path, _ = _normalize_cann_path_list(
                value, cann_root, allow_uri=False
            )
            if portable_path is None:
                raise ValueError(
                    f"command[{command_index}][{argv_index}] contains "
                    f"non-portable absolute path: {argument}"
                )
            value = portable_path
        if value.startswith(("-Wl,", "-Wa,")):
            fields = value.split(",")
            for field_index in range(1, len(fields)):
                field_parts = _split_absolute_path_argument(fields[field_index])
                prefix, path = (
                    field_parts
                    if field_parts is not None
                    else ("", fields[field_index])
                )
                allow_uri = not _is_linker_path_list_value(fields, field_index)
                portable_path, _ = _normalize_cann_path_list(
                    path, cann_root, allow_uri=allow_uri
                )
                if portable_path is None:
                    raise ValueError(
                        f"command[{command_index}][{argv_index}] contains "
                        f"non-portable absolute path: {argument}"
                    )
                fields[field_index] = prefix + portable_path
            value = ",".join(fields)
        absolute_parts = _split_absolute_path_argument(value)
        if absolute_parts is not None:
            prefix, path = absolute_parts
            portable_path, canonical_path = _normalize_cann_path_list(
                path, cann_root, allow_uri=prefix not in _JOINED_PATH_PREFIXES
            )
            if portable_path is not None:
                value = prefix + portable_path
            else:
                standalone_include = (
                    prefix == ""
                    and argv_index > 0
                    and argv[argv_index - 1] == "-I"
                )
                include_prefix = prefix in {"-I", "-I="} or standalone_include
                if (
                    include_prefix
                    and ":" not in path
                    and "," not in path
                    and not os.path.isdir(canonical_path)
                ):
                    if standalone_include and result[-1] == "-I":
                        result.pop()
                    continue
                raise ValueError(
                    f"command[{command_index}][{argv_index}] contains "
                    f"non-portable absolute path: {argument}"
                )
        env_path = re.search(r"(\$\{env:[A-Z_][A-Z0-9_]*\})/([^}]*)$", value)
        if env_path:
            normalized_suffix = posixpath.normpath(env_path.group(2))
            value = (
                value[:env_path.start()]
                + env_path.group(1)
                + "/"
                + normalized_suffix
            )
        if _contains_literal_absolute_path(value):
            raise ValueError(
                f"command[{command_index}][{argv_index}] contains "
                f"non-portable absolute path: {argument}"
            )
        result.append(value)
    return result


def _relative_path(value, context):
    if not isinstance(value, str) or not value:
        raise ValueError(f"{context} must be a non-empty string")
    raw_parts = value.split("/")
    if "\\" in value or any(part in {"", ".", ".."} for part in raw_parts):
        raise ValueError(f"{context} must stay inside the resource directory: {value}")
    path = PurePosixPath(value)
    if path.is_absolute() or ".." in path.parts or "." in path.parts:
        raise ValueError(f"{context} must stay inside the resource directory: {value}")
    return path.as_posix()


def _resource_reference_path(value, context):
    logical_path = _relative_path(value, context)
    if logical_path == _RESERVED_MACHINE_MANIFEST:
        raise ValueError(f"{context} names the reserved machine manifest")
    if logical_path == "manifest.json":
        raise ValueError(f"{context} names the reserved author manifest")
    return logical_path


def _validate_target(target, context):
    allowed = {"name", "required", "parameter_index", "byte_size"}
    unknown = set(target) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if not isinstance(target.get("name"), str) or not target["name"]:
        raise ValueError(f"{context}.name must be a non-empty string")
    if "required" in target and not isinstance(target["required"], bool):
        raise ValueError(f"{context}.required must be bool")
    parameter_index = target.get("parameter_index")
    byte_size = target.get("byte_size")
    if (
        type(parameter_index) is not int
        or parameter_index < 0
        or parameter_index > _MAX_INT64
    ):
        raise ValueError(f"{context}.parameter_index must be a nonnegative integer")
    if type(byte_size) is not int or byte_size <= 0:
        raise ValueError(f"{context}.byte_size must be a positive integer")
    if byte_size > _MAX_TARGET_BYTES:
        raise ValueError(f"{context}.byte_size is outside the supported range")


def _validate_patch(patch, context, target_names, resource_paths):
    allowed = {"file", "token", "value", "format"}
    unknown = set(patch) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if set(patch) - {"format"} != {"file", "token", "value"}:
        raise ValueError(f"{context} must contain file, token, and value")
    file_path = _resource_reference_path(patch["file"], f"{context}.file")
    if file_path not in resource_paths:
        raise ValueError(f"{context}.file references missing resource: {file_path}")
    if not isinstance(patch["token"], str) or not patch["token"]:
        raise ValueError(f"{context}.token must be a non-empty string")
    if patch["value"] not in target_names:
        raise ValueError(f"{context}.value references unknown target: {patch['value']}")
    if patch.get("format", "cpp_uint8_array") not in {
        "cpp_uint8_array", "raw_text", "hex"
    }:
        raise ValueError(f"{context}.format is unsupported")


def _validate_argument(argument, context, resource_paths):
    if not isinstance(argument, str) or not argument:
        raise ValueError(f"{context} must be a non-empty string")
    legacy = re.search(r"\$\{(?:resource|artifact)\.", argument)
    if legacy:
        raise ValueError(f"{context} contains a legacy placeholder")
    placeholders = list(re.finditer(r"\$\{([^}:]+):([^}]+)\}", argument))
    scrubbed = argument
    for match in placeholders:
        namespace, path = match.groups()
        if namespace == "env":
            if not re.fullmatch(r"[A-Z_][A-Z0-9_]*", path):
                raise ValueError(f"{context} has invalid environment placeholder")
            suffix = argument[match.end():]
            if suffix.startswith("/"):
                _relative_path(suffix[1:], context)
        elif namespace in {"resource", "output"}:
            logical_path = _relative_path(path, context)
            if match.end() != len(argument):
                raise ValueError(
                    f"{context} has a path-changing suffix after "
                    f"${{{namespace}:{path}}}"
                )
            if namespace == "resource":
                logical_path = _resource_reference_path(path, context)
                if logical_path not in resource_paths:
                    raise ValueError(
                        f"{context} references missing resource: {logical_path}"
                    )
        elif not (namespace == "context" and path == "block_dim"):
            raise ValueError(f"{context} has unsupported placeholder namespace")
        scrubbed = scrubbed.replace(match.group(0), "PLACEHOLDER")
    if "${" in scrubbed or _contains_literal_absolute_path(scrubbed):
        raise ValueError(f"{context} contains a non-portable path or placeholder")


def _validate_recipe(recipe, context, kernel_name, target_names, resource_paths):
    if not isinstance(recipe, dict):
        raise ValueError(f"{context} must be an object")
    unknown = set(recipe) - {"commands", "artifact", "patches"}
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    commands = recipe.get("commands")
    if (
        not isinstance(commands, list)
        or not commands
        or len(commands) > _MAX_COMMANDS
    ):
        raise ValueError(f"{context}.commands must be a bounded non-empty array")
    for command_index, command in enumerate(commands):
        if (
            not isinstance(command, list)
            or not command
            or len(command) > _MAX_ARGS
        ):
            raise ValueError(
                f"{context}.commands[{command_index}] must be a bounded "
                "non-empty argv array"
            )
        expect_linker_path = False
        for argv_index, argument in enumerate(command):
            if expect_linker_path and _contains_compound_absolute_path(
                argument, allow_uri=False
            ):
                raise ValueError(
                    f"{context}.commands[{command_index}][{argv_index}] "
                    f"contains a non-portable absolute path: {argument}"
                )
            _validate_argument(
                argument,
                f"{context}.commands[{command_index}][{argv_index}]",
                resource_paths,
            )
            expect_linker_path = argument == "-R"
    artifact = recipe.get("artifact")
    if isinstance(artifact, str):
        _relative_path(artifact, f"{context}.artifact")
    elif isinstance(artifact, dict):
        unknown = set(artifact) - {"file", "entry", "format"}
        if unknown or "file" not in artifact:
            raise ValueError(f"{context}.artifact has invalid fields")
        _relative_path(artifact["file"], f"{context}.artifact.file")
        entry = artifact.get("entry", kernel_name)
        if not isinstance(entry, str) or not entry:
            raise ValueError(f"{context}.artifact.entry must be a non-empty string")
        if artifact.get("format", "elf") not in {"elf", "archive"}:
            raise ValueError(f"{context}.artifact.format is unsupported")
    else:
        raise ValueError(f"{context}.artifact must be a path or object")
    if "patches" in recipe and (
        not isinstance(recipe["patches"], list) or not recipe["patches"]
    ):
        raise ValueError(f"{context}.patches must be a non-empty array when present")
    for index, patch in enumerate(recipe.get("patches", [])):
        if not isinstance(patch, dict):
            raise ValueError(f"{context}.patches[{index}] must be an object")
        _validate_patch(
            patch, f"{context}.patches[{index}]", target_names, resource_paths
        )


def _normalize_and_validate_kernel(kernel, context, resource_paths):
    allowed = {"kernel_name", "targets", "patches", "recipes"}
    unknown = set(kernel) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if not isinstance(kernel.get("kernel_name"), str) or not kernel["kernel_name"]:
        raise ValueError(f"{context}.kernel_name must be a non-empty string")
    targets = kernel.get("targets", [])
    if "targets" in kernel and (
        not isinstance(targets, list)
        or not targets
        or len(targets) > _MAX_TARGET_COUNT
    ):
        raise ValueError(
            f"{context}.targets must be a bounded non-empty array when present"
        )
    target_names = set()
    for index, target in enumerate(targets):
        if not isinstance(target, dict):
            raise ValueError(f"{context}.targets[{index}] must be an object")
        _validate_target(target, f"{context}.targets[{index}]")
        if target["name"] in target_names:
            raise ValueError(f"{context} has duplicate target {target['name']}")
        target_names.add(target["name"])
    patches = kernel.get("patches", [])
    if "patches" in kernel and (
        not isinstance(patches, list) or not patches
    ):
        raise ValueError(f"{context}.patches must be a non-empty array when present")
    for index, patch in enumerate(patches):
        if not isinstance(patch, dict):
            raise ValueError(f"{context}.patches[{index}] must be an object")
        _validate_patch(
            patch, f"{context}.patches[{index}]", target_names, resource_paths
        )
    recipes = kernel.get("recipes")
    if not isinstance(recipes, dict) or not recipes:
        raise ValueError(f"{context}.recipes must be a non-empty object")
    unknown_recipes = set(recipes) - {"normal", "super_kernel"}
    if unknown_recipes:
        raise ValueError(f"{context}.recipes has unknown recipes: {sorted(unknown_recipes)}")
    for name, recipe in recipes.items():
        _validate_recipe(
            recipe, f"{context}.recipes.{name}", kernel["kernel_name"],
            target_names, resource_paths,
        )
    normalized = copy.deepcopy(kernel)
    for target in normalized.get("targets", []):
        if target.get("required") is True:
            target.pop("required")
    for patch in normalized.get("patches", []):
        if patch.get("format") == "cpp_uint8_array":
            patch.pop("format")
    for recipe in normalized["recipes"].values():
        for patch in recipe.get("patches", []):
            if patch.get("format") == "cpp_uint8_array":
                patch.pop("format")
        artifact = recipe.get("artifact")
        if isinstance(artifact, dict):
            if artifact.get("entry") == kernel["kernel_name"]:
                artifact.pop("entry")
            if artifact.get("format") == "elf":
                artifact.pop("format")
    return normalized


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
    resource_id,
    resources,
    kernels,
):
    if (
        not isinstance(kernel_base, str)
        or not kernel_base
        or kernel_base in {".", ".."}
        or os.path.basename(kernel_base) != kernel_base
        or "\\" in kernel_base
    ):
        raise ValueError("kernel_base must be one path component")
    if not isinstance(resource_id, str) or re.fullmatch(
        r"[A-Za-z0-9][A-Za-z0-9_.-]*", resource_id
    ) is None:
        raise ValueError("resource_id must be a non-empty portable name")
    if not isinstance(resources, list) or not resources:
        raise ValueError("resources must be a non-empty list")
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

        seen_kernels = set()
        normalized_kernels = []
        for index, kernel in enumerate(kernels):
            if not isinstance(kernel, dict):
                raise ValueError(f"kernels[{index}] must be an object")
            normalized = _normalize_and_validate_kernel(
                kernel, f"kernels[{index}]", seen_paths
            )
            if normalized["kernel_name"] in seen_kernels:
                raise ValueError(
                    f"duplicate kernel symbol: {normalized['kernel_name']}"
                )
            seen_kernels.add(normalized["kernel_name"])
            normalized_kernels.append(normalized)

        manifest = {
            "schema_version": "4.0",
            "resource_id": resource_id,
            "kernels": normalized_kernels,
        }
        manifest_path = os.path.join(stage_dir, "manifest.json")
        with open(manifest_path, "w", encoding="utf-8") as file_obj:
            json.dump(manifest, file_obj, indent=2, sort_keys=True)
            file_obj.write("\n")
        _publish_directory(stage_dir, target_dir)
        stage_dir = ""
        return target_dir
    finally:
        if stage_dir and os.path.isdir(stage_dir):
            shutil.rmtree(stage_dir)
