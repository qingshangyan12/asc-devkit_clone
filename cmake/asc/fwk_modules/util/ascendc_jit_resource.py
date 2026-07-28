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

import glob
import os
import subprocess


def collect_custom_op_resources(generator, custom_op_root, collection_root, working_dir):
    manifests = sorted(glob.glob(
        os.path.join(working_dir, "kernel_*", "**", "*_manifest.json"),
        recursive=True,
    ))
    if not manifests:
        raise RuntimeError(
            "JIT resource collection found no manifest for {}".format(working_dir)
        )

    command = [
        generator,
        "--collect_custom_op_resources",
        custom_op_root,
        collection_root,
        *manifests,
    ]
    try:
        subprocess.run(command, check=True)
    except subprocess.CalledProcessError as error:
        raise RuntimeError(
            "JIT resource collection failed for {}".format(working_dir)
        ) from error
