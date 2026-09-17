#!/usr/bin/env python3
"""Run FIFO-only DumpTensor host fixtures and architecture compile checks."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host-only", action="store_true")
    parser.add_argument("--cann-path", default=os.getenv("CANN_PATH") or os.getenv("ASCEND_HOME_PATH") or
                        os.getenv("ASCEND_TOOLKIT_HOME"))
    args = parser.parse_args()
    test_dir = Path(__file__).resolve().parent
    root = test_dir.parents[3]
    cann = Path(args.cann_path) if args.cann_path else None
    compiler = cann / "tools/bisheng_compiler/bin/bisheng" if cann else None
    if compiler is None or not compiler.is_file():
        resolved = shutil.which("bisheng")
        if not resolved:
            parser.error("Set --cann-path or CANN_PATH to a CANN installation containing bisheng")
        compiler = Path(resolved).resolve()
        cann = compiler.parents[3]

    def run(command, label):
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                text=True, env={**os.environ, "ASCEND_HOME_PATH": str(cann)})
        if result.returncode:
            print(result.stdout)
            raise SystemExit(f"FAIL: {label} (exit {result.returncode})")
        print(f"PASS: {label}", flush=True)

    run(["python3", str(test_dir / "test_fifo_scope.py")], "FIFO scope")
    with tempfile.TemporaryDirectory(prefix="dump-tensor-v2-") as directory:
        output = Path(directory)
        run(["c++", "-std=c++17", "-I" + str(root), str(test_dir / "test_super_tensor_engine.cpp"),
             "-o", str(output / "super_tensor_engine")], "engine compile")
        run([str(output / "super_tensor_engine")], "engine host fixture")
        capability_test = str(test_dir / "test_super_tensor_capability.cpp")
        run(["c++", "-std=c++17", "-I" + str(root), capability_test,
             "-o", str(output / "capability_disabled")], "capability disabled compile")
        run(["c++", "-std=c++17", "-I" + str(root),
             "-DASCENDC_DUMP_SUPER_TENSOR=1", capability_test,
             "-o", str(output / "capability_protocol_only")], "capability protocol-only compile")
        run(["c++", "-std=c++17", "-I" + str(root),
             "-DASCENDC_DUMP_SUPER_TENSOR=1",
             "-DASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY=1", capability_test,
             "-o", str(output / "capability_enabled")], "capability enabled compile")
        if args.host_only:
            return

        includes = ["-I" + str(root / path) for path in
                    (".", "include", "include/basic_api", "include/utils", "impl",
                     "impl/basic_api", "impl/utils")]
        includes.append("-I" + str(cann / "aarch64-linux/ascendc/include/highlevel_api"))
        targets = [
            ("dav-c220-vec", "dump_tensor_v2_compile.cpp"),
            ("dav-c220-cube", "dump_tensor_v2_compile.cpp"),
            ("dav-c310-vec", "dump_tensor_3510_compile.cpp"),
            ("dav-c310-cube", "dump_tensor_3510_compile.cpp"),
            ("dav-510r2", "dump_tensor_v2_compile.cpp"),
            ("dav-m200", "dump_tensor_2002_compile.cpp"),
            ("dav-m300", "dump_tensor_legacy_compile.cpp"),
            ("dav-l300", "dump_tensor_legacy_compile.cpp"),
            ("dav-l311", "dump_tensor_legacy_compile.cpp"),
        ]
        for arch, source in targets:
            run([str(compiler), "-x", "cce", "-std=c++17", "--cce-aicore-only",
                 "--cce-aicore-arch=" + arch, "-DASCENDC_DUMP=1",
                 "-DASCENDC_DUMP_SUPER_TENSOR=1",
                 "-DASCENDC_DUMP_SUPER_TENSOR_RUNTIME_CAPABILITY=1",
                 "-Wno-deprecated-declarations", "-O2",
                 *includes, "-c", str(test_dir / source), "-o", str(output / (arch + ".o"))], arch)


if __name__ == "__main__":
    main()
