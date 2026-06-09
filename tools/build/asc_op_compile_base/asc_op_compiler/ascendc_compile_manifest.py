#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
"""
Ascend C compile manifest recorder.
"""

import hashlib
import json
import os
import re
import shlex
import stat

from asc_op_compile_base.common.utils.log_utils import AscendCLogLevel
from .ascendc_common_utility import CommonUtility, CompileInfo
from .ascendc_constants import CompileOptionTuple
from .get_op_tiling import OpInfo, TilingInfo
from .global_storage import global_var_storage

MANIFEST_VERSION = '1.0'
CANN_PATH_PLACEHOLDER = '{CANN_PATH_PLACEHOLDER}'
SRC_FILE_PLACEHOLDER = '{SRC_FILE_PLACEHOLDER}'
KERNEL_TYPE_PLACEHOLDER = '{kernel_type_placeholder}'
DYNAMIC_COMPILE_PATH_PATTERN = r"(?P<prefix>(?:-I)?)[^\s]*?/dynamic/"
DYNAMIC_COMPILE_PATH_REPLACEMENT = r"\g<prefix>"


class OpCompileManifestRecorder:
    _instance = None
    COMPILE_LOG_STAGE = "// Stage: CompileStage.COMPILE"

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(OpCompileManifestRecorder, cls).__new__(cls)
            cls._instance.reset()
        return cls._instance

    def reset(self):
        self.manifest_path = None
        self.kernel_meta_dir = None
        self.compile_log_path = None
        self.use_compile_options_from_log = False
        self.manifest = {
            "version": MANIFEST_VERSION,
            "basic_info": {},
            "artifacts": {},
            "tiling_struct_info": {
                "default": "",
                "by_tiling_key": {},
                "sources": []
            },
            "compile_options": {
                "source": "compile_option_tuple",
                "include_paths": [],
                "include_files": [],
                "macros": [],
                "undef_macros": [],
                "options": [],
                "mllvm_options": [],
                "raw_options": []
            }
        }

    def start(self, op_info: OpInfo, origin_func_name: str, cce_file: str, kernel_meta_dir: str):
        self.reset()
        safe_op_type = self._safe_filename_part(op_info.op_type)
        op_hash = self._get_kernel_hash(op_info, origin_func_name, cce_file)
        op_name = "{}_{}".format(safe_op_type, op_hash)
        manifest_name = "{}_manifest.json".format(op_name)
        self.manifest_path = os.path.join(kernel_meta_dir, manifest_name)
        self.kernel_meta_dir = kernel_meta_dir
        self.manifest["basic_info"].update({
            "op_type": op_info.op_type,
            "op_name": op_name,
            "kernel_name": op_info.kernel_name,
            "origin_func_name": origin_func_name,
            "soc_version": self._get_soc_version()
        })

    def set_compile_log_path(self, compile_log_path):
        self.compile_log_path = compile_log_path
        self.use_compile_options_from_log = compile_log_path is not None

    def update_basic_info(self, compile_info: CompileInfo = None, tiling_info: TilingInfo = None):
        if compile_info is not None:
            tiling_key_group_map = self._normalize_tiling_key_group_map_for_list(
                compile_info.tiling_key_group_map, compile_info.tiling_key_list)
            basic_info = {
                "kernel_name": compile_info.kernel_name,
                "origin_func_name": compile_info.origin_func_name,
                "op_type": compile_info.op_type,
                "code_channel": compile_info.code_channel,
                "tiling_key_list": self._normalize_tiling_key_list(
                    compile_info.tiling_key_list, tiling_key_group_map),
                "soc_version": self._get_soc_version()
            }
            if tiling_key_group_map:
                basic_info["tiling_key_group_map"] = tiling_key_group_map
            else:
                self.manifest["basic_info"].pop("tiling_key_group_map", None)
            self.manifest["basic_info"].update(basic_info)
            self.set_compile_log_path(compile_info.compile_log_path)
        if tiling_info is not None:
            self.manifest["basic_info"].update({
                "static_shape": getattr(tiling_info, "static_shape_flag", None),
                "tiling_key": getattr(tiling_info, "tiling_key", None),
                "block_num": getattr(tiling_info, "block_num", None)
            })

    def update_compile_options(self, compile_option_tuple: CompileOptionTuple):
        raw_compile_options = self._clean_compile_options_for_manifest(
            self._to_jsonable_list(compile_option_tuple.compile_options))
        raw_mllvm_options = self._to_jsonable_list(compile_option_tuple.mllvm_options)
        compile_option_info = self._categorize_compile_options(raw_compile_options)
        compile_option_info["source"] = "compile_option_tuple"
        compile_option_info["mllvm_options"].extend(raw_mllvm_options)
        self.manifest["compile_options"] = {
            "source": compile_option_info["source"],
            "include_paths": compile_option_info["include_paths"],
            "include_files": compile_option_info["include_files"],
            "macros": compile_option_info["macros"],
            "undef_macros": compile_option_info["undef_macros"],
            "options": compile_option_info["options"],
            "mllvm_options": compile_option_info["mllvm_options"],
            "raw_options": compile_option_info["raw_options"]
        }

    def record_wrapper_body(self, compile_info: CompileInfo):
        if self.kernel_meta_dir is None or compile_info is None:
            return
        wrapper_body_file = self._get_wrapper_body_filename()
        if wrapper_body_file is None:
            return
        wrapper_body_path = os.path.join(self.kernel_meta_dir, wrapper_body_file)
        try:
            with open(compile_info.gen_kernel_func_file, 'r') as fd:
                source_lines = fd.readlines()
            wrapper_body_lines, replaced = self._replace_src_include_with_placeholder(
                source_lines, compile_info.src_file)
            if not replaced:
                CommonUtility.print_compile_log(compile_info.kernel_name, \
                    "record wrapper body did not find source include line", \
                    AscendCLogLevel.LOG_WARNING)
            wrapper_body_lines, inserted = self._insert_kernel_type_placeholder(wrapper_body_lines)
            if not inserted:
                CommonUtility.print_compile_log(compile_info.kernel_name, \
                    "record wrapper body did not find kernel function declaration", \
                    AscendCLogLevel.LOG_WARNING)
            with open(wrapper_body_path, 'w') as fd:
                fd.writelines(wrapper_body_lines)
            os.chmod(wrapper_body_path, stat.S_IRUSR + stat.S_IWUSR + stat.S_IRGRP + stat.S_IROTH)
            self.manifest.setdefault("artifacts", {})["wrapper_body"] = wrapper_body_file
        except Exception as err:
            CommonUtility.print_compile_log(compile_info.kernel_name, \
                "record wrapper body failed, reason is: {}".format(err), \
                AscendCLogLevel.LOG_WARNING)

    def record_tiling_struct(self, tiling_data_file_path):
        if self.kernel_meta_dir is None:
            return
        tiling_struct_file = self._get_tiling_struct_filename()
        if tiling_struct_file is None:
            return
        tiling_struct_path = os.path.join(self.kernel_meta_dir, tiling_struct_file)
        kernel_name = self.manifest.get("basic_info", {}).get("kernel_name", "")
        try:
            with open(tiling_data_file_path, 'r') as fd:
                source_lines = fd.readlines()
            tiling_struct_lines = self._extract_tiling_struct_definitions(source_lines)
            if not tiling_struct_lines:
                CommonUtility.print_compile_log(kernel_name, \
                    "record tiling struct did not find struct definition", \
                    AscendCLogLevel.LOG_WARNING)
                return
            with open(tiling_struct_path, 'w') as fd:
                fd.writelines(tiling_struct_lines)
            os.chmod(tiling_struct_path, stat.S_IRUSR + stat.S_IWUSR + stat.S_IRGRP + stat.S_IROTH)
            self.manifest.setdefault("artifacts", {})["tiling_struct"] = tiling_struct_file
        except Exception as err:
            CommonUtility.print_compile_log(kernel_name, \
                "record tiling struct failed, reason is: {}".format(err), \
                AscendCLogLevel.LOG_WARNING)

    def record_tiling_struct_info(self, tiling_key_struct_map=None, default_tiling_struct=None, source=None,
                                  tiling_key_list=None, tiling_key_group_map=None):
        tiling_struct_info = self.manifest.setdefault("tiling_struct_info", {
            "default": "",
            "by_tiling_key": {},
            "sources": []
        })
        if default_tiling_struct is not None:
            tiling_struct_info["default"] = str(default_tiling_struct)
        by_tiling_key = tiling_struct_info.setdefault("by_tiling_key", {})
        normalized_tiling_key_group_map = self._normalize_tiling_key_group_map_for_list(
            tiling_key_group_map, tiling_key_list)
        normalized_tiling_key_struct_map = self._normalize_tiling_key_struct_map(tiling_key_struct_map)
        normalized_tiling_key_struct_map = self._expand_tiling_key_struct_map_with_group(
            normalized_tiling_key_struct_map, normalized_tiling_key_group_map)
        normalized_tiling_key_list = self._normalize_tiling_key_list(
            tiling_key_list, normalized_tiling_key_group_map)
        if normalized_tiling_key_list:
            normalized_tiling_keys = set(normalized_tiling_key_list)
            for tiling_key in list(by_tiling_key.keys()):
                if tiling_key not in normalized_tiling_keys:
                    by_tiling_key.pop(tiling_key)
            normalized_tiling_key_struct_map = {
                tiling_key: tiling_struct
                for tiling_key, tiling_struct in normalized_tiling_key_struct_map.items()
                if tiling_key in normalized_tiling_keys
            }
        by_tiling_key.update(normalized_tiling_key_struct_map)
        if tiling_key_list and tiling_struct_info.get("default", ""):
            default_tiling_struct = tiling_struct_info["default"]
            for tiling_key in normalized_tiling_key_list:
                by_tiling_key.setdefault(tiling_key, default_tiling_struct)
        if source:
            sources = tiling_struct_info.setdefault("sources", [])
            if source not in sources:
                sources.append(source)

    def record_kernel_type_info(self, tiling_key_kernel_type=None, default_kernel_type=None,
                                tiling_key_list=None, tiling_key_group_map=None):
        kernel_type_info = self.manifest.setdefault("kernel_type_info", {
            "default": "",
            "by_tiling_key": {}
        })
        if default_kernel_type is not None:
            kernel_type_info["default"] = self._normalize_kernel_type(default_kernel_type)

        by_tiling_key = kernel_type_info.setdefault("by_tiling_key", {})
        normalized_kernel_type_map = self._normalize_tiling_key_kernel_type_map(tiling_key_kernel_type)
        normalized_tiling_key_list = self._normalize_tiling_key_list(
            tiling_key_list,
            self._normalize_tiling_key_group_map_for_list(tiling_key_group_map, tiling_key_list))
        if normalized_tiling_key_list:
            normalized_tiling_keys = set(normalized_tiling_key_list)
            for tiling_key in list(by_tiling_key.keys()):
                if tiling_key not in normalized_tiling_keys:
                    by_tiling_key.pop(tiling_key)
            normalized_kernel_type_map = {
                tiling_key: kernel_type
                for tiling_key, kernel_type in normalized_kernel_type_map.items()
                if tiling_key in normalized_tiling_keys
            }
        by_tiling_key.update(normalized_kernel_type_map)
        if normalized_tiling_key_list and kernel_type_info.get("default", ""):
            default_kernel_type = kernel_type_info["default"]
            for tiling_key in normalized_tiling_key_list:
                by_tiling_key.setdefault(tiling_key, default_kernel_type)

    def write(self, compile_info: CompileInfo = None, tiling_info: TilingInfo = None,
              compile_option_tuple: CompileOptionTuple = None):
        if self.manifest_path is None:
            return
        try:
            self.update_basic_info(compile_info=compile_info, tiling_info=tiling_info)
            if not self.update_compile_options_from_log() and compile_option_tuple is not None:
                self.update_compile_options(compile_option_tuple)
            self.update_kernel_type_info_from_compile_options(compile_info)
            with open(self.manifest_path, 'w') as fd:
                json.dump(self.manifest, fd, indent=2, sort_keys=True)
                fd.write('\n')
            os.chmod(self.manifest_path, stat.S_IRUSR + stat.S_IWUSR + stat.S_IRGRP + stat.S_IROTH)
        except Exception as err:
            CommonUtility.print_compile_log("", \
                "write ascendc compile manifest failed, reason is: {}".format(err), \
                AscendCLogLevel.LOG_WARNING)

    @staticmethod
    def _normalize_tiling_key_struct_map(tiling_key_struct_map):
        if not tiling_key_struct_map:
            return {}
        normalized_map = {}
        for tiling_key, tiling_struct in tiling_key_struct_map.items():
            if isinstance(tiling_struct, (list, tuple)):
                tiling_struct = tiling_struct[0] if tiling_struct else ""
            elif isinstance(tiling_struct, dict):
                tiling_struct = tiling_struct.get("tiling_struct", tiling_struct.get("struct", ""))
            if tiling_struct is None or str(tiling_struct) == "":
                continue
            normalized_map[OpCompileManifestRecorder._normalize_tiling_key(tiling_key)] = str(tiling_struct)
        return normalized_map

    @staticmethod
    def _normalize_tiling_key(tiling_key):
        return str(tiling_key).rstrip("ULul")

    @staticmethod
    def _append_unique(items, item):
        if item not in items:
            items.append(item)

    @staticmethod
    def _normalize_tiling_key_list(tiling_key_list, tiling_key_group_map=None):
        if not tiling_key_list:
            return []
        normalized_list = []
        for tiling_key in tiling_key_list:
            normalized_tiling_key = OpCompileManifestRecorder._normalize_tiling_key(tiling_key)
            OpCompileManifestRecorder._append_unique(normalized_list, normalized_tiling_key)
            if tiling_key_group_map and normalized_tiling_key in tiling_key_group_map:
                for tiling_key_slave in tiling_key_group_map[normalized_tiling_key]:
                    OpCompileManifestRecorder._append_unique(normalized_list, tiling_key_slave)
        return normalized_list

    @staticmethod
    def _normalize_tiling_key_group_map(tiling_key_group_map):
        if not tiling_key_group_map:
            return {}
        normalized_map = {}
        for tiling_key_master, tiling_key_slaves in tiling_key_group_map.items():
            normalized_master = OpCompileManifestRecorder._normalize_tiling_key(tiling_key_master)
            if isinstance(tiling_key_slaves, (list, tuple, set)):
                slave_iter = tiling_key_slaves
            else:
                slave_iter = [tiling_key_slaves]
            normalized_slaves = []
            for tiling_key_slave in slave_iter:
                normalized_slave = OpCompileManifestRecorder._normalize_tiling_key(tiling_key_slave)
                OpCompileManifestRecorder._append_unique(normalized_slaves, normalized_slave)
            normalized_map[normalized_master] = normalized_slaves
        return normalized_map

    @staticmethod
    def _normalize_tiling_key_group_map_for_list(tiling_key_group_map, tiling_key_list):
        normalized_group_map = OpCompileManifestRecorder._normalize_tiling_key_group_map(tiling_key_group_map)
        if not normalized_group_map:
            return {}
        if not tiling_key_list:
            return normalized_group_map
        normalized_tiling_keys = set(
            OpCompileManifestRecorder._normalize_tiling_key(tiling_key) for tiling_key in tiling_key_list)
        return {
            tiling_key_master: tiling_key_slaves
            for tiling_key_master, tiling_key_slaves in normalized_group_map.items()
            if tiling_key_master in normalized_tiling_keys
        }

    @staticmethod
    def _expand_tiling_key_struct_map_with_group(tiling_key_struct_map, tiling_key_group_map):
        if not tiling_key_struct_map or not tiling_key_group_map:
            return tiling_key_struct_map
        expanded_map = dict(tiling_key_struct_map)
        for tiling_key_master, tiling_key_slaves in tiling_key_group_map.items():
            if tiling_key_master not in expanded_map:
                continue
            for tiling_key_slave in tiling_key_slaves:
                expanded_map.setdefault(tiling_key_slave, expanded_map[tiling_key_master])
        return expanded_map

    @staticmethod
    def _normalize_tiling_key_kernel_type_map(tiling_key_kernel_type):
        if not tiling_key_kernel_type:
            return {}
        normalized_map = {}
        for tiling_key, kernel_type in tiling_key_kernel_type.items():
            if kernel_type is None:
                continue
            normalized_map[OpCompileManifestRecorder._normalize_tiling_key(tiling_key)] = \
                OpCompileManifestRecorder._normalize_kernel_type(kernel_type)
        return normalized_map

    @staticmethod
    def _normalize_kernel_type(kernel_type):
        kernel_type_name = getattr(kernel_type, "name", None)
        if kernel_type_name:
            return str(kernel_type_name)
        return str(kernel_type)

    @staticmethod
    def _short_hash(value):
        return hashlib.sha256(str(value).encode("utf-8")).hexdigest()[:16]

    @staticmethod
    def _get_kernel_hash(op_info: OpInfo, origin_func_name: str, cce_file: str):
        kernel_name = op_info.kernel_name
        kernel_prefix = "{}_".format(op_info.op_type)
        if isinstance(kernel_name, str) and kernel_name.startswith(kernel_prefix):
            return kernel_name[len(kernel_prefix):]
        return OpCompileManifestRecorder._short_hash(kernel_name or origin_func_name or op_info.op_type or cce_file)

    @staticmethod
    def _safe_filename_part(value):
        return re.sub(r"[^0-9A-Za-z_.-]", "_", str(value))

    def _get_wrapper_body_filename(self):
        op_name = self.manifest.get("basic_info", {}).get("op_name")
        if not op_name:
            return None
        return "{}_wrapper_body.cpp".format(op_name)

    def _get_tiling_struct_filename(self):
        op_name = self.manifest.get("basic_info", {}).get("op_name")
        if not op_name:
            return None
        return "{}_tiling_strcut.h".format(op_name)

    @staticmethod
    def _replace_src_include_with_placeholder(source_lines, src_file):
        src_include_pattern = re.compile(
            r"^(?P<indent>\s*)#\s*include\s+\"{}\"\s*$".format(re.escape(str(src_file))))
        wrapper_body_lines = []
        replaced = False
        for line in source_lines:
            stripped_line = line.rstrip("\r\n")
            line_ending = line[len(stripped_line):]
            match = src_include_pattern.match(stripped_line)
            if not replaced and match:
                wrapper_body_lines.append(
                    "{}#include \"{}\"{}".format(match.group("indent"), SRC_FILE_PLACEHOLDER, line_ending))
                replaced = True
            else:
                wrapper_body_lines.append(line)
        return wrapper_body_lines, replaced

    @staticmethod
    def _insert_kernel_type_placeholder(source_lines):
        kernel_func_pattern = re.compile(
            r"(?P<prefix>\b__global__(?:(?!\bvoid\b).)*?\[aicore\]\s+)"
            r"(?P<suffix>void\s+[A-Za-z_]\w*\s*\()")
        wrapper_body_lines = []
        inserted = False
        for line in source_lines:
            if KERNEL_TYPE_PLACEHOLDER in line:
                wrapper_body_lines.append(line)
                inserted = True
                continue

            def _replace(match):
                nonlocal inserted
                inserted = True
                return "{}{} {}".format(
                    match.group("prefix"), KERNEL_TYPE_PLACEHOLDER, match.group("suffix"))

            wrapper_body_lines.append(kernel_func_pattern.sub(_replace, line))
        return wrapper_body_lines, inserted

    @staticmethod
    def _extract_tiling_struct_definitions(source_lines):
        first_definition_idx = None
        top_level_type_pattern = re.compile(r"^(?:class|struct)\s+[A-Za-z_]\w*(?:\s|:|\{|$)")
        for idx, line in enumerate(source_lines):
            if top_level_type_pattern.match(line):
                first_definition_idx = idx
                break
        if first_definition_idx is None:
            return []

        tiling_struct_lines = list(source_lines[:first_definition_idx])
        current_definition = []
        in_definition = False
        brace_depth = 0
        saw_open_brace = False
        for line in source_lines[first_definition_idx:]:
            if not in_definition:
                if not top_level_type_pattern.match(line):
                    continue
                current_definition = [line]
                in_definition = True
                saw_open_brace = "{" in line
                brace_depth = line.count("{") - line.count("}")
            else:
                current_definition.append(line)
                saw_open_brace = saw_open_brace or "{" in line
                brace_depth += line.count("{") - line.count("}")

            if in_definition and saw_open_brace and brace_depth <= 0 and ";" in line:
                tiling_struct_lines.extend(current_definition)
                if not tiling_struct_lines[-1].endswith("\n"):
                    tiling_struct_lines[-1] += "\n"
                tiling_struct_lines.append("\n")
                current_definition = []
                in_definition = False
                brace_depth = 0
                saw_open_brace = False

        while tiling_struct_lines and tiling_struct_lines[-1].strip() == "":
            tiling_struct_lines.pop()
        if tiling_struct_lines:
            final_endif = OpCompileManifestRecorder._get_final_header_guard_endif(source_lines)
            if final_endif is not None:
                tiling_struct_lines.append("\n")
                tiling_struct_lines.append(final_endif)
            tiling_struct_lines.append("\n")
        return tiling_struct_lines

    @staticmethod
    def _get_final_header_guard_endif(source_lines):
        if not source_lines:
            return None
        has_header_guard = any(line.lstrip().startswith("#ifndef") for line in source_lines[:5]) and \
            any(line.lstrip().startswith("#define") for line in source_lines[:5])
        if not has_header_guard:
            return None
        for line in reversed(source_lines):
            if re.match(r"^\s*#endif(?:\s*//.*)?\s*$", line):
                return line
        return None

    @staticmethod
    def _get_soc_version():
        soc_version = global_var_storage.get_variable("ascendc_short_soc_version")
        if soc_version is None:
            soc_version = os.environ.get("SOC_VERSION")
        return soc_version

    @staticmethod
    def _to_jsonable_list(values):
        jsonable_values = []
        for value in list(values):
            if value is None or isinstance(value, (str, int, float, bool)):
                jsonable_values.append(value)
            else:
                jsonable_values.append(str(value))
        return jsonable_values

    def update_compile_options_from_log(self):
        compile_cmds = self._get_compile_stage_cmds()
        if not compile_cmds:
            return False
        compile_records = [self._parse_compile_cmd(cmd) for cmd in compile_cmds]
        compile_records = [record for record in compile_records if record["tiling_key"] is not None]
        if not compile_records:
            return False
        common_raw_options = self._common_options([record["raw_options"] for record in compile_records])
        by_tiling_key = {}
        core_types = []
        for record in compile_records:
            tiling_key = record["tiling_key"]
            core_type = record["core_type"]
            core_types.append(core_type)
            tiling_info = by_tiling_key.setdefault(tiling_key, {
                "cores": {},
                "is_mix_compile": False
            })
            raw_options = self._subtract_options(record["raw_options"], common_raw_options)
            compile_option_info = self._categorize_compile_options(raw_options)
            compile_option_info["arch"] = record["arch"]
            tiling_info["cores"][core_type] = compile_option_info
            tiling_info["is_mix_compile"] = len(tiling_info["cores"]) > 1

        core_types = self._unique_keep_order(core_types)
        self.manifest["compile_options"] = {
            "source": "compile_log",
            "stage": "CompileStage.COMPILE",
            "compile_count": len(compile_records),
            "is_mix_compile": len(core_types) > 1,
            "core_types": core_types,
            "common": self._categorize_compile_options(common_raw_options),
            "by_tiling_key": by_tiling_key
        }
        return True

    def update_kernel_type_info_from_compile_options(self, compile_info):
        if compile_info is None or getattr(compile_info, "no_set_kernel_type", False) is not True:
            return
        compile_options = self.manifest.get("compile_options", {})
        by_tiling_key = compile_options.get("by_tiling_key", {})
        if not by_tiling_key:
            return
        tiling_key_kernel_type = self._infer_kernel_type_info_from_compile_options(by_tiling_key)
        if not tiling_key_kernel_type:
            return
        default_kernel_type = self._infer_default_kernel_type(tiling_key_kernel_type)
        self.record_kernel_type_info(
            tiling_key_kernel_type,
            default_kernel_type,
            tiling_key_list=getattr(compile_info, "tiling_key_list", None),
            tiling_key_group_map=getattr(compile_info, "tiling_key_group_map", None))

    @staticmethod
    def _infer_kernel_type_info_from_compile_options(by_tiling_key):
        tiling_key_kernel_type = {}
        for tiling_key, tiling_info in by_tiling_key.items():
            if not isinstance(tiling_info, dict):
                continue
            cores = tiling_info.get("cores", {})
            if not isinstance(cores, dict):
                continue
            kernel_type = OpCompileManifestRecorder._infer_kernel_type_from_cores(cores)
            if kernel_type:
                tiling_key_kernel_type[
                    OpCompileManifestRecorder._normalize_tiling_key(tiling_key)] = kernel_type
        return tiling_key_kernel_type

    @staticmethod
    def _infer_kernel_type_from_cores(cores):
        has_aic = "aic" in cores
        has_aiv = "aiv" in cores
        if has_aic and has_aiv:
            return "KERNEL_TYPE_MIX_AIC_1_2"
        if has_aiv:
            if OpCompileManifestRecorder._core_options_has_auto_gen_suffix(cores.get("aiv", {}), "_mix_aiv"):
                return "KERNEL_TYPE_MIX_AIV_1_0"
            return "KERNEL_TYPE_AIV_ONLY"
        if has_aic:
            if OpCompileManifestRecorder._core_options_has_auto_gen_suffix(cores.get("aic", {}), "_mix_aic"):
                return "KERNEL_TYPE_MIX_AIC_1_0"
            return "KERNEL_TYPE_AIC_ONLY"
        return ""

    @staticmethod
    def _core_options_has_auto_gen_suffix(core_options, suffix):
        if not isinstance(core_options, dict):
            return False
        for option_group in ("macros", "raw_options"):
            for option in core_options.get(option_group, []):
                macro_name, macro_value = OpCompileManifestRecorder._split_macro_option(option)
                if macro_name.startswith("auto_gen_") and macro_value.endswith(suffix):
                    return True
        return False

    @staticmethod
    def _split_macro_option(option):
        if not isinstance(option, str):
            return "", ""
        macro = option[2:] if option.startswith("-D") else option
        if "=" not in macro:
            return macro, ""
        macro_name, macro_value = macro.split("=", 1)
        return macro_name, macro_value.strip("\"'")

    @staticmethod
    def _infer_default_kernel_type(tiling_key_kernel_type):
        kernel_types = OpCompileManifestRecorder._unique_keep_order(list(tiling_key_kernel_type.values()))
        return kernel_types[0] if len(kernel_types) == 1 else ""

    def _get_compile_stage_cmds(self):
        log_path = self.compile_log_path
        if log_path is None or not os.path.exists(log_path):
            log_path = self._find_compile_log_path()
        if log_path is None:
            return []
        compile_cmds = []
        try:
            with open(log_path, 'r') as fd:
                lines = fd.readlines()
        except Exception:
            return []
        need_cmd = False
        for line in lines:
            stripped_line = line.strip()
            if stripped_line == self.COMPILE_LOG_STAGE:
                need_cmd = True
                continue
            if need_cmd and stripped_line:
                compile_cmds.append(stripped_line)
                need_cmd = False
        return compile_cmds

    def _find_compile_log_path(self):
        if self.kernel_meta_dir is None or not os.path.isdir(self.kernel_meta_dir):
            return None
        log_files = []
        try:
            log_files = [
                os.path.join(self.kernel_meta_dir, filename)
                for filename in os.listdir(self.kernel_meta_dir)
                if filename.endswith(".log")
            ]
        except Exception:
            return None
        if not log_files:
            return None
        log_files.sort(key=lambda path: os.path.getmtime(path), reverse=True)
        return log_files[0]

    def _parse_compile_cmd(self, compile_cmd):
        try:
            tokens = shlex.split(compile_cmd)
        except ValueError:
            tokens = compile_cmd.split()
        raw_options = self._clean_compile_options_for_manifest(self._extract_compile_options(tokens))
        return {
            "tiling_key": self._get_tiling_key(raw_options),
            "core_type": self._get_core_type(tokens),
            "arch": self._get_arch(tokens),
            "raw_options": raw_options
        }

    @staticmethod
    def _extract_compile_options(tokens):
        raw_options = []
        idx = 0
        while idx < len(tokens):
            token = tokens[idx]
            if idx == 0 and (token.endswith("bisheng") or token.endswith("ccec")):
                idx += 1
                continue
            if token == "-o" and idx + 1 < len(tokens):
                idx += 2
                continue
            if OpCompileManifestRecorder._is_compile_input(token):
                idx += 1
                continue
            raw_options.append(token)
            idx += 1
        return raw_options

    @staticmethod
    def _is_compile_input(token):
        return token.endswith((".cpp", ".cc", ".cxx", ".cce", ".c"))

    @staticmethod
    def _get_tiling_key(options):
        tiling_key_prefix = "-DTILING_KEY_VAR="
        for option in options:
            if isinstance(option, str) and option.startswith(tiling_key_prefix):
                tiling_key = option[len(tiling_key_prefix):]
                return tiling_key.rstrip("ULul")
        return None

    @staticmethod
    def _is_tiling_key_option(option):
        return isinstance(option, str) and option.startswith("-DTILING_KEY_VAR=")

    @staticmethod
    def _get_arch(tokens):
        arch_prefix = "--cce-aicore-arch="
        for token in tokens:
            if token.startswith(arch_prefix):
                return token[len(arch_prefix):]
        return ""

    @staticmethod
    def _get_core_type(tokens):
        joined_tokens = " ".join(tokens)
        if "_mix_aic" in joined_tokens:
            return "aic"
        if "_mix_aiv" in joined_tokens:
            return "aiv"
        arch = OpCompileManifestRecorder._get_arch(tokens)
        if arch.endswith("-cube"):
            return "aic"
        if arch.endswith("-vec"):
            return "aiv"
        return "unknown"

    @staticmethod
    def _common_options(option_lists):
        if not option_lists:
            return []
        common_options = []
        remaining_option_lists = [list(options) for options in option_lists[1:]]
        for option in option_lists[0]:
            if OpCompileManifestRecorder._is_tiling_key_option(option):
                continue
            in_all_lists = True
            for remaining_options in remaining_option_lists:
                if option in remaining_options:
                    remaining_options.remove(option)
                else:
                    in_all_lists = False
                    break
            if in_all_lists:
                common_options.append(option)
        return common_options

    @staticmethod
    def _subtract_options(options, common_options):
        remain_options = list(options)
        for option in common_options:
            if option in remain_options:
                remain_options.remove(option)
        return remain_options

    @staticmethod
    def _unique_keep_order(values):
        unique_values = []
        for value in values:
            if value not in unique_values:
                unique_values.append(value)
        return unique_values

    @staticmethod
    def _clean_compile_options_for_manifest(compile_options):
        cleaned_options = []
        idx = 0
        while idx < len(compile_options):
            option = compile_options[idx]
            if option == "-include" and idx + 1 < len(compile_options):
                include_file = compile_options[idx + 1]
                if OpCompileManifestRecorder._is_tiling_data_header(include_file):
                    idx += 2
                    continue
                cleaned_options.append(option)
                cleaned_options.append(OpCompileManifestRecorder._normalize_manifest_path(include_file))
                idx += 2
                continue
            if isinstance(option, str) and option.startswith("-include") and len(option) > len("-include"):
                include_file = option[len("-include"):]
                if OpCompileManifestRecorder._is_tiling_data_header(include_file):
                    idx += 1
                    continue
            cleaned_options.append(OpCompileManifestRecorder._normalize_manifest_path(option))
            idx += 1
        return cleaned_options

    @staticmethod
    def _is_tiling_data_header(path):
        if not isinstance(path, str):
            return False
        basename = os.path.basename(path)
        return basename == "tiling_data.h" or basename.endswith("_tiling_data.h")

    @staticmethod
    def _normalize_manifest_path(value):
        if not isinstance(value, str):
            return value
        normalized_value = value
        normalized_value = re.sub(
            DYNAMIC_COMPILE_PATH_PATTERN,
            DYNAMIC_COMPILE_PATH_REPLACEMENT,
            normalized_value)
        for path_prefix in OpCompileManifestRecorder._get_cann_path_prefixes():
            normalized_value = normalized_value.replace(path_prefix, CANN_PATH_PLACEHOLDER)
        normalized_value = re.sub(
            r"/usr/local/Ascend/cann-[^/\s]+/(?:aarch64-linux|x86_64-linux)",
            CANN_PATH_PLACEHOLDER,
            normalized_value)
        return normalized_value

    @staticmethod
    def _get_cann_path_prefixes():
        path_prefixes = [
            "/usr/local/Ascend/cann-9.1.0/aarch64-linux",
            "/usr/local/Ascend/cann-9.1.0/x86_64-linux"
        ]
        for env_name in ("ASCEND_HOME_PATH", "ASCEND_INSTALL_PATH"):
            env_path = os.environ.get(env_name)
            if not env_path:
                continue
            env_path = os.path.normpath(env_path)
            if env_path.endswith(("aarch64-linux", "x86_64-linux")):
                path_prefixes.append(env_path)
            else:
                path_prefixes.append(os.path.join(env_path, "aarch64-linux"))
                path_prefixes.append(os.path.join(env_path, "x86_64-linux"))
        return sorted(OpCompileManifestRecorder._unique_keep_order(path_prefixes), key=len, reverse=True)

    @staticmethod
    def _categorize_compile_options(compile_options):
        compile_options = OpCompileManifestRecorder._clean_compile_options_for_manifest(compile_options)
        include_paths, include_files, macros, undef_macros, options = \
            OpCompileManifestRecorder._split_compile_options(compile_options)
        mllvm_options = []
        filtered_options = []
        skip_next = False
        for idx, option in enumerate(options):
            if skip_next:
                skip_next = False
                continue
            if option == "-mllvm" and idx + 1 < len(options):
                mllvm_options.append(options[idx + 1])
                skip_next = True
            else:
                filtered_options.append(option)
        return {
            "include_paths": include_paths,
            "include_files": include_files,
            "macros": macros,
            "undef_macros": undef_macros,
            "options": filtered_options,
            "mllvm_options": mllvm_options,
            "raw_options": compile_options
        }

    @staticmethod
    def _split_compile_options(compile_options):
        include_paths = []
        include_files = []
        macros = []
        undef_macros = []
        options = []
        skip_next = False
        for idx, option in enumerate(compile_options):
            if skip_next:
                skip_next = False
                continue
            if not isinstance(option, str):
                options.append(option)
                continue
            if option in ("-I", "--include-directory") and idx + 1 < len(compile_options):
                include_paths.append(compile_options[idx + 1])
                skip_next = True
            elif option.startswith("-I") and len(option) > 2:
                include_paths.append(option[2:])
            elif option.startswith("--include-directory="):
                include_paths.append(option.split("=", 1)[1])
            elif option == "-include" and idx + 1 < len(compile_options):
                include_files.append(compile_options[idx + 1])
                skip_next = True
            elif option.startswith("-include") and len(option) > len("-include"):
                include_files.append(option[len("-include"):])
            elif option.startswith("-D") and len(option) > 2:
                macros.append(option[2:])
            elif option.startswith("-U") and len(option) > 2:
                undef_macros.append(option[2:])
            else:
                options.append(option)
        return include_paths, include_files, macros, undef_macros, options
