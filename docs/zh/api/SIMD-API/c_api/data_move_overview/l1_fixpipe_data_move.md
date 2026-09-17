# L1到Fixpipe Buffer数据搬运

Fixpipe Buffer保存L0C搬出时使用的随路Vector量化参数和随路ReLU参数。DAV_3510通过[asc_copy_l12fb](../cube_datamove/asc_copy_l12fb/asc_copy_l12fb_arch_3510.md)从L1装载参数，接口运行在PIPE_FIX，仅在AIC侧生效。

## 搬运模式

- 前n个数据搬运通过`size`搬运一段连续参数。
- 高维切分搬运通过`n_burst`、`len_burst`、`src_gap_size`和`dst_gap_size`描述多个数据块。
- 目的地址的`dst[19:16]`字段用于区分随路量化参数空间和随路ReLU参数空间，两类地址空间独立。

## 关键约束

- L1源地址需要32字节对齐。
- 随路量化参数目的地址需要128字节对齐，容量上限为4 KB；随路ReLU参数目的地址需要64字节对齐，容量上限为2 KB。
- 随路量化场景的部分长度和间隔参数必须为2的倍数。
- 装载参数与L0C结果搬出都使用PIPE_FIX。参数在对应L0C搬出指令消费前必须准备完成，不得在仍被使用时覆盖。
