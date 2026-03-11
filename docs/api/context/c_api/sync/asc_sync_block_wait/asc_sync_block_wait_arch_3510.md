# asc_sync_block_wait

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

FFTS块/子块同步指令，每个flagID均分配有计数器，若该flagID被FFTS设置，计数器将递增；若执行了相同标志位的asc_sync_block_wait指令，计数器将递减。当计数器为0时，所有指令将被阻塞，直到计数器变为非0。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的管道。|
| flagID | 输入 | 事件标号，取值范围[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- flagID的取值范围为0至15，每个flagID的计数器范围为0至15
- 必须保证每个flagID的四位计数器不溢出，否则将引发异常
- 必须保证相同的ID在同一时间仅被一条管道等待

## 调用示例

```cpp
int64_t flagID = 1;
asc_sync_block_arrive(PIPE_S, flagID);
asc_sync_block_wait(flagID);  
```
