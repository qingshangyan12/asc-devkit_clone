# sys_var概述

Tensor API用于描述Tensor、layout及矩阵计算流程，不提供完整的核调度信息和系统状态查询能力。在进行多核任务切分、分离模式调度或性能分析时，可配合使用C API提供的系统变量接口。除试验接口外，使用相关接口时需要额外引入`c_api/sys_var.h`；试验接口的头文件和使能方式以对应C API资料为准。

Tensor API开发中常用的系统变量相关C API如下表所示。

| 接口 | 用途 |
| :--- | :--- |
| [asc_get_arch_ver](../../c_api/sys_var/asc_get_arch_ver.md) | 获取当前AI处理器架构版本号。 |
| [asc_get_block_idx](../../c_api/sys_var/asc_get_block_idx.md) | 获取当前运行核的索引，用于多核任务切分。 |
| [asc_get_block_num](../../c_api/sys_var/asc_get_block_num.md) | 获取参与执行的AI核数。 |
| [asc_get_core_id](../../c_api/sys_var/asc_get_core_id.md) | 获取当前核的编号。 |
| [asc_get_sub_block_id](../../c_api/sys_var/asc_get_sub_block_id.md) | 获取AI Core上当前Vector Core的ID。 |
| [asc_get_sub_block_num](../../c_api/sys_var/asc_get_sub_block_num.md) | 在分离模式下获取一个AI Core上的Cube Core或Vector Core数量。 |
| [asc_get_system_cycle](../../c_api/sys_var/asc_get_system_cycle.md) | 获取当前系统Cycle数，可用于性能统计。 |
| [asc_get_program_counter](../../c_api/sys_var/asc_get_program_counter.md) | 获取程序计数器指针，用于定位当前程序执行位置。 |
| [指令发射队列空闲槽位查询](../../c_api/sys_var/asc_get_pipe_idle_slot_count.md) | 查询指定流水线发射队列的空闲槽位数量。该接口为试验接口。 |

`c_api/sys_var`目录还包含寄存器和底层搬运状态配置接口。Tensor API已经在内部完成相关通路配置时，不应在同一调用过程中重复修改对应状态；确需使用时，应同时满足Tensor API通路和C API接口的约束。

