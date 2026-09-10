#!/usr/bin/env python3
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Strict AIV core-type regression; AIC output must fail."""
import argparse
import json
import math
import os
from pathlib import Path
import re
import signal
import subprocess
import time


def verify(text, blocks=1, repeats=1, control=0):
    records = re.findall(
        r'^\[(AIC|AIV) Block (\d+)\] DumpTensor: desc=(100|101), addr=0x[0-9a-fA-F]+, '
        r'data_type=float32, position=(REG|UB), dump_size=8\s*\n\[([^\]\r\n]+)\]\s*$', text, re.M)
    if len(records) != blocks * repeats * 2 or text.count('DumpTensor:') != len(records):
        raise ValueError('missing, extra or malformed dump')
    order = {str(b): [] for b in range(blocks)}
    for core, block, desc, position, data in records:
        if core != 'AIV':
            raise ValueError(f'expected AIV, got {core}')
        if block not in order or position != ('REG' if desc == '100' else 'UB'):
            raise ValueError('wrong block or position')
        values = list(map(float, data.split(',')))
        if len(values) != 8 or not all(math.isclose(v, i*1.1, abs_tol=1e-6, rel_tol=1e-6) for i, v in enumerate(values)):
            raise ValueError('wrong dump values')
        order[block].append(desc)
    if any(items != ['100', '101'] * repeats for items in order.values()):
        raise ValueError('per-block order or count mismatch')
    flags = re.findall(r'^CORE_FLAGS block=(\d+) initial=(\d+) before=(\d+) after=(\d+) actual_aiv=(\d+)\s*$', text, re.M)
    expected = {(str(b), '0', str(control), '1', '1') for b in range(blocks)}
    if len(flags) != blocks or set(flags) != expected:
        raise ValueError(f'wrong GM flags: {flags}')
    if text.count('CORE_TYPE_CONTROL') != blocks * control:
        raise ValueError('wrong control invocation count')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('binary', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--blocks', type=int, choices=[1, 2], default=1)
    parser.add_argument('--repeats', type=int, choices=[1, 2], default=1)
    parser.add_argument('--control', type=int, choices=[0, 1], default=0)
    parser.add_argument('--runs', type=int, default=1)
    parser.add_argument('--timeout', type=int, default=180)
    args = parser.parse_args()
    if args.runs < 1 or args.timeout < 1:
        parser.error('runs and timeout must be positive')
    args.output.mkdir(parents=True, exist_ok=True)
    for i in range(args.runs):
        folder = args.output.resolve() / f'run-{i+1}'
        folder.mkdir()
        command = [str(args.binary.resolve()), str(args.config.resolve()), str(args.control), str(args.blocks), str(args.repeats)]
        meta = dict(command=command, verified=False, timed_out=False, blocks=args.blocks, repeats=args.repeats, control=args.control)
        started = time.monotonic()
        with (folder / 'result.log').open('w') as log:
            process = subprocess.Popen(command, cwd=folder, stdout=log, stderr=subprocess.STDOUT, start_new_session=True)
            try:
                process.wait(timeout=args.timeout)
            except subprocess.TimeoutExpired:
                meta['timed_out'] = True
                os.killpg(process.pid, signal.SIGKILL)
                process.wait()
        meta.update(returncode=process.returncode, seconds=time.monotonic()-started)
        try:
            if process.returncode != 0 or meta['timed_out']:
                raise ValueError('process failed or timed out')
            verify((folder/'result.log').read_text(errors='replace'), args.blocks, args.repeats, args.control)
            meta['verified'] = True
        except ValueError as error:
            meta['error'] = str(error)
        (folder/'result.json').write_text(json.dumps(meta, indent=2)+'\n')
        if not meta['verified']:
            raise SystemExit(f"FAIL: {meta['error']}; {folder}")
        print(f'PASS: {folder}', flush=True)


if __name__ == '__main__':
    main()
