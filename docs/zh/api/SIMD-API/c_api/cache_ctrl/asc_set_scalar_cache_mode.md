# asc_set_scalar_cache_mode

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

配置标量单元访问Global Memory时的L2 Cache管理策略。接口通过修改CTRL寄存器对应比特域生效：

- 传入[asc_load_l2_cache_mode](../enum/asc_load_l2_cache_mode.md)时，设置标量读（load）策略。
- 传入[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)时，设置标量写（store）策略。

设置后，后续标量路径上的GM读写将按该策略访问L2 Cache，直至再次调用本接口修改。

## 函数原型

```cpp
__aicore__ inline void asc_set_scalar_cache_mode(asc_load_l2_cache_mode l2_cache_mode)
__aicore__ inline void asc_set_scalar_cache_mode(asc_store_l2_cache_mode l2_cache_mode)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| l2_cache_mode | 输入 | L2 Cache管理策略。load重载传入[asc_load_l2_cache_mode](../enum/asc_load_l2_cache_mode.md)枚举值；store重载传入[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)枚举值。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口仅影响**标量访存路径**的L2 Cache默认策略，不会修改向量搬运类接口（如[asc_copy_gm2ub_align](../vector_data_move/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)、[asc_copy_ub2gm_align](../vector_data_move/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)、[asc_ndim_copy_gm2ub](../vector_data_move/asc_ndim_copy_gm2ub.md)等）参数中的`l2_cache_mode`；二者相互独立。
- load与store策略分属CTRL不同比特域，设置其一不会覆盖另一侧已配置的值。
- 本接口采用读-改-写方式更新CTRL，不会破坏CTRL上其他无关比特位。

## 调用示例

场景：算子启动时分别配置标量读、写路径的L2 Cache默认策略。标量路径用于读取GM上的tiling元数据；向量主路径通过[asc_copy_gm2ub_align](../vector_data_move/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)搬运tile数据，二者相互独立。

```cpp
constexpr uint32_t tile_elems = 1024;
__gm__ int32_t* tiling_gm;   // GM上的tiling元数据
__gm__ half* src_gm;
__ubuf__ half* dst_ub;

// 标量读：tiling元数据会被多次访问，启用L2复用
asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
// 标量写：写GM控制字段时避免额外L2分配
asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NOTALLOC_CLEAN);

int32_t tile_count = tiling_gm[0];  // 标量读GM元数据
for (int32_t tile = 0; tile < tile_count; ++tile) {
    // 向量块搬运走MTE，l2_cache_mode由asc_copy_gm2ub_align参数单独控制
    asc_copy_gm2ub_align(dst_ub, src_gm + tile * tile_elems,
        tile_elems * sizeof(half));
    // ... 向量计算 ...
}
```
