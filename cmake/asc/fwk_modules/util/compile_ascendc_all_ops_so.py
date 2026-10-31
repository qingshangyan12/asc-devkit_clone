#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

import argparse
import multiprocessing
import os
import shlex
import subprocess

import const_var

MAKEFILE_TEMPLATE = """
# shared library name
LIBRARY_NAME := {}

# output dir
BUILD_DIR := {}
BUILD_DIR_SHELL := {}
LIB_DIR := $(BUILD_DIR)
OBJ_DIR := $(BUILD_DIR)/$(LIBRARY_NAME)
OBJ_DIR_SHELL := {}

# source and object files
{}
{}
OBJECTS := {}
OBJECTS_SHELL := {}

CXX := {}
CXXFLAGS := {}
LDFLAGS := -shared

TARGET := $(LIB_DIR)/lib$(LIBRARY_NAME).so

all: $(TARGET)
	{}

$(LIB_DIR) $(OBJ_DIR):
	mkdir -p "$@"

$(TARGET): $(OBJECTS) | $(LIB_DIR)
	$(CXX) $(LDFLAGS) -o "$@" $(OBJECTS_SHELL) {}

{}

clean:
	rm -rf $(BUILD_DIR_SHELL)

.PHONY: all clean
"""


def _make_escape(value):
    """Escape a filesystem path for use in a Makefile assignment or target."""
    return value.replace("\\", "\\\\").replace("$", "$$").replace("#", "\\#").replace(" ", "\\ ").replace("\t", "\\\t")


def _shell_words(values):
    """Quote recipe words, escaping dollars for Make's additional expansion pass."""
    return " ".join(shlex.quote(value) for value in values).replace("$", "$$")


def _make_shell_word(value):
    """Quote one shell word for use in a Makefile recipe variable."""
    return shlex.quote(value).replace("$", "$$")


def args_parse():
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--src-file", nargs="*", required=True, help="input src files")
    parser.add_argument("-d", "--output-dir", required=True, help="output dir")
    parser.add_argument("-o", "--output", required=True, help="output")
    parser.add_argument("-p", "--cann-path", required=True, help="cann path")
    parser.add_argument("-j", "--parallel-jobs", required=False, help="compile parallel jobs")
    parser.add_argument("-c", "--compile-options", nargs="*", required=False, help="compile options")
    parser.add_argument("-l", "--link-options", nargs="*", required=False, help="compile options")

    parser.add_argument("-c++", "--cxx-compiler", required=False, help="compile options")
    return parser.parse_args()


def gen_compile_make_file(param_args):
    mkfile = os.path.join(param_args.output_dir, "ascendc_all_ops.make")
    debug_content = ""
    compile_options = []
    link_options = []
    if param_args.link_options is not None:
        link_options = [option for value in param_args.link_options for option in shlex.split(value)]
    if param_args.compile_options is not None:
        compile_options = [option for value in param_args.compile_options for option in shlex.split(value)]
        if "-g" not in compile_options:
            debug_content = "rm -rf $(OBJ_DIR_SHELL)"
    else:
        debug_content = "rm -rf $(OBJ_DIR_SHELL)"

    source_variables = []
    object_variables = []
    objects = []
    object_files_shell = []
    object_targets = []
    for index, src_file in enumerate(param_args.src_file):
        object_file = os.path.join(param_args.output_dir, param_args.output, f"source_{index}.o")
        source_variables.append(f"SRC_{index} := {_make_escape(src_file)}")
        object_variables.append(f"OBJ_{index} := $(OBJ_DIR)/source_{index}.o")
        objects.append(f"$(OBJ_{index})")
        object_files_shell.append(_make_shell_word(object_file))
        object_targets.append(f'$(OBJ_{index}): $(SRC_{index}) | $(OBJ_DIR)\n\t$(CXX) $(CXXFLAGS) -c -o "$@" "$<"')
    object_dir_shell = _make_shell_word(os.path.join(param_args.output_dir, param_args.output))
    build_dir_shell = _make_shell_word(param_args.output_dir)
    compile_flags = ["-fPIC", "-D_GLIBCXX_USE_CXX11_ABI=0", "-std=c++11"]
    compile_flags.extend(compile_options)
    compile_flags.append(f"-I{param_args.cann_path}/include")
    link_flags = list(link_options)
    link_flags.extend(["-lexe_graph", "-lregister", "-ltiling_api", f"-L{param_args.cann_path}/lib64"])

    with os.fdopen(os.open(mkfile, const_var.WFLAGS, const_var.WMODES), "w") as fd:
        compile_cmd = MAKEFILE_TEMPLATE.format(
            _make_escape(param_args.output),
            _make_escape(param_args.output_dir),
            build_dir_shell,
            object_dir_shell,
            "\n".join(source_variables),
            "\n".join(object_variables),
            " ".join(objects),
            " ".join(object_files_shell),
            _make_shell_word(param_args.cxx_compiler),
            _shell_words(compile_flags),
            debug_content,
            _shell_words(link_flags),
            "\n\n".join(object_targets),
        )
        fd.write(compile_cmd)


def run_compile_cmd(param_args):
    mkfile = os.path.join(param_args.output_dir, "ascendc_all_ops.make")
    if not os.path.exists(mkfile):
        raise RuntimeError("ascendc_all_ops.make not exist")
    system_cpu_count = 2 * multiprocessing.cpu_count() - 2
    if param_args.parallel_jobs is not None:
        system_cpu_count = min(system_cpu_count, int(param_args.parallel_jobs))
    parallel_compile_job = system_cpu_count if system_cpu_count > 0 else 1
    ret = subprocess.run(["make", "-f", mkfile, f"-j{parallel_compile_job}"], check=False)
    if ret.returncode != 0:
        raise RuntimeError("compile ascend all ops failed")


if __name__ == "__main__":
    args = args_parse()
    gen_compile_make_file(args)
    run_compile_cmd(args)
