# asc_icache_preload

## 产品支持情况

| 产品 | 是否支持  |
| :-----------| :------: |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    √     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    √     |

## 功能说明

从指令所在DDR地址预加载数据到对应的cacheline中。

## 函数原型

```cpp
__aicore__ inline void asc_icache_preload(const void *addr);

__aicore__ inline void asc_icache_preload(const void *addr, int16_t prefetch_len);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| addr | 输入| 加载数据的地址。 |
| offset | 输入 | 加载数据的长度。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
// total_length 指参与计算的数据长度
constexpr uint64_t total_length = 32;
uint64_t src[total_length];
int16_t len = 16; 
asc_icache_preload(src, len);
```
