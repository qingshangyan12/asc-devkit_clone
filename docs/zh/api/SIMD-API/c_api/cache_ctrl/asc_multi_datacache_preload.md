# asc_multi_datacache_preload

## 产品支持情况

- Ascend 960DT系列产品：支持。

## 功能说明

头文件路径为：`"c_api/cache_ctrl/cache_ctrl.h"`。

从Global Memory的指定地址开始，将连续多个Cache Line预加载到DCache中，为后续Scalar单元读取提前填充缓存。每个Cache Line大小为64字节。

预取起始地址为`address + offset`，`offset`的单位为字节。与[asc_datacache_preload](asc_datacache_preload.md)相比，本接口可通过`cacheline_num`指定连续预取的Cache Line数量。

## 函数原型

```cpp
__aicore__ inline void asc_multi_datacache_preload(__gm__ uint64_t* address, int64_t offset, uint32_t cacheline_num)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| address | 输入 | 源操作数在Global Memory中的地址。 |
| offset | 输入 | 相对`address`的有符号字节偏移。预取起始地址为`address + offset`。 |
| cacheline_num | 输入 | 连续预取的Cache Line数量，取值范围为[1, 65536]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 仅支持Ascend 960DT系列产品。
- 连续预取的数据范围为从预取起始地址开始的`cacheline_num * 64`字节。
- 本接口用于提升后续Scalar单元读取Global Memory的缓存命中率，不建立同步或缓存一致性关系。
- 频繁调用本接口可能导致保留站（Reservation Station，用于暂存待执行指令的硬件队列）发生拥塞。在此情况下，该指令将被视为NOP指令，进而阻塞Scalar流水线。因此，建议连续调用次数不要超过4次。

## 调用示例

```cpp
__global__ __aicore__ void DataCachePreloadKernel(__gm__ uint64_t* data)
{
    constexpr uint32_t cachelineNum = 4;
    asc_multi_datacache_preload(data, 0, cachelineNum);
    uint64_t value = data[0];
    (void)value;
}
```
