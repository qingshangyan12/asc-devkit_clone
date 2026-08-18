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

场景：先将GM上一段int32数据通过标量路径逐元素读到UB，再将UB中的结果逐元素写回GM。调用前分别配置标量读、写的L2 Cache策略。

```cpp
constexpr int32_t data_len = 16;
__ubuf__ int32_t* tmp_ub;
__gm__ int32_t* src_gm;
__gm__ int32_t* dst_gm;

// 配置标量读策略，后续标量读GM按该策略访问L2
asc_set_scalar_cache_mode(asc_load_l2_cache_mode::NORMAL_LAST_VICTIM);
// 标量GM读：将src_gm中的数据逐元素搬运到tmp_ub
for (int32_t i = 0; i < data_len; ++i) {
    tmp_ub[i] = src_gm[i];
}

// 配置标量写策略，后续标量写GM按该策略访问L2
asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NOTALLOC_CLEAN);
// 标量GM写：将tmp_ub中的数据逐元素搬运到dst_gm
for (int32_t i = 0; i < data_len; ++i) {
    dst_gm[i] = tmp_ub[i];
}
```
