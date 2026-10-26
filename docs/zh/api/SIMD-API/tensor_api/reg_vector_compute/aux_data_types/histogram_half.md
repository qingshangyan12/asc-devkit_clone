# histogram_half

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/histogram_compute.h"`。

`histogram_half`用于指定[histograms](../histogram_compute/histograms.md)统计的输入值范围。

## 类型定义

```cpp
enum class histogram_half {
    low,
    high,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| low | 统计输入值`[0, 127]`。 |
| high | 统计输入值`[128, 255]`。 |
