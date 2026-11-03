# 附录<a name="ZH-CN_TOPIC_0000002515978690"></a>

- **[API流水类型汇总](api_pipeline_type_summary.md)**

- **[Cube指令理论性能汇总](cube_instruction_theoretical_perf_summary.md)**

- **[Vector指令理论性能汇总](vector_instruction_theoretical_perf_summary.md)**

- **[Mask接口汇总](Mask_interface_summary.md)**

- **[Mmad接口边界值汇总](mmad_interface_boundary_value_summary.md)**

- **[MmadMx接口边界值汇总](mmad_mx_interface_boundary_value_summary.md)**

- **[矩阵计算搬出相关接口边界值汇总](matrix_compute_move_out_interface_boundary_value_summary.md)**

- **[Memory矢量计算接口边界值汇总](memory_vector_compute_interface_boundary_value_summary.md)**

- **[Reg矢量计算接口边界值汇总](reg_vector_compute_interface_boundary_value_summary.md)**

- **[Reg矢量计算接口MergeMode支持情况汇总](reg_vector_compute_interface_mergemode_support_summary.md)**

- **[Reg矢量计算接口软仿情况汇总](reg_vector_compute_interface_soft_simulation_summary.md)**

- **[Reg矢量计算接口精度标准汇总](reg_vector_compute_interface_precision_standard_summary.md)**

- **[Reg矢量计算接口指令单双发汇总](reg_vector_compute_interface_instruction_single_dual_issue_summary.md)**

- **[Reg矢量计算接口理论性能汇总](reg_vector_compute_interface_theoretical_perf_summary.md)**

- **[C-API-Reg矢量计算接口理论性能汇总](c_api_reg_vector_compute_theoretical_perf_summary.md)**

- **[原子操作类接口边界值汇总](atomic_operations_interface_boundary_value_summary.md)**

- **[SIMT接口软仿情况汇总](SIMT_interface_soft_simulation_summary.md)**

- **[SIMT接口精度标准汇总](SIMT_interface_precision_standard_summary.md)**

- **[SIMT指令理论性能汇总](SIMT_instruction_theoretical_perf_summary.md)**

- **[废弃接口](deprecated_interface.md)**

- **[接口变更说明](interface_change_description.md)**

- **[Release Notes](Release_Notes/release_notes.md)**




https://gitcode.com/cann/asc-devkit/issues/1943的描述中
// GetValue为Scalar操作，与后续的Duplicate存在数据依赖
// 因此Vector流水需要等待Scalar操作结束
float inputVal = srcLocal.GetValue(0);
SetFlagHardEvent::S_V(eventID1);
WaitFlagHardEvent::S_V(eventID1);
AscendC::Duplicate(dstLocal, inputVal, srcDataSize);

// SetValue为Scalar操作，与后续的数据搬运操作存在数据依赖
// 因此MTE3流水需要等待Scalar操作结束
srcLocal.SetValue(0, value);
SetFlagHardEvent::S_MTE3(eventID2);
WaitFlagHardEvent::S_MTE3(eventID2);
AscendC::DataCopy(dstGlobal, srcLocal, srcDataSize);

前一个例子中的S_V的同步已经专家讨论过是不需要的，因此后续会将此处描述修改。
而S_MTE3的同步经确认是需求的。根据以上信息将以下回复重写，注意重写后是输出到本地，不需要帮我评论：

您好，针对您的问题给出如下说明：

根据Ascend C硬件架构说明第223～226行，Scalar指令由Scalar单元直接执行，Vector指令则先被发射到Vector指令队列，再由Vector单元执行。Scalar单元和Vector单元属于两个不同执行单元可以异步并行工作，因此，不能仅凭源码中的先后顺序判断跨流水的数据依赖已经得到满足。

该示例在srcLocal.GetValue与AscendC::Duplicate之间使用SetFlag<HardEvent::S_V>和WaitFlag<HardEvent::S_V>建立同步关系，以保证源流水的前序操作完成后，目的流水再执行后续操作。

如果您还有疑问，欢迎继续交流~