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

"""Build OPC arguments and the shared resource directory from CMake parameters."""

import os
import shlex
from pathlib import Path


def task_directory(root, current_op_type, soc, task_id=None):
    """Use the same resource path for reset, OPC output and the success receipt."""
    from opdesc_parser import SOC_TO_SHORT_SOC_MAP

    if not root or not os.path.isabs(root):
        raise ValueError("kernel-spec-dir must be an absolute path")
    value = soc.lower()
    mapping = {key.lower(): item.lower() for key, item in SOC_TO_SHORT_SOC_MAP.items()}
    short_soc = value if value in mapping.values() else mapping.get(value, value)
    directory = Path(root) / short_soc
    if task_id:
        directory = directory / (".ascendc-" + task_id) / value / current_op_type
    return directory


def shell_options(mode, root, current_op_type, soc, task_id=None):
    """Convert the package mode and root directly to shell-ready OPC options."""
    if mode not in {"None", "Normal", "SK"}:
        raise ValueError("kernel-spec must be None, Normal or SK")
    if mode == "None":
        return ""
    directory = task_directory(root, current_op_type, soc, task_id)
    return " " + shlex.join(["--kernel-spec=" + mode, "--kernel-spec-dir=" + str(directory)])
