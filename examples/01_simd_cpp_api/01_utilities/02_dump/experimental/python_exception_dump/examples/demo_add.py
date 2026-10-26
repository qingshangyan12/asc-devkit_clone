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

"""Add normal execution, device fault injection, and offline dump self-check.

Each run uses a fresh directory and separate normal/fault worker processes.
The fault worker never reads the invalid output or reuses the failed context.
"""

import argparse
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

NUMEL = 16384
FAULT_OUTPUT_PTR = 0x100000000000


def _read_tensor(directory, stage, name):
    products = list(directory.glob(f"{stage}_{name}_*.bin"))
    if len(products) != 1:
        raise ValueError(f"expected one {stage} {name} tensor, got {len(products)}")
    binary = products[0]
    meta = json.loads(binary.with_suffix(".json").read_text())
    data = binary.read_bytes()
    if (
        meta["name"] != name
        or meta["stage"] != stage
        or meta["dtype"] != "torch.float16"
        or meta["itemsize"] != 2
        or meta["shape"] != [meta["numel"]]
        or meta["numel"] <= 0
        or meta["nbytes"] != meta["numel"] * 2
        or len(data) != meta["nbytes"]
        or meta["bin_file"] != binary.name
    ):
        raise ValueError(f"invalid tensor data/metadata: {binary.name}")
    return struct.unpack(f"<{meta['numel']}e", data), int(meta["data_ptr"], 16)


def check_dump_products(dump_dir):
    """Check actual Add data and callback products, without importing torch/NPU."""
    directory = Path(dump_dir)
    try:
        normal_x, _ = _read_tensor(directory, "input", "add_normal_x")
        normal_y, _ = _read_tensor(directory, "input", "add_normal_y")
        output, _ = _read_tensor(directory, "output", "add_normal_ret")
        fault_x, fault_x_ptr = _read_tensor(directory, "input", "add_fault_x")
        fault_y, fault_y_ptr = _read_tensor(directory, "input", "add_fault_y")
        if (
            normal_x != fault_x
            or normal_y != fault_y
            or normal_x != tuple(float(i % 1024) for i in range(len(normal_x)))
            or normal_y != (1.0,) * len(normal_x)
            or output != tuple(x + y for x, y in zip(normal_x, normal_y))
        ):
            raise ValueError("normal/fault input contents or normal Add result are incorrect")
        print("[DATA-OK] normal/fault input tensors and normal x + y output")
        fault = json.loads((directory / "fault.json").read_text())
        if int(fault["output_ptr"], 16) != FAULT_OUTPUT_PTR:
            raise ValueError("unexpected injected output address")
        infos = list(directory.glob("*_info.txt"))
        if not infos:
            raise ValueError("missing Runtime exception callback products")
        for info in infos:
            fields = dict(line.split(":", 1) for line in info.read_text().splitlines() if ":" in line)
            fields = {key.strip(): value.strip() for key, value in fields.items()}
            if "add" not in fields["kernel_name"].lower() or not int(fields["error_code"], 16):
                raise ValueError(f"not an Add kernel exception: {info.name}")
            args = info.with_name(info.name.replace("_info.txt", "_args.bin")).read_bytes()
            if fields["args_dumped"] != "yes" or not args or len(args) != int(fields["args_bytes"]):
                raise ValueError(f"kernel args size/content mismatch: {info.name}")
            for pointer in (fault_x_ptr, fault_y_ptr, FAULT_OUTPUT_PTR):
                if not pointer or struct.pack("<Q", pointer) not in args:
                    raise ValueError(f"input/injected output pointer missing from kernel args: {info.name}")
            print(f"[INFO-OK] {fields['kernel_name']}, error={fields['error_code']}")
            print(f"[SIZE-OK] args.bin: {len(args)} bytes == callback args_bytes")
        print("[DATA-OK] input and injected output pointers found in raw kernel args")
    except (OSError, ValueError, KeyError, TypeError, struct.error) as error:
        print(f"[FAIL] {error}")
        return False
    print("[PASS] Add exception dump OK")
    return True


def run_worker(mode, dump_dir):
    # Configure dumping before importing the plugin or initializing the device.
    os.environ["NPUOPS_DUMP_DIR"] = str(dump_dir)
    os.environ["NPUOPS_DUMP_LEVEL"] = "1"
    package = Path(__file__).resolve().parents[1] / "npuops_exception_dump" / "python"
    sys.path.insert(0, str(package))
    import torch
    import torch_npu  # noqa: F401
    from aclnn_add import AclnnAdd, AddExecutionError
    from npuops.debug import enable_exception_dump, run_with_tensor_dump

    torch.npu.set_device(0)
    x = torch.arange(NUMEL).remainder(1024).to(dtype=torch.float16, device="npu")
    y = torch.ones(NUMEL, dtype=torch.float16).npu()
    torch.npu.synchronize()
    add = AclnnAdd()
    # Runtime kernel names differ by product/version. This isolated worker only
    # launches Add after registering the callback, so capture all kernel names.
    enable_exception_dump()
    enable_exception_dump()  # Also exercise idempotent registration.
    if mode == "normal":
        result = run_with_tensor_dump(add, x=x, y=y, tag="add_normal")
        torch.testing.assert_close(result.cpu(), x.cpu() + y.cpu(), rtol=0, atol=0)
        print("normal Add run OK", flush=True)
        return
    (dump_dir / "fault.json").write_text(json.dumps({"output_ptr": hex(FAULT_OUTPUT_PTR)}, indent=2))
    try:
        run_with_tensor_dump(add, x=x, y=y, fault=True, tag="add_fault", dump_outputs=False)
    except AddExecutionError as error:
        print(f"caught expected device execution error: {error}", flush=True)
        # The parent checks real callback products; a Python error alone cannot pass.
        # Avoid device-memory teardown or further NPU calls in a poisoned context.
        sys.stdout.flush()
        sys.stderr.flush()
        os._exit(0)
    raise RuntimeError("expected a device-side Add execution exception")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--dump-dir",
        default="./exception_dump_add",
        help="parent directory; each invocation creates a fresh run_* subdirectory",
    )
    parser.add_argument("--worker", choices=("normal", "fault"), help=argparse.SUPPRESS)
    options = parser.parse_args()
    if options.worker:
        run_worker(options.worker, Path(options.dump_dir).resolve())
        return 0
    base = Path(options.dump_dir).resolve()
    base.mkdir(parents=True, exist_ok=True)
    directory = Path(tempfile.mkdtemp(prefix="run_", dir=base))
    print(f"dump directory: {directory}", flush=True)
    for mode in ("normal", "fault"):
        print(f"\n===== {mode} Add run =====", flush=True)
        try:
            result = subprocess.run(
                [sys.executable, str(Path(__file__).resolve()), "--worker", mode, "--dump-dir", str(directory)],
                timeout=180,
            )
        except subprocess.TimeoutExpired:
            print(f"[FAIL] {mode} worker timed out")
            return 1
        if result.returncode:
            print(f"[FAIL] {mode} worker exited with {result.returncode}")
            return 1
    print("\n===== check dump products =====")
    return 0 if check_dump_products(directory) else 1


if __name__ == "__main__":
    sys.exit(main())
