# 系统资源和变量接口总览

系统资源和变量接口用于读取AI Core运行时的系统寄存器、硬件资源信息、地址信息和计时信息，以及配置核间同步寄存器基地址。接口统一声明在`c_api/utils/sys_var.h`中，在Vector Function（`__simd_vf__`标记的函数）内使用。

## 接口分类

### 系统信息和硬件资源

| 接口 | 功能 | 返回值或参数 |
| --- | --- | --- |
| [asc_get_arch_ver](asc_get_arch_ver.md) | 获取当前AI处理器架构版本号。 | 通过引用参数返回`uint32_t`架构版本号。 |
| [asc_get_core_id](asc_get_core_id.md) | 获取当前AI Core相对其他AI Core的编号。 | 返回`int64_t`编号。 |
| [asc_get_sub_block_id](asc_get_sub_block_id.md) | 分离模式下获取当前逻辑AI Core中AIC或AIV的逻辑索引。 | 返回`int64_t`索引。 |
| [asc_get_sub_block_num](asc_get_sub_block_num.md) | 分离模式下获取当前AI Core中AIC或AIV的数量。 | 返回`int64_t`数量。 |
| [asc_get_vf_len](asc_get_vf_len.md) | 获取当前芯片的矢量寄存器位宽VL。 | 返回`int64_t`元素数量。 |
| [asc_get_smmu_tag_version](asc_get_smmu_tag_version.md) | 获取SMMU版本信息。 | 返回`int64_t`版本值。 |
| [asc_get_status](asc_get_status.md) | 读取当前核上下文中STATUS特殊寄存器的64bit整体值。 | 返回`int64_t`状态值。 |

### 系统计时

| 接口 | 功能 | 单位或使用方式 |
| --- | --- | --- |
| [asc_get_system_cycle](asc_get_system_cycle.md) | 读取系统cycle计数器的当前计数值。 | 返回累计cycle数，适合性能统计和耗时测量。 |
| [asc_get_system_clock](asc_get_system_clock.md) | 读取系统时钟计数器并换算为当前时间值。 | 返回微秒值。 |

`asc_get_system_cycle`和`asc_get_system_clock`都用于计时：前者返回硬件累计cycle数，后者在接口内部按芯片时钟频率换算并直接返回微秒值。需要微秒级时间时，可优先使用`asc_get_system_clock`。

### 地址和程序执行位置

| 接口 | 功能 | 参数或返回值 |
| --- | --- | --- |
| [asc_get_phy_buf_addr](asc_get_phy_buf_addr.md) | 根据偏移量获取片上实际物理地址。 | 输入`uint64_t offset`，返回`uint64_t`物理地址。 |
| [asc_get_phy_stack_base](asc_get_phy_stack_base.md) | 获取物理堆栈基地址。 | 返回`int64_t`地址。 |
| [asc_get_sys_virtual_base](asc_get_sys_virtual_base.md) | 读取系统虚拟基地址特殊寄存器。 | 返回`int64_t`地址。 |
| [asc_get_program_counter](asc_get_program_counter.md) | 获取当前程序计数器的指针。 | 返回`int64_t`程序计数器值。 |

### 核间同步寄存器基地址

| 接口 | 功能 | 使用关系 |
| --- | --- | --- |
| [asc_get_ffts_base_addr](asc_get_ffts_base_addr.md) | 获取核间同步寄存器的基地址。 | 与`asc_set_ffts_base_addr`配套使用。 |
| [asc_set_ffts_base_addr](asc_set_ffts_base_addr.md) | 设置核间同步寄存器的基地址。 | 在`asc_sync_block_wait`和`asc_sync_block_arrive`前调用。 |

`asc_get_ffts_base_addr`用于读取已配置的基地址，`asc_set_ffts_base_addr`用于写入基地址。基地址需要由Host侧`aclrtGetHardwareSyncAddr`获取后传入设置接口。

## 与相关接口对比

当前`utils/sys_var`目录中的接口没有Basic API同名封装，以下对比用于区分功能相近的C API接口：

| 对比接口 | 相同点 | 差异 |
| --- | --- | --- |
| `asc_get_system_cycle`与`asc_get_system_clock` | 都读取系统计时信息，可用于性能测量。 | `asc_get_system_cycle`返回原始cycle计数；`asc_get_system_clock`完成频率换算后返回微秒值。 |
| `asc_get_ffts_base_addr`与`asc_set_ffts_base_addr` | 都操作核间同步寄存器基地址。 | 前者读取地址，后者设置地址；设置接口应在核间同步接口前调用。 |
| `asc_get_core_id`与`asc_get_sub_block_id` | 都用于获取执行位置索引。 | 前者表示AI Core之间的编号；后者表示分离模式下AI Core内部AIC或AIV的逻辑索引。 |
| `asc_get_sub_block_id`与`asc_get_sub_block_num` | 都面向分离模式下的AIC/AIV信息。 | 前者返回当前子块索引；后者返回当前AI Core上的子块数量。 |
| `asc_get_phy_buf_addr`与`asc_get_sys_virtual_base` | 都可用于地址推导。 | 前者根据输入偏移计算物理地址；后者读取系统虚拟基地址特殊寄存器。 |

## 废弃接口

| 接口 | 说明 |
| --- | --- |
| [asc_get_block_idx（废弃）](../../deprecated_interface/asc_get_block_idx_deprecated.md) | 已废弃接口。 |
| [asc_get_block_num（废弃）](../../deprecated_interface/asc_get_block_num_deprecated.md) | 已废弃接口。 |
