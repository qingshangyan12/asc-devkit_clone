# L1到BiasTable数据搬运

BiasTable Buffer保存Mmad使用的bias。DAV_3510通过[asc_copy_l12bt](../cube_datamove/asc_copy_l12bt/asc_copy_l12bt_arch_3510.md)将L1中的偏置数据搬入BiasTable，接口运行在PIPE_MTE1，仅在AIC侧生效。

## 搬运模式

- 连续模式通过`size`搬运一段连续数据。
- 高维切分模式通过`n_burst`、`len_burst`、`src_gap`和`dst_gap`描述多个数据块。
- 源类型支持`half`、`bfloat16_t`、`int32_t`和`float`。`bfloat16_t`会转换为`float`；`half`可通过`conv_control`选择是否转换为`float`。

## 关键约束

- L1源地址需要32字节对齐，BiasTable目的地址需要64字节对齐。
- BiasTable容量上限为4 KB，L1容量上限为512 KB，源、目的访问均不得越界。
- `float`或`int32_t`场景对搬运长度和目的间隔还有偶数约束。
- L1数据由前序流水产生时，应在`asc_copy_l12bt`前完成同步；BiasTable写入完成后，Mmad读取bias前也需要建立PIPE_MTE1到PIPE_M的依赖。
