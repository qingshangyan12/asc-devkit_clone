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

"""Route kernel specialization options to OPC, never to the device compiler."""

import os
import shlex
from pathlib import Path

KEYS = {"--kernel-spec", "--kernel-spec-dir"}


def split_and_write_options(options, config_file, op_type, compute_unit):
    remaining, selected = [], {}
    for option in options:
        key, sep, value = option.partition("=")
        if key not in KEYS:
            remaining.append(option)
            continue
        if not sep or not value or any(c in value for c in "@\n\r;"):
            raise ValueError("Invalid kernel specialization option: " + option)
        if key in selected and selected[key] != value:
            raise ValueError("Conflicting kernel specialization option: " + key)
        selected[key] = value
    if selected:
        mode = selected.get("--kernel-spec")
        if mode not in {"None", "Normal", "SK"}:
            raise ValueError("kernel-spec must be None, Normal or SK")
        if mode != "None" and "--kernel-spec-dir" not in selected:
            raise ValueError("kernel-spec-dir is required")
        if "--kernel-spec-dir" in selected and not os.path.isabs(
            selected["--kernel-spec-dir"]
        ):
            raise ValueError("kernel-spec-dir must be absolute")
        row = "@".join(
            [op_type, compute_unit] + [k + "=" + v for k, v in sorted(selected.items())]
        )
        with open(config_file, "a", encoding="utf-8") as f:
            f.write(row + "\n")
    return remaining


def _soc_key(soc):
    from opdesc_parser import SOC_TO_SHORT_SOC_MAP

    mapping = {k.lower(): v.lower() for k, v in SOC_TO_SHORT_SOC_MAP.items()}
    if soc.lower() in set(mapping.values()):
        return soc.lower()
    return mapping.get(soc.lower(), soc.lower())


def shell_options(config_file, op_type, soc):
    """Resolve scope with exact op precedence and reject conflicting same-scope values."""
    if not config_file or not Path(config_file).is_file():
        return ""
    scopes = {}
    for line in Path(config_file).read_text().splitlines():
        row = line.split("@")
        if len(row) < 3 or row[0] not in {"ALL", op_type}:
            continue
        units = [u for u in row[1].split(";") if u]
        if units and _soc_key(soc) not in [_soc_key(u) for u in units]:
            continue
        values = {}
        for item in row[2:]:
            key, sep, value = item.partition("=")
            if key in KEYS:
                if not sep:
                    raise ValueError("Missing value for " + key)
                values[key] = value
        if not values:
            continue
        rank = (int(row[0] == op_type), int(bool(units)))
        previous = scopes.setdefault(rank, {})
        for key, value in values.items():
            if key in previous and previous[key] != value:
                raise ValueError(
                    "Conflicting kernel specialization scope for " + op_type
                )
            previous[key] = value
    if not scopes:
        return ""
    # Configuration is an atomic pair: never borrow the directory from another scope.
    selected = scopes[max(scopes)]
    mode = selected.get("--kernel-spec")
    if mode not in {"None", "Normal", "SK"}:
        raise ValueError("Invalid kernel-spec mode")
    if mode == "None":
        return ""
    directory = selected.get("--kernel-spec-dir", "")
    if not os.path.isabs(directory):
        raise ValueError("kernel-spec-dir must be absolute")
    # Each SoC owns an output tree; compile_op creates per-binary subdirectories.
    directory = os.path.join(directory, _soc_key(soc))
    return " " + shlex.join(["--kernel-spec=" + mode, "--kernel-spec-dir=" + directory])


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("--config-file", required=True)
    parser.add_argument("--op-type", required=True)
    parser.add_argument("--soc", required=True)
    args = parser.parse_args()
    print(shell_options(args.config_file, args.op_type, args.soc))
