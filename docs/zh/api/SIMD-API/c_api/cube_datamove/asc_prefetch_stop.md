# asc_prefetch_stop

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

将没有执行的Prefetch指令删除，用于在预加载流程发生变化时清理尚未生效的预加载指令,Cube核与Vector核均支持。

本接口需要与[asc_prefetch_gm2l2cache](asc_prefetch_gm2l2cache.md)、[asc_prefetch_gm2l2cache_dn2nz](asc_prefetch_gm2l2cache_dn2nz.md)、[asc_prefetch_gm2l2cache_nd2nz](asc_prefetch_gm2l2cache_nd2nz.md)等预取接口配合使用。

## 函数原型

```c
__aicore__ inline void asc_prefetch_stop()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 仅删除尚未执行的Prefetch指令，已在执行中的Prefetch指令不受影响。

## 调用示例

```c
// 设置源操作数，A矩阵按128×128的half类型分片（tile）在GM中连续存放
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
constexpr uint32_t tileNum = 4;
__gm__ half* a;

// 预加载过程中的配置：每个tile按行预加载，
// 每行为1个连续传输数据块（128×2=256字节），共128个数据块
uint32_t burstNum = M;                       // 数据块个数：128
uint32_t burstLen = K * sizeof(half);        // 每个数据块长度：256字节
uint64_t burstSrcStride = K * sizeof(half);  // 相邻数据块间隔：256字节
bool needStop = false;                       // 迭代终止条件，由实际计算逻辑决定

// 分tile迭代处理：每轮先预加载下一轮tile，使预加载与当前轮计算重叠
for (uint32_t tile = 0; tile < tileNum; ++tile) {
    // 当前轮：从GM搬入第tile个分片并执行计算，
    // 访问的GM地址可命中L2 Cache中已预加载的数据。

    // 预加载下一轮tile数据（若存在）
    if (tile + 1 < tileNum) {
        asc_prefetch_gm2l2cache(a + (tile + 1) * M * K, burstNum, burstLen, burstSrcStride);
    }

    // 若预加载流程发生变化（如满足终止条件提前退出迭代），
    // 已下发的下一轮tile预加载指令不再需要
    if (needStop) {
        // 删除尚未执行（未生效）的Prefetch指令，
        // 避免无效预加载占用搬运带宽和L2 Cache空间；
        // 已执行完成的预加载数据不受影响，仍保留在L2 Cache中
        asc_prefetch_stop();
        break;
    }
}
```
