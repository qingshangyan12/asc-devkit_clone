# asc_lock

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT | √    |

## 功能说明

用于AI Core内部异步流水线同步的指令，用于阻塞指定流水线的执行。

## 函数原型

```cpp
enum ascMutexExecuteMode {
    ASC_LOCK_BLOCK = 0,
    ASC_LOCK_NON_BLOCK = 1
};

__aicore__ inline void asc_lock(pipe_t pipe, uint8_t mutex_id, ascMutexExecuteMode mode)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。|
| mutex_id | 输入 | mutex标号。取值范围[0, 31]。|
| mode | 输入 | 可选，是否阻塞流水线，默认值为ASC_LOCK_BLOCK。<br>&bull; ASC_LOCK_BLOCK：该指令阻塞pipe所对应的流水线的执行，直到程序顺序中，相同mutex_id的所有前序asc_unlock指令执行完成。<br>&bull; ASC_LOCK_NON_BLOCK：该指令不阻塞pipe流水线的执行。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- asc_lock与asc_unlock必须按严格的顺序成对使用，且需要使用相同的mutex_id与mode。此外，asc_unlock必须始终插在对应的asc_lock指令之后，否则硬件行为无定义。
- 具有相同mutex_id的asc_lock与asc_unlock组合，无论pipe和mode是否相同，均不得在编程顺序中嵌套，否则硬件行为将不可预测。
- 成对的asc_lock和asc_unlock必须使用相同的mode，否则硬件行为将不可预测。
- 对于程序顺序中连续出现的，具有相同pipe与mutex_id的指令对，后一个asc_lock将不再阻塞流水线运行，若需实现同一流水线的依赖关系，则必须使用指令asc_sync_pipe。

## 调用示例

```cpp
uint8_t mutex_id = 1;
asc_lock(PIPE_S, mutex_id);
asc_unlock(PIPE_S, mutex_id);
```
