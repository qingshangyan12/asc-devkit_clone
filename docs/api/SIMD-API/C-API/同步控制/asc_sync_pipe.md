# asc_sync_pipe

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| <cann-filter npu_type="950"><term>Ascend 950PR/Ascend 950DT</term>  | √ </cann-filter>|
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

阻塞相同流水，具有数据依赖的相同流水之间需要插入此同步。

## 函数原型

```cpp
__aicore__ inline void asc_sync_pipe(pipe_t pipe)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| pipe | 输入 | 阻塞的流水类别。如果不关注流水类别，希望阻塞所有流水，可以传入PIPE_ALL。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

Scalar流水之间的同步由硬件自动保证，调用asc_sync_pipe(PIPE_S)可能引发硬件错误。

## 调用示例

```cpp
// total_length指参与计算的数据总长度。
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
__ubuf__ float dst0[total_length];
__ubuf__ float dst1[total_length];

// asc_mul指令的输入dst0是asc_add指令的输出，两个矢量运算指令产生依赖，需要插入asc_sync_pipe保证两条指令的执行顺序。
asc_add(dst0, src0, src1, total_length);
asc_sync_pipe(PIPE_V);
asc_mul(dst1, dst0, src1, total_length);
```
