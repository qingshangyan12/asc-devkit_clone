# 输出 shape 依赖计算

> 类型：扩展

某些算子（如 NonZero）在计算完成前无法确定输出 shape，需要在 kernel 计算完成后将实际输出 shape 写入出参。这类算子在原型定义时需要使用 `OutputShapeDependOnCompute()` 标识，并在 kernel 中按约定格式写入 shape 信息。

## 参数约定

在核函数参数列表中，所有**正常输出参数之后、workspace 和 tiling 参数之前**，增加一个 `GM_ADDR` 类型的输出参数（通常命名为 `shape_out`），用于存放输出 shape 信息。参数位置示例：

```cpp
extern "C" __global__ __aicore__ void xxx_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                                  GM_ADDR shape_out,      // shape 输出参数
                                                  GM_ADDR workspace,
                                                  GM_ADDR tiling);
```

## shape 信息排布格式

shape 信息以 `uint64_t` 数组的形式写入，排布规则如下：

- **总大小**：n × 9 个 `uint64_t` 元素（n 为需要刷新 shape 的输出个数）
- **每个输出占 9 个元素**：第 1 个元素存放维度数（dim），后续 8 个元素存放各维度的 shape 值（未使用的维度留空）

![shape信息排布示意图](../../../../../../figures/流水任务运行示意图-33.png)

> [!NOTE] 说明
> - 多个输出的顺序与原型定义中 Output 的声明顺序一致。
> - 当输出 tensor 的数据类型为 `uint64_t` 时，需要将维度数的 `uint32_t` 高位设置为 1（即 `0x0000000080000000 | dim`），表示以 `uint64_t` 类型解析该 tensor。

## 示例：单个输出（uint32\_t 类型）

输出 tensor 类型为 `uint32_t`，计算完成后 shape 为 `[32, 64]`（2 维）：

```cpp
extern "C" __global__ __aicore__ void xxx_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                                  GM_ADDR shape_out,
                                                  GM_ADDR workspace, GM_ADDR tiling)
{
    // 算子主计算逻辑（此处省略，聚焦 shape 信息写入）
    constexpr uint32_t SHAPEOUT_SIZE = 9;  // 每个输出占 9 个 uint64_t 元素
    GlobalTensor<uint64_t> shapeoutGlobal;
    shapeoutGlobal.SetGlobalBuffer((__gm__ uint64_t*)shape_out, SHAPEOUT_SIZE);  // 绑定 GM 地址
    shapeoutGlobal.SetValue(0, 2);    // 在 index=0 处写入维度数 = 2
    shapeoutGlobal.SetValue(1, 32);   // 在 index=1 处写入 dim[0] = 32
    shapeoutGlobal.SetValue(2, 64);   // 在 index=2 处写入 dim[1] = 64
    // index[3]~index[8] 为占位，无需设置
}
```

## 示例：单个输出（uint64\_t 类型）

输出 tensor 类型为 `uint64_t`，计算完成后 shape 为 `[1, 64, 128, 128]`（4 维）：

```cpp
extern "C" __global__ __aicore__ void xxx_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                                  GM_ADDR shape_out,
                                                  GM_ADDR workspace, GM_ADDR tiling)
{
    // 算子主计算逻辑（此处省略，聚焦 shape 信息写入）
    constexpr uint32_t SHAPEOUT_SIZE = 9;
    GlobalTensor<uint64_t> shapeoutGlobal;
    shapeoutGlobal.SetGlobalBuffer((__gm__ uint64_t*)shape_out, SHAPEOUT_SIZE);
    shapeoutGlobal.SetValue(0, 0x0000000080000000 | 4);  // uint64_t 类型标记 + 维度数 = 4
    shapeoutGlobal.SetValue(1, 1);     // dim[0] = 1
    shapeoutGlobal.SetValue(2, 64);    // dim[1] = 64
    shapeoutGlobal.SetValue(3, 128);   // dim[2] = 128
    shapeoutGlobal.SetValue(4, 128);   // dim[3] = 128
    // index[5]~index[8] 为占位，无需设置
}
```

## 示例：多个输出

两个输出，tensor 类型均为 `uint64_t`，shape 分别为 `[16, 32]` 和 `[1, 16, 16, 32]`：

```cpp
extern "C" __global__ __aicore__ void xxx_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z,
                                                  GM_ADDR shape_out,
                                                  GM_ADDR workspace, GM_ADDR tiling)
{
    // 算子主计算逻辑（此处省略，聚焦 shape 信息写入）
    // 两个输出，各占 9 个 uint64_t，共 18 个
    constexpr uint32_t SHAPEOUT_SIZE = 18;
    GlobalTensor<uint64_t> shapeoutGlobal;
    shapeoutGlobal.SetGlobalBuffer((__gm__ uint64_t*)shape_out, SHAPEOUT_SIZE);

    // 第一个输出：shape = [16, 32]，2 维
    shapeoutGlobal.SetValue(0, 0x0000000080000000 | 2);
    shapeoutGlobal.SetValue(1, 16);
    shapeoutGlobal.SetValue(2, 32);
    // index[3]~index[8] 为占位

    // 第二个输出：shape = [1, 16, 16, 32]，4 维
    shapeoutGlobal.SetValue(9, 0x0000000080000000 | 4);
    shapeoutGlobal.SetValue(10, 1);
    shapeoutGlobal.SetValue(11, 16);
    shapeoutGlobal.SetValue(12, 16);
    shapeoutGlobal.SetValue(13, 32);
    // index[14]~index[17] 为占位，无需设置
}
```

## 相关文档

- [基本流程](./基本流程.md) — Kernel 函数格式、TilingData 处理和信息获取
