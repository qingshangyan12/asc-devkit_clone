# GM与L1 Buffer或L0 Buffer数据搬运

## GM->L1 Buffer

| 场景 | 接口 | 说明 |
| --- | --- | --- |
| 连续、分块或矩阵搬运 | [asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md) | 2201和3510架构的重载不同。 |
| ND->NZ格式转换 | [asc_copy_gm2l1_nd2nz](../asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md) | 使用专用格式转换接口，不能写成普通`asc_copy_gm2l1`。 |
| DN->NZ格式转换 | [asc_copy_gm2l1_dn2nz](../asc_copy_gm2l1_dn2nz.md) | 仅在单接口文档列出的产品上使用。 |
| 非对齐搬运 | [asc_copy_gm2l1_align](../asc_copy_gm2l1_align.md) | 支持的Padding、stride及Cache参数以完整原型为准。 |

## GM->L0A Buffer或L0B Buffer

- [asc_copy_gm2l0a](../asc_copy_gm2l0a.md)：GM->L0A Buffer。
- [asc_copy_gm2l0b](../asc_copy_gm2l0b.md)：GM->L0B Buffer。

若目标产品不支持GM直接搬运到L0A Buffer或L0B Buffer，应拆分为GM->L1 Buffer和L1 Buffer->L0A Buffer/L0B Buffer两步，具体以单接口产品支持情况为准。

## L1 Buffer->GM

[asc_copy_l12gm](../asc_copy_l12gm.md)用于其单接口文档所列产品上的L1 Buffer->GM搬运。不能将`asc_copy_l12gm`与`asc_copy_gm2l1`视为同一个双向接口。

## 参数说明

这些C API直接使用标量参数和带地址空间限定符的指针。不同架构的重载差异较大，概览不复制函数原型，调用前应进入对应单接口文档核对参数顺序、单位和约束。
