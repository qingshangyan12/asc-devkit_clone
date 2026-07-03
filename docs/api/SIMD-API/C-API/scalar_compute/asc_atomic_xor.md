# asc\_atomic\_xor

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|

## 功能说明

对Global Memory中的数据与指定数据执行原子异或操作，即将val按位异或到address指针指向的Global Memory地址中的元素上。

## 函数原型

```cpp
__aicore__ inline int32_t asc_atomic_xor(__gm__ int32_t *address, int32_t val)

__aicore__ inline uint32_t asc_atomic_xor(__gm__ uint32_t *address, uint32_t val)

__aicore__ inline int64_t asc_atomic_xor(__gm__ int64_t *address, int64_t val)

__aicore__ inline uint64_t asc_atomic_xor(__gm__ uint64_t *address, uint64_t val)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| address | 输出 | Global Memory的地址。 |
| val | 输入 | 源操作数。 |

## 返回值说明

address地址中计算前的原始数据。

## 流水类型

PIPE_S

## 约束说明

- 在开启编译器自动同步功能的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步。但是，asc_atomic_xor为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。
- Scalar原子操作会绕过DCache，需要调用[asc_dcci](../cache_ctrl/asc_dcci.md)接口确保GM与DCache的一致性。
## 调用示例

```c++
uint32_t parity = 0x0;     // 初始值
asc_dcci_entire_all();            // 手动同步
uint32_t old = asc_atomic_xor(&parity, 0x1);  // parity = 0x1, old = 0x0
```
