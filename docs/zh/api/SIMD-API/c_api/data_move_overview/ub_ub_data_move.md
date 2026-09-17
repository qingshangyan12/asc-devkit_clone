# UB与UB数据搬运

DAV_3510通过[asc_copy_ub2ub](../vector_datamove/asc_copy_ub2ub.md)在UB内部复制数据。该接口运行在PIPE_V，仅在AIV侧生效；共用接口[asc_copy](../vector_datamove/asc_copy.md)不支持Ascend 950PR/Ascend 950DT。

## 搬运模式

- 连续模式按`size`指定的字节数复制。
- 高维切分模式通过`n_burst`、`len_burst`、`src_gap`和`dst_gap`描述多个DataBlock，其中长度和间隔以32字节DataBlock为单位。

## 关键约束

- 源、目的起始地址均需要32字节对齐。
- 实际参与搬运的源、目的DataBlock不得重叠。即使两段地址的整体跨度交叠，也必须保证每个有效搬运块互不重叠。
- 源、目的地址及搬运跨度不得超过实际分配的UB范围。
- 后续PIPE_MTE3或其他流水读取目的数据前，需要建立从PIPE_V到消费者流水的同步关系。
