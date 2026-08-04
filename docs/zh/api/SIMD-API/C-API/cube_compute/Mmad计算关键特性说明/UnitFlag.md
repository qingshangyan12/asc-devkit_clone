# UnitFlag<a name="ZH-CN_TOPIC_00000025690709788"></a>

**特性说明：**

unitFlag的核心功能体现为：`asc_mmad`和`asc_copy_l0c2gm`接口引入了单元标志（unit-flag）机制，通过以内存块为粒度实现精细化的数据同步，从而有效降低同步延迟，提升系统整体性能。当UnitFlag开关打开后，对于L0C Buffer中的每个内存块（512B），提供一个单元标志位，用于指示该块是否可读或可写。

`asc_mmad`和`asc_copy_l0c2gm`接口设置unitFlag值为2/3后，系统会启动单元标志位。

当`unit_flag` == 2（0b10），开启unitFlag功能，在硬件执行完指令后，不改变单元标志位；

- 对于写操作（`asc_mmad`接口），如果单元标志位0，则硬件直接写入L0C Buffer；否则，如果单元标志位为1，则写操作会等待直到单元标志变为0；执行完成后将单元标志位保持为0；
- 对于读操作（`asc_copy_l0c2gm`接口），如果单元标志位1，则硬件直接读取L0C Buffer；否则，如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位保持为1；

当`unit_flag` == 3（0b11），开启unitFlag功能，在硬件执行完指令后，改变单元标志位；

- 对于写操作（`asc_mmad`接口），如果单元标志位0，则硬件直接写入L0C Buffer；否则，如果单元标志位为1，则写操作会等待直到单元标志变为0；执行完成后将单元标志位设置成1；
- 对于读操作（`asc_copy_l0c2gm`接口），如果单元标志位1，则硬件直接读取L0C Buffer；否则，如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位设置成0；

根据上述特性，如果用户在进行A矩阵\[128, 1024\]、B矩阵为\[1024, 128\]的矩阵乘计算时，需要沿着K轴进行迭代循环，假设每次迭代K长度为128，则需要迭代8次，此时8次`asc_mmad`对应1次`asc_copy_l0c2gm`操作；

- 前7次`asc_mmad`的unitFlag都设置成2，写入后将单元标记位始终为0，保证后续`asc_mmad`可以写入L0C Buffer；
- 最后1次`asc_mmad`设置成3，写入后将单元标志位设置成1，保证`asc_copy_l0c2gm`可以读取L0C Buffer；
- `asc_copy_l0c2gm`的unitFlag设置为3，读取后将单元标记位设置为0，保证后续`asc_mmad`接口可以顺利写入L0C Buffer数据；

如果用户需要单次`asc_mmad`的结果分多次搬出时，譬如`asc_mmad`计算结果的L0C Buffer为M\(128\) x N\(256\)，沿N轴分两次搬出；这样一次`asc_mmad`会对应两次`asc_copy_l0c2gm`；

- `asc_mmad`的时候需要设置unitFlag = 3，保证`asc_copy_l0c2gm`可以读取L0C Buffer数据；
- 每一次`asc_copy_l0c2gm`的unitFlag都设置为3，读取后将单元标记位设置为0，保证后续其他`asc_mmad`接口在复用这块L0C Buffer地址时可以顺利写入数据；

当开启unitFlag后，`asc_mmad`和`asc_copy_l0c2gm`会对同一块分形的L0C Buffer进行读写操作，因此`asc_mmad`计算和`asc_copy_l0c2gm`保持一致的读写顺序，有助于获得更优的性能表现。

在调用`asc_mmad`时，需要通过[asc_set_mmad_direction_m](../asc_set_mmad_direction_m.md)或[asc_set_mmad_direction_n](../asc_set_mmad_direction_n.md)设置计算方向。当`asc_copy_l0c2gm`开启NZ2ND或Channel Merge等布局变换时，需调用`asc_set_mmad_direction_n()`设置N方向优先；反之，调用`asc_set_mmad_direction_m()`设置M方向优先。

**图1** `asc_mmad`和`asc_copy_l0c2gm`同时沿M方向写/读

![`asc_mmad`和`asc_copy_l0c2gm`同时沿M方向写-读](../../../../figures/mmad_unitflag.png "`asc_mmad`和`asc_copy_l0c2gm`同时沿M方向写-读")

**特性约束：**

- `asc_mmad`和`asc_copy_l0c2gm`接口均提供了unitFlag参数来控制该功能的启用，需确保两者同步开启，才能正常生效。
- 当希望控制同一块L0C Buffer内存空间能持续只被多条`asc_mmad`或多条`asc_copy_l0c2gm`指令操作时，需将对应的前n-1条指令的unitFlag值设置为2，维持被操作内存空间的持续占用状态，最后一条指令设置为3，解除被占用状态。
- 当启用unitFlag功能后，建议`asc_mmad`的计算数据量与`asc_copy_l0c2gm`搬出的数据量保持一致。若`asc_mmad`计算了大块数据（M × N = 128 × 128），但`asc_copy_l0c2gm`只搬出了其中一部分数据（M × N = 64 × 64），则可能会导致执行异常，可通过[asc_set_l0c2gm_config](../asc_set_l0c2gm_config.md)重新配置L0C Buffer搬出状态。

**沿K轴迭代循环使用示例片段：**

```cpp
// 调用k_round次asc_mmad。
for (uint32_t k_index = 0; k_index < k_round; ++k_index) {
    uint8_t unit_flag = (k_index == k_round - 1) ? 3 : 2;
    asc_mmad(c, a, b, m, k, n, unit_flag, k_direction_align,
        c_matrix_source, c_matrix_init_val);
    if ((m / ALIGN_NUM) * (n / ALIGN_NUM) < LIMIT_MNSIZE) {
        asc_sync_pipe(PIPE_M);
    }
}
// asc_copy_l0c2gm一次搬出，unit_flag_ctl设置为3。
uint8_t unit_flag_ctl = 3;
asc_copy_l0c2gm(c_gm, c, n_size, m_size, loop_dst_stride, loop_src_stride,
    l2_cache_ctl, clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en,
    nz2nd_en, quant_post, relu_post, clip_relu_post, eltwise_op,
    eltwise_antq_en, c0_pad_en, broadcast_en, nz2dn_en);
```

完整接口参数请参考[asc_mmad](../asc_mmad.md)和[asc_copy_l0c2gm](../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)。

**表1** 性能示例说明（以输入数据类型half为例，无业务实测表示忽略指令的前后序操作，只考虑单指令性能，包含带宽延迟等开销）

| M | N | K | UnitFlag | LOOP_COUNT | `asc_mmad`理论值（cycle） | `asc_mmad`无业务实测值（cycle） |
| --- | --- | --- | --- | --- | --- | --- |
| 128 | 256 | 512 | Enable | 8 | 4156 | 4209 |
| 128 | 256 | 512 | Disable | 8 | 4156 | 4225 |
| 128 | 128 | 512 | Enable | 8 | 2108 | 2172 |
| 128 | 128 | 512 | Disable | 8 | 2108 | 2172 |
