#!/usr/bin/env python3
# -----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# -----------------------------------------------------------------------------------------------------------

"""Check that the two search skills have byte-identical distributed files."""
import argparse
import hashlib
import json
from pathlib import Path


def files(root):
    return {p.relative_to(root).as_posix(): hashlib.sha256(p.read_bytes()).hexdigest()
            for p in root.rglob('*') if p.is_file() and '__pycache__' not in p.parts
            and p.suffix != '.pyc' and p.relative_to(root).parts[0] not in {'index', 'tmp'}}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--target', type=Path, required=True, help='cannbot-knowledge checkout root')
    args = parser.parse_args()
    source = Path(__file__).resolve().parents[2]
    errors = {}
    for name in ('asc-search', 'asc-devkit-search'):
        left = files(source/name)
        right = files(args.target/'.agents/skills'/name)
        delta = sorted(p for p in left.keys() | right.keys() if left.get(p) != right.get(p))
        if delta:
            errors[name] = delta
    print(json.dumps({'status': 'different' if errors else 'identical', 'differences': errors}))
    return 1 if errors else 0


if __name__ == '__main__':
    raise SystemExit(main())
