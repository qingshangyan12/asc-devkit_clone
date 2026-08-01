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


_MAX_STATIC_INPUT_COUNT = 256
_MAX_STATIC_INPUT_BYTES = 16 * 1024 * 1024
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
_COMMA_OPTION_PREFIXES = ("-Wl,", "-Wa,", "-Wp,")
_COMMAND_TYPES = {
    "compile": "bisheng",
    "link": "ld.lld",
    "objcopy": "llvm-objcopy",
    "archive": "llvm-ar",
}
_PROGRAM_TO_COMMAND_TYPE = {
    "${env:ASCEND_HOME_PATH}/bin/bisheng": "bisheng",
    "${env:ASCEND_HOME_PATH}/bin/ld.lld": "ld.lld",
    "${env:ASCEND_HOME_PATH}/bin/llvm-objcopy": "llvm-objcopy",
    "${env:ASCEND_HOME_PATH}/bin/llvm-ar": "llvm-ar",
}
_ALLOWED_COMMAND_TYPES = frozenset(_PROGRAM_TO_COMMAND_TYPE.values()) | {"aclrt"}


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
    if value.startswith(_COMMA_OPTION_PREFIXES):
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
        if value.startswith(_COMMA_OPTION_PREFIXES):
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


def normalize_static_command_object(kind, argv, ascend_home, command_index):
    """Normalize one captured process invocation into a v6 command object."""
    normalized = normalize_static_command(kind, argv, ascend_home, command_index)
    return {"type": _COMMAND_TYPES[kind], "options": normalized[1:]}


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


def _validate_static_input(static_input, context, resource_paths):
    allowed = {"name", "required", "parameter_index", "byte_size", "patches"}
    unknown = set(static_input) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if not isinstance(static_input.get("name"), str) or not static_input["name"]:
        raise ValueError(f"{context}.name must be a non-empty string")
    if "required" in static_input and not isinstance(static_input["required"], bool):
        raise ValueError(f"{context}.required must be bool")
    parameter_index = static_input.get("parameter_index")
    byte_size = static_input.get("byte_size")
    if (
        type(parameter_index) is not int
        or parameter_index < 0
        or parameter_index > _MAX_INT64
    ):
        raise ValueError(f"{context}.parameter_index must be a nonnegative integer")
    if type(byte_size) is not int or byte_size <= 0:
        raise ValueError(f"{context}.byte_size must be a positive integer")
    if byte_size > _MAX_STATIC_INPUT_BYTES:
        raise ValueError(f"{context}.byte_size is outside the supported range")
    patches = static_input.get("patches", [])
    if "patches" in static_input and (
        not isinstance(patches, list) or not patches
    ):
        raise ValueError(f"{context}.patches must be a non-empty array when present")
    for index, patch in enumerate(patches):
        if not isinstance(patch, dict):
            raise ValueError(f"{context}.patches[{index}] must be an object")
        _validate_patch(patch, f"{context}.patches[{index}]", resource_paths)


def _validate_patch(patch, context, resource_paths):
    allowed = {"file", "token", "format"}
    unknown = set(patch) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if set(patch) - {"format"} != {"file", "token"}:
        raise ValueError(f"{context} must contain file and token")
    file_path = _resource_reference_path(patch["file"], f"{context}.file")
    if file_path not in resource_paths:
        raise ValueError(f"{context}.file references missing resource: {file_path}")
    if not isinstance(patch["token"], str) or not patch["token"]:
        raise ValueError(f"{context}.token must be a non-empty string")
    if patch.get("format", "cpp_uint8_array") not in {
        "cpp_uint8_array", "raw_text", "hex"
    }:
        raise ValueError(f"{context}.format is unsupported")


def _resource_path_exists(logical_path, resource_paths):
    if logical_path in resource_paths:
        return True
    prefix = logical_path.rstrip("/") + "/"
    return any(path.startswith(prefix) for path in resource_paths)


