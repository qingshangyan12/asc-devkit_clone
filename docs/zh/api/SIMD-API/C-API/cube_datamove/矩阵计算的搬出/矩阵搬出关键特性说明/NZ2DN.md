# NZ2DN

NZ2DN用于在支持的产品上将L0C Buffer中的NZ分形排布转换为DN排布，典型场景是将卷积结果转换为NCHW格式后搬出到GM。

该能力由3510架构[asc_copy_l0c2gm](../../asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)重载中的`enable_nz2dn`及相关矩阵尺寸、stride参数控制。

## 主要约束

- `enable_nz2dn`只能用于对应架构文档明确列出的重载。
- N、M方向有效大小、源NZ stride、目的DN stride和多矩阵间隔均按函数原型中的参数定义配置。
- 源、目的地址及矩阵间隔必须满足对齐和非重叠约束。
- 与量化、ReLU、Channel Split等能力组合时，应使用单接口文档支持的参数组合。

其他架构不应使用3510架构的NZ2DN参数或示例。
