# Bank冲突优化<a name="ZH-CN_TOPIC_0000002543422600"></a>

避免Bank冲突的方法有两种：优化计算逻辑和优化地址分配。

## 优化计算逻辑

对shape为`(8, 16, 16)`的输入执行`(1, 0, 2)`的transpose操作，输出shape为`(16, 8, 16)`。通过将计算逻辑由“跳读、连续写”调整为“连续读、跳写”，可避免冲突。

**原始实现**

**图 1**  跳读连续写示意图

![](../../../../figures/skip_read_continue_write.png)

上图展示了“跳读、连续写”的计算逻辑，示例代码如下。该逻辑会导致同一Repeat内输入的8个DataBlock位于同一个bank group，从而发生读读冲突。

```cpp
uint64_t mask = 128;
UnaryRepeatParams params;
params.dstBlkStride = 1;
params.srcBlkStride = 16;
for (uint32_t i = 0; i < 16; i++) {
    AscendC::Adds(dstLocal[i * 128], srcLocal[i * 16], 0, mask, 1, params);
}
```

**优化实现**

**图 2**  连续读跳写示意图

![](../../../../figures/continue_read_skip_write.png)

上图展示了“连续读、跳写”的计算逻辑，示例代码如下。优化后，同一个Repeat内输入的8个DataBlock不再位于同一个bank group内，可避免读读冲突。

```cpp
uint64_t mask = 128;
UnaryRepeatParams params;
params.dstBlkStride = 8;
params.srcBlkStride = 1;
for (uint32_t i = 0; i < 8; i++) {
    AscendC::Adds(dstLocal[i * 16], srcLocal[i * 256], 0, mask, 2, params);
}
```

## 优化地址分配

以连续4096个`float`元素的加法`z = x + y`为例，通过在分配内存时适当扩大申请空间，保证在一个Repeat内，x和y不会同时位于同一个bank group，且x或y不会与z同时位于同一个bank。完整样例可参考[避免Bank冲突优化样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/04_best_practices/01_bank_conflict/bank_conflict/README.md)。

**原始实现**

**图 3**  地址分配优化前示意图

![](../../../../figures/address_allocation_original.png)

不做地址优化时，直接使用`InitBuffer`分配内存，各个Tensor的地址分别为：

- x：起始地址为`0x0`，tensor长度为`4096 * sizeof(float)`字节。
- y：起始地址为`0x4000`，tensor长度为`4096 * sizeof(float)`字节。
- z：起始地址为`0x8000`，tensor长度为`4096 * sizeof(float)`字节。

在一个Repeat内，x和y会同时读取同一个bank group，且x或y会与z同时读写同一个bank。代码示例如下：

```cpp
pipe.InitBuffer(inQueueX, 1, 4096 * sizeof(float));
pipe.InitBuffer(inQueueY, 1, 4096 * sizeof(float));
pipe.InitBuffer(outQueueZ, 1, 4096 * sizeof(float));
```

**优化实现**

**图 4**  地址分配优化后示意图

![](../../../../figures/address_allocation_optimized.png)

优化地址时，使用`InitBuffer`分配内存，并适当扩大内存申请，各个Tensor的地址分别为：

- x：起始地址为`0x0`，tensor长度为`4096 * sizeof(float) + 256`字节。
- y：起始地址为`0x4100`，tensor长度为`64 * 1024 - (4096 * sizeof(float) + 256)`字节。
- z：起始地址为`0x10000`，tensor长度为`4096 * sizeof(float)`字节。

x多申请256字节，避免一个Repeat内x和y同时读取同一个bank group；y多申请空间，确保z不会与x或y落入同一个bank。代码示例如下：

```cpp
pipe.InitBuffer(inQueueX, 1, 4096 * sizeof(float) + 256); // 多申请256字节
// 多申请空间，确保z不会与x或y落入同一个bank。
// 64 * 1024是16个bank的空间，4096 * sizeof(float) + 256是x所占的空间。
pipe.InitBuffer(inQueueY, 1, 64 * 1024 - (4096 * sizeof(float) + 256));
pipe.InitBuffer(outQueueZ, 1, 4096 * sizeof(float));
```
