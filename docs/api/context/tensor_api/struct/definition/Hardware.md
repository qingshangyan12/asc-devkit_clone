# Hardware

## 功能说明

Hardware枚举用于定义硬件位置，表示数据存储的物理位置。

## 枚举定义

```cpp
enum class Hardware : uint8_t {
    GM,      // Global Memory
    UB,      // Unified Buffer
    L1,      // L1 Buffer
    L0A,     // L0A Buffer
    L0B,     // L0B Buffer
    L0C,     // L0C Buffer
    BIAS,    // Bias Table Buffer
    FIXBUF,  // Fixpipe Buffer
    MAX      // 最大值，用于边界检查
};
```

## 枚举值说明

| 枚举值 | 描述 |
|--------|------|
| GM | Global Memory，全局内存 |
| UB | Unified Buffer，统一缓冲区 |
| L1 | L1 Buffer，一级缓存 |
| L0A | L0A Buffer，L0A缓冲区，用于矩阵乘法的左矩阵 |
| L0B | L0B Buffer，L0B缓冲区，用于矩阵乘法的右矩阵 |
| L0C | L0C Buffer，L0C缓冲区，用于矩阵乘法的结果 |
| BIAS | Bias Table Buffer，偏置表缓冲区 |
| FIXBUF | Fixpipe Buffer，固定管道缓冲区 |
| MAX | 最大值，用于边界检查 |

## 约束说明

- Hardware枚举值必须对应有效的硬件位置
- 不同硬件位置有不同的对齐要求和容量限制

## 调用示例

```cpp
// 使用Hardware枚举
auto gmLayout = AscendC::MakeTensorTrait<half, TPosition::GM>(layout);
auto ubLayout = AscendC::MakeTensorTrait<half, TPosition::UB>(layout);
```