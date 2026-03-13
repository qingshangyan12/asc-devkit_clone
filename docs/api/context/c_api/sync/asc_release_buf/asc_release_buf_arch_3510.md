# asc_release_buf
## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

用于AICore内部异步通道同步的指令，与asc_get_buf搭配使用。

## 函数原型

```cpp
__aicore__ inline void asc_release_buf(pipe_t pipe, uint8_t buf_id, bool mode)
__aicore__ inline void asc_release_buf(pipe_t pipe, uint64_t buf_id, bool mode)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| buf_id | 输入 | buffer标号。|
| mode | 输入 | 执行模式。当mode = false时，该指令在pipe指示的管道中所有前置指令完成后执行；当mode = true时，该指令在pipe指示的管道中所有前置指令完成且具有相同buf_id的所有asc_release_buf指令执行完成后执行。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 必须与asc_get_buf搭配使用，且需按相同buf_id与mode使用，即asc_release_buf在编程顺序中必须始终插在成对的asc_get_buf之后，否则硬件行为无定义。
- 具有相同buf_id的asc_get_buf与asc_release_buf配对，无论pipe与mode是否相同，均不得在编程顺序中嵌套。否则硬件行为将不可预测。
- 对于程序顺序中连续出现的，具有相同pipe与buf_id的指令对，后一个asc_get_buf将不再阻塞流水线运行，若需实现同一流水线的依赖关系，则必须使用指令asc_sync_data_barrier。

## 调用示例

```cpp
int64_t buf_id = 1;
asc_get_buf(PIPE_S, buf_id, false);  
asc_release_buf(PIPE_S, buf_id, false);  
```
