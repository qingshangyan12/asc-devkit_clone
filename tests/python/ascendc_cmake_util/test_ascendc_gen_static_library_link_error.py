#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details before copying and using this software.
# ----------------------------------------------------------------------------------------------------------

import os
import subprocess
import sys
from pathlib import Path

import pytest


REPO_ROOT = Path(__file__).resolve().parents[3]
SCRIPT = REPO_ROOT / "cmake/asc/fwk_modules/util/ascendc_gen_static_library.py"


def _run_packer(tmp_path, fail_on_call):
    object_files_dir = tmp_path / "objects"
    kernel_libs_dir = tmp_path / "kernels"
    output_dir = tmp_path / "output"
    tmp_obj_dir = tmp_path / "tmp_objects"
    bin_dir = tmp_path / "bin"
    package_dir = tmp_path / "python/asc_op_compile_base"
    utils_dir = package_dir / "common/utils"
    for directory in (
        object_files_dir,
        kernel_libs_dir / "registry",
        kernel_libs_dir / "kernels",
        output_dir,
        bin_dir,
        utils_dir,
    ):
        directory.mkdir(parents=True, exist_ok=True)

    for init_file in (
        package_dir / "__init__.py",
        package_dir / "common/__init__.py",
        package_dir / "common/utils/__init__.py",
    ):
        init_file.touch()
    (utils_dir / "log_utils.py").write_text(
        "from enum import Enum\n"
        "class AscendCLogLevel(Enum):\n    LOG_ERROR = 3\n"
        "class LogUtil:\n"
        "    class Option(Enum):\n        NON_SOC = 1\n"
        "    @staticmethod\n"
        "    def print_compile_log(*args):\n        pass\n",
        encoding="utf-8",
    )

    input_object = tmp_path / "input.o"
    input_object.write_bytes(b"fake object")
    (object_files_dir / "host.txt").write_text(f"{input_object}\n", encoding="utf-8")
    (kernel_libs_dir / "registry/libopregistry.a").touch()
    (kernel_libs_dir / "kernels/libkernels.a").touch()

    ar = bin_dir / "ar"
    ar.write_text(
        "#!/usr/bin/env python3\nimport sys\nif sys.argv[1] == '-t':\n    print('archive_member.o')\n", encoding="utf-8"
    )
    ar.chmod(0o755)

    ld = bin_dir / "ld"
    call_file = tmp_path / "ld_calls"
    ld.write_text(
        "#!/usr/bin/env python3\n"
        "import pathlib\n"
        "import sys\n"
        f"call_file = pathlib.Path({str(call_file)!r})\n"
        "call = int(call_file.read_text()) + 1 if call_file.exists() else 1\n"
        "call_file.write_text(str(call))\n"
        f"if {fail_on_call!r} == call:\n    sys.exit(9)\n"
        "output = pathlib.Path(sys.argv[sys.argv.index('-o') + 1])\n"
        "output.write_bytes(b'fake relocatable object')\n",
        encoding="utf-8",
    )
    ld.chmod(0o755)

    env = os.environ.copy()
    env["PATH"] = f"{bin_dir}{os.pathsep}{env['PATH']}"
    env["PYTHONPATH"] = str(tmp_path / "python")
    return (
        subprocess.run(
            [
                sys.executable,
                str(SCRIPT),
                "-f",
                str(object_files_dir),
                "-k",
                str(kernel_libs_dir),
                "-t",
                str(tmp_obj_dir),
                "-o",
                str(output_dir / "libsample.a"),
                "-p",
                "sample",
            ],
            cwd=tmp_path,
            env=env,
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            check=False,
        ),
        output_dir / "sample_ascendc_final.o",
        call_file,
    )


@pytest.mark.parametrize("fail_on_call", (1, 2))
def test_link_failure_makes_packer_cli_fail_without_final_object(tmp_path, fail_on_call):
    result, final_object, call_file = _run_packer(tmp_path, fail_on_call=fail_on_call)

    assert result.returncode != 0
    assert not final_object.exists()
    assert not (tmp_path / "tmp_objects").exists()
    assert int(call_file.read_text()) == fail_on_call


def test_successful_link_creates_final_object(tmp_path):
    result, final_object, call_file = _run_packer(tmp_path, fail_on_call=None)

    assert result.returncode == 0, result.stdout
    assert final_object.is_file()
    assert not (tmp_path / "tmp_objects").exists()
    assert int(call_file.read_text()) == 2