def _option_reference(argument):
    match = re.fullmatch(r"\$\{options:([A-Za-z0-9_.-]+)\}", argument)
    return match.group(1) if match else None


def _validate_argument(argument, context, resource_paths, *, allow_block_dim=False):
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
                if not _resource_path_exists(logical_path, resource_paths):
                    raise ValueError(
                        f"{context} references missing resource: {logical_path}"
                    )
        elif namespace == "context" and path == "block_dim":
            if not allow_block_dim:
                raise ValueError(f"{context} uses block_dim outside super_kernel")
        else:
            raise ValueError(f"{context} has unsupported placeholder namespace")
        scrubbed = scrubbed.replace(match.group(0), "PLACEHOLDER")
    if "${" in scrubbed or _contains_literal_absolute_path(scrubbed):
        raise ValueError(f"{context} contains a non-portable path or placeholder")


def _expand_and_validate_arguments(
    arguments, context, resource_paths, options, used_options, *, allow_empty=False,
    allow_block_dim=False,
):
    if not isinstance(arguments, list) or (not allow_empty and not arguments):
        qualifier = "an array" if allow_empty else "a non-empty array"
        raise ValueError(f"{context} must be {qualifier}")
    expanded = []
    for index, argument in enumerate(arguments):
        argument_context = f"{context}[{index}]"
        if not isinstance(argument, str) or not argument:
            raise ValueError(f"{argument_context} must be a non-empty string")
        key = _option_reference(argument)
        if key is not None:
            if key not in options:
                raise ValueError(f"{argument_context} references unknown options key {key}")
            used_options.add(key)
            expanded.extend(options[key])
            continue
        if "${options:" in argument:
            raise ValueError(
                f"{argument_context} must use ${{options:key}} as a complete array element"
            )
        expanded.append(argument)
    if len(expanded) > _MAX_ARGS:
        raise ValueError(f"{context} exceeds the expanded argument limit")
    for index, argument in enumerate(expanded):
        _validate_argument(
            argument, f"{context}[expanded:{index}]", resource_paths,
            allow_block_dim=allow_block_dim,
        )
    return expanded


def _validate_command_recipe(
    recipe, context, resource_paths, options, used_options, *, super_kernel=False
):
    if not isinstance(recipe, dict):
        raise ValueError(f"{context} must be an object")
    unknown = set(recipe) - {"commands", "artifact"}
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
        if _option_reference(command[0]) is not None:
            raise ValueError(f"{context}.commands[{command_index}] executable cannot be an options reference")
        expanded = _expand_and_validate_arguments(
            command, f"{context}.commands[{command_index}]", resource_paths,
            options, used_options, allow_block_dim=super_kernel,
        )
        expect_linker_path = False
        for argv_index, argument in enumerate(expanded):
            if expect_linker_path and _contains_compound_absolute_path(
                argument, allow_uri=False
            ):
                raise ValueError(
                    f"{context}.commands[{command_index}][expanded:{argv_index}] "
                    f"contains a non-portable absolute path: {argument}"
                )
            expect_linker_path = argument == "-R"
    artifact = recipe.get("artifact")
    if not isinstance(artifact, str) or not artifact:
        raise ValueError(f"{context}.artifact must be a non-empty path string")
    _relative_path(artifact, f"{context}.artifact")


def _validate_options(options, resource_paths):
    if options is None:
        return {}
    if not isinstance(options, dict) or not options:
        raise ValueError("options must be a non-empty object when present")
    normalized = copy.deepcopy(options)
    for key, values in normalized.items():
        if not isinstance(key, str) or not re.fullmatch(r"[A-Za-z0-9_.-]+", key):
            raise ValueError(f"options key is not portable: {key}")
        if not isinstance(values, list) or not values:
            raise ValueError(f"options.{key} must be a non-empty array")
        for index, value in enumerate(values):
            if not isinstance(value, str) or not value:
                raise ValueError(f"options.{key}[{index}] must be a non-empty string")
            if "${options:" in value:
                raise ValueError(f"options.{key} cannot reference another options key")
            _validate_argument(value, f"options.{key}[{index}]", resource_paths)
    return normalized


