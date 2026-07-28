# asc_unlock

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

用于AI Core内部异步通道同步的指令，用于释放由asc_lock指令申请的缓存。

## 函数原型

```cpp
enum ascMutexExecuteMode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id)
__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id, const ascMutexExecuteMode mode)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| mutex_id | 输入 | buffer标号。取值范围[0, 31]。|
| mode | 输入 | 可选参数，用于指定是否阻塞流水线，默认值为ASC_LOCK_BLOCK。<br>&bull; ASC_LOCK_BLOCK：该指令等待pipe所对应的流水线中所有前置指令完成后执行。<br>&bull; ASC_LOCK_NON_BLOCK：该指令等待pipe所对应的流水线中所有前置指令完成且相同mutex_id的所有asc_unlock指令执行完成后执行。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 必须与asc_lock搭配使用，且需要使用相同的mutex_id与mode。此外，asc_unlock必须始终插在对应的asc_lock指令之后，否则会出现未定义的硬件行为。
- 具有相同mutex_id的asc_lock与asc_unlock组合，无论pipe与mode是否相同，均不得在编程顺序中嵌套，否则硬件行为将不可预测。
- 对于程序顺序中连续出现的，具有相同pipe与mutex_id的指令对，后一个asc_lock将不再阻塞流水线运行，若需实现同一流水线的依赖关系，则必须使用指令asc_sync_pipe。

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
    // 数据搬入完成后释放mutex_id，使后续相同mutex_id的asc_lock不再阻塞
    asc_unlock(PIPE_MTE2, mutex_id);

    // 2. PIPE_V流水线：矢量计算（asc_lock阻塞，等待上方PIPE_MTE2的asc_unlock执行完成）
    asc_lock(PIPE_V, mutex_id);
    uint32_t count = 256;
    add_vf(x_local, y_local, z_local, count, 128, 2);
    // 计算完成后释放mutex_id，使后续PIPE_MTE3的asc_lock不再阻塞
    asc_unlock(PIPE_V, mutex_id);

    // 3. PIPE_MTE3流水线：数据从UB搬出至GM
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

### 多重循环场景：双 buffer 流水线优化

当多轮循环中需要相邻轮次的 MTE2（搬入）与 MTE3（搬出）并行执行时，可使用**两个 mutex_id 交替**配合**双 buffer**。`asc_unlock` 释放 mutex_id 后，下一轮使用不同 mutex_id 的 `asc_lock` 不会阻塞，从而允许相邻轮次的搬运与计算重叠执行。

#### 优化前：单 mutex_id 串行（所有 tile 共用一个 mutex_id）

所有 tile 使用相同 mutex_id，`asc_unlock(PIPE_MTE3)` 释放后，下一轮的 `asc_lock(PIPE_MTE2)` 才解除阻塞，三阶段严格串行。

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

__aicore__ inline void add_kernel_serial(__gm__ half* x_gm, __gm__ half* y_gm, __gm__ half* z_gm)
{
    __ubuf__ half x_local[256];
    __ubuf__ half y_local[256];
    __ubuf__ half z_local[256];
    uint8_t mutex_id = 0;

    for (uint32_t i = 0; i < TILE_NUM; i++) {
        uint32_t offset = i * 256;

        // 下一轮 MTE2 必须等待上一轮 MTE3 完成后释放 mutex_id（相同 mutex_id）
        asc_lock(PIPE_MTE2, mutex_id);
        asc_copy_gm2ub_align(x_local, x_gm + offset, 256 * sizeof(half));
        asc_copy_gm2ub_align(y_local, y_gm + offset, 256 * sizeof(half));
        // 释放 mutex_id，使本轮 PIPE_V 的 asc_lock 不再阻塞
        asc_unlock(PIPE_MTE2, mutex_id);

        asc_lock(PIPE_V, mutex_id);
        uint32_t count = 256;
        add_vf(x_local, y_local, z_local, count, 128, 2);
        // 释放 mutex_id，使本轮 PIPE_MTE3 的 asc_lock 不再阻塞
        asc_unlock(PIPE_V, mutex_id);

        asc_lock(PIPE_MTE3, mutex_id);
        asc_copy_ub2gm_align(z_gm + offset, z_local, 256 * sizeof(half));
        // 释放 mutex_id，使下一轮 PIPE_MTE2 的 asc_lock 不再阻塞
        asc_unlock(PIPE_MTE3, mutex_id);
    }
}
```

优化前的流水线时序，各轮次之间严格串行：

```
MTE2: |==id0==|                |==id0==|                |==id0==|                |==id0==|
V:          |==id0==|                |==id0==|                |==id0==|                |==id0==|
MTE3:            |==id0==|                |==id0==|                |==id0==|                |==id0==|
```

#### 优化后：双 mutex_id 交替 + 双 buffer

使用两个 mutex_id 交替配合双 buffer，`asc_unlock` 释放 mutex_id 后，下一轮使用不同 mutex_id 的 `asc_lock` 不会阻塞，从而实现相邻轮次的搬运与计算重叠执行。

```cpp
__aicore__ inline void add_kernel_pingpong(__gm__ half* x_gm, __gm__ half* y_gm, __gm__ half* z_gm)
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
        asc_lock(PIPE_MTE2, mutex_id);
        asc_copy_gm2ub_align(x_buf[mutex_id], x_gm + offset, 256 * sizeof(half));
        asc_copy_gm2ub_align(y_buf[mutex_id], y_gm + offset, 256 * sizeof(half));
        // 释放 mutex_id，使本轮 PIPE_V 的 asc_lock 不再阻塞
        asc_unlock(PIPE_MTE2, mutex_id);

        // 2. PIPE_V流水线：矢量计算
        asc_lock(PIPE_V, mutex_id);
        uint32_t count = 256;
        add_vf(x_buf[mutex_id], y_buf[mutex_id], z_buf[mutex_id], count, 128, 2);
        // 释放 mutex_id，使本轮 PIPE_MTE3 的 asc_lock 不再阻塞
        asc_unlock(PIPE_V, mutex_id);

        // 3. PIPE_MTE3流水线：数据从UB搬出至GM
        // 释放 mutex_id 后，下一轮使用不同 mutex_id 的 asc_lock 不会阻塞
        // 因此下一轮的 MTE2 可与本轮 MTE3 并行执行
        asc_lock(PIPE_MTE3, mutex_id);
        asc_copy_ub2gm_align(z_gm + offset, z_buf[mutex_id], 256 * sizeof(half));
        asc_unlock(PIPE_MTE3, mutex_id);
    }
}
```

优化后的流水线时序，相邻轮次的 MTE2 与 MTE3 并行执行：

```
MTE2: |==id0==|        |==id1==|        |==id0==|        |==id1==|
V:          |==id0==|        |==id1==|        |==id0==|        |==id1==|
MTE3:            |==id0==|        |==id1==|        |==id0==|        |==id1==|
```
