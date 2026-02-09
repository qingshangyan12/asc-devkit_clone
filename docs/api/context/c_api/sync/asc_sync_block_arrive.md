# asc_sync_block_arrive

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    √     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    √     |

## 功能说明

该指令用于发送同步信息数据到FFTS，设置同步点。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, uint64_t config)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 表示同步点在哪个pipeline中，当该pipeline前面所有指令执行完毕后到达同步点。|
| config | 输入 | 包含了同步mode和同步ID，格式：0x1 \| (mode << 4) \| (flagID << 8)。mode范围为[0, 2]，flagID范围为[0,15]。|


## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
uint64_t config = 0x101;
asc_sync_block_arrive(PIPE_S, config);
```