# asc_mmad_int8 测试问题记录

## 现象
```
A[0:2]=-3 -2 -1  B[0:2]=-2 -1 0
output[0:2]=-1 -1 -11
golden[0:2]=-1 9 -1
[FAIL] asc_mmad_int8 max_diff=17
```

## 分析
- int8 类型 L0A 分形为 16×32 (M×K)，L0B 为 32×16 (K×N)
- MMAD 的 K=16 只使用了分形的一半 K 维度
- GM→L1→L0 搬运参数正确（k_step=1=32B=32int8）
- C[0][0] 正确 (-1 匹配 golden)，但后续元素错误
- 推测: int8 分形内部数据布局与预期不同，MMAD 读取了错误的 K 位置

## 尝试的修复
- k_step: 8→1 (修正为正确的 K-step)
- FRACTAL_K=32 作为数据步长
- 缓冲区放大 2 倍确保对齐
- 均未解决输出错误问题

## 建议
- int8 分形内部格式需要进一步确认
- 可能需要参考 CANN 中的 int8 MMAD 示例
