# asc_clear_ar_spr

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

对AR特殊寄存器进行清零操作。AR寄存器用于配合[asc_squeeze_with_status](../compare_and_select/asc_squeeze_with_status.md)使用：当调用asc_squeeze_with_status时，有效元素的总字节数会被存入AR寄存器。在调用asc_squeeze_with_status之前，需调用该接口将AR寄存器清零。

## 函数原型

```c
__simd_callee__ inline void asc_clear_ar_spr()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- 本接口清零的地址寄存器持续生效，直至再次写入新值或本接口再次清零；调用本接口后再调用以该地址寄存器为寻址基础的矢量指令，会读到值为0的地址寄存器，按地址0寻址得到非预期数据。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用：地址寄存器使用完毕后调用本接口清零，避免残留地址值被后续逻辑误用。
- 如果需要执行多条本接口指令，且与写入同一地址寄存器的接口存在执行顺序依赖，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。

## 调用示例

```cpp
__simd_vf__ inline void clear_ar_spr_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint32_t one_repeat_size, uint16_t repeat_time)
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