def _validate_aclrtc_backend(backend, context, resource_paths, options, used_options):
    if not isinstance(backend, dict):
        raise ValueError(f"{context} must be an object")
    unknown = set(backend) - {"entry_file", "compile_options", "link_options"}
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    entry_file = _resource_reference_path(backend.get("entry_file", ""), f"{context}.entry_file")
    if entry_file not in resource_paths:
        raise ValueError(f"{context}.entry_file references missing resource: {entry_file}")
    compile_options = _expand_and_validate_arguments(
        backend.get("compile_options"), f"{context}.compile_options",
        resource_paths, options, used_options,
    )
    link_options = _expand_and_validate_arguments(
        backend.get("link_options"), f"{context}.link_options",
        resource_paths, options, used_options, allow_empty=True,
    )
    if len(compile_options) + len(link_options) > _MAX_ARGS:
        raise ValueError(f"{context} exceeds the combined ACLRTC option limit")


def _legacy_recipes_to_commands(recipes, context):
    """Temporary compile_op bridge; emitted manifests are v6-only."""
    if not isinstance(recipes, dict) or len(recipes) != 1:
        raise ValueError(f"{context}.recipes must select exactly one backend")
    if "aclrtc" in recipes:
        backend = recipes["aclrtc"]
        return [{
            "type": "aclrt",
            "input": backend.get("entry_file"),
            "options": list(backend.get("compile_options", []))
            + list(backend.get("link_options", [])),
        }]
    if "command" not in recipes or not isinstance(recipes["command"], dict):
        raise ValueError(f"{context}.recipes has unsupported backend")
    commands = []
    for recipe_name, mode in (("normal", None), ("super_kernel", ["sk"])):
        recipe = recipes["command"].get(recipe_name)
        if recipe is None:
            continue
        artifact = recipe.get("artifact")
        legacy_commands = recipe.get("commands")
        if not isinstance(legacy_commands, list) or not legacy_commands:
            raise ValueError(
                f"{context}.recipes.command.{recipe_name}.commands must be a bounded non-empty array"
            )
        for index, argv in enumerate(legacy_commands):
            if not isinstance(argv, list) or not argv:
                raise ValueError(
                    f"{context}.recipes.command.{recipe_name}.commands[{index}] must be a non-empty argv array"
                )
            command_type = _PROGRAM_TO_COMMAND_TYPE.get(argv[0])
            legacy_options = list(argv[1:])
            if command_type is None:
                # Old generator callers used argv[0] as data in validation-only
                # fixtures. Keep it visible to v6 validation during the temporary
                # compile_op bridge; published compile_op commands use the strict map.
                command_type = "bisheng"
                legacy_options = list(argv)
            command = {"type": command_type, "options": legacy_options}
            if mode is not None:
                command["mode"] = mode
                command["options"] = [
                    "${runtime_options:kernel_compile}"
                    if value == "-D__SUPER_KERNEL_STATIC_BLOCK_NUM__=${context:block_dim}"
                    else value
                    for value in command["options"]
                ]
            if index + 1 == len(legacy_commands):
                command["output"] = artifact
                output_reference = "${output:" + str(artifact) + "}"
                command["options"] = [
                    "${command:output}" if value == output_reference else value
                    for value in command["options"]
                ]
            commands.append(command)
    if not commands:
        raise ValueError(f"{context}.recipes.command has no commands")
    return commands


