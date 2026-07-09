# docs明确逻辑位置与物理位置混用清单

## 扫描口径

- 只统计明确混用：同一通路中裸逻辑位置和裸物理位置直接组合，例如`CO1->UB`。
- 若上一层已给出物理通路、下一层只是补充说明，例如`Unified Buffer -> L1 Buffer`下面的`UB -> C1`，不列为问题。
- 已排除带映射注释的写法，包括`L0C Buffer(CO1)`、`CO1(L0C)`、`CO1\(L0C\)`、`UB（VECIN）`。
- `GM/Global Memory`不作为问题触发条件，因为它既可作为逻辑位置，也可作为物理位置语义使用。
- 已过滤HTML注释标签，避免把`<!-- ... -->`误识别为通路箭头。

## 明确混用：10处

| 位置 | 命中内容 |
|---|---|
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过FP-Buffer存放量化参数实现高效随路量化.md:20` | - 将矩阵乘的结果从CO1搬运到workspace上； |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过FP-Buffer存放量化参数实现高效随路量化.md:25` | 相比于正确示例多增加了CO1-\>workspace、workspace-\>UB的搬运过程和量化的vector计算。 |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过FP-Buffer存放量化参数实现高效随路量化.md:135` | // 将矩阵乘的计算结果从CO1搬运到workspace |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过BT-Buffer实现高效的bias计算.md:22` | 当循环n次该计算过程，则分别增加了n次CO1-\>workspace、workspace-\>UB的搬运。 |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过BT-Buffer实现高效的bias计算.md:131` | // 将矩阵乘的计算结果从CO1搬运到workspace |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md:17` | - 将前一次矩阵乘的计算结果从CO1搬运到workspace上，再从workspace搬运到UB上； |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md:21` | 当需要累加n次矩阵乘时，分别增加了n次CO1-\>workspace、workspace-\>UB搬运以及n次Add运算。 |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md:136` | // 将矩阵乘的计算结果从CO1搬运到workspace |
| `docs/guide/算子实践参考/SIMD算子性能优化/矩阵计算/通过L0C-Buffer数据暂存实现高效的矩阵乘结果累加.md:173` | // 将矩阵乘的计算结果从CO1搬运到workspace |
| `docs/api/SIMD-API/基础API/资源管理/Pipe和Que框架/TSCM/TSCM简介.md:79` | auto scmTensor = tscm.AllocTensor<float>(); // 在搬运数据从UB->TSCM前分配Buffer |
