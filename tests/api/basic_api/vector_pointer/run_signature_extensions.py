# Copyright (c) 2026 ddplys.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Exercise otherwise shadowed explicit overloads and compare/select mask forms."""

import argparse
import hashlib
import json
import os
import re
import subprocess
from pathlib import Path
from source_utils import replace_cpp

COMPARE = r"""
template<class X> __aicore__ inline auto Slice(X x, uint32_t offset) {
    if constexpr (AscendC::VectorOperand::Traits<X>::isPointer) { return x + offset; }
    else { return x[offset]; }
}
template <int Op, typename D, typename M, typename A, typename B>
__aicore__ inline void Compute(D d, M m, A a, B b, uint32_t count)
{
    for (uint32_t off=0;off<count;off+=64) {
        uint64_t active=count-off<64?count-off:64;
        uint64_t bits[2]={active==64?~uint64_t(0):(uint64_t(1)<<active)-1,0};
        auto dd=Slice(d,off); auto aa=Slice(a,off); auto bb=Slice(b,off);
        AscendC::BinaryRepeatParams p{1,1,1,8,8,8};
        AscendC::UnaryRepeatParams u{1,1,8,8};
        if constexpr (Op==0) { AscendC::Compare(m,aa,bb,AscendC::CMPMODE::LT,MASK,1,p); }
        else if constexpr (Op==1) { AscendC::CompareScalar(m,aa,float(2),AscendC::CMPMODE::LT,MASK,1,u); }
        else if constexpr (Op==2) { AscendC::Compares(m,aa,float(2),AscendC::CMPMODE::LT,MASK,1,u); }
        else if constexpr (Op==8) { AscendC::Compare(aa,bb,AscendC::CMPMODE::LT,MASK,p); AscendC::GetCmpMask(m); }
        else if constexpr (Op==9) { AscendC::SetCmpMask(m); AscendC::GetCmpMask(m); }
        AscendC::PipeBarrier<PIPE_ALL>();
        if constexpr (Op==4) { AscendC::Select(dd,m,aa,float(2),AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE,MASK,1,p); }
        else if constexpr (Op==5) { AscendC::Select(dd,m,float(2),bb,AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE,MASK,1,p); }
        else if constexpr (Op==6) { AscendC::Select<float,uint8_t,true,AscendC::DEFAULT_BINARY_CONFIG>(dd,m,aa,b,AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE,MASK,1,p); }
        else if constexpr (Op==7) { AscendC::Select<float,uint8_t,true,SCALAR_FIRST>(dd,m,a,bb,AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE,MASK,1,p); }
        else { AscendC::Select(dd,m,aa,bb,AscendC::SELMODE::VSEL_TENSOR_TENSOR_MODE,MASK,1,p); }
        AscendC::PipeBarrier<PIPE_ALL>();
    }
}
"""


