# asc_store_dev

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

不经过DCache向GM地址上写数据。
当多核操作GM地址时，如果数据无法对齐到Cache Line，经过DCache的方式下，由于按照Cache Line大小进行读写，会导致多核数据随机覆盖的问题。此时，可以采用不经过DCache直接读写GM地址的方式，从而避免上述随机覆盖的问题。

## 函数原型

```cpp
__aicore__ inline void asc_store_dev(__gm__ int8_t* addr, int8_t value)

__aicore__ inline void asc_store_dev(__gm__ uint8_t* addr, uint8_t value)

__aicore__ inline void asc_store_dev(__gm__ int16_t* addr, int16_t value)

__aicore__ inline void asc_store_dev(__gm__ uint16_t* addr, uint16_t value)

__aicore__ inline void asc_store_dev(__gm__ int32_t* addr, int32_t value)

__aicore__ inline void asc_store_dev(__gm__ uint32_t* addr, uint32_t value)

__aicore__ inline void asc_store_dev(__gm__ int64_t* addr, int64_t value)

__aicore__ inline void asc_store_dev(__gm__ uint64_t* addr, uint64_t value)
```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
|------------|------------|-----------|
| addr     | 输出     | 目标GM地址。|
| value     | 输入     | 待写入目标的数据。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- `addr`起始地址须按写入dtype字节宽度对齐，未对齐触发异常。
- `addr`须落在GM可访问地址空间内；若地址无法被访存分类逻辑识别，可能被兜底归入其他地址空间，导致写入到非预期区域，软件须保证地址合法。
- 若需写入多个相邻地址且彼此之间存在数据依赖，须在两次写入之间插入必要的同步指令，保证写入顺序，防止出现异常数据。
- 本接口运行在标量流水上，与后续依赖该写入结果的指令之间存在标量数据依赖；如后续有读取同一GM地址的指令，须通过上述同步指令建立依赖顺序，标量流水本身的顺序执行不保证跨指令访存可见性。
- 本接口访问GM时绕过DCache，不维护缓存一致性。若其他核或其他通路通过缓存访问同一GM地址，调用方需使用[asc_dcci](../cache_ctrl/asc_dcci.md)清理或失效对应Cache Line，并使用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)保证相关访存操作的执行顺序和数据可见性。详情可参考[Scalar原子操作与DCache一致性](../../../../guide/编程指南/高级编程/内存模型/缓存一致性.md#scalar原子操作与dcache一致性)。

## 调用示例

```cpp
// addr是外部输入的GM地址，value是待写入GM内存的数据，类型为 int32_t
int32_t value = 2;
asc_store_dev(addr, value);
```
