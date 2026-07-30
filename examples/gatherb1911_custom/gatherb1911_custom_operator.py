#!/usr/bin/python3
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

import numpy as np


SRC_COUNT = 16320
DST_COUNT = 16320
OFFSET_COUNT = 2040
SOURCE_BLOCK_COUNT = 2040
FLOATS_PER_BLOCK = 8
BLOCKS_PER_REPEAT = 8
REPEAT_TIME = 255
DST_BLOCK_STRIDE = 2
DST_REPEAT_STRIDE = 8
LOCAL_DST_BLOCK_COUNT = (
    (REPEAT_TIME - 1) * DST_REPEAT_STRIDE
    + (BLOCKS_PER_REPEAT - 1) * DST_BLOCK_STRIDE
    + 1
)
LOCAL_DST_COUNT = LOCAL_DST_BLOCK_COUNT * FLOATS_PER_BLOCK


def gen_golden_data(params, data_dir):
    param_dict = {param.name: param for param in params}
    rng = np.random.RandomState(1911)

    src = rng.uniform(-100.0, 100.0, SRC_COUNT).astype(param_dict["src"].np_dtype)
    offset = rng.permutation(SOURCE_BLOCK_COUNT).astype(param_dict["offset"].np_dtype)
    golden_local = np.zeros(LOCAL_DST_COUNT, dtype=param_dict["dst"].np_dtype)

    for index, source_block in enumerate(offset):
        repeat_index, block_index = divmod(index, BLOCKS_PER_REPEAT)
        src_start = int(source_block) * FLOATS_PER_BLOCK
        dst_block = repeat_index * DST_REPEAT_STRIDE + block_index * DST_BLOCK_STRIDE
        dst_start = dst_block * FLOATS_PER_BLOCK
        golden_local[dst_start:dst_start + FLOATS_PER_BLOCK] = src[src_start:src_start + FLOATS_PER_BLOCK]

    src.tofile(str(data_dir / param_dict["src"].data_path))
    offset.tofile(str(data_dir / param_dict["offset"].data_path))
    golden_local[:DST_COUNT].tofile(str(data_dir / param_dict["dst"].golden_path))


def gen_yaml(data_dir):
    import ruamel.yaml

    yaml_body = {
        "testcases": [
            {
                "dir_name": "gatherb1911_custom",
                "file_basename": "gatherb1911_custom_operator",
                "soc_version": ["ascend910D1"],
                "kernel_name": "gatherb1911_custom_kernel",
                "core_type": "VectorCore",
                "core_num": 1,
                "params": [
                    {
                        "name": "src",
                        "shape": [SRC_COUNT],
                        "cpp_dtype": "float",
                        "param_type": "input",
                        "data_path": "src.bin",
                    },
                    {
                        "name": "offset",
                        "shape": [OFFSET_COUNT],
                        "cpp_dtype": "uint32_t",
                        "param_type": "input",
                        "data_path": "offset.bin",
                    },
                    {
                        "name": "dst",
                        "shape": [DST_COUNT],
                        "cpp_dtype": "float",
                        "param_type": "output",
                        "data_path": "output.bin",
                        "golden_path": "golden.bin",
                    },
                ],
            }
        ]
    }

    with (data_dir / "gatherb1911_custom_testcases.yaml").open("w", encoding="utf-8") as yaml_file:
        yaml = ruamel.yaml.YAML()
        yaml.default_flow_style = None
        yaml.dump(yaml_body, yaml_file)


if __name__ == "__main__":
    from pathlib import Path

    gen_yaml(Path("."))
