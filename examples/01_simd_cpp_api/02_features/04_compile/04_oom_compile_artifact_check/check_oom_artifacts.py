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
"""Check compatibility contracts in generated OOM compile artifacts."""

import argparse
import os
from pathlib import Path


EXIT_OK = 0
EXIT_CHECK_FAILED = 1
EXIT_ENVIRONMENT_ERROR = 2


def parse_args():
    default_devkit = Path(__file__).resolve().parents[6]
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--devkit-path", type=Path, default=default_devkit)
    parser.add_argument(
        "--cann-path",
        type=Path,
        default=os.environ.get("ASCEND_HOME_PATH"),
    )
    return parser.parse_args()


def validate_environment(devkit_path, cann_path):
    required_paths = (
        devkit_path
        / "tools/build/asc_op_compile_base/asc_op_compiler/ascendc_compile_gen_code.py",
        devkit_path / "tools/build/asc_op_compile_base/asc_op_compiler/compile_op.py",
    )
    if cann_path is None:
        raise ValueError("CANN path is required; use --cann-path or ASCEND_HOME_PATH")
    required_paths += (
        cann_path
        / "python/site-packages/asc_op_compile_base/c_api/libasc_platform.so",
    )
    missing = [str(path) for path in required_paths if not path.is_file()]
    if missing:
        raise ValueError("required path does not exist: " + ", ".join(missing))


def main():
    args = parse_args()
    try:
        validate_environment(args.devkit_path.resolve(), args.cann_path.resolve())
    except (OSError, ValueError) as error:
        print(f"ERROR {error}")
        return EXIT_ENVIRONMENT_ERROR

    print("FAIL checks are not implemented")
    return EXIT_CHECK_FAILED


if __name__ == "__main__":
    raise SystemExit(main())
