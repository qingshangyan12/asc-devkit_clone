# CCE vcvt与builtin编译优化对比

本样例比较同一个`float -> half`精度转换在两种前端写法下的编译和运行结果：

```cpp
vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
dst = __builtin_cce_vcvtff_f322f16_x(src, mask, ROUND_R.value,
    RS_DISABLE.value, PART_EVEN.value);
```

`vcvt`版本默认启用，等价的builtin调用以注释紧邻保留在其下方。传入`-DUSE_VCVT_BUILTIN=1`可切换为直接builtin版本。两条路径的其余指令和Host输入完全相同。

## 场景

kernel包含GM/UB搬运、float缩放、残差相加、`f32->f16->f32`量化往返和后处理加法。输入包含RINT舍入边界、正负零、最小half非规格化数、half范围边界和溢出边界。Host侧实现IEEE 754 RN-even的float/half位级转换，逐元素比较最终float结果的原始位编码。

`asc_float2half_rn`的DAV 3510实现使用的正是同一条`vcvt(... ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING)`指令。工具链`__clang_cce_vector_intrinsics.h`中，非V920目标的该`vcvt`封装直接返回`__builtin_cce_vcvtff_f322f16_x`结果；因此预期`-O2`生成的设备代码等价。

## 编译运行

在仓库根目录执行：

```bash
SRC=examples/02_simd_c_api/00_introduction/06_cce_builtin_vcvt_compare/cce_builtin_vcvt_compare.asc
"$ASCEND_HOME_PATH/bin/bisheng" -DUSE_VCVT_BUILTIN=0 --npu-arch=dav-3510 -O2 -std=c++17 \
  -Wno-deprecated-declarations --asc-aicore-lang "$SRC" -lm -o /tmp/cce_vcvt_compare
timeout --signal=TERM --kill-after=5s 30s /tmp/cce_vcvt_compare
"$ASCEND_HOME_PATH/bin/bisheng" -DUSE_VCVT_BUILTIN=1 --npu-arch=dav-3510 -O2 -std=c++17 \
  -Wno-deprecated-declarations --asc-aicore-lang "$SRC" -lm -o /tmp/builtin_vcvt_compare
timeout --signal=TERM --kill-after=5s 30s /tmp/builtin_vcvt_compare
```

当前CANN的`bisheng --asc-aicore-lang`会同时生成宿主和设备产物，不能用单一的`-o`导出唯一的设备LLVM文本。可直接提取并比较两个可执行文件中优化后的设备二进制段：

```bash
objcopy --dump-section .aicore_binary=/tmp/cce_vcvt.aicore /tmp/cce_vcvt_compare
objcopy --dump-section .aicore_binary=/tmp/builtin_vcvt.aicore /tmp/builtin_vcvt_compare
sha256sum /tmp/cce_vcvt.aicore /tmp/builtin_vcvt.aicore
cmp -s /tmp/cce_vcvt.aicore /tmp/builtin_vcvt.aicore && echo '[Success] identical device binary'
```

这比对排除了Host侧`Path`打印字符串造成的文件差异，只覆盖实际下发给NPU的`.aicore_binary`。实际编译、实机运行和设备二进制比较结果见[VALIDATION.md](./VALIDATION.md)。
