# histogram_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/histogram_compute.h"`。

`histogram_mode`用于指定[histograms](../histogram_compute/histograms.md)的统计方式。

## 类型定义

```cpp
enum class histogram_mode {
    frequency,
    cumulative,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `frequency` | 每个输出元素表示对应输入值出现的次数。 |
| `cumulative` | 每个输出元素表示当前值及之前各值的累计次数。 |

