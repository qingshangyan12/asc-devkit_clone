#!/usr/bin/python
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

"""Route the first-phase RUN package Kernel Spec options to OPC."""

import os
import shlex
from pathlib import Path


KERNEL_SPEC_KEYS = {"--kernel-spec", "--kernel-spec-dir"}
KERNEL_SPEC_MODES = {"None", "Normal", "SK"}


def _split_option(option):
    key, separator, value = option.partition("=")
    if key not in KERNEL_SPEC_KEYS:
        return None
    if not separator or not value or any(char in value for char in "@\n\r;"):
        raise ValueError("Invalid kernel specialization option: " + option)
    return key, value


def _validate_pair(selected):
    mode = selected.get("--kernel-spec")
    if mode not in KERNEL_SPEC_MODES:
        raise ValueError("kernel-spec must be None, Normal or SK")
    if mode == "None":
        if "--kernel-spec-dir" in selected:
            raise ValueError("kernel-spec-dir cannot be used with kernel-spec=None")
        return
    directory = selected.get("--kernel-spec-dir")
    if not directory:
        raise ValueError("kernel-spec-dir is required")
    if not os.path.isabs(directory):
        raise ValueError("kernel-spec-dir must be absolute")


def split_and_write_options(options, config_file, op_type, compute_unit):
    """Remove Kernel Spec options from device options and append one OPC row."""
    remaining = []
    kernel_spec_options = {}
    for option in options:
        parsed = _split_option(option)
        if parsed is None:
            remaining.append(option)
            continue
        key, value = parsed
        if key in kernel_spec_options and kernel_spec_options[key] != value:
            raise ValueError("Conflicting kernel specialization option: " + key)
        kernel_spec_options[key] = value

    if not kernel_spec_options:
        return remaining

    _validate_pair(kernel_spec_options)
    row = "@".join([op_type, compute_unit] + [key + "=" + value for key, value in sorted(kernel_spec_options.items())])
    with open(config_file, "a", encoding="utf-8") as config:
        config.write(row + "\n")
    return remaining


def _normalize_soc(soc):
    from opdesc_parser import SOC_TO_SHORT_SOC_MAP

    value = soc.lower()
    mapping = {key.lower(): item.lower() for key, item in SOC_TO_SHORT_SOC_MAP.items()}
    if value in mapping.values():
        return value
    return mapping.get(value, value)


def _parse_row(line):
    fields = line.strip().split("@")
    if len(fields) < 3:
        return None
    options = {}
    for item in fields[2:]:
        parsed = _split_option(item)
        if parsed is None:
            continue
        key, value = parsed
        if key in options and options[key] != value:
            raise ValueError("Conflicting kernel specialization option: " + key)
        options[key] = value
    return fields[0], [item for item in fields[1].split(";") if item], options


def _match_compute_units(compute_units, current_soc):
    if not compute_units or any(compute_unit.upper() == "ALL" for compute_unit in compute_units):
        return True, False
    normalized_units = {_normalize_soc(compute_unit) for compute_unit in compute_units}
    return current_soc in normalized_units, True


def shell_options(config_file, current_op_type, soc, task_id=None):
    """Resolve the most specific op and SoC scope and return shell-ready OPC options."""
    if not config_file or not Path(config_file).is_file():
        return ""

    current_soc = _normalize_soc(soc)
    options_by_rank = {}
    for line in Path(config_file).read_text(encoding="utf-8").splitlines():
        row = _parse_row(line)
        if row is None:
            continue
        op_type, compute_units, options = row
        if op_type not in {"ALL", current_op_type}:
            continue
        if not options:
            continue
        soc_matches, exact_soc = _match_compute_units(compute_units, current_soc)
        if not soc_matches:
            continue
        _validate_pair(options)
        rank = (int(op_type == current_op_type), int(exact_soc))
        if rank in options_by_rank and options_by_rank[rank] != options:
            raise ValueError("Conflicting kernel specialization scope")
        options_by_rank[rank] = options

    if not options_by_rank:
        return ""

    selected_options = options_by_rank[max(options_by_rank)]
    if selected_options["--kernel-spec"] == "None":
        return ""

    output_dir = os.path.join(selected_options["--kernel-spec-dir"], current_soc)
    if task_id:
        # Each CMake producer owns a separate subtree; the public option root is unchanged.
        output_dir = os.path.join(output_dir, ".ascendc-" + task_id, soc.lower(), current_op_type)
    return " " + shlex.join(["--kernel-spec=" + selected_options["--kernel-spec"], "--kernel-spec-dir=" + output_dir])


def task_directory(config_file, current_op_type, soc, task_id):
    """Return the same private producer directory used by command generation."""
    for option in shlex.split(shell_options(config_file, current_op_type, soc, task_id)):
        if option.startswith("--kernel-spec-dir="):
            return Path(option.partition("=")[2])
    return None
