# A. 术语表

| 术语 | 英文 | 说明 |
|------|------|------|
| 分形布局 | Fractal Layout | NPU 特有的数据组织方式，将矩阵切分为固定大小的小块进行存储 |
| C0 | C0 Dimension | NPU 硬件数据总线宽度决定的维度大小，通常为 32 字节宽，换算成元素数为 32/sizeof(T) |
| NZL布局 | NZ Layout | 16×C0 分形块的行优先排列，Cube 计算 A 矩阵的标准格式 |
| ZN 布局 | ZN Layout | C0×16 分形块的列优先排列，Cube 计算 B 矩阵的标准格式 |
| ND 布局 | ND Layout | 行优先连续存储（Row-Major），与传统 C 数组排列一致 |
| DN 布局 | DN Layout | 列优先连续存储（Column-Major） |
| GM | Global Memory | 片外主存 (HBM/DDR)，容量最大但延迟最高 |
| L1 | L1 Buffer / Cbuffer | 片内中转缓存，Cube 数据通路的入口 |
| UB | Unified Buffer | 片内通用缓冲区，Vector 指令的操作数来源 |
| L0A | L0A Buffer | Cube 矩阵计算单元的 A 矩阵输入缓存 |
| L0B | L0B Buffer | Cube 矩阵计算单元的 B 矩阵输入缓存 |
| L0C | L0C Buffer | Cube 矩阵计算单元的累加器/输出缓存 |
| BIAS | BIAS Buffer | Cube 矩阵计算的 Bias 输入缓存 |
| FIXBUF | Fix Buffer | Fixpipe 专用的修正缓冲区 |
| Fixpipe | Fixpipe | L0C → GM 的直接写回通路，支持量化/ReLU 等后处理 |
| MMAD | Matrix Multiply-Add | 矩阵乘加运算 C = A × B + Bias |
| Layout Pattern | 布局模式标记 | 嵌入在 Layout 类型中的编译期标签，标识数据的分形格式 |
| TraitType | 特征类型 | 嵌入 Layout 的编译期配置，携带元素类型 T 和 C0 大小 |
| ViewEngine | 视图引擎 | 封装 NPU 指针并提供偏移和缓存控制能力 |
| Atom | 原子操作 | 封装具体硬件操作和配置的类型级抽象 |
| Crd2Idx | Coordinate to Index | 将多维坐标转换为线性地址的算法 |
| Tiling | 分块/切片 | 将大矩阵切分为小块的策略，使数据适配有限的片内存储 |
| Stride | 步长 | 相邻元素在内存中的地址间距 |
| Capacity | 容量 | Shape × Stride 的最大值，代表内存区域所需的最小空间 |
| Cosize | Co-size | 以 Stride 为单位的元素索引范围 |
