# asc_lock

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

头文件路径：`"c_api/sync/sync.h"`。

该接口用于AI Core内部异步流水线之间的同步，可按需阻塞指定流水线的执行。

## 函数原型

```cpp
enum ascMutexExecuteMode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id)
__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id, const ascMutexExecuteMode mode)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 指定当前接口所属的流水类型。|
| mutex_id | 输入 | mutex标号，取值范围为[0, 31]。|
| mode | 输入 | 可选参数，用于指定是否阻塞流水线，默认值为ASC_LOCK_BLOCK。<br>&bull; ASC_LOCK_BLOCK：阻塞pipe对应流水线的执行，直到代码中位于当前asc_lock之前且mutex_id相同的所有asc_unlock调用均已执行完成。<br>&bull; ASC_LOCK_NON_BLOCK：不阻塞pipe对应流水线的执行。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- asc_lock与asc_unlock必须严格成对使用，并使用相同的mutex_id与mode。此外，对应的asc_unlock必须始终写在asc_lock之后，否则硬件行为无定义。
- 对于mutex_id相同的asc_lock与asc_unlock组合，无论pipe和mode是否相同，都不得在代码中嵌套使用，否则硬件行为将不可预测。
- 成对使用的asc_lock和asc_unlock必须配置相同的mode，否则硬件行为将不可预测。
- 如果代码中连续出现多个pipe和mutex_id均相同的调用，则后一个asc_lock不会再阻塞流水线运行。若需要表达同一流水线内的依赖关系，必须使用接口asc_sync_pipe。

## 调用示例

```cpp
__simd_vf__ inline void add_vf(__ubuf__ half* src0Addr, __ubuf__ half* src1Addr, __ubuf__ half* dstAddr,
                               uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src0, src1, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0Addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1Addr, one_repeat_size);
        asc_add(dst, src0, src1, mask);
        asc_storealign_postupdate(dstAddr, dst, one_repeat_size, mask);
    }
}

__aicore__ inline void add_kernel(__gm__ half* x_gm, __gm__ half* y_gm, __gm__ half* z_gm)
{
    __ubuf__ half x_local[256];
    __ubuf__ half y_local[256];
    __ubuf__ half z_local[256];
    uint8_t mutex_id = 1;

    // 1. PIPE_MTE2流水线：数据从GM搬入UB
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2ub_align(x_local, x_gm, 256 * sizeof(half));
    asc_copy_gm2ub_align(y_local, y_gm, 256 * sizeof(half));
    asc_unlock(PIPE_MTE2, mutex_id);

    // 2. PIPE_V流水线：矢量计算
    // BLOCK模式下（默认），asc_lock阻塞PIPE_V执行，直到程序顺序中之前所有相同mutex_id的asc_unlock执行完成
    asc_lock(PIPE_V, mutex_id);
    uint32_t count = 256;
    add_vf(x_local, y_local, z_local, count, 128, 2);
    asc_unlock(PIPE_V, mutex_id);

    // 3. PIPE_MTE3流水线：数据从UB搬出至GM
    // asc_lock阻塞PIPE_MTE3执行，直到PIPE_V的asc_unlock执行完成
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_ub2gm_align(z_gm, z_local, 256 * sizeof(half));
    asc_unlock(PIPE_MTE3, mutex_id);
}
```

上述示例的流水线时序如下，三条流水线严格串行执行：

```
MTE2: |==搬入==|
V:              |==计算==|
MTE3:                   |==搬出==|
```

### 多重循环场景：双 buffer 流水线

当多轮循环中需要相邻轮次的 MTE2（搬入）与 MTE3（搬出）并行执行时，可使用**两个 mutex_id 交替**配合**双 buffer**，实现软件流水线。同一 mutex_id 仅在间隔一轮（buffer 复用）时才产生等待，从而重叠相邻轮次的搬运与计算。

```cpp
__simd_vf__ inline void add_vf(__ubuf__ half* src0Addr, __ubuf__ half* src1Addr, __ubuf__ half* dstAddr,
                               uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half src0, src1, dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign_postupdate(src0, src0Addr, one_repeat_size);
        asc_loadalign_postupdate(src1, src1Addr, one_repeat_size);
        asc_add(dst, src0, src1, mask);
        asc_storealign_postupdate(dstAddr, dst, one_repeat_size, mask);
    }
}

__aicore__ inline void add_kernel(__gm__ half* x_gm, __gm__ half* y_gm, __gm__ half* z_gm)
{
    // 双 buffer：buf0 和 buf1 交替使用
    __ubuf__ half x_buf[2][256];
    __ubuf__ half y_buf[2][256];
    __ubuf__ half z_buf[2][256];

    for (uint32_t i = 0; i < TILE_NUM; i++) {
        // 交替使用 mutex_id 0 和 1
        uint8_t mutex_id = i % 2;
        uint32_t offset = i * 256;

        // 1. PIPE_MTE2流水线：数据从GM搬入UB buf[mutex_id]
        // 当 i >= 2 时，asc_lock阻塞，等待 i-2 轮的 PIPE_MTE3 asc_unlock完成（buffer复用约束）
        asc_lock(PIPE_MTE2, mutex_id);
        asc_copy_gm2ub_align(x_buf[mutex_id], x_gm + offset, 256 * sizeof(half));
        asc_copy_gm2ub_align(y_buf[mutex_id], y_gm + offset, 256 * sizeof(half));
        asc_unlock(PIPE_MTE2, mutex_id);

        // 2. PIPE_V流水线：矢量计算
        // asc_lock阻塞，等待本轮 PIPE_MTE2 的 asc_unlock 完成
        asc_lock(PIPE_V, mutex_id);
        uint32_t count = 256;
        add_vf(x_buf[mutex_id], y_buf[mutex_id], z_buf[mutex_id], count, 128, 2);
        asc_unlock(PIPE_V, mutex_id);

        // 3. PIPE_MTE3流水线：数据从UB搬出至GM
        // asc_lock阻塞，等待本轮 PIPE_V 的 asc_unlock 完成
        // 本轮 MTE3 可与下一轮（mutex_id不同）的 MTE2 并行执行
        asc_lock(PIPE_MTE3, mutex_id);
        asc_copy_ub2gm_align(z_gm + offset, z_buf[mutex_id], 256 * sizeof(half));
        asc_unlock(PIPE_MTE3, mutex_id);
    }
}
```

上述示例的流水线时序如下，相邻轮次的 MTE2 与 MTE3 可并行执行：

```
MTE2: |==id0==|        |==id1==|        |==id0==|        |==id1==|
V:          |==id0==|        |==id1==|        |==id0==|        |==id1==|
MTE3:            |==id0==|        |==id1==|        |==id0==|        |==id1==|
```
