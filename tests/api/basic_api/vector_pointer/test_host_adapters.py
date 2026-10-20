# Copyright (c) 2026 ddplys.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Exercise production binary adapters with a CPU backend (not an ASC/NPU test)."""

import argparse
import re
import subprocess
import tempfile
from pathlib import Path

from source_utils import ROOT, functions, strip_diagnostics

OPERATIONS = {
    "Add": ("float", "a[i]+b[i]"),
    "Sub": ("float", "a[i]-b[i]"),
    "Mul": ("float", "a[i]*b[i]"),
    "Max": ("float", "std::max(a[i],b[i])"),
    "Min": ("float", "std::min(a[i],b[i])"),
    "And": ("int32_t", "a[i]&b[i]"),
    "Or": ("int32_t", "a[i]|b[i]"),
    "AddRelu": ("float", "std::max(a[i]+b[i],Scalar(0))"),
    "SubRelu": ("float", "std::max(a[i]-b[i],Scalar(0))"),
}


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--operation", choices=list(OPERATIONS))
    args = parser.parse_args()
    public = ROOT / "include/basic_api/kernel_operator_vec_binary_intf.h"
    private = ROOT / "impl/basic_api/kernel_operator_vec_binary_intf_impl.h"
    helper = (ROOT / "impl/basic_api/utils/kernel_vector_operand.h").as_posix()
    preamble = r"""
#include <type_traits>
#include <algorithm>
#include <cstdint>
#include <cassert>
#include <iostream>
#define __ubuf__
#define __aicore__
#define __ASC_USE_RESERVED_UBUF__(...)
namespace AscendC {
namespace Std = std;
template<typename T> using PrimT = T;
template<typename T> struct LocalTensor {
    T* ptr;
    T* GetPhyAddr() const { return ptr; }
};
struct BinaryRepeatParams { int dstBlkStride=1,src0BlkStride=1,src1BlkStride=1;
    int dstRepStride=8,src0RepStride=8,src1RepStride=8; };
static int calls=0;
static bool lastSetMask=true;
template<typename T> void AddImpl(T* d,T* a,T* b,int32_t n) {
    ++calls; for(int i=0;i<n;++i) d[i]=a[i]+b[i];
}
template<typename T,bool M> void AddImpl(T* d,T* a,T* b,uint64_t n,uint8_t repeats,const BinaryRepeatParams&) {
    lastSetMask=M;
    AddImpl(d,a,b,int32_t(n)*repeats);
}
template<typename T,bool M> void AddImpl(T* d,T* a,T* b,uint64_t* mask,uint8_t repeats,const BinaryRepeatParams&) {
    lastSetMask=M;
    ++calls; for(unsigned r=0;r<repeats;++r) for(unsigned i=0;i<64;++i)
        if((mask[0]>>i)&1) d[r*64+i]=a[r*64+i]+b[r*64+i];
}
}
"""
    checks = r"""
namespace AscendC {
template<typename D,typename A,typename B,typename=void> struct CanAdd:std::false_type {};
template<typename D,typename A,typename B> struct CanAdd<D,A,B,
    std::void_t<decltype(Add(std::declval<D>(),std::declval<A>(),std::declval<B>(),int32_t(1)))>>:std::true_type {};
static_assert(CanAdd<float*,LocalTensor<float>,float*>::value);
static_assert(!CanAdd<float*,int*,float*>::value);
static_assert(!CanAdd<int,float*,float*>::value);
static_assert(!CanAdd<const float*,float*,float*>::value);
static_assert(VectorOperand::Traits<float[32]>::isPointer);
template<int Bits> void Run(float* d,float* a,float* b,int n) {
    LocalTensor<float> td{d},ta{a},tb{b};
    auto dst=[&](){if constexpr(Bits&1)return d;else return td;}();
    auto src0=[&](){if constexpr(Bits&2)return a;else return ta;}();
    auto src1=[&](){if constexpr(Bits&4)return b;else return tb;}();
    Add(dst,src0,src1,n);
    for(int i=0;i<n;++i) assert(d[i]==a[i]+b[i]);
    Add<float>(dst,src0,src1,n);
    for(int i=0;i<n;++i) assert(d[i]==a[i]+b[i]);
    Add<float,false>(dst,src0,src1,uint64_t(32),uint8_t(1),BinaryRepeatParams{});
    assert(!lastSetMask);
    uint64_t mask[2]={0x55555555,0};
    for(int i=0;i<32;++i)d[i]=-999;
    Add(dst,src0,src1,mask,uint8_t(1),BinaryRepeatParams{});
    assert(lastSetMask);
    for(int i=0;i<32;++i) assert(d[i]==((i%2==0)?a[i]+b[i]:-999));
}
}
int main() {
    using namespace AscendC;
    float a[2048],b[2048],d[2048];
    for(int i=0;i<2048;++i) { a[i]=float(i%201)-100; b[i]=float((i*7)%201)-100; }
    for(int n:{1,32,1024,2048}) {
        Run<0>(d,a,b,n); Run<1>(d,a,b,n); Run<2>(d,a,b,n); Run<3>(d,a,b,n);
        Run<4>(d,a,b,n); Run<5>(d,a,b,n); Run<6>(d,a,b,n); Run<7>(d,a,b,n);
    }
    Add(d,a,b,32); // UB-array syntax used by the task examples.
    for(int i=0;i<32;++i)assert(d[i]==a[i]+b[i]);
    assert(GetUnderlyingPtr(a)==a);
    assert(GetUnderlyingPtr(LocalTensor<float>{a})==a);
    assert(calls==129);
    std::cout << "PASS: 129 backend calls, 8 operand combinations, 4 shapes, mask and explicit templates\n";
}
"""
    with tempfile.TemporaryDirectory(prefix="asc-vector-host-") as temp:
        for name, (dtype, expression) in OPERATIONS.items():
            if args.operation and name != args.operation:
                continue
            declarations = "\n".join(
                f["template"] + f["prefix"] + f["params"] + ";" for f in functions(public.read_text(), {name})
            )
            definitions = "\n".join(
                f["template"] + f["prefix"] + f["params"] + "\n" + strip_diagnostics(f["body"])
                for f in functions(private.read_text(), {name})
            )
            backend = preamble.replace("a[i]+b[i]", "(" + expression + ")")
            backend = backend.replace("a[r*64+i]+b[r*64+i]", "(" + expression.replace("[i]", "[r*64+i]") + ")")
            cases = checks.replace("a[i]+b[i]", "(" + expression + ")")
            # Keep the incompatible dtype different even when the API uses integer data.
            cases = cases.replace("CanAdd<float*,int*,float*>", "CanAdd<float*,double*,float*>")
            backend = backend.replace("#include <cstdint>", "#include <cstdint>\nusing Scalar = " + dtype + ";")
            cases = re.sub(r"\bAdd\b", name, cases.replace("float", "Scalar"))
            backend = re.sub(r"\bAddImpl\b", name + "Impl", backend)
            source = (
                backend
                + f'\n#include "{helper}"\nnamespace AscendC {{\n'
                + declarations
                + "\n"
                + definitions
                + "\n}\n"
                + cases
            )
            cpp = Path(temp) / f"{name}.cpp"
            exe = Path(temp) / f"{name}.exe"
            cpp.write_text(source, encoding="utf-8")
            subprocess.run(
                [args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", str(cpp), "-o", str(exe)], check=True
            )
            subprocess.run([str(exe)], check=True)
            print(f"{name}: host adapter tests passed", flush=True)


if __name__ == "__main__":
    main()
