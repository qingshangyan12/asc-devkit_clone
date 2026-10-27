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

"""Deterministic metadata shared by offline template compilation."""


def parse_levels(value):
    """None denotes the historical boolean-only selection path."""
    if value is None or value == "" or value == -1 or value == "-1":
        return None
    values = value if isinstance(value, (list, tuple)) else str(value).split(",")
    if not values or any(str(item).strip() not in {"0", "1", "2", "3"} for item in values):
        raise ValueError("deterministic levels must contain only 0, 1, 2, 3")
    levels = tuple(int(item) for item in values)
    if len(set(levels)) != len(levels):
        raise ValueError("duplicate deterministic levels")
    if 0 in levels and len(levels) != 1:
        raise ValueError("level 0 cannot share a binary with deterministic levels")
    return levels


def normalize_deterministic(deterministic=None, levels=None):
    """Complete false/0 and true/1 defaults, rejecting contradictory metadata."""
    levels = parse_levels(levels)
    if deterministic is not None and str(deterministic).lower() not in {"true", "false", "0", "1"}:
        raise ValueError("deterministic must be true or false")
    flag = None if deterministic is None else str(deterministic).lower() in {"true", "1"}
    if levels is None:
        return bool(flag), (1,) if flag else (0,)
    inferred_flag = levels != (0,)
    if flag is not None and flag != inferred_flag:
        raise ValueError("deterministic and deterministic levels conflict")
    return inferred_flag, levels


def level_simplified_keys(keys, levels):
    """One binary may advertise multiple dt keys; d remains the boolean field."""
    levels = parse_levels(levels)
    if levels is None:
        return keys
    result = []
    for key in keys:
        op_type, arguments, tensors = key.split("/", 2)
        boolean_fields = [item for item in arguments.split(",") if item.startswith("d=")]
        expected = "d=0" if levels == (0,) else "d=1"
        if boolean_fields != [expected]:
            raise ValueError("simplified key boolean deterministic field conflicts with levels")
        if any(item.startswith("dt=") for item in arguments.split(",")):
            raise ValueError("simplified key already contains deterministic level")
        for level in levels:
            result.append(f"{op_type}/{arguments},dt={level}/{tensors}")
    return result
