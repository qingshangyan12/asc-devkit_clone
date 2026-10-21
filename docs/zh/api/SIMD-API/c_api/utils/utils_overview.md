# 工具接口概述

工具接口提供AI Core系统初始化、空操作以及系统资源和变量读取能力，主要用于初始化核状态、获取核编号与架构版本、读取时间与Vector Length（VL）、获取地址及调试信息等场景。大部分接口为只读查询接口，不修改寄存器或存储状态。

工具接口的产品支持范围因接口而异：部分接口仅在Ascend 950PR&950DT系列产品上支持，部分接口仅在Atlas A3/A2系列产品上支持，使用前需确认目标产品的支持状态。

## 接口概览

### 系统初始化

**表1** 系统初始化接口

| 接口名称 | 功能简述 | 返回值 | 支持产品 |
| --- | --- | --- | --- |
| [asc_init](sys_init/asc_init.md) | 将当前AI Core上可能由前序算子遗留的全局控制状态恢复为接口约定的初始状态，仅初始化控制状态，不清零Global Memory、Local Memory或用户数据。 | 无 | Ascend 950PR&950DT、Atlas A3、Atlas A2 |

### 系统其他操作

**表2** 系统其他操作接口

| 接口名称 | 功能简述 | 返回值 | 支持产品 |
| --- | --- | --- | --- |
| [asc_nop](sys_misc/asc_nop.md) | 生成一条空操作指令，不执行实际计算，每调用一次阻塞一拍流水。 | 无 | Ascend 950PR&950DT |

### 系统资源和变量

**表3** 系统资源和变量接口

| 接口名称 | 功能简述 | 返回值 | 支持产品 |
| --- | --- | --- | --- |
| [asc_get_arch_ver](sys_var/asc_get_arch_ver.md) | 获取当前AI处理器架构版本号。 | 无；通过引用参数`core_version`输出 | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_core_id](sys_var/asc_get_core_id.md) | 获取当前AI Core相对其他AI Core的编号，用于按核划分数据分片。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_ffts_base_addr](sys_var/asc_get_ffts_base_addr.md) | 获取核间同步寄存器基地址。 | int64_t | Atlas A3、Atlas A2 |
| [asc_get_phy_buf_addr](sys_var/asc_get_phy_buf_addr.md) | 基于偏移量`offset`获取片上实际物理地址。 | uint64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_phy_stack_base](sys_var/asc_get_phy_stack_base.md) | 获取物理堆栈基地址。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_program_counter](sys_var/asc_get_program_counter.md) | 获取程序计数器指针，用于定位当前执行位置。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_smmu_tag_version](sys_var/asc_get_smmu_tag_version.md) | 获取系统内存管理单元（SMMU）版本信息。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_status](sys_var/asc_get_status.md) | 读取STATUS特殊寄存器的64bit整体值。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_sub_block_id](sys_var/asc_get_sub_block_id.md) | 在分离模式下获取逻辑AI Core上AIC或AIV的逻辑索引。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_sub_block_num](sys_var/asc_get_sub_block_num.md) | 在分离模式下获取逻辑AI Core上AIC或AIV的数量。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_sys_virtual_base](sys_var/asc_get_sys_virtual_base.md) | 读取系统虚拟基地址。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_system_clock](sys_var/asc_get_system_clock.md) | 读取当前系统时钟计数器，返回微秒级时间值。 | int64_t，单位微秒 | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_system_cycle](sys_var/asc_get_system_cycle.md) | 读取当前系统cycle计数器，返回原始cycle值。 | int64_t | Ascend 950PR&950DT、Atlas A3、Atlas A2 |
| [asc_get_vf_len](sys_var/asc_get_vf_len.md) | 获取当前芯片的矢量寄存器位宽VL。 | constexpr int64_t | Ascend 950PR&950DT |
| [asc_set_ffts_base_addr](sys_var/asc_set_ffts_base_addr.md) | 设置核间同步寄存器基地址。 | 无 | Atlas A3、Atlas A2 |

## 关键特性

工具接口涉及核初始化、运行状态、时间和地址获取，部分接口受产品范围或编译模式限制。以下从算子实现角度说明这些特殊能力和易混接口的差异。

### 获取时间信息

**表4** 时间获取接口对比

| 接口名称 | 返回内容 | 单位 | 换算方式 |
| --- | --- | --- | --- |
| [asc_get_system_clock](sys_var/asc_get_system_clock.md) | 当前系统时间 | 微秒 | 接口内部按芯片频率换算：$clock = cycle / 1000$（Ascend 950PR&950DT）、$clock = cycle / 50$（Atlas A3/A2）。 |
| [asc_get_system_cycle](sys_var/asc_get_system_cycle.md) | 当前系统cycle计数 | cycle | 返回原始计数值，调用方按芯片频率自行换算。 |

两个接口读取同一个底层计数器，区别仅在于是否在接口内部完成频率换算。`asc_get_system_clock`的返回值已经是微秒，适合直接用于耗时统计；`asc_get_system_cycle`返回原始cycle，写算子做性能统计时需要按目标芯片频率自行换算：Ascend 950PR&950DT为1GHz（除以1000），Atlas A3/A2为50MHz（除以50）。

