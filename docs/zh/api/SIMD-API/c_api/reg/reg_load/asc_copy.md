# asc_copy

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

完成数据在寄存器间的搬运，支持以下三种模式：

- 矢量数据寄存器复制到矢量数据寄存器：将src中被mask筛选的有效元素复制到dst对应位置，未被mask筛选的位置保持dst原值。

- 掩码寄存器复制到掩码寄存器：将src的所有bit位复制到dst中。支持带mask和不带mask两种重载：
    - 带mask：将src中被mask筛选的bit复制到dst。对于b8模式，mask的每个bit控制src的一个bit；对于b16模式，mask的每2bit为一组、仅LSB控制src的一个bit；对于b32模式，mask的每4bit为一组、仅LSB控制src的一个bit。
    - 不带mask：将src的所有bit复制到dst。

- 矢量数据寄存器复制到掩码寄存器：
    - 数据类型为uint16_t时，矢量数据寄存器（256B）分成16个数据块（16B），用户通过参数可选择某一个数据块，数据块的每个bit broadcast成2bit后变为32B，即输出的mask。
    - 数据类型为uint32_t时，矢量数据寄存器（256B）分成32个数据块（8B），用户通过参数可选择某一个数据块，数据块的每个bit broadcast成4bit后变为32B，即输出的mask。
    
    具体搬运原理请参考[关键特性说明](#关键特性说明)。

## 函数原型

- 矢量数据寄存器复制到矢量数据寄存器

    ```cpp
    __simd_callee__ inline void asc_copy(vector_<dtype>& dst,
                                         vector_<dtype> src,
                                         vector_bool mask)
    ```

    支持的数据类型：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

- 矢量数据寄存器复制到掩码寄存器（**part的值必须在编译期确定，可传入整数字面量或constexpr整数常量，不支持传入运行时变量。**）

    ```cpp
    __simd_callee__ inline void asc_copy(vector_bool& dst,
                                         vector_uint16_t src,
                                         int16_t part)
    __simd_callee__ inline void asc_copy(vector_bool& dst,
                                         vector_uint32_t src,
                                         int16_t part)
    ```

- 掩码寄存器复制到掩码寄存器

    ```cpp
    __simd_callee__ inline void asc_copy(vector_bool& dst,
                                         vector_bool src,
                                         vector_bool mask)
    __simd_callee__ inline void asc_copy(vector_bool& dst,
                                         vector_bool src)
    ```

- 典型示例

```cpp
// 示例：对half矢量数据寄存器复制到矢量数据寄存器
__simd_callee__ inline void asc_copy(vector_half& dst,
                                     vector_half src,
                                     vector_bool mask)
```

## 参数说明

**表1** 参数说明（矢量数据寄存器复制到矢量数据寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中保持原值。 |

**表2** 参数说明（矢量数据寄存器复制到掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（掩码寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| part | 输入 | part决定了src中需要搬运的数据块，具体描述请参考[关键特性说明](#关键特性说明)。<br>&bull; 当数据类型为uint16_t，part∈[0, 15]。<br>&bull; 当数据类型为uint32_t，part∈[0, 31]。 |

**表3** 参数说明（掩码寄存器复制到掩码寄存器）

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（掩码寄存器）。 |
| src | 输入 | 源操作数（掩码寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 针对矢量数据寄存器复制到矢量数据寄存器：dst中未被mask筛选的位置保持原值。
- 针对矢量寄存器复制到掩码寄存器：**part的值必须在编译期确定，可传入整数字面量或constexpr整数常量，不支持传入运行时变量。**
    - 当数据类型为uint16_t，part∈[0, 15]；
    - 当数据类型为uint32_t，part∈[0, 31]。
- 针对掩码寄存器复制到掩码寄存器：带mask的接口dst中未被mask筛选的位置填0。


### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与src均为矢量数据寄存器或掩码寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- mask必须为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。

### 带掩码搬运约束

- dst与src的dtype必须完全一致（int8_t/uint8_t/int16_t/uint16_t/half/int32_t/uint32_t/float/bfloat16_t之一，或两者均为`vector_bool`），否则触发异常。
- 参与搬运的元素个数由矢量长度（VL）决定：矢量数据寄存器搬运中元素个数 = VL ÷ sizeof(dtype)；`vector_bool`掩码寄存器搬运中比特个数 = VL。
- mask比特位按元素大小步进过滤：矢量数据寄存器搬运中每个元素占用`sizeof(dtype)`个比特位，元素i的有效mask比特位于`i × sizeof(dtype)`位置；`vector_bool`掩码寄存器搬运中每个比特独立对应1个mask比特位。
- mask比特位为0的位置采用Merging模式：dst对应位置保留dst原值。

### 无掩码搬运约束

- dst与src必须均为`vector_bool`掩码寄存器，dtype不一致触发异常。
- 不区分有效位置，按全部比特位整体复制；dst的所有比特位被src的对应比特位覆盖。

### 矢量到掩码部分搬运约束

- dst必须为`vector_bool`掩码寄存器，src必须为`vector_uint16_t`或`vector_uint32_t`矢量数据寄存器，dtype组合不符触发异常。
- `part`取值需落在该源dtype允许的位段编号范围内，超出范围的`part`值会导致提取的掩码比特与预期不符。
- 提取出的掩码比特按矢量长度（VL）依次填入dst的对应比特位，未填充位置内容不确定，使用前应通过掩码设置接口明确其状态。

## 关键特性说明

**针对于矢量数据寄存器复制到掩码寄存器：**

由[掩码寄存器](../reg_data_types/data_type_definition.md#掩码寄存器)原理可知，当操作数类型为b16时，每一个element对应2bit的掩码寄存器，且仅2bit中的最低位是有效的；当操作数类型为b32时，每一个element对应4bit的掩码寄存器，且仅4bit中的最低位是有效的。

搬运原理如[图1](#fig-maskgen-b16)、[图2](#fig-maskgen-b32)所示。

- 当操作数类型为uint16_t时，矢量数据寄存器的一个数据块大小为16B，每个bit经过broadcast成2bit后变为32B，即一个MaskReg（32B）。此外，一个矢量数据寄存器有256B/16B = 16个数据块，偏移量part取值范围为[0, 15]。
- 当操作数类型为b32时，矢量数据寄存器的一个数据块大小为8B，每个bit经过broadcast成4bit后变为32B，即一个MaskReg（32B）。此外，一个矢量数据寄存器有256B/8B = 32个数据块，偏移量part取值范围为[0, 31]。

**图1**  矢量寄存器复制到掩码寄存器（uint16_t）<a id="fig-maskgen-b16"></a>
![矢量寄存器复制到掩码寄存器（uint16_t）](../../figures/capi_copy_reg2mask_b16.png)

**图2**  矢量寄存器复制到掩码寄存器（uint32_t）<a id="fig-maskgen-b32"></a>
![矢量寄存器复制到掩码寄存器（uint32_t）](../../figures/capi_copy_reg2mask_b32.png)

## 调用示例

- 矢量数据寄存器复制到矢量数据寄存器

    ```cpp
    __simd_vf__ inline void copy_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
    {
        vector_half dst;
        vector_half src;
        vector_bool mask;
        for (uint16_t i = 0; i < repeat_time; ++i) {
            mask = asc_update_mask_b16(count);
            asc_loadalign(src, src_addr + i * one_repeat_size);
            asc_copy(dst, src, mask);
            asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
        }
    }
    ```

- 掩码寄存器复制到掩码寄存器

    ```cpp
    __simd_vf__ inline void copy_mask_vf(__ubuf__ uint32_t* dst_addr, __ubuf__ uint32_t* src_addr, uint32_t count, uint16_t one_block_size, uint16_t repeat_time)
    {
        vector_bool dst;
        vector_bool src;
        vector_bool mask;
        for (uint16_t i = 0; i < repeat_time; ++i) {
            mask = asc_update_mask_b16(count);
            asc_loadalign_postupdate(src, src_addr);
            asc_copy(dst, src, mask);
            asc_storealign_postupdate(dst_addr, dst, one_block_size);
        }
    }
    ```

- 矢量数据寄存器复制到掩码寄存器

    ```cpp
    __simd_vf__ inline void copy_vector_to_mask_vf(__ubuf__ uint16_t* dst_addr, __ubuf__ uint16_t* src_addr)
    {
        vector_uint16_t src;
        vector_bool mask;
        asc_loadalign(src, src_addr);
        asc_copy(mask, src, 0);
        asc_storealign(dst_addr, mask);
    }
    ```
