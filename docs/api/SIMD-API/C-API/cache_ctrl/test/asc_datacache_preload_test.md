# asc_datacache_preload

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->

## 功能说明

- 接口声明文件：`"c_api/cache_ctrl/cache_ctrl.h"`
- 核心功能：从源地址所在的特定GM地址预加载数据到Data Cache中，每次调用只能预加载一个Cache Line大小的数据。
- 实现原理/公式：将指定GM地址的数据预取到Data Cache中，后续Scalar单元访问该地址时可直接从Cache读取，减少访问延迟。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)
```

<!-- end id6 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| address | 输入 | 源操作数的起始地址，类型为__gm__ uint64_t*。 |
| offset | 输入 | 在源操作数上偏移offset大小开始加载数据，单位为Bytes。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| address | 输入 | 源操作数的起始地址，类型为__gm__ uint64_t*。 |
| offset | 输入 | 在源操作数上偏移offset大小开始加载数据，单位为Bytes。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| address | 输入 | 源操作数的起始地址，类型为__gm__ uint64_t*。 |
| offset | 输入 | 在源操作数上偏移offset大小开始加载数据，单位为Bytes。 |

<!-- end id9 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 频繁调用此接口可能会导致保留站（Reservation Station）拥塞，此时该接口将被视为NOP指令，阻塞Scalar流水线。因此不建议频繁调用此接口，建议连续调用次数不要超过4次。

## 调用示例

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
int64_t cacheOffset = 0;
asc_datacache_preload(reinterpret_cast<__gm__ uint64_t*>(x_gm), cacheOffset);
```

- 完整样例链接：`<TODO>`

<!-- end id10 -->
<!-- npu="A3" id11 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
int64_t cacheOffset = 0;
asc_datacache_preload(reinterpret_cast<__gm__ uint64_t*>(x_gm), cacheOffset);
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
<!-- npu="910b" id12 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 假设x为外部传入的地址，每个核计算16个数据，block_idx为内置变量，通过block_idx计算偏移实现多核间数据隔离
__gm__ float* x_gm = x + block_idx * 16;
int64_t cacheOffset = 0;
asc_datacache_preload(reinterpret_cast<__gm__ uint64_t*>(x_gm), cacheOffset);
```

- 完整样例链接：`<TODO>`

<!-- end id12 -->
