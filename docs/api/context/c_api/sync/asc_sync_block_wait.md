# asc_sync_block_wait

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| <term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> |    √     |
| <term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> |    √     |

## 功能说明

和[asc_sync_block_arrive](asc_sync_block_arrive.md)配合使用（通过flagID关联），用于等待所有同步对象到达flagID对应的同步点。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_wait(int64_t flagID)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| flagID | 输入 | 事件标号，取值范围[0,15]。|


## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t flagID = 1;
asc_sync_block_wait(flagID);  
```