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

from __future__ import annotations

import argparse
import logging
import sys
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_CANN_ROOT = Path("/usr/local/Ascend/cann")
DEFAULT_OUTPUT = PROJECT_ROOT / ".clangd.local"
TEMPLATE = PROJECT_ROOT / ".clangd.in"
REQUIRED_DIRS = (
    "asc/include",
    "asc/impl",
    "tools/cpudebug/lib/include",
    "include/ascendc/highlevel_api",
    "include/ascendc/host_api",
)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate a clangd config for a local CANN installation.")
    parser.add_argument(
        "--cann-root",
        default=str(DEFAULT_CANN_ROOT),
        help="CANN package root directory. Default: /usr/local/Ascend/cann",
    )
    parser.add_argument("--npu-arch", default="2201", choices=("2201", "3510"), help="Value for __NPU_ARCH__.")
    parser.add_argument(
        "--output",
        default=str(DEFAULT_OUTPUT),
        help="Output config path. Default: .clangd.local",
    )
    parser.add_argument("-f", "--force", action="store_true", help="Overwrite the output file if it already exists.")
    return parser.parse_args()


def missing_required_dirs(cann_root: Path) -> list[str]:
    return [rel_path for rel_path in REQUIRED_DIRS if not (cann_root / rel_path).is_dir()]


def render_template(cann_root: Path, npu_arch: str) -> str:
    template = TEMPLATE.read_text(encoding="utf-8")
    return template.replace("@CANN_ROOT@", cann_root.as_posix()).replace("@NPU_ARCH@", npu_arch)


def configure_logging() -> None:
    logging.basicConfig(level=logging.INFO, format="%(message)s")


def main() -> int:
    configure_logging()
    args = parse_args()
    cann_root = Path(args.cann_root).expanduser().resolve()
    output = Path(args.output).expanduser()
    if not output.is_absolute():
        output = PROJECT_ROOT / output
    output = output.resolve()

    missing = missing_required_dirs(cann_root)
    if missing:
        logging.error("Missing required CANN directories:")
        for rel_path in missing:
            logging.error("  - %s", cann_root / rel_path)
        return 1

    if output.exists() and not args.force:
        logging.error("%s already exists. Use --force to overwrite it.", output)
        return 1

    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(render_template(cann_root, args.npu_arch), encoding="utf-8")
    logging.info("Generated %s", output)
    logging.info("Copy it to .clangd or point clangd user config to these flags if you want clangd to load it.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
