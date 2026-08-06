# asc_create_mask

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

创建掩码寄存器（vector_bool），支持b8、b16、b32三种位宽模式。pat_mode参数指定mask的模式，即指定哪些位置的元素参与计算。

位宽模式说明：
- b8模式：每个bit对应一个8bit元素（共256元素）。
- b16模式：每2个bit为一组对应一个16bit元素（共128元素）。
- b32模式：每4个bit为一组对应一个32bit元素（共64元素）。

**图1** asc_create_mask原理

![asc_create_mask原理](../../figures/capi_create_mask.png)

## 函数原型

```cpp
asc_create_mask_b8(pat_mode)
asc_create_mask_b16(pat_mode)
asc_create_mask_b32(pat_mode)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| pat_mode | 输入 | mask模式，取值如下：<br>&bull; PAT_ALL：所有元素设置为有效数据，全部参与计算<br>&bull; PAT_VL1：最低1个元素设置为有效数据<br>&bull; PAT_VL2：最低2个元素设置为有效数据<br>&bull; PAT_VL3：最低3个元素设置为有效数据<br>&bull; PAT_VL4：最低4个元素设置为有效数据<br>&bull; PAT_VL8：最低8个元素设置为有效数据<br>&bull; PAT_VL16：最低16个元素设置为有效数据<br>&bull; PAT_VL32：最低32个元素设置为有效数据<br>&bull; PAT_VL64：最低64个元素设置为有效数据<br>&bull; PAT_VL128：最低128个元素设置为有效数据<br>&bull; PAT_M3：下标为3的倍数的元素设置为有效数据<br>&bull; PAT_M4：下标为4的倍数的元素设置为有效数据<br>&bull; PAT_H：低一半的元素设置为有效数据<br>&bull; PAT_Q：低四分之一的元素设置为有效数据<br>&bull; PAT_ALLF：所有元素设置为无效元素，均不参与计算 |

## 返回值说明

vector_bool，掩码寄存器。

## 约束说明

掩码寄存器的数量上限为8，超过上限的掩码寄存器会写入预留的8K UB内存中，可能引起性能劣化。编译器会自动复用生命周期结束的寄存器和预留内存，若两者均可用，优先复用寄存器。


### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- `pat_mode`必须使用已定义的15种模式取值之一（全真、最低N元素、每N元素、半数、四分之一、全假），传入其他值会在编译期触发`static_assert`失败，编译无法通过。
- 重载需与后续矢量计算接口的dtype严格匹配：8 bit dtype（uint8_t/int8_t/bool/fp8_e4m3fn_t/fp8_e5m2_t等）使用`asc_create_mask_b8`，16 bit dtype（half/bfloat16_t/uint16_t/int16_t等）使用`asc_create_mask_b16`，32 bit dtype（float/uint32_t/int32_t等）使用`asc_create_mask_b32`。重载与dtype不匹配会导致掩码比特与元素位置错位，矢量计算接口取到非预期的有效元素集合。
- 掩码寄存器返回后需立即用于矢量计算接口的`mask`入参；若需在多处复用，可直接复制到另一个`vector_bool`变量保存，无需重复调用本接口生成相同掩码。
- 生成掩码的有效比特位由矢量长度与dtype位宽共同决定：矢量长度为`VL`字节时，b8重载的有效比特数为`VL÷1`、b16重载为`VL÷2`、b32重载为`VL÷4`，超出该范围的比特位不参与矢量计算接口的掩码过滤。

### b8/b16/b32共用约束

- 全真模式实际生成的有效比特位数为矢量长度÷sizeof(dtype)，并非无限延伸；矢量长度外的比特位不参与矢量计算。
- 半数（半数元素）模式的实际有效元素个数=矢量长度÷sizeof(dtype)÷2，dtype位宽越大实际置1的比特数越少（如矢量长度256字节时：b8置128个、b16置64个、b32置32个）。
- 四分之一（四分之一元素）模式的实际有效元素个数=矢量长度÷sizeof(dtype)÷4，dtype位宽越大实际置1的比特数越少（如矢量长度256字节时：b8置64个、b16置32个、b32置16个）。
- 全假模式生成的掩码寄存器所有有效比特位为0，传给矢量计算接口的`mask`入参时所有元素位置采用清零模式（ZEROING），不执行任何计算，结果位置写0。
- 每3元素/每4元素模式按每3或4个元素置1个有效比特位的方式生成掩码，与最低N元素的连续置1不同，适用于稀疏选择场景。

## 调用示例

```cpp
__simd_vf__ inline void create_mask_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_half src;
    vector_store_unalign ureg;
    vector_bool mask = asc_create_mask_b16(PAT_M4);
    asc_clear_ar_spr();
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_squeeze_with_status(dst, src, mask);
        asc_storeunalign_postupdate(dst_addr, ureg, dst);
    }
}
```
