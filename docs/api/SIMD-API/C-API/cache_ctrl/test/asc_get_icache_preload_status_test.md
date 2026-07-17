# asc_get_icache_preload_status

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
- 核心功能：获取ICache的Preload的状态。该接口为调试接口，在[asc_icache_preload](./asc_icache_preload.md)后调用，用于获取ICache的PreLoad的状态。
- 实现原理/公式：读取ICache预加载状态寄存器，返回当前预加载状态。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline int64_t asc_get_icache_preload_status()
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline int64_t asc_get_icache_preload_status()
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline int64_t asc_get_icache_preload_status()
```

<!-- end id6 -->

## 参数说明

无

## 返回值说明

int64_t类型，0表示ICache的PreLoad已完成（空闲），1表示ICache的PreLoad未完成（忙）。

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 该接口为调试接口，建议在[asc_icache_preload](./asc_icache_preload.md)后调用，用于确认预加载是否完成。

## 调用示例

<!-- npu="950" id7 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 获取ICache预加载状态，0表示已完成（空闲），1表示未完成（忙）。
int64_t status = asc_get_icache_preload_status();
```

- 完整样例链接：`<TODO>`

<!-- end id7 -->
<!-- npu="A3" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 获取ICache预加载状态，0表示已完成（空闲），1表示未完成（忙）。
int64_t status = asc_get_icache_preload_status();
```

- 完整样例链接：`<TODO>`

<!-- end id8 -->
<!-- npu="910b" id9 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 获取ICache预加载状态，0表示已完成（空闲），1表示未完成（忙）。
int64_t status = asc_get_icache_preload_status();
```

- 完整样例链接：`<TODO>`

<!-- end id9 -->