def execution(text, expression):
    result, count = re.subn(r"bool\s+passed\s*=\s*.*?;", "bool passed = " + expression + ";", text, count=1, flags=re.S)
    assert count == 1
    return result


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--output", type=Path, required=True)
    ap.add_argument("--only")
    args = ap.parse_args()
    here = Path(__file__).resolve().parent
    root = here.parents[3]
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    src = root / "examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic"
    suites = {}
    elem = (src / "vector_pointer_regression.asc").read_text()
    typed = replace_cpp(elem, "AscendC::API(dst, src0, T(2),", "AscendC::API<T,T>(dst, src0, T(2),")
    for name in ["LeakyRelu", "ShiftLeft", "ShiftRight"]:
        typed = replace_cpp(
            typed, "AscendC::" + name + "(dst, src0, T(2),", "AscendC::" + name + "<T,T>(dst, src0, T(2),"
        )
    typed = execution(
        typed,
        " && ".join(
            f'Run<{op}, {dtype}>("{name}", stream)'
            for op, dtype, name in [
                (23, "float", "Adds"),
                (24, "float", "Muls"),
                (25, "float", "Maxs"),
                (26, "float", "Mins"),
                (43, "float", "LeakyRelu"),
                (44, "uint32_t", "ShiftLeft"),
                (45, "uint32_t", "ShiftRight"),
            ]
        ),
    )
    suites["explicit_scalar_types"] = typed
    abs_text = replace_cpp(
        elem,
        "if constexpr (Op == 14) { RUN_UNARY(Abs); }",
        "if constexpr (Op == 14) { AscendC::Abs<float,float>(dst,src0,int32_t(count)); }",
    )
    abs_text = replace_cpp(
        abs_text, "constexpr bool countOnly = Op == 22", "constexpr bool countOnly = Op == 14 || Op == 22"
    )
    suites["abs_two_types"] = execution(abs_text, 'Run<14,float>("Abs",stream)')
    shifts = replace_cpp(elem, "x[i] = T(i % 101);", "x[i] = T(i % 101); y[i] = T(2);")
    shifts = replace_cpp(
        shifts, "constexpr bool countOnly = Op == 22", "constexpr bool countOnly = Op == 44 || Op == 45 || Op == 22"
    )
    for name in ["ShiftLeft", "ShiftRight"]:
        shifts = replace_cpp(
            shifts,
            f"AscendC::{name}(dst, src0, T(2), int32_t(count))",
            f"AscendC::{name}<T,T>(dst, src0, src1, int32_t(count))",
        )
    suites["vector_shift_counts"] = execution(
        shifts, 'Run<44,int32_t>("ShiftLeft",stream) && Run<45,int32_t>("ShiftRight",stream)'
    )
    conv = (
        (src / "vector_pointer_conversion_regression.asc")
        .read_text()
        .replace(
            "BINARY(AddDeqRelu);",
            """
        if(form==0) { AscendC::AddDeqRelu<half,int32_t>(dst,a,b,count); }
        else if(form==1) { AscendC::AddDeqRelu<half,int32_t>(dst,a,b,active,repeats,bparam); }
        else { AscendC::AddDeqRelu<half,int32_t>(dst,a,b,bits,repeats,bparam); }""",
        )
    )
    suites["deq_explicit_types"] = execution(conv, 'Run<6,int32_t,half>("AddDeqRelu",stream)')
    cmp = (src / "vector_pointer_compare_regression.asc").read_text()
    begin = cmp.index("template <int Op, typename D, typename M, typename A, typename B>")
    end = cmp.index("template <int Op, typename T>", begin)
    for form in [1, 2]:
        body = COMPARE.replace("MASK", "active" if form == 1 else "bits")
        text = cmp[:begin] + body + cmp[end:]
        text = text.replace("form=0 modes=16", f"form={form} modes=16")
        suites[f"compare_select_mask{form}"] = text
    level0 = r"""
template<class X> __aicore__ inline auto Slice(X x, uint32_t offset) {
    if constexpr (AscendC::VectorOperand::Traits<X>::isPointer) { return x + offset; }
    else { return x[offset]; }
}
template <int Op, typename D, typename M, typename A, typename B>
__aicore__ inline void Compute(D d, M m, A a, B b, uint32_t count) {
    for(uint32_t off=0;off<count;off+=64) {
        uint32_t active=count-off<64?count-off:64;
        AscendC::SetMaskNorm();
        AscendC::SetVectorMask<float>(int32_t(active));
        AscendC::BinaryRepeatParams p{1,1,1,8,8,8};
        if constexpr(Op==3) {
            AscendC::SetCmpMask(m);
            AscendC::Select<float,AscendC::SELMODE::VSEL_CMPMASK_SPR>(Slice(d,off),Slice(a,off),Slice(b,off),1,p);
        } else {
            AscendC::SetCmpMask((__ubuf__ uint8_t*)AscendC::GetUnderlyingPtr(b));
            AscendC::Select<float,uint8_t>(Slice(d,off),m,Slice(a,off),1,p);
        }
        AscendC::PipeBarrier<PIPE_ALL>();
    }
    AscendC::SetVectorMask<float>(int32_t(64));
}
"""
    level_text = cmp[:begin] + level0 + cmp[end:]
    level_text = replace_cpp(level_text, "void *dx = nullptr", "y[0] = T(2);\n    void *dx = nullptr")
    suites["select_level0"] = execution(
        level_text, 'Run<3,float>("SelectSPRLevel0",stream) && Run<4,float>("SelectScalarLevel0",stream)'
    )
    results = []
    for name, text in suites.items():
        if args.only and name not in args.only.split(","):
            continue
        source = out / (name + ".asc")
        source.write_text(text)
        binary = out / name
        cmd = ["bisheng", "-x", "asc", "--npu-arch=dav-3510", "-std=c++17", "-O2"]
        cmd += [
            "-iquote" + str(root / x)
            for x in [".", "include", "include/basic_api", "include/utils", "impl", "impl/basic_api"]
        ]
        cmd += [
            "-I" + str(root),
            str(source),
            "-L" + os.environ["ASCEND_HOME_PATH"] + "/lib64",
            "-lascendcl",
            "-lm",
            "-o",
            str(binary),
        ]
        record = {"suite": name, "command": cmd, "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest()}
        with (out / (name + "_build.log")).open("w") as f:
            r = subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT)
        record["build_returncode"] = r.returncode
        if not r.returncode:
            with (out / (name + "_npu.log")).open("w") as f:
                r = subprocess.run([str(binary)], stdout=f, stderr=subprocess.STDOUT, timeout=180)
            record["run_returncode"] = r.returncode
        results.append(record)
        (out / "summary.json").write_text(json.dumps(results, indent=2))
        print(name, record.get("run_returncode", "build failed"), flush=True)
    if any(r.get("build_returncode") != 0 or r.get("run_returncode") != 0 for r in results):
        raise SystemExit(1)


if __name__ == "__main__":
    main()
