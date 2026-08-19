# select（mask）

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

根据mask的比特位值，从源操作数src0，src1中选择元素，得到目的操作数dst。选择的规则为：当mask的比特位为1时，从src0中选取对应位置的数；当mask的比特位为0时，从src1中选取对应位置的数。

计算公式如下：

```
dst_i = (mask_i == 1) ? src0_i : src1_i
```

## 3. 函数原型

```cpp
namespace asc::te {

reg_tensor<bool> select(reg_tensor<bool> cond,
                        reg_tensor<bool> a,
                        reg_tensor<bool> b);
// cond ? a : b，结果仍是谓词。

}
```

## 4. 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（掩码寄存器或矢量数据寄存器）。 |
| src0 | 输入 | 源操作数0（掩码寄存器或矢量数据寄存器）。 |
| src1 | 输入 | 源操作数1（掩码寄存器或矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。指定选择src0或src1为有效数据。mask的比特位为1时，选取src0；mask的比特位为0时，选取src1。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[data_type_definition.md](../reg_data_types/data_type_definition.md)。

## 5. 返回值说明

返回 `reg_tensor<T>` 谓词寄存器。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 模板参数 `T` 须与后续 `load` / `compute` 元素类型一致。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_select_mask(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto pred = select(make_mask<mask_pattern::vl8, half>(), all_mask<half>(), none_mask<half>());
        auto x = src_ub.load(make_coord(i * VL)).with_mask(pred);    // load 不带 mask，需链式绑定
        auto y = relu(x);                                          // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                      // 写回有效 vec_len
    }
}
```
