# interleave（mask）

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

将源操作数src0和src1中的元素交织存入目的操作数dst0和dst1中。操作数为掩码寄存器时，不同数据类型的接口决定了交织的位宽大小，例如对于b32类型，交织时以4bit为一组。

![asc_intlf示例](../../figures/asc_intlf示例.png)

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>   // T 只选位宽：half→b16，float→b32
decltype(auto) interleave(reg_tensor<bool> a, reg_tensor<bool> b);
// 返回 reg_pair<bool>。

}
```

## 4. 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| dst0       | 输出    | 目的操作数（矢量数据寄存器或掩码寄存器）。 |
| dst1       | 输出    | 目的操作数（矢量数据寄存器或掩码寄存器）。 |
| src0       | 输入    | 源操作数（矢量数据寄存器）。 |
| src1       | 输入    | 源操作数（矢量数据寄存器）。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[data_type_definition.md](../reg_data_types/data_type_definition.md)。

## 5. 返回值说明

返回 `reg_tensor<T>` 谓词寄存器。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 模板参数 `T` 须与后续 `load` / `compute` 元素类型一致。
- 数据 intlv/deintlv 时谓词须同步重排。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_interleave_mask(local_tensor<half> src0_ub, local_tensor<half> src1_ub, local_tensor<half> dst0_ub, local_tensor<half> dst1_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto m_even = all_mask<half>();
        auto m_odd  = all_mask<half>();
        auto [m0, m1] = interleave<half>(m_even, m_odd);
        auto x0 = src0_ub.load(make_coord(i * VL)).with_mask(m0);      // load 不带 mask，需链式绑定
        auto x1 = src1_ub.load(make_coord(i * VL)).with_mask(m1);
        dst0_ub.store(make_coord(i * VL), x0, m);                     // 写回有效 vec_len
        dst1_ub.store(make_coord((i + 1) * VL), x1, m);
    }
}
```
