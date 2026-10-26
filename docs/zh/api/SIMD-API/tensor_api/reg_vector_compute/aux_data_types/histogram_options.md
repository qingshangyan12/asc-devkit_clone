# histogram_options

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/histogram_compute.h"`。

`histogram_options`用于集中配置[histograms](../histogram_compute/histograms.md)的统计范围和统计方式。

## 类型定义

```cpp
struct histogram_options {
    histogram_half half = histogram_half::low;
    histogram_mode mode = histogram_mode::frequency;
};
```

| 成员 | 描述 |
| --- | --- |
| `half` | 统计范围，详情参见[histogram_half](histogram_half.md)。 |
| `mode` | 统计方式，详情参见[histogram_mode](histogram_mode.md)。 |

`histogram_options`需要定义为编译期常量，并作为`histograms`的模板参数传入。

