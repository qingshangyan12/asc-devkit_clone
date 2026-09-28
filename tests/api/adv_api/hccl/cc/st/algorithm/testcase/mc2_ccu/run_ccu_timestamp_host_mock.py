#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
"""Run host lifecycle/formatter mocks using an existing compile_commands.json."""

import argparse
import json
import pathlib
import shlex
import subprocess
import tempfile


def main():
    here = pathlib.Path(__file__).resolve().parent
    root = next(p for p in here.parents if (p / "impl/adv_api/detail/hccl/cc/src").is_dir())
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compile-commands", type=pathlib.Path, default=root / "build/compile_commands.json")
    options = parser.parse_args()
    entries = json.loads(options.compile_commands.read_text())
    entry = next(e for e in entries if e["file"].endswith("/ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc"))
    original = entry.get("arguments") or shlex.split(entry["command"])
    flags = []
    it = iter(original[1:])
    for flag in it:
        if flag in ("-o", "-MF", "-MT", "-MQ"):
            next(it)
        elif flag not in ("-c", "-MD", "-MMD", entry["file"]) and not flag.startswith("-O"):
            flags.append(flag)
    production = root / "impl/adv_api/detail/hccl/cc/src/ops/op_common/template/ccu"
    with tempfile.TemporaryDirectory(prefix="ccu-clock-mock-") as tmp:
        tmp = pathlib.Path(tmp)
        (tmp / "log.h").write_text(
            "#pragma once\n#define HCCL_RUN_WARNING(...) do {} while (0)\n#define HCCL_RUN_INFO(...) do {} while (0)\n"
        )
        command = [
            original[0],
            "-I" + str(tmp),
            *flags,
            "-UNDEBUG",
            "-O0",
            "-ffunction-sections",
            "-fdata-sections",
            str(here / "ccu_timestamp_host_mock.cc"),
            str(production / "ccu_timestamp.cc"),
            str(production / "ccu_print.cc"),
            "-Wl,--gc-sections",
            "-pthread",
            "-o",
            str(tmp / "test"),
        ]
        subprocess.run(command, cwd=entry["directory"], check=True)
        subprocess.run([str(tmp / "test")], check=True)


if __name__ == "__main__":
    main()