def _validate_v6_command(command, context, resource_paths, options, used_options):
    if not isinstance(command, dict):
        raise ValueError(f"{context} must be an object")
    unknown = set(command) - {"type", "mode", "options", "input", "output"}
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    command_type = command.get("type")
    if command_type not in _ALLOWED_COMMAND_TYPES:
        raise ValueError(f"{context}.type is not a supported command type")
    modes = command.get("mode", ["standard"])
    if not isinstance(modes, list) or not modes:
        raise ValueError(f"{context}.mode must be a non-empty array")
    if any(
        not isinstance(mode, str)
        or re.fullmatch(r"[A-Za-z0-9_.-]+", mode) is None
        for mode in modes
    ):
        raise ValueError(f"{context}.mode contains a non-portable name")
    if len(set(modes)) != len(modes):
        raise ValueError(f"{context}.mode contains duplicates")
    if command_type == "aclrt" and modes != ["standard"]:
        raise ValueError(f"{context} aclrt only supports standard mode")
    for field in ("input", "output"):
        if field in command:
            command[field] = _relative_path(command[field], f"{context}.{field}")
    if command_type == "aclrt":
        input_path = command.get("input")
        if input_path is None:
            raise ValueError(f"{context} aclrt requires input")
        if input_path not in resource_paths:
            raise ValueError(f"{context}.input references missing resource: {input_path}")

    arguments = command.get("options")
    if not isinstance(arguments, list) or len(arguments) > _MAX_ARGS:
        raise ValueError(f"{context}.options must be a bounded array")
    expanded_count = 0
    for index, argument in enumerate(arguments):
        argument_context = f"{context}.options[{index}]"
        if not isinstance(argument, str) or not argument:
            raise ValueError(f"{argument_context} must be a non-empty string")
        if (
            index > 0
            and arguments[index - 1] == "-R"
            and _contains_compound_absolute_path(argument, allow_uri=False)
        ):
            raise ValueError(
                f"{argument_context} contains a non-portable absolute path: {argument}"
            )
        key = _option_reference(argument)
        if key is not None:
            if key not in options:
                raise ValueError(f"{argument_context} references unknown options key {key}")
            used_options.add(key)
            expanded_count += len(options[key])
            continue
        if "${options:" in argument:
            raise ValueError(
                f"{argument_context} must use ${{options:key}} as a complete array element"
            )
        runtime = re.fullmatch(r"\$\{runtime_options:([A-Za-z0-9_.-]+)\}", argument)
        if runtime:
            expanded_count += 1
            continue
        if "${runtime_options:" in argument:
            raise ValueError(
                f"{argument_context} must use ${{runtime_options:key}} as a complete array element"
            )
        if argument in {"${command:input}", "${command:output}"}:
            field = argument[len("${command:"):-1]
            if field not in command:
                raise ValueError(f"{argument_context} uses {argument} without {field}")
            expanded_count += 1
            continue
        if "${command:" in argument:
            raise ValueError(f"{argument_context} has unsupported command placeholder")
        _validate_argument(argument, argument_context, resource_paths)
        expanded_count += 1
    if expanded_count > _MAX_ARGS:
        raise ValueError(f"{context}.options exceeds the expanded argument limit")
    return modes


