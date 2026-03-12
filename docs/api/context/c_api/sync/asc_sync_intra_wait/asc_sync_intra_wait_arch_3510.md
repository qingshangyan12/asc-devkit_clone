# asc_sync_intra_wait

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明

block内同步指令，与[asc_sync_intra_arrive](../asc_sync_intra_arrive/asc_sync_intra_arrive_arch_3510.md)配合使用，是混合模式的block中不同内核的流水线同步指令。对于一个混合模式的block，指令将分发至一个CUBECORE与一个VECCORE，CUBECORE拥有32个ID，VECCORE拥有16个ID。asc_sync_intra_arrive将使其他CORE对应的计数器加一，且不阻塞同一流水线的后续指令。asc_sync_intra_wait会在ID计数器为0时阻塞流水线，直至ID被其他核心设置，若计数器非0，则减少计数器的数值。

## 函数原型

```cpp
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint8_t sync_id)
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint64_t sync_id)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| sync_id | 输入 | 事件标号。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需要保证每一个ID的4位计数器不会溢出，否则会出现异常。
- 需要保证相同的ID只会被一个流水线控制。
- 每个ID对应一个4位计数器，特定CORE的同步ID计数器可通过其他CORE发起的asc_sync_intra_arrive递增；也可通过本身发起的asc_sync_intra_wait递减。 
- 三个CORE的ID映射规则如下：
<br>&bull; 1：ID为0~15的VECC0RE0的asc_sync_intra_arrive指令会被映射到CUBECORE的0~15的ID。 
<br>&bull; 2：ID为0~15的VECC0RE1的asc_sync_intra_arrive指令会被映射到CUBECORE的16~31的ID。 
<br>&bull; 3：ID为0~15的CUBECORE的asc_sync_intra_arrive指令会被映射到VECC0RE0的0~15的ID。 
<br>&bull; 4：ID为16~31的CUBECORE的asc_sync_intra_arrive指令会被映射到VECC0RE0的0~15的ID。 

## 调用示例

```cpp
int64_t sync_id = 1;
asc_sync_intra_arrive(PIPE_S, sync_id);
asc_sync_intra_wait(sync_id);  
```
