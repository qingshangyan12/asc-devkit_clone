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
- 核心功能：从指令所在DDR地址预加载数据到对应的cacheline中。开发者手动调用该接口，能够从指令所在GM地址预加载指令到ICache中。
- 实现原理/公式：将指定地址的指令数据预取到ICache中，后续执行该地址指令时可直接从ICache读取，减少指令访问延迟。

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

**表 1**  参数说明

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| addr | 输入 | 预加载数据的地址。 |
| prefetch_len | 输入 | 预加载数据的长度，单位为2K Byte，取值需满足 preFetchLen < ICache大小 / 2K。其中，AIC和AIV的ICache大小分别为32KB和16KB。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| addr | 输入 | 预加载数据的地址。 |
| prefetch_len | 输入 | 预加载数据的长度，单位为2K Byte，取值需满足 preFetchLen < ICache大小 / 2K。其中，AIC和AIV的ICache大小分别为32KB和16KB。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| addr | 输入 | 预加载数据的地址。 |
| prefetch_len | 输入 | 预加载数据的长度，单位为2K Byte，取值需满足 preFetchLen < ICache大小 / 2K。其中，AIC和AIV的ICache大小分别为32KB和16KB。 |

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

- prefetch_len参数单位为2K Byte，取值需满足 preFetchLen < ICache大小 / 2K。其中，AIC和AIV的ICache大小分别为32KB和16KB。

## 调用示例

<!-- npu="950" id10 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 预取指令长度为32（单位为2K Byte）。
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id10 -->
<!-- npu="A3" id11 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 预取指令长度为32（单位为2K Byte）。
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id11 -->
<!-- npu="910b" id12 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 预取指令长度为32（单位为2K Byte）。
int64_t prefetch_length = 32;
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), prefetch_length);
```

- 完整样例链接：`<TODO>`

<!-- end id12 -->
