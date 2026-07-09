# asc_icache_preload

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
- 核心功能：从指令所在GM地址预加载指令到ICache中。
- 实现原理/公式：ICache（Instruction Cache）用于缓存Scalar单元最近使用或频繁使用的指令。通过Scalar单元读取GM指令时，优先从ICache中读取指令；当ICache未命中时，再从L2 Cache或GM中加载。asc_icache_preload接口允许软件主动将指定GM地址的指令预加载到ICache中，减少ICache未命中的延迟，提升后续指令读取速度。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)
```

<!-- end id6 -->

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| addr | 输入 | 预加载数据的地址。 |
| prefetch_len | 输入 | 预加载数据的长度。单位为2K Byte，取值应小于ICache的大小/2K。AIC和AIV的ICache大小分别为32KB和16KB。 |

## 数据类型支持

该接口不涉及数据类型选择，操作对象为内存地址。

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

无

## 调用示例

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id9 -->
