# operator&

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

对掩码寄存器操作：根据mask对源操作数src0，src1的有效bit进行逻辑与运算，得到新的掩码寄存器。
对矢量数据寄存器操作：根据mask对源操作数src0，src1执行按元素与（&）操作，将结果写入目的操作数dst。

计算公式如下：

```
dst_i = src0_i & src1_i
```

## 3. 函数原型

```cpp
namespace asc::te {

decltype(auto) operator&(reg_tensor<T> src0, reg_tensor<T> src1);

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `src0` | 输入 | 左源操作数；内嵌 `mask` 决定有效 lane，结果 mask 跟随 `src0`。 |
| `src1` | 输入 | 右源操作数；类型 `T` 须与 `src0` 一致。 |

## 5. 返回值说明

返回 `reg_tensor<T>`：
- `value`：活跃 lane 为 `dst_i = src0_i & src1_i`；非活跃 lane 置零。
- `mask`：等于 `src0.mask`。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- `src0.mask` 须预先通过 `update_mask`、`make_mask` 等绑定（通常经 `load(...).with_mask(m)`）；未赋值 mask 内容不确定。
- 二元运算 mask 跟随**左操作数** `src0`。
- 参与计算的元素个数 = VL ÷ sizeof(T)；非活跃 lane 置零（ZEROING）。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_bitwise(local_tensor<int32_t> a_ub, local_tensor<int32_t> b_ub, local_tensor<int32_t> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<int32_t>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<int32_t>(total);                           // 生成本拍谓词，并扣减 rem
        auto a = a_ub.load(make_coord(i * VL)).with_mask(m);               // load 不带 mask，需链式绑定
        auto b = b_ub.load(make_coord(i * VL)).with_mask(m);
        auto y = (a & b) | (a ^ b);                                     // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                            // 写回有效 vec_len
    }
}
```
