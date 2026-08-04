# F32 Channel Split

F32 Channel Split用于在矩阵结果搬出时，将`float`类型的16×16分形拆分为两个16×8小分形。

该能力由[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm.md)对应架构重载中的Channel Split参数控制。2201和3510架构的参数名称、组合能力及约束不同，应从接口汇总页进入对应架构文档核对。

## 约束

- 输入和输出数据类型均为`float`。
- 输出保持NZ排布，且不能与不兼容的NZ2ND、NZ2DN或unit flag组合使用。
- 目标为L1 Buffer或UB时是否支持该能力，以对应搬出接口文档为准。
- N方向有效元素和小分形数量必须满足目标架构的Channel Split约束。

完整参数和调用示例参见[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm.md)。
