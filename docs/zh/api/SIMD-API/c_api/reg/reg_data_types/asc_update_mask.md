# asc_update_mask

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

根据元素个数value生成mask，并自动将value减去当前向量处理单元的元素个数。支持b8、b16、b32三种位宽模式，由于VL=256B，各模式的向量处理单元元素个数为：

- b8模式：每次处理256个元素。
- b16模式：每次处理128个元素。
- b32模式：每次处理64个元素。

**图1**  asc_update_mask更新流程

![asc_update_mask更新流程](../../figures/capi_update_mask.png)

## 函数原型

```cpp
__simd_callee__ inline vector_bool asc_update_mask_b8(uint32_t& value)
__simd_callee__ inline vector_bool asc_update_mask_b16(uint32_t& value)
__simd_callee__ inline vector_bool asc_update_mask_b32(uint32_t& value)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| value | 输入/输出 | 元素个数。调用后自动减去当前向量处理单元的元素个数（b8模式、b16模式、b32模式分别为256、128、64）。<br>执行完一次该接口后，value = (value < VL_T) ? 0 : (value - VL_T)，VL_T表示位宽为VL的矢量数据寄存器中，可存放数据类型T的元素个数。<br>例如，有320个b16数据类型的元素，每次处理128个元素：<br>第一次调用接口，生成的mask对应的元素全为有效数据，value = 320-128 = 192。<br>第二次调用接口，生成的mask对应的元素全为有效数据，value = 192-128 = 64。<br>第三次调用接口，生成的mask对应的元素只有低半部分为有效数据，value = 0。|

## 返回值说明

vector_bool，掩码寄存器。

## 约束说明

掩码寄存器的数量上限为8，超过上限的掩码寄存器会写入预留的8K UB内存中，可能引起性能劣化。编译器会自动复用生命周期结束的寄存器和预留内存，若两者均可用，优先复用寄存器。


### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口需在Vector Function（`__simd_vf__`标记的函数）内调用：典型用法是在HardwareLoop循环体首行调用本接口按剩余`value`生成当前迭代的掩码，再传给同循环内的Reg数据搬入搬出接口与Reg计算接口的`mask`入参。
- `value`入参为`uint32_t`引用，POST_UPDATE语义要求每次调用后由本接口把`value`减去本次已生效的元素个数并写回；调用方不可在调用本接口后、下一次调用本接口前手工修改`value`，否则下一次迭代掩码与实际剩余元素数不匹配。
- 重载需与循环内矢量计算接口的dtype严格匹配：8 bit dtype（uint8_t/int8_t/bool/fp8_e4m3fn_t/fp8_e5m2_t等）使用`asc_update_mask_b8`，16 bit dtype（half/bfloat16_t/uint16_t/int16_t等）使用`asc_update_mask_b16`，32 bit dtype（float/uint32_t/int32_t等）使用`asc_update_mask_b32`。重载与dtype不匹配会导致掩码比特与元素位置错位，矢量计算接口取到非预期的有效元素集合。
- 循环内每次迭代消耗一个VL长度的元素，调用方需保证循环次数与`value`初始值满足`value = 循环次数 × VL元素个数`或`value`略大于循环次数 × VL元素个数（最后一次迭代掩码不全开）；若`value`初始值小于循环次数 × VL元素个数，会在迭代过程中提前减为0，后续迭代掩码全假，矢量计算接口对应位置清零，得到非预期结果。
- 生成的掩码寄存器需立即用于同一次迭代的矢量计算接口`mask`入参；若需在多处复用同一掩码，可直接复制到另一个`vector_bool`变量保存，无需在同一迭代内重复调用本接口生成相同掩码（重复调用会把`value`多扣多次）。

### b8/b16/b32共用约束

- `value`入参单位为元素个数，不是字节数；dtype标识（b8/b16/b32）决定本接口内部VL元素个数 = VL ÷ sizeof(dtype)的换算关系，调用方只需按剩余元素个数传入`value`，无需自行换算为字节。
- `value`等于0时返回全假掩码，传给矢量计算接口的`mask`入参时所有元素位置采用清零模式（ZEROING），不执行任何计算，结果位置写0；该行为不会触发异常，但若循环内仍执行Reg数据搬入搬出接口，搬入搬出按掩码全假处理。
- 同一循环内若dtype标识切换（如先16 bit后32 bit），需为不同dtype分别调用对应重载并维护各自的`value`计数器，不可共用一个`value`跨dtype调用本接口（不同重载VL元素个数不同，扣减量不同）。

## 调用示例

```cpp
__simd_vf__ inline void update_mask_vf(__ubuf__ float* dst_addr, __ubuf__ float* src0_addr, __ubuf__ float* src1_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_float src0;
    vector_float src1;
    vector_float dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b32(count);
        asc_loadalign_postupdate(src0, src0_addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1_addr, one_repeat_size);
        asc_add(dst, src0, src1, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
