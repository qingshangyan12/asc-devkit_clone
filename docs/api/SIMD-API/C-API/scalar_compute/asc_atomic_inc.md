# asc_atomic_inc

## 产品支持情况

|产品   | 是否支持 |
| ------------|:----:|
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|

## 功能说明

对Global Memory中address指向的计数器执行原子递增操作，如果address上的数值大于等于指定数值val，则对address赋值为0，否则将address上数值加1。

## 函数原型

```cpp
__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t *address, uint32_t val)

__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t *address, uint64_t val)
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

- 在开启编译器自动同步功能的前提下，编译器能够自动在PIPE_MTE2/PIPE_MTE3与PIPE_S之间插入同步。但是，asc_atomic_inc为标量计算，在读写GM时如果与搬运单元（MTE2/MTE3）存在数据依赖，编译器却无法自动插入同步，开发者需要根据实际情况手动插入同步。
- Scalar原子操作会绕过DCache，需要调用[asc_dcci](../cache_ctrl/asc_dcci.md)接口确保GM与DCache的一致性。
## 调用示例

```c++
uint32_t counter = 0;      // 初始值
asc_dcci_entire_all();            // 手动同步
uint32_t old = asc_atomic_inc(&counter, 9);  // counter = 1, old = 0
```
