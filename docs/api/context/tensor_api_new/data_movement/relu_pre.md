# 随路Relu

随路 Relu 是矩阵计算结果从 L0C 搬出到 GM 或 UB 过程中的随路激活能力。Tensor API 通过 `CopyL0C2GMTrait::enableRelu` 或 `CopyL0C2UBTrait::enableRelu` 控制是否使能该能力。

## 功能说明

`enableRelu` 默认值为 `false`，表示不使能随路 Relu。设置为 `true` 时，搬出过程中对输出数据执行 Normal Relu 激活。

```cpp
constexpr CopyL0C2GMTrait l0c2gmTrait = {
    RoundMode::DEFAULT,
    true,
    false
};

struct CopyL0C2GMTraitCustom {
    using TraitType = CopyL0C2GMTrait;
    static constexpr const TraitType value = l0c2gmTrait;
};
```

L0C 到 UB 搬运同样通过 `CopyL0C2UBTrait::enableRelu` 控制：

```cpp
constexpr CopyL0C2UBTrait l0c2ubTrait = {
    RoundMode::DEFAULT,
    true,
    false,
    DUAL_DST_DISABLE
};
```

## 支持范围

参考底层 Fixpipe 能力，随路 Relu 包含 Normal Relu、Leaky Relu 和 PRelu 等模式。当前 Tensor API 的 L0C 到 GM/UB 搬运通过 `enableRelu` 暴露 Normal Relu 能力；当前不支持随路 PRelu。

## 约束说明

- `enableRelu` 为编译期 trait 配置，默认不使能。
- 量化和 Relu 参数不能为 INF、NaN 或非规格化数。
- 当前 Tensor API 不支持随路 PRelu。
- 使能随路 Relu 时，目的地址写入区域不能重叠。若目的地址存在重叠写入，硬件不保证写入顺序。
