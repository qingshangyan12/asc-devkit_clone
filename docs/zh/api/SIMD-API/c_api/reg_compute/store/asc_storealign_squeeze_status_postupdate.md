# asc_storealign_squeeze_status_postupdate

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

将[asc_squeeze_and_storeunalign](../reg_permute_sel/asc_squeeze_and_storeunalign.md)操作后保存在AR特殊寄存器中的有效数据长度写入Unified Buffer（UB），并在写入后按字节偏移自动更新目的地址指针。该接口适用于在硬件循环中连续保存squeeze状态的场景。

本接口仅在AIV上生效。

## 函数原型

```cpp
__simd_callee__ inline void asc_storealign_squeeze_status_postupdate(
    __ubuf__ uint32_t*& dst, int32_t offset)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输入、输出 | 目的操作数在UB上的基地址，类型为`__ubuf__ uint32_t*&`。接口执行前将当前AR值写入该地址，执行后将指针增加`offset`字节。 |
| offset | 输入 | 目的地址更新量，类型为`int32_t`，单位为字节。 |

## 返回值说明

无

## 约束说明

- 非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- 初始目的地址及每次Post Update后的目的地址均需32字节对齐，且不得超出实际可用UB范围。
- 调用本接口前，需使用[asc_squeeze_and_storeunalign](../reg_permute_sel/asc_squeeze_and_storeunalign.md)完成squeeze操作；写入的值是其当前累计的有效数据字节数。
- 如果本接口与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化。

## 调用示例

```cpp
__simd_vf__ inline void StoreSqueezeStatus(
    __ubuf__ uint32_t* status, __ubuf__ float* packed, __ubuf__ float* input)
{
    vector_float src;
    vector_float dst;
    vector_store_unalign unalign_reg;
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    asc_loadalign(src, input);
    // 执行squeeze操作，使AR记录有效数据总字节数。
    asc_squeeze_and_storeunalign_init();
    asc_squeeze_and_storeunalign(packed, dst, unalign_reg, src, mask);
    asc_squeeze_and_storeunalign_finalize(packed, unalign_reg);

    __ubuf__ uint32_t* status_ptr = status + 16;
    // 写入状态后，status_ptr自动增加32字节。
    asc_storealign_squeeze_status_postupdate(status_ptr, 32);
}
```
