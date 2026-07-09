# asc_sync_intra_arrive

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：block内同步指令，与[asc_sync_intra_wait](asc_sync_intra_wait.md)配合使用，是block内不同Core的流水线同步指令，用于向每个Core的同步寄存器发送同步信号。每个sync_id对应Vector Core与Cube Core中的一个初始值为0的计数器。asc_sync_intra_arrive指令后其他Core对应的计数器增加1；执行asc_sync_intra_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。
- 实现原理/公式：对应CrossCoreSetFlag的模式4——单个AI Core内AIC与单个AIV之间的同步（block内不同Core），arrive对应SetFlag。该接口仅Ascend 950PR/Ascend 950DT支持，与模式4仅950支持一致。

## 函数原型

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint8_t sync_id)
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint64_t sync_id)
```

<!-- end id2 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id3 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| sync_id | 输入 | 事件标号。 |

<!-- end id3 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：占用核间同步计数器资源，每个sync_id对应Vector Core与Cube Core中的一个计数器。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与sync_id参数取值有关。

## 约束说明

- 需要保证每一个ID的计数器不会溢出，否则会出现异常。
- 需要保证相同的ID只会被一个流水线控制。
- 每个ID对应一个计数器，特定CORE的同步ID计数器可通过其他CORE发起的asc_sync_intra_arrive递增；也可通过本身发起的asc_sync_intra_wait递减。
- 三个CORE的ID映射规则如下：
  - ID为0~15的VECCORE0的asc_sync_intra_arrive指令会被映射到CUBECORE的0~15的ID。
  - ID为0~15的VECCORE1的asc_sync_intra_arrive指令会被映射到CUBECORE的16~31的ID。
  - ID为0~15的CUBECORE的asc_sync_intra_arrive指令会被映射到VECCORE0的0~15的ID。
  - ID为16~31的CUBECORE的asc_sync_intra_arrive指令会被映射到VECCORE1的0~15的ID。

## 调用示例

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
//设置同步指令事件标号为1
uint64_t sync_id = 1;
asc_sync_intra_arrive(PIPE_S, sync_id);
asc_sync_intra_wait(PIPE_S, sync_id);
```

<!-- end id4 -->
