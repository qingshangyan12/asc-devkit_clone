# asc\_atomic\_exch

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|

## 功能说明

对Global Memory地址做原子赋值操作，即将指定数据赋值到Global Memory地址中。

## 函数原型

```c++
inline float asc_atomic_exch(float *address, float val)
```

```c++
inline int32_t asc_atomic_exch(int32_t *address, int32_t val)
```

```c++
inline uint32_t asc_atomic_exch(uint32_t *address, uint32_t val)
```

```c++
inline int64_t asc_atomic_exch(int64_t *address, int64_t val)
```

```c++
inline uint64_t asc_atomic_exch(uint64_t *address, uint64_t val)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数。 |

## 返回值说明

Global Memory上的初始数据。

## 流水类型

PIPE_S

## 约束说明

在开启编译器自动同步功能（参见[自动同步使用约束说明](../同步控制/核内同步/关键特性说明.md#自动同步使用约束说明)）的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步（参见[编译器自动同步](../同步控制/核内同步/关键特性说明.md#tpipe-tque框架编程范式和编译器自动同步支持情况)）。但是，asc_atomic_exch为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。

## 调用示例

```c++
__gm__ int32_t dst = 10;          // 初始值
asc_dcci_entire_all();            // 手动同步
int32_t old = asc_atomic_exch(&dst, 20);  // dst = 20, old = 10
```
