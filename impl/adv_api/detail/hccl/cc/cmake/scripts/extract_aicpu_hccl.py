#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
"""Strip the Huawei signed envelope (header magic AA55 AA55) from
$ASCEND_HOME_PATH/opp/built-in/op_impl/aicpu/kernel/aicpu_hccl.tar.gz and
extract the device-side libscatter_aicpu_kernel.so + libhccl_kernel_compat.so
into a build directory so they can be linked at CMake link time.

The script is robust to CANN minor-version drift: instead of assuming the
gzip payload begins at a fixed offset, it scans for the gzip magic
``1f 8b 08``.
"""

import argparse
import os
import shutil
import sys
import tarfile
import tempfile


GZIP_MAGIC = b"\x1f\x8b\x08"
EXPECTED_MEMBERS = (
    "aicpu_kernels_device/libscatter_aicpu_kernel.so",
    "aicpu_kernels_device/libhccl_kernel_compat.so",
)


def find_gzip_offset(blob: bytes) -> int:
    offset = blob.find(GZIP_MAGIC)
    if offset < 0:
        raise RuntimeError(
            "no gzip magic (1f 8b 08) found inside signed envelope; "
            "the file is not a recognized CANN aicpu_hccl.tar.gz"
        )
    return offset


def extract(src: str, out_dir: str) -> None:
    if not os.path.isfile(src):
        raise FileNotFoundError(
            "input not found: {} -- is CANN installed and "
            "$ASCEND_HOME_PATH correctly set?".format(src)
        )

    with open(src, "rb") as fh:
        blob = fh.read()
    offset = find_gzip_offset(blob)

    os.makedirs(out_dir, exist_ok=True)

    with tempfile.NamedTemporaryFile(
        prefix="aicpu_hccl_inner_", suffix=".tar.gz", delete=False
    ) as tmp:
        tmp.write(blob[offset:])
        inner_path = tmp.name

    try:
        with tarfile.open(inner_path, "r:gz") as tf:
            members = [m for m in tf.getmembers() if m.name in EXPECTED_MEMBERS]
            if len(members) != len(EXPECTED_MEMBERS):
                names = ", ".join(sorted(m.name for m in tf.getmembers()))
                raise RuntimeError(
                    "tarball is missing expected members.\n"
                    "  expected: {}\n  found:    {}".format(
                        ", ".join(EXPECTED_MEMBERS), names
                    )
                )
            for member in members:
                # Strip the "aicpu_kernels_device/" prefix so the .so files
                # end up directly under out_dir.
                stripped_name = os.path.basename(member.name)
                fobj = tf.extractfile(member)
                if fobj is None:
                    raise RuntimeError(
                        "could not read {} from tarball".format(member.name)
                    )
                dst = os.path.join(out_dir, stripped_name)
                with open(dst, "wb") as out:
                    shutil.copyfileobj(fobj, out)
                os.chmod(dst, 0o755)
                print("[extract_aicpu_hccl] wrote {}".format(dst))
    finally:
        try:
            os.unlink(inner_path)
        except OSError:
            pass


def main() -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Extract device-side libscatter_aicpu_kernel.so and "
            "libhccl_kernel_compat.so from the CANN signed aicpu_hccl.tar.gz."
        )
    )
    parser.add_argument(
        "--input",
        required=True,
        help="Path to CANN $ASCEND_HOME_PATH/opp/built-in/op_impl/aicpu/kernel/aicpu_hccl.tar.gz",
    )
    parser.add_argument(
        "--out-dir",
        required=True,
        help="Directory where the extracted .so files will be written",
    )
    args = parser.parse_args()

    try:
        extract(args.input, args.out_dir)
    except Exception as exc:  # pylint: disable=broad-except
        sys.stderr.write("[extract_aicpu_hccl] ERROR: {}\n".format(exc))
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
