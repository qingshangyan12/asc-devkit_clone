# asc_get_mask_spr

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

从SPR（Special Purpose Register）中读取mask值，并按数据类型格式化后返回。

- 对于b16数据类型：从SPR读取mask共128bit数据，将每个bit复制为2bit。
- 对于b32数据类型：从SPR读取mask共64bit数据，将每个bit复制为4bit。

## 函数原型

```c
__simd_callee__ inline vector_bool asc_get_mask_spr_b16()
__simd_callee__ inline vector_bool asc_get_mask_spr_b32()
```

## 参数说明

无

## 返回值说明

vector_bool，掩码寄存器。

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口由接口封装实现，不触发硬件访存动作；返回的`vector_bool`掩码寄存器仅作为后续Reg矢量计算接口的`mask`入参使用。
- 本接口需在Vector Function（`__simd_vf__`标记的函数）内调用；典型用法是先通过配套setter接口配置MASK SPR的具体比特值，再调用本接口读取并扩展为`vector_bool`，最后传给同Vector Function内的Reg矢量计算接口的`mask`入参。
- MASK SPR由配套setter接口写入具体比特值，本接口仅读取当前MASK SPR值并按所选重载规则扩展为256bit掩码返回，不修改MASK SPR内容。
- 每条矢量流水（PIPE_V）拥有独立的MASK SPR，本接口读取的是当前矢量流水的MASK SPR值；如需读取其他矢量流水的MASK SPR，须切换至对应矢量流水后重新调用本接口。
- 本接口读取的MASK SPR值为配套setter接口最近一次写入值；若配套setter接口尚未调用，读取到的是MASK SPR复位初值（默认全1，即所有元素默认参与计算）。

### b16/b32共用约束

- `asc_get_mask_spr_b16`读取MASK0与MASK1两段共128bit，每1bit扩展为2bit；MASK0控制256bit掩码的低128bit（对应前64个16bit元素位置），MASK1控制高128bit（对应后64个16bit元素位置）。
- `asc_get_mask_spr_b32`仅读取MASK0一段64bit，每1bit扩展为4bit，控制256bit掩码的全部64个32bit元素位置；本重载不读MASK1，MASK1的值不影响`asc_get_mask_spr_b32`的返回结果。
- 调用顺序应为先调用配套setter接口写入MASK SPR值，再调用本接口读取扩展为`vector_bool`，最后调用Reg矢量计算接口；若顺序颠倒，本接口读到的是上一次写入或复位初值，与本批次矢量计算预期的掩码不一致。
- 同一Vector Function内若同时存在16bit与32bit dtype的矢量计算，须分别为两种dtype调用对应重载读取掩码，不可共用一个`vector_bool`跨dtype传给矢量计算接口（两重载读取的MASK SPR范围与扩展规则不同，掩码比特与元素位置映射关系不同）。

## 调用示例

```cpp
__simd_vf__ inline void add_vf(__ubuf__ int16_t* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_int16_t src;
    vector_int16_t dst;
    vector_bool add_mask = asc_get_mask_spr_b16();
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_add_scalar(dst, src, 0, add_mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
