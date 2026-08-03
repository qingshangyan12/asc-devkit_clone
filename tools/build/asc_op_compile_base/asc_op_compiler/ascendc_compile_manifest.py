#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# ----------------------------------------------------------------------------------------------------------
"""Generate the schema 4 compile-resource manifest for one Ascend C kernel."""

import hashlib
import json
import os
import re
import shlex
import shutil
import stat

from asc_op_compile_base.common.utils.log_utils import AscendCLogLevel
from asc_op_compile_base.common.platform.platform_info import get_soc_spec
from .ascendc_common_utility import CommonUtility


SCHEMA_VERSION = "4.0"
BIN_ID_DOMAIN = "ascendc-bin-id-v1\0"
STATIC_TILING_TOKEN = "@@STATIC_VALUE_tiling_data@@"
LOG_STAGE_PATTERN = re.compile(r"^// Stage: CompileStage\.([A-Z_]+)$")
RECIPE_STAGES = {"COMPILE", "FATBIN", "LINKRELOCATE"}
TILING_KEY_PATTERN = re.compile(r"^-DTILING_KEY_VAR=([^U]+)UL$")
SOURCE_DIR_OPTION = "-I${source_dir}"
COMMON_INCLUDE_PREFIX = "-I${env:ASCEND_HOME_PATH}"
COMMON_INCLUDE_MARKER = "${options:common_include}"


