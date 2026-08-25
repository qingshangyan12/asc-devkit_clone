# asc_get_scalar_load_cache_mode

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

读取当前标量读（load）路径的L2 Cache管理策略，并返回[asc_load_l2_cache_mode](../enum/asc_load_l2_cache_mode.md)枚举值。

常用于临时切换策略前保存原配置，或在设置后做回读校验。

## 函数原型

```cpp
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_load_cache_mode()
```

## 参数说明

无

## 返回值说明

当前标量load路径的L2 Cache管理策略，类型为[asc_load_l2_cache_mode](../enum/asc_load_l2_cache_mode.md)。

## 流水类型

PIPE_S

## 约束说明

- 本接口仅反映标量load侧策略，与标量store侧以及向量搬运接口参数中的`l2_cache_mode`无关。
- 若CTRL对应4bit编码不属于已定义枚举值，接口仍按该编码原样强转为枚举返回。开发者应优先通过[asc_set_scalar_cache_mode](asc_set_scalar_cache_mode.md)写入合法枚举值。

## 调用示例

场景：向量主路径通过[asc_copy_gm2ub_align](../vector_data_move/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)搬运tile数据；标量路径仅读取GM上的tiling元数据（如`tile_count`）。该元数据在tile循环内会被多次标量读取，进入循环前查询并保存当前load策略，临时切换为`NORMAL_FIRST_VICTIM`以提升重复标量读的L2复用，循环结束后恢复原策略。若元数据只读一次、后续不再访问，可改用`NOTALLOC_KEEP`跳过L2写入。

```cpp
constexpr uint32_t tile_elems = 1024;
__gm__ int32_t* tiling_gm;   // GM上的tiling元数据，tile_count位于index 0
__gm__ half* src_gm;
__ubuf__ half* dst_ub;

// 查询并保存当前标量load策略
asc_load_l2_cache_mode old_mode = asc_get_scalar_load_cache_mode();
// tiling元数据会被多次标量读取，启用L2复用
asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);

int32_t tile_count = tiling_gm[0];  // 标量读GM元数据
for (int32_t tile = 0; tile < tile_count; ++tile) {
    // 向量块搬运走MTE，不受标量cache mode影响
    asc_copy_gm2ub_align(dst_ub, src_gm + tile * tile_elems,
        tile_elems * sizeof(half));
    // ... 向量计算 ...
}

// 恢复进入本段逻辑前的标量load策略
asc_set_scalar_cache_mode(old_mode);
```
