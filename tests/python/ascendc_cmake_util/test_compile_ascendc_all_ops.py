# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software; you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See the License for the specific language governing permissions and limitations under the License.
# ----------------------------------------------------------------------------------------------------------

import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
SCRIPT = REPO_ROOT / "cmake/asc/fwk_modules/util/compile_ascendc_all_ops_so.py"


def test_op_code_gen_preserves_source_and_build_paths_with_spaces(tmp_path):
    source_dir = tmp_path / "kernel source"
    source_dir.mkdir()
    sources = [source_dir / "first kernel.cpp", source_dir / "second kernel.cpp"]
    for source in sources:
        source.write_text("int kernel() { return 0; }\n", encoding="utf-8")

    build_dir = tmp_path / "build output"
    cann_dir = tmp_path / "cann package"
    fake_compiler = tmp_path / "compiler tools" / "fake c++"
    fake_compiler.parent.mkdir()
    fake_compiler.write_text(
        "#!/usr/bin/env python3\n"
        "import pathlib\n"
        "import sys\n"
        "args = sys.argv[1:]\n"
        "output = pathlib.Path(args[args.index('-o') + 1])\n"
        f"sources = {list(map(str, sources))!r}\n"
        f"include_dir = '-I{cann_dir}/include'\n"
        f"library_dir = '-L{cann_dir}/lib64'\n"
        "if '-c' in args:\n"
        "    if include_dir not in args or output.parent.name != 'ascend_all_ops' or args[-1] not in sources:\n"
        "        raise SystemExit('source path was split')\n"
        "elif (library_dir not in args or not all(\n"
        "    str(output.parent / 'ascend_all_ops' / f'source_{index}.o') in args\n"
        "    for index in range(len(sources))\n"
        ")):\n"
        "    raise SystemExit('link paths or object paths were split')\n"
        "output.parent.mkdir(parents=True, exist_ok=True)\n"
        "output.touch()\n",
        encoding="utf-8",
    )
    fake_compiler.chmod(0o755)

    op_build = cann_dir / "tools" / "opbuild" / "op_build"
    op_build.parent.mkdir(parents=True)
    op_build.write_text("#!/bin/sh\nexit 0\n", encoding="utf-8")
    op_build.chmod(0o755)

    project_file = source_dir / "CMakeLists.txt"
    project_file.write_text(
        f'''cmake_minimum_required(VERSION 3.16)
project(compile_all_ops_test NONE)
set(ASCENDC_CMAKE_SCRIPTS_PATH "{(REPO_ROOT / "cmake/asc/fwk_modules").as_posix()}")
set(ASCEND_CANN_PACKAGE_PATH "{cann_dir.as_posix()}")
set(ASCEND_PYTHON_EXECUTABLE "{sys.executable}")
set(ASCENDC_CMAKE_COMPILER "{fake_compiler.as_posix()}")
set(ASCEND_COMPUTE_UNIT "ascend910b")
set(ASCEND_CHECK_OPTYPE_DUPLICATE OFF)
include("${{ASCENDC_CMAKE_SCRIPTS_PATH}}/func.cmake")
file(MAKE_DIRECTORY "${{CMAKE_BINARY_DIR}}/generated output")
npu_op_code_gen(
    SRC "{sources[0].as_posix()}" "{sources[1].as_posix()}"
    OUT_DIR "${{CMAKE_BINARY_DIR}}/generated output"
    PACKAGE compile_all_ops_test
)
''',
        encoding="utf-8",
    )

    result = subprocess.run(
        ["cmake", "-S", str(source_dir), "-B", str(build_dir)],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )

    assert result.returncode == 0, result.stdout
    assert (build_dir / "generated output/libascend_all_ops.so").is_file()