def _normalize_and_validate_kernel(
    kernel, context, resource_paths, options, used_options
):
    allowed = {"kernel_name", "static_inputs", "commands", "recipes"}
    unknown = set(kernel) - allowed
    if unknown:
        raise ValueError(f"{context} has unknown fields: {sorted(unknown)}")
    if not isinstance(kernel.get("kernel_name"), str) or not kernel["kernel_name"]:
        raise ValueError(f"{context}.kernel_name must be a non-empty string")
    static_inputs = kernel.get("static_inputs", [])
    if "static_inputs" in kernel and (
        not isinstance(static_inputs, list)
        or not static_inputs
        or len(static_inputs) > _MAX_STATIC_INPUT_COUNT
    ):
        raise ValueError(
            f"{context}.static_inputs must be a bounded non-empty array when present"
        )
    input_names = set()
    for index, static_input in enumerate(static_inputs):
        if not isinstance(static_input, dict):
            raise ValueError(f"{context}.static_inputs[{index}] must be an object")
        _validate_static_input(
            static_input, f"{context}.static_inputs[{index}]", resource_paths
        )
        if static_input["name"] in input_names:
            raise ValueError(
                f"{context} has duplicate static input {static_input['name']}"
            )
        input_names.add(static_input["name"])
    normalized = copy.deepcopy(kernel)
    if "commands" in normalized and "recipes" in normalized:
        raise ValueError(f"{context} cannot contain both commands and recipes")
    if "recipes" in normalized:
        normalized["commands"] = _legacy_recipes_to_commands(
            normalized.pop("recipes"), context
        )
    commands = normalized.get("commands")
    if not isinstance(commands, list) or not commands or len(commands) > _MAX_COMMANDS:
        raise ValueError(f"{context}.commands must be a bounded non-empty array")
    mode_last_command = {}
    for index, command in enumerate(commands):
        modes = _validate_v6_command(
            command, f"{context}.commands[{index}]", resource_paths,
            options, used_options,
        )
        for mode in modes:
            mode_last_command[mode] = command
    if "standard" not in mode_last_command:
        raise ValueError(f"{context} must provide standard mode")
    for mode, command in mode_last_command.items():
        if command["type"] != "aclrt" and "output" not in command:
            raise ValueError(f"{context}.{mode} requires a final explicit output")

    for static_input in normalized.get("static_inputs", []):
        if static_input.get("required") is True:
            static_input.pop("required")
        for patch in static_input.get("patches", []):
            if patch.get("format") == "cpp_uint8_array":
                patch.pop("format")
    return normalized

    recipes = kernel.get("recipes")
    if not isinstance(recipes, dict) or not recipes:
        raise ValueError(f"{context}.recipes must be a non-empty object")
    unknown_recipes = set(recipes) - {"command", "aclrtc"}
    if unknown_recipes:
        raise ValueError(f"{context}.recipes has unknown recipes: {sorted(unknown_recipes)}")
    if len(recipes) != 1:
        raise ValueError(f"{context}.recipes must select exactly one of command or aclrtc")
    if "command" in recipes:
        command = recipes["command"]
        if not isinstance(command, dict) or not command:
            raise ValueError(f"{context}.recipes.command must be a non-empty object")
        unknown_commands = set(command) - {"normal", "super_kernel"}
        if unknown_commands:
            raise ValueError(
                f"{context}.recipes.command has unknown recipes: {sorted(unknown_commands)}"
            )
        for name, recipe in command.items():
            _validate_command_recipe(
                recipe, f"{context}.recipes.command.{name}", resource_paths,
                options, used_options, super_kernel=name == "super_kernel",
            )
    else:
        _validate_aclrtc_backend(
            recipes["aclrtc"], f"{context}.recipes.aclrtc", resource_paths,
            options, used_options,
        )
    normalized = copy.deepcopy(kernel)
    for static_input in normalized.get("static_inputs", []):
        if static_input.get("required") is True:
            static_input.pop("required")
        for patch in static_input.get("patches", []):
            if patch.get("format") == "cpp_uint8_array":
                patch.pop("format")
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
    options=None,
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

        normalized_options = _validate_options(options, seen_paths)
        used_options = set()
        seen_kernels = set()
        normalized_kernels = []
        for index, kernel in enumerate(kernels):
            if not isinstance(kernel, dict):
                raise ValueError(f"kernels[{index}] must be an object")
            normalized = _normalize_and_validate_kernel(
                kernel, f"kernels[{index}]", seen_paths,
                normalized_options, used_options,
            )
            if normalized["kernel_name"] in seen_kernels:
                raise ValueError(
                    f"duplicate kernel symbol: {normalized['kernel_name']}"
                )
            seen_kernels.add(normalized["kernel_name"])
            normalized_kernels.append(normalized)

        unused_options = set(normalized_options) - used_options
        if unused_options:
            raise ValueError(f"unused options keys: {sorted(unused_options)}")

        manifest = {
            "schema_version": "6.0",
            "resource_id": resource_id,
            "kernels": normalized_kernels,
        }
        if normalized_options:
            manifest["options"] = normalized_options
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