class OpCompileManifestRecorder:
    """Collect only the inputs required to build the new compile-resource manifest."""

    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(OpCompileManifestRecorder, cls).__new__(cls)
            cls._instance.reset()
        return cls._instance

    def reset(self):
        self.kernel_meta_dir = None
        self.resource_dir = None
        self.manifest_path = None
        self.op_type = ""
        self.op_name = ""
        self.source_file = ""
        self.soc_version = ""
        self.tiling_header = ""
        self.resource_sources = {}
        self.merged_tiling_file = ""
        self.bin_id = ""
        self.command_ascend_roots = []

    def start(self, op_info, origin_func_name, cce_file, kernel_meta_dir):
        self.reset()
        kernel_prefix = "{}_".format(op_info.op_type)
        if isinstance(op_info.kernel_name, str) and op_info.kernel_name.startswith(kernel_prefix):
            op_hash = op_info.kernel_name[len(kernel_prefix):]
        else:
            seed = op_info.kernel_name or origin_func_name or op_info.op_type or cce_file
            op_hash = hashlib.sha256(str(seed).encode("utf-8")).hexdigest()[:16]
        self.op_type = op_info.op_type
        self.op_name = "{}_{}".format(self._safe_name(op_info.op_type), op_hash)
        self.source_file = os.path.normpath(cce_file)
        self.soc_version = get_soc_spec("FULL_SOC_VERSION")
        self.kernel_meta_dir = kernel_meta_dir
        self.resource_dir = os.path.join(kernel_meta_dir, "resources")
        self.manifest_path = os.path.join(kernel_meta_dir, "{}_manifest.json".format(self.op_name))
        self.merged_tiling_file = "resources/{}_tiling_data.h".format(self.op_name)

    def set_tiling_header(self, tiling_header):
        self.tiling_header = os.path.normpath(tiling_header)

    def prepare_binary_identity(self, compile_info, compile_option_tuple):
        if self.bin_id:
            return self.bin_id
        try:
            with open(compile_info.src_file, "rb") as source:
                source_hash = hashlib.sha256(source.read()).hexdigest()
            options = [str(value) for value in compile_option_tuple.compile_options]
            options.extend("-mllvm={}".format(value) for value in compile_option_tuple.mllvm_options)
            identity = {
                "source_hash": source_hash,
                "compile_options": self._normalize_identity_options(options),
            }
            canonical = json.dumps(identity, sort_keys=True, separators=(",", ":"), ensure_ascii=False)
            self.bin_id = hashlib.sha256((BIN_ID_DOMAIN + canonical).encode("utf-8")).hexdigest()
        except Exception as error:
            CommonUtility.print_compile_log(
                compile_info.kernel_name,
                "generate compile resource binId failed: {}".format(error),
                AscendCLogLevel.LOG_WARNING)
        return self.bin_id

    def get_bin_id(self):
        return self.bin_id

    def record_wrapper_body(self, compile_info, role="normal"):
        mode = "sk" if role in ("sk", "super_kernel") else "normal"
        if not self.resource_dir:
            return
        try:
            os.makedirs(self.resource_dir, exist_ok=True)
            suffix = "_sk_kernel.cpp" if mode == "sk" else "_kernel.cpp"
            resource_file = "resources/{}{}".format(self.op_name, suffix)
            self._record_resource_file(compile_info.gen_kernel_func_file, resource_file, mode)
        except Exception as error:
            CommonUtility.print_compile_log(
                compile_info.kernel_name,
                "record {} kernel wrapper failed: {}".format(mode, error),
                AscendCLogLevel.LOG_WARNING)

    def record_sk_bind(self, source_file, kernel_name):
        """Record the generated SK binding source used by the real SK command chain."""
        try:
            resource_file = "resources/{}_sk_bind.cpp".format(self.op_name)
            self._record_resource_file(source_file, resource_file, "sk")
        except Exception as error:
            CommonUtility.print_compile_log(
                kernel_name, "record SK binding source failed: {}".format(error),
                AscendCLogLevel.LOG_WARNING)

    def write(self, compile_info, tiling_info, tiling_parameter_index):
        try:
            if not self.bin_id or "normal" not in self._resource_modes() or not self.tiling_header:
                raise RuntimeError("binId, normal wrapper, or tiling header is missing")
            self._sync_resource_files()
            entries = self._tiling_entries(compile_info, tiling_info)
            if not entries:
                raise RuntimeError("no tiling-key structure information is available")
            self._write_merged_tiling_header(entries)
            commands = self._commands_by_tiling_key(compile_info.compile_log_path, entries)
            files = self._manifest_files()
            kernels = []
            for tiling_key, tiling_struct, byte_size in entries:
                recipe_commands = self._recipe_commands(tiling_key, commands.get(tiling_key, []))
                if not recipe_commands:
                    raise RuntimeError("no real compile command found for tiling key {}".format(tiling_key))
                kernel_name = "{}_{}".format(self.op_name, tiling_key)
                kernels.append({
                    "kernel_name": kernel_name,
                    "constants_info": [{
                        "name": tiling_struct,
                        "parameter_index": int(tiling_parameter_index),
                        "byte_size": byte_size,
                        "file": os.path.basename(self.merged_tiling_file),
                        "template": STATIC_TILING_TOKEN,
                    }],
                    "commands": recipe_commands,
                })
            common_includes = self._extract_common_includes(kernels)
            manifest = {
                "schema_version": SCHEMA_VERSION,
                "binId": self.bin_id,
                "soc_version": self.soc_version,
                "source_file": os.path.basename(self.source_file),
                "options": {"common_include": common_includes},
                "files": files,
                "kernels": kernels,
            }
            with open(self.manifest_path, "w") as output:
                json.dump(manifest, output, indent=2)
                output.write("\n")
            os.chmod(self.manifest_path, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH)
        except Exception as error:
            raise RuntimeError(
                "write compile resource manifest failed for {}: {}".format(
                    compile_info.kernel_name, error)) from error

    def _tiling_entries(self, compile_info, tiling_info):
        struct_map = {
            str(key): value
            for key, value in (getattr(tiling_info, "tiling_key_struct_map", {}) or {}).items()
        }
        struct_map.update({
            str(key): value
            for key, value in (getattr(compile_info, "tiling_key_struct_map", {}) or {}).items()
        })
        default_struct = getattr(tiling_info, "default_tiling_struct", "") or \
            getattr(compile_info, "default_tiling_struct", "")
        data_sizes = {
            str(key): value
            for key, value in (getattr(tiling_info, "tiling_key_data_size", {}) or {}).items()
        }
        struct_sizes = getattr(compile_info, "tiling_struct_size_map", {}) or {}
        default_size = getattr(tiling_info, "default_tiling_size", 0) or \
            getattr(tiling_info, "tiling_data_size", 0)
        entries = []
        for value in compile_info.tiling_key_list:
            tiling_key = str(value)
            tiling_struct = struct_map.get(tiling_key, default_struct)
            byte_size = struct_sizes.get(tiling_struct, data_sizes.get(tiling_key, default_size))
            if not tiling_struct or not isinstance(byte_size, int) or byte_size <= 0:
                raise RuntimeError(
                    "invalid tiling metadata for key {}: struct={!r}, byte_size={!r}".format(
                        tiling_key, tiling_struct, byte_size))
            entries.append((tiling_key, str(tiling_struct), int(byte_size)))
        return entries

    def _write_merged_tiling_header(self, entries):
        with open(self.tiling_header, "r") as source:
            content = source.read()
        macro_start = content.find("#ifdef ASCENDC_TIME_STAMP_ON")
        if macro_start < 0:
            raise RuntimeError("dynamic tiling header has no GET_TILING_DATA macro section")
        init_tiling_start = content.find("\n#if defined(ASCENDC_CPU_DEBUG)\n", 0, macro_start)
        resource_end = init_tiling_start if init_tiling_start >= 0 else macro_start
        prefix = content[:resource_end].rstrip() + "\n\n"
        lines = [prefix]
        for index, (tiling_key, tiling_struct, byte_size) in enumerate(entries):
            lines.append("#{} TILING_KEY_VAR == {}UL\n".format("if" if index == 0 else "elif", tiling_key))
            lines.append("#define TILING_DATA_LENGTH {}\n".format(byte_size))
            lines.append("#define TILING_DATA_TYPE {}\n".format(tiling_struct))
        lines.extend([
            "#else\n",
            "#error \"unsupported TILING_KEY_VAR in static tiling resource\"\n",
            "#endif\n\n",
            "#define TILING_BYTES_STREAM {}\n\n".format(STATIC_TILING_TOKEN),
            "#define GET_TILING_DATA(tiling_data, tiling_arg)                                      \\\n",
            "    alignas(TILING_DATA_TYPE) const uint8_t __ascendc_arr_##tiling_data[TILING_DATA_LENGTH] = TILING_BYTES_STREAM; \\\n",
            "    const TILING_DATA_TYPE &tiling_data = *reinterpret_cast<const TILING_DATA_TYPE *>(__ascendc_arr_##tiling_data);\n\n",
            "#define GET_TILING_DATA_WITH_STRUCT(tiling_struct, tiling_data, tiling_arg)           \\\n",
            "    alignas(tiling_struct) const uint8_t __ascendc_arr_##tiling_data[TILING_DATA_LENGTH] = TILING_BYTES_STREAM; \\\n",
            "    const tiling_struct &tiling_data = *reinterpret_cast<const tiling_struct *>(__ascendc_arr_##tiling_data);\n\n",
            "#define GET_TILING_DATA_MEMBER(tiling_type, member, var, tiling)                      \\\n",
            "    alignas(tiling_type) const uint8_t __ascendc_arr_##var[TILING_DATA_LENGTH] = TILING_BYTES_STREAM; \\\n",
            "    const tiling_type &__ascendc_point##var = *reinterpret_cast<const tiling_type *>(__ascendc_arr_##var); \\\n",
            "    const auto &var = __ascendc_point##var.member;\n\n",
            "#endif // __TILINGDATA_HEADER__\n",
        ])
        destination = os.path.join(self.kernel_meta_dir, self.merged_tiling_file)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        with open(destination, "w") as output:
            output.writelines(lines)
        os.chmod(destination, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH)

    def _commands_by_tiling_key(self, log_path, entries):
        entry_keys = {item[0] for item in entries}
        with open(log_path, "r") as source:
            lines = source.read().splitlines()
        raw_commands = []
        for index, line in enumerate(lines[:-1]):
            match = LOG_STAGE_PATTERN.match(line.strip())
            if match and match.group(1) in RECIPE_STAGES and lines[index + 1].strip():
                raw_commands.append((match.group(1).lower(), shlex.split(lines[index + 1])))

        roots = []
        env_root = os.environ.get("ASCEND_HOME_PATH", "")
        if env_root:
            roots.append(os.path.normpath(env_root))
        for _, tokens in raw_commands:
            if tokens and tokens[0].endswith("/bin/bisheng"):
                roots.append(os.path.dirname(os.path.dirname(os.path.normpath(tokens[0]))))
        self.command_ascend_roots = sorted(set(roots), key=len, reverse=True)

        produced = {}
        grouped = {key: [] for key in entry_keys}
        for stage, tokens in raw_commands:
            direct_key = self._command_tiling_key(tokens)
            direct_mode = self._command_mode(tokens)
            keys = {direct_key} if direct_key in entry_keys else set()
            output_index = self._output_index(tokens)
            dependencies = {}
            modes = set()
            for index, token in enumerate(tokens):
                if index == output_index:
                    continue
                dependency = produced.get(os.path.normpath(token))
                if dependency:
                    dependencies[index] = dependency
                    keys.update(dependency)
                    modes.update(item[1] for item in dependency.values())
            if stage == "compile" and direct_mode == "sk" and direct_key in ("0", ""):
                keys.update(entry_keys)
            if not keys or (stage == "compile" and not direct_mode):
                continue
            mode = direct_mode or ("sk" if "sk" in modes else "normal")
            output = self._output_path(tokens)
            projected_outputs = {}
            for key in sorted(keys):
                projected = list(tokens)
                for index, dependency in dependencies.items():
                    if key not in dependency:
                        projected[index] = None
                    else:
                        projected[index] = dependency[key][0]
                projected = [token for token in projected if token is not None]
                projected_output = output
                if output:
                    input_version = None
                    normalized_output = os.path.normpath(output)
                    for index, dependency in dependencies.items():
                        if os.path.normpath(tokens[index]) == normalized_output and key in dependency:
                            input_version = dependency[key][0]
                            break
                    if input_version:
                        projected_output = input_version
                    elif len(keys) > 1:
                        projected_output = self._keyed_output(output, key, mode)
                    projected[self._output_index(projected)] = projected_output
                    projected_outputs[key] = (os.path.normpath(projected_output), mode)
                grouped[key].append((stage, projected, projected_output, mode))
            if output:
                produced[os.path.normpath(output)] = projected_outputs
        return grouped

    def _recipe_commands(self, tiling_key, commands):
        if not commands:
            return []
        final_by_mode = {}
        for _, _, output, mode in commands:
            if output:
                final_by_mode[mode] = os.path.normpath(output)
        artifact_files = {
            mode: "artifacts/{}_{}{}.o".format(
                self.op_name, tiling_key, "_sk" if mode == "sk" else "")
            for mode in final_by_mode
        }
        output_markers = {}
        for _, _, output, mode in commands:
            if output:
                normalized = os.path.normpath(output)
                output_markers[normalized] = "${output:intermediates/%s/%s}" % (
                    mode, os.path.basename(output))
        for mode, output in final_by_mode.items():
            output_markers[output] = "${output:%s}" % artifact_files[mode]

        result = []
        for stage, tokens, output, mode in commands:
            # LINKRELOCATE finalizes the aggregate object containing every tiling key.
            # A per-key replay recipe must end at its relocatable FATBIN output.
            if not tokens or not output or stage == "linkrelocate":
                continue
            output_index = self._output_index(tokens)
            options = [
                self._normalize_command_token(token, output_markers)
                for index, token in enumerate(tokens[1:], start=1)
                if index not in (output_index - 1, output_index) and
                not self._is_nonportable_common_include(token)
            ]
            command_type = self._command_type(tokens[0], stage)
            if command_type == "bisheng" and SOURCE_DIR_OPTION not in options:
                options.insert(0, SOURCE_DIR_OPTION)
            result.append({
                "type": command_type,
                "mode": mode,
                "options": options,
                "output": self._normalize_command_token(output, output_markers),
            })
        return result

    def _normalize_command_token(self, token, output_markers):
        normalized = os.path.normpath(token) if token.startswith("/") else token
        if normalized in output_markers:
            return output_markers[normalized]
        replacements = {
            source: "${resource:%s}" % details[0]
            for source, details in self.resource_sources.items()
        }
        replacements[self.tiling_header] = "${resource:%s}" % self.merged_tiling_file
        for source, replacement in replacements.items():
            if normalized == source:
                return replacement
            if source and source in token:
                token = token.replace(source, replacement)
        for source, replacement in output_markers.items():
            if source in token:
                token = token.replace(source, replacement)

        if not token.startswith("-") and "/" not in token:
            executable = shutil.which(token)
            if executable:
                token = os.path.normpath(executable)
        for ascend_root in self.command_ascend_roots:
            if ascend_root in token:
                token = token.replace(ascend_root, "${env:ASCEND_HOME_PATH}")
        return token

    def _is_nonportable_common_include(self, token):
        """Drop build-workspace ascendc/common paths from replayable recipes."""
        if not token.startswith("-I") or len(token) == 2:
            return False
        include_path = os.path.normpath(token[2:])
        if not os.path.isabs(include_path) or not include_path.endswith(
                os.path.join("ascendc", "common")):
            return False
        for ascend_root in self.command_ascend_roots:
            try:
                if os.path.commonpath([include_path, ascend_root]) == ascend_root:
                    return False
            except ValueError:
                continue
        return True

    @staticmethod
    def _extract_common_includes(kernels):
        common_includes = None
        for kernel in kernels:
            for command in kernel["commands"]:
                includes = [
                    option for option in command["options"]
                    if option.startswith(COMMON_INCLUDE_PREFIX)
                ]
                if not includes:
                    continue
                if common_includes is None:
                    common_includes = includes
                elif includes != common_includes:
                    raise RuntimeError("commands contain inconsistent common include options")

        if common_includes is None:
            return []

        for kernel in kernels:
            for command in kernel["commands"]:
                options = command["options"]
                first = next((index for index, option in enumerate(options)
                              if option.startswith(COMMON_INCLUDE_PREFIX)), None)
                if first is None:
                    continue
                command["options"] = [
                    option for option in options
                    if not option.startswith(COMMON_INCLUDE_PREFIX)
                ]
                command["options"].insert(first, COMMON_INCLUDE_MARKER)
        return common_includes

    @staticmethod
    def _command_tiling_key(tokens):
        for token in tokens:
            match = TILING_KEY_PATTERN.match(token)
            if match:
                return match.group(1)
        return ""

    @staticmethod
    def _output_path(tokens):
        for index, token in enumerate(tokens[:-1]):
            if token == "-o":
                return tokens[index + 1]
        return ""

    @staticmethod
    def _output_index(tokens):
        for index, token in enumerate(tokens[:-1]):
            if token == "-o":
                return index + 1
        return -1

    def _command_mode(self, tokens):
        for token in tokens:
            normalized = os.path.normpath(token) if token.startswith("/") else token
            for source, details in self.resource_sources.items():
                if normalized == source or source in token:
                    return details[1]
        return ""

    @staticmethod
    def _command_type(executable, stage):
        name = os.path.basename(executable)
        if name == "ld.lld":
            return "ld"
        if name in ("llvm-objcopy", "objcopy"):
            return "objcopy"
        return name or stage

    @staticmethod
    def _keyed_output(output, tiling_key, mode):
        root, extension = os.path.splitext(output)
        return "{}_jit_{}_{}{}".format(root, mode, tiling_key, extension)

    def _record_resource_file(self, source_file, resource_file, mode):
        source = os.path.normpath(source_file)
        self.resource_sources[source] = (resource_file, mode)
        self._copy_resource_file(source, resource_file)

    def _sync_resource_files(self):
        """Copy generated sources after compilation has finished mutating them."""
        for source, (resource_file, _) in self.resource_sources.items():
            self._copy_resource_file(source, resource_file)

    def _copy_resource_file(self, source, resource_file):
        destination = os.path.join(self.kernel_meta_dir, resource_file)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        shutil.copyfile(source, destination)
        if resource_file.endswith("_kernel.cpp"):
            self._rewrite_source_include(destination)
        os.chmod(destination, stat.S_IRUSR | stat.S_IWUSR | stat.S_IRGRP | stat.S_IROTH)

    def _rewrite_source_include(self, destination):
        with open(destination, "r") as source:
            content = source.read()
        include_pattern = re.compile(
            r'^([ \t]*#[ \t]*include[ \t]*)[<"]{}[>"]([ \t]*)$'.format(
                re.escape(self.source_file)),
            re.MULTILINE)
        rewritten = include_pattern.sub(
            lambda match: '{}"{}"{}'.format(
                match.group(1), os.path.basename(self.source_file), match.group(2)), content)
        if rewritten != content:
            with open(destination, "w") as output:
                output.write(rewritten)

    def _resource_modes(self):
        return {details[1] for details in self.resource_sources.values()}

    def _manifest_files(self):
        files = [
            {
                "file_name": os.path.basename(resource_file),
                "file_path": "/manifests/{}/{}".format(self.op_name, resource_file),
            }
            for resource_file, _ in self.resource_sources.values()
        ]
        files.append({
            "file_name": os.path.basename(self.merged_tiling_file),
            "file_path": "/manifests/{}/{}".format(self.op_name, self.merged_tiling_file),
        })
        return sorted(files, key=lambda item: item["file_name"])

    @staticmethod
    def _normalize_identity_options(options):
        normalized = []
        for option in options:
            value = re.sub(r"/usr/local/Ascend/cann-[^/\s]+", "${ASCEND_HOME_PATH}", option)
            value = re.sub(r"[^\s]*?/dynamic/", "", value)
            normalized.append(value)
        return normalized

    @staticmethod
    def _safe_name(value):
        return re.sub(r"[^0-9A-Za-z_.-]", "_", str(value))
