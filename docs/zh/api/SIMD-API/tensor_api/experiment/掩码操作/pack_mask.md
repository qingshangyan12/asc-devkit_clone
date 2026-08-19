# pack（mask）

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

将源操作数中的元素选取低8位（b16）、低16位（b32）、低32位（b64）写入目的操作数的低半部分或高半部分。

- asc_pack：将源操作数写入目的操作数的低半部分。

- asc_pack_v2：将源操作数写入目的操作数的高半部分。

## 3. 函数原型

```cpp
enum class high_low_part : uint8_t {
template <high_low_part h = high_low_part::low>
template <high_low_part h = high_low_part::low>

auto pack(reg_tensor<bool> src);     // 全宽谓词 2→1 压进半区
```

## 4. 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数（矢量数据寄存器/掩码寄存器）。            |
| src | 输入    | 源操作数（矢量数据寄存器/掩码寄存器）。            |

矢量数据寄存器和掩码寄存器的详细说明请参见[data_type_definition.md](../reg_data_types/data_type_definition.md)。

## 5. 返回值说明

返回 `reg_tensor<T>` 谓词寄存器。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 模板参数 `T` 须与后续 `load` / `compute` 元素类型一致。
- 窄类型 pack/unpack 时 mask 须与数据 lane 对齐。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_pack_unpack_mask(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto m_wide  = unpack(make_mask<mask_pattern::vl8, half>()); // 1→2
        auto m_store = pack<high_low_part::low>(m_wide);               // 2→1
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m_wide);  // load 不带 mask，需链式绑定
        auto y = relu(x);                                          // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m_store);                 // 写回有效 vec_len
    }
}
```
