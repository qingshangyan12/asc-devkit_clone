# UB bank冲突及地址重叠场景验证及分析（基于Mul指令）

## UB（Unified Buffer） 内存结构

- UB内存结构图
    ![UB内存结构图](./figures/UB内存结构图.jpg)

    - A2/A3架构上，UB空间大小是192KB；
    - 共有48个`bank`；
    - bank间组成为16个`bank group`，每个`group`包含3个`bank`（`bank_id` = 1,2,3）。
    - bank内由128行组成（`BANK_DEPTH`），每行长度是32B（`BANK_WIDTH`），每个`bank`大小为4KB。

## UB地址编码规则
- UB地址编码遵循如下规则  
    `UB_ADDR[17:0] = {BANK[1:0], BANK_DEPTH[6:0], BG[3:0], BANK_WIDTH[4:0]}`  
    即：
    | part1 | part2 | part3 | part4 |
    | --- | --- | --- | --- |
    | 2bits | 7bits | 4bits | 5bits|
    | bank_id | row_id | bank_group_id | offset |
- 具体示例  
    地址`0x10020`，对应地址编码是`0001 0000 0000 0010 0000`，得出以下信息：
    - bank_id = 1
    - row_id = 0
    - bank_group_id = 1
    - offset = 0  
    结合[UB内存结构图](#ubunified-buffer-内存结构)，即表示bank17的0地址。

## UB bank冲突类型
- 读写冲突：读操作和写操作同时尝试访问同一个bank；
- 写写冲突：多个写操作同时尝试访问同一个bank group；
- 读读冲突：多个读操作同时尝试访问同一个bank group；

## 测试场景概览
包含bank冲突、地址重叠等场景的验证和分析。构造场景用例的输入数据类型是float，输入数据长度是4096，以连续读写为例

| case_num | 计算公式 | 场景描述 | src0_addr | src1_addr | dst_addr | 打点性能（越低越好） | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | dst = src0 * src1 | 无bank读读、读写冲突，地址不重叠 | 0 | 0x10100 | 0x20000 | 2528 |  |
| 2 | dst = src0 * src1 | 有bank读读冲突（同一个bank），地址不重叠 | 0 | 0x4000 | 0x10000 | 4306 |  |
| 3 | dst = src0 * src1 | 有bank读读冲突（同一个bank group），地址不重叠 | 0 | 0x10000 | 0x14100 | 4306 |  |

每个case的具体分析过程，请参考下述各章节

## case1 无读读冲突、无读写冲突场景分析

- src0的首地址是`0`，对应地址编码是 `0000 0000 0000 0000 0000`；故对应的`bank_id` = 0，`bank_group_id` = 0。
- src1的首地址是`0x10100`，对应地址编码是 `0001 0000 0001 0000 0000`；故对应的`bank_id` = 1，`bank_group_id` = 8。
- dst的首地址是`0x20000`，对应地址编码是 `0010 0000 0000 0000 0000`；故对应的`bank_id` = 2，`bank_group_id` = 0。

- UB读写示例图如下：
    ![case1](./figures/case1.jpg)
    - 每8个绿色数据块（32B），代表src0每次repeat读取的数据（256B）；
    - 每8个黄色数据块（32B），代表src1每次repeat读取的数据（256B）；
    - 每8个蓝色数据块（32B），代表dst每次repeat写入的数据（256B）；
    - 处理完所有数据，迭代次数repeat_times = 数据长度 * sizeof(float) / 每次读取的数据量 = 4096 * 4 / 256 = 64。

- 分析结论
    - src0和src1读取的数据，每次repeat都在不同的bank和bank_group，故不存在读读冲突。
    - src0/src1读取的数据和dst写入的数据，每次repeat都在不同的bank，故不存在读写冲突。

## case2 有读写冲突（同一个bank）场景分析

- src0的首地址是`0`，对应地址编码是 `0000 0000 0000 0000 0000`；故对应的`bank_id` = 0，`bank_group_id` = 0。
- src1的首地址是`0x4000`，对应地址编码是 `0000 0100 0000 0000 0000`；故对应的`bank_id` = 0，`bank_group_id` = 0。
- dst的首地址是`0x10000`，对应地址编码是 `0001 0000 0000 0000 0000`；故对应的`bank_id` = 1，`bank_group_id` = 0。

- UB读写示例图如下：
    ![case1](./figures/case2.jpg)
    - 每8个绿色数据块（32B），代表src0每次repeat读取的数据（256B）；
    - 每8个黄色数据块（32B），代表src1每次repeat读取的数据（256B）；
    - 每8个蓝色数据块（32B），代表dst每次repeat写入的数据（256B）；
    - 处理完所有数据，迭代次数repeat_times = 数据长度 * sizeof(float) / 每次读取的数据量 = 4096 * 4 / 256 = 64。


- 分析结论
    - src0和src1读取的数据，每次repeat都在相同的bank，故存在读读冲突。
    - src0/src1读取的数据和dst写入的数据，每次repeat都在不同的bank，故不存在读写冲突。

## case3 有读写冲突（同一个bank group）场景分析

- src0的首地址是`0`，对应地址编码是 `0000 0000 0000 0000 0000`；故对应的`bank_id` = 0，`bank_group_id` = 0。
- src1的首地址是`0x10000`，对应地址编码是 `0001 0000 0000 0000 0000`；故对应的`bank_id` = 1，`bank_group_id` = 0。
- dst的首地址是`0x14100`，对应地址编码是 `0001 0100 0001 0000 0000`；故对应的`bank_id` = 1，`bank_group_id` = 8。

- UB读写示例图如下：
    ![case1](./figures/case3.jpg)
    - 每8个绿色数据块（32B），代表src0每次repeat读取的数据（256B）；
    - 每8个黄色数据块（32B），代表src1每次repeat读取的数据（256B）；
    - 每8个蓝色数据块（32B），代表dst每次repeat写入的数据（256B）；
    - 处理完所有数据，迭代次数repeat_times = 数据长度 * sizeof(float) / 每次读取的数据量 = 4096 * 4 / 256 = 64。


- 分析结论
    - src0和src1读取的数据，每次repeat都在相同的bank group，故存在读读冲突。
    - src0/src1读取的数据和dst写入的数据，每次repeat都在不同的bank，故不存在读写冲突。