### 获取核编号相关信息

**表5** 核编号相关接口对比

| 接口名称 | 返回内容 | 统计层级 | 使用场景 |
| --- | --- | --- | --- |
| [asc_get_core_id](sys_var/asc_get_core_id.md) | AI Core相对其余AI Core的编号 | AI Core之间 | 按核编号划分任务，使不同AI Core处理不同数据分片。 |
| [asc_get_sub_block_id](sys_var/asc_get_sub_block_id.md) | 逻辑AI Core上AIC或AIV的逻辑索引 | AI Core内部 | 在分离模式下区分同一个逻辑AI Core内的AIC和AIV。 |
| [asc_get_sub_block_num](sys_var/asc_get_sub_block_num.md) | 逻辑AI Core上AIC或AIV的数量 | AI Core内部 | 与`asc_get_sub_block_id`配合，确定索引范围和任务划分。 |

`asc_get_core_id`获取的是AI Core之间的编号，与内置变量中的`block_idx`、`sub_block_idx`层级不同。`asc_get_sub_block_id`和`asc_get_sub_block_num`仅在分离模式下有意义，分别返回索引和数量，二者需配合使用。

### 获取地址信息

**表6** 地址类接口对比

| 接口名称 | 返回内容 | 说明 |
| --- | --- | --- |
| [asc_get_phy_buf_addr](sys_var/asc_get_phy_buf_addr.md) | 由偏移量转换得到的片上物理地址 | 常用于调试、地址推导或Tiling数据地址计算；不能和以数组方式申请内存的方法混用。 |
| [asc_get_phy_stack_base](sys_var/asc_get_phy_stack_base.md) | 物理堆栈基地址 | 常用于调试时定位当前核的堆栈位置。 |
| [asc_get_sys_virtual_base](sys_var/asc_get_sys_virtual_base.md) | 系统虚拟基地址 | 用于将相对偏移量转换为系统虚拟地址，适用于地址推导、跨核地址对齐等场景。 |
| [asc_get_ffts_base_addr](sys_var/asc_get_ffts_base_addr.md) | 核间同步寄存器基地址 | 需与`asc_set_ffts_base_addr`配合使用，仅Atlas A3/A2支持。 |
| [asc_set_ffts_base_addr](sys_var/asc_set_ffts_base_addr.md) | 无；设置核间同步寄存器基地址 | 需在Host侧调用`aclrtGetHardwareSyncAddr`获取基地址后传入，仅Atlas A3/A2支持。 |

`asc_get_phy_buf_addr`、`asc_get_phy_stack_base`和`asc_get_sys_virtual_base`分别对应片上物理缓冲区地址、物理堆栈基地址和系统虚拟基地址，地址空间和用途不同，不能互相替代。`asc_get_ffts_base_addr`和`asc_set_ffts_base_addr`是一对读写接口，分别用于获取和设置核间同步寄存器基地址。

### 获取版本与状态信息

**表7** 版本与状态类接口对比

| 接口名称 | 返回内容 | 典型取值或说明 |
| --- | --- | --- |
| [asc_get_arch_ver](sys_var/asc_get_arch_ver.md) | AI处理器架构版本号 | Ascend 950PR&950DT为3510，Atlas A3/A2为2201。 |
| [asc_get_smmu_tag_version](sys_var/asc_get_smmu_tag_version.md) | SMMU版本信息 | 低28bit包含版本、年、月、日和子版本。 |
| [asc_get_status](sys_var/asc_get_status.md) | STATUS寄存器64bit值 | 各状态位记录浮点溢出、下溢、NaN/INF等告警事件。 |
| [asc_get_vf_len](sys_var/asc_get_vf_len.md) | 矢量寄存器位宽VL | 表示单次Repeat可处理的元素个数，仅Ascend 950PR&950DT支持。 |

`asc_get_arch_ver`用于区分AI处理器架构，`asc_get_smmu_tag_version`用于判断SMMU版本及发布日期，二者维度不同。`asc_get_status`读取的是保持型告警状态位，状态位置位后会保持到复位或被读取清除，写算子时可用来判断前序计算是否发生溢出、下溢或NaN/INF。`asc_get_vf_len`是constexpr接口，返回值在编译期即可确定，可直接用于推导循环次数、mask长度或作为模板参数。

## 通用约束

- 工具接口的产品支持范围存在差异：`asc_nop`、`asc_get_vf_len`仅Ascend 950PR&950DT支持；`asc_get_ffts_base_addr`、`asc_set_ffts_base_addr`仅Atlas A3/A2支持；其余接口在Ascend 950PR&950DT、Atlas A3、Atlas A2上均支持。
- `asc_init`仅初始化控制状态，不清零Global Memory、Local Memory或用户数据；不同产品的初始化范围不同，详见[asc_init](sys_init/asc_init.md)。
- `asc_get_phy_buf_addr`不能和以数组方式申请内存的方法混用。
- `asc_get_sys_virtual_base`、`asc_get_vf_len`不支持纯SIMT编译模式，在该模式下包含对应头文件会触发编译错误。
- `asc_get_system_clock`和`asc_get_system_cycle`为只读查询接口，返回值随系统时钟单调不减。
