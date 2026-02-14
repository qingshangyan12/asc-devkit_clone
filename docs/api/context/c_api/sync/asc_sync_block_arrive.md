# asc_sync_block_arrive

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

和[asc_sync_block_wait](asc_sync_block_wait.md)配合使用（通过flagID关联），用于向核间同步寄存器发送同步信号，标识当前pipeline已到达flagID对应的同步点。

## 函数原型

```cpp
 __aicore__ inline void asc_sync_block_arrive(pipe_t pipe, uint8_t mode, int64_t flagID)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 表示同步点在哪个pipeline中，当该pipeline前面所有指令执行完毕后到达同步点。|
| mode | 输入 | 硬同步模式，用于指定同步的行为类型。共有三种硬同步模式：<br/>mode 0：所有用到的AIC直接进行同步或所有AIV间进行同步。适用场景为所有核分别计算中间结果后，由一个核进行reduce运算。<br/>mode 1：同一个Group下的AIV间同步。<br/>mode 2：同一个Group下的AIC和AIV间同步。多用于AIC/AIV分别负责算子执行中的不同子流程（有严格顺序要求）。取值范围为[0, 2]。|
| flagID | 输入 | 事件标号，用于标识同一组同步信号。取值范围范围为[0,15]。|


## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
uint8_t mode = 0;
int64_t flagID = 1;
asc_sync_block_arrive(PIPE_S, mode, flagID);
```