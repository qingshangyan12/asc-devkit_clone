# asc_get_scalar_store_cache_mode

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

读取当前标量写（store）路径的L2 Cache管理策略，并返回[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)枚举值。

常用于临时切换策略前保存原配置，或在设置后做回读校验。

## 函数原型

```cpp
__aicore__ inline asc_store_l2_cache_mode asc_get_scalar_store_cache_mode()
```

## 参数说明

无

## 返回值说明

当前标量store路径的L2 Cache管理策略，类型为[asc_store_l2_cache_mode](../enum/asc_store_l2_cache_mode.md)。

## 流水类型

PIPE_S

## 约束说明

- 本接口仅反映标量store侧策略，与标量load侧以及向量搬运接口参数中的`l2_cache_mode`无关。
- 若编码不属于已定义枚举值，接口回退返回`asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM`。

## 调用示例

场景：将UB上一段int32数据通过标量路径逐元素写回GM。先查询并保存当前store策略，再切换为不allocate模式后完成UB到GM的标量搬运，最后恢复原策略。

```cpp
constexpr int32_t data_len = 16;
__gm__ int32_t* dst_gm;
__ubuf__ int32_t* src_ub;

// 查询当前标量store策略
asc_store_l2_cache_mode old_mode = asc_get_scalar_store_cache_mode();
// 切换为NOTALLOC_CLEAN，后续标量写GM按该策略访问L2
asc_set_scalar_cache_mode(asc_store_l2_cache_mode::NOTALLOC_CLEAN);

// 标量GM写：将src_ub中的数据逐元素搬运到dst_gm
for (int32_t i = 0; i < data_len; ++i) {
    dst_gm[i] = src_ub[i];
}

// 恢复原store策略
asc_set_scalar_cache_mode(old_mode);
```
