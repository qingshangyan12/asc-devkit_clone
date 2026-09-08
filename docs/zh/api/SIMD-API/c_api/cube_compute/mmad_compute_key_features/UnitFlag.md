# UnitFlag<a name="ZH-CN_TOPIC_00000025690709788"></a>

**特性说明：**

unitFlag的核心功能体现为：asc_mmad和L0C搬出接口引入了单元标志（unit-flag）机制，通过以内存块为粒度实现精细化的数据同步，从而有效降低同步延迟，提升系统整体性能。当UnitFlag开关打开后，对于L0C Buffer中的每个内存块（512B），提供一个单元标志位，用于指示该块是否可读或可写。

asc_mmad和L0C搬出接口设置unit_flag_mode为ENABLE_KEEP/ENABLE_UPDATE后，系统会启动单元标志位。

当unit_flag_mode == asc_unit_flag_mode::ENABLE_KEEP（0b10），开启unitFlag功能，在硬件执行完指令后，不改变单元标志位；

- 对于写操作（asc_mmad接口），如果单元标志位0，则硬件直接写入L0C Buffer；否则，如果单元标志位为1，则写操作会等待直到单元标志变为0；执行完成后将单元标志位保持为0；
- 对于读操作（L0C搬出接口），如果单元标志位1，则硬件直接读取L0C Buffer；否则，如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位保持为1；

当unit_flag_mode == asc_unit_flag_mode::ENABLE_UPDATE（0b11），开启unitFlag功能，在硬件执行完指令后，改变单元标志位；

- 对于写操作（asc_mmad接口），如果单元标志位0，则硬件直接写入L0C Buffer；否则，如果单元标志位为1，则写操作会等待直到单元标志变为0；执行完成后将单元标志位设置成1；
- 对于读操作（L0C搬出接口），如果单元标志位1，则硬件直接读取L0C Buffer；否则，如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位设置成0；

根据上述特性，如果用户在进行A矩阵\[128, 1024\]、B矩阵为\[1024, 128\]的矩阵乘计算时，需要沿着K轴进行迭代循环，假设每次迭代K长度为128，则需要迭代8次，此时8次asc_mmad对应1次Fixpipe操作；

- 前7次asc_mmad的unitFlag都设置成2，写入后将单元标记位始终为0，保证后续asc_mmad可以写入L0C Buffer；
- 最后1次asc_mmad设置成3，写入后将单元标志位设置成1，保证Fixpipe可以读取L0C Buffer；
- Fixpipe的unitFlag设置为3，读取后将单元标记位设置为0，保证后续asc_mmad接口可以顺利写入L0C Buffer数据；

如果用户需要单次asc_mmad的结果分多次搬出时，譬如asc_mmad计算结果的L0C Buffer为M\(128\) x N\(256\)，沿N轴分两次搬出；这样一次asc_mmad会对应两次Fixpipe；

- asc_mmad的时候需要设置unitFlag = 3，保证Fixpipe可以读取L0C Buffer数据；
- 每一次Fixpipe的unitFlag都设置为3，读取后将单元标记位设置为0，保证后续其他asc_mmad接口在复用这块L0C Buffer地址时可以顺利写入数据；

当开启unitFlag后，asc_mmad和Fixpipe会对同一块分形的L0C Buffer进行读写操作，因此asc_mmad计算和Fixpipe保持一致的读写顺序，有助于获得更优的性能表现。

在调用asc_mmad接口时，通过[asc_set_mmad_direction_m](../asc_set_mmad_direction_m.md)或[asc_set_mmad_direction_n](../asc_set_mmad_direction_n.md)配置计算方向。搬出启用NZ2ND或Channel Merge时优先采用N方向计算；普通NZ输出采用M方向，以匹配生产与消费顺序。

**图1** asc_mmad和Fixpipe同时沿M方向写/读

![asc_mmad和Fixpipe同时沿M方向写-读](../../../../figures/mmad_unitflag.png "asc_mmad和Fixpipe同时沿M方向写-读")

**特性约束：**

- asc_mmad和L0C搬出接口均提供了UnitFlag控制参数来控制该功能的启用，需确保两者同步开启，才能正常生效。
- 当希望控制同一块L0C Buffer内存空间能持续只被多条asc_mmad或多条Fixpipe指令操作时，需将对应的前n-1条指令的unitFlag值设置为2，维持被操作内存空间的持续占用状态，最后一条指令设置为3，解除被占用状态。
- 当启用unitFlag功能后，建议asc_mmad的计算数据量与Fixpipe搬出的数据量保持一致。若asc_mmad计算了大块数据（M × N = 128 × 128），但Fixpipe只搬出了其中一部分数据（M × N = 64 × 64），则可能会导致执行异常，必须先等待相关计算和搬出结束，再重置未被释放的状态。3510使用`asc_set_l0c_copy_config(relu_index, quant_index, true)`；2201使用`asc_set_l0c2gm_config(relu_index, quant_index, true)`。两个索引须保留当前所需系数地址，清标志不代替等待。

**沿K轴迭代循环使用示例片段：**

以下为3510调用片段，假设K迭代数大于0，A/B各K块已经搬入独立且足够大的L0缓冲区，并完成MTE1→M同步；`a_blocks[i]`/`b_blocks[i]`为对应块指针，M、N、K块大小均为16的倍数。结果按ND输出到GM，`dst_stride=n`、`src_stride=m`。

```cpp
#if __NPU_ARCH__ == 3510
asc_set_mmad_direction_n();
for (uint16_t i = 0; i < k_round; ++i) {
    const auto flag = (i + 1 == k_round)
        ? asc_unit_flag_mode::ENABLE_UPDATE
        : asc_unit_flag_mode::ENABLE_KEEP;
    asc_mmad(c, a_blocks[i], b_blocks[i], m, k_tile, n, flag,
             false, false, i == 0 /* 仅首次清零 */);
    asc_sync_pipe(PIPE_M); // 保证本例连续累加的依赖
}
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(c_gm, c, n, m, n, m,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM,
    asc_unit_flag_mode::ENABLE_UPDATE, asc_quant_mode::NoQuant,
    asc_relu_pre_mode::NONE, false, true, false, false);
// 后续消费者读取GM或复用缓冲区前，仍需相应同步。
#endif
```
