# Reg矢量计算API样例

## 概述

本样例展示了使用C_API构建Add算子样例的编译流程。

## 算子开发样例

|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [00_tensor_api_matmul](./00_tensor_api_matmul) | 本样例展示了使用Tensor_API 基于Layout和Tensor构造分形完成Mmad样例的编译流程，并展示了多样化的调用形式,支持main函数和kernel函数在同一个cpp文件中实现 |
| [03_tensor_api_matmul_multicore](./03_tensor_api_matmul_multicore) | 本样例展示了使用Tensor_API 基于Layout和Tensor构造分形完成多核场景Matmul样例的编译流程，并展示了多样化的调用形式,支持main函数和kernel函数在同一个cpp文件中实现 |

AddVisualization算子样例介绍
样例概述
本样例以向量加法算子为例,深入演示了基于Ascend C的向量算子执行流程与优化技术。样例实现了两个形状为 640×270 的half类型张量的逐元素加法运算,对应数学表达式为 z = x + y,重点展示了多核并行计算、双缓冲流水线调度及访存对齐优化等核心技术。

算子功能描述
算子类型: Add

输入规格:

输入张量 x: 形状 640×270,数据类型 half,格式 ND
输入张量 y: 形状 640×270,数据类型 half,格式 ND
输出规格:

输出张量 z: 形状 640×270,数据类型 half,格式 ND
计算逻辑: 完成两个输入张量的逐元素相加运算,输出计算结果。

核心技术实现
1. 多核并行计算架构
样例采用多核并行策略,将输入数据按行维度均匀划分至多个AI Core核上进行并行处理。通过 AscendC::GetBlockNum() 获取可用核数,每个核独立处理 rowPerBlock = row / BlockNum 行数据,实现计算任务的负载均衡与并行加速。

add_visualization.ascL45-L48
// Calculate the number of rows each Block needs to process. 
this->rowPerBlock = row / AscendC::GetBlockNum();
// Calculate the total number of elements each Block needs to process.
this->blockLength = this->rowPerBlock * column;
2. 双缓冲流水线优化
样例引入双缓冲机制(DB_NUMBER = 2),实现数据搬运与向量计算的流水线并行执行。双缓冲技术的核心优势在于:

访存计算重叠: 当一个缓冲区执行向量计算时,另一缓冲区可同时进行数据搬运,显著降低访存延迟对计算性能的影响
资源利用率提升: 通过流水线调度最大化利用AI Core的存储搬运单元与向量计算单元,提升整体硬件执行效率
吞吐量优化: 实现GM(Global Memory)与UB(Unified Buffer)之间数据传输与向量计算任务的高效协同
add_visualization.ascL60-L63
// Initialize double buffer (memory access-computation pipeline)
pipe->InitBuffer(inQueueSrc0, DB_NUMBER, columnAlignedByte * this->rowPerBlock / DB_NUMBER);
pipe->InitBuffer(inQueueSrc1, DB_NUMBER, columnAlignedByte * this->rowPerBlock / DB_NUMBER);
pipe->InitBuffer(outQueueDst, DB_NUMBER, columnAlignedByte * this->rowPerBlock / DB_NUMBER);
add_visualization.ascL69-L78
__aicore__ inline void Process()
{
    // Double buffer pipeline: parallel execution of read-compute-write
    uint32_t loopCount = DB_NUMBER;
    for (uint32_t i = 0; i < loopCount; i++) {
        CopyIn(i);
        Compute(i);
        CopyOut(i);
    }
}
3. 32字节访存对齐优化
样例针对AI Core硬件访存特性,采用32字节对齐策略优化内存访问效率:

对齐填充机制: 通过 DataCopyPad 接口在数据搬运过程中自动完成右边界零填充,确保数据满足32字节对齐要求
硬件效率提升: 对齐访存可有效提升GM到UB的数据搬运带宽利用率,降低访存延迟
add_visualization.ascL56-L58
// 32B alignment for column dimension
this->columnAlignedByte = AscendC::CeilDivision(sizeof(half) * column, BLOCK_SIZE) * BLOCK_SIZE;
this->columnAligned = columnAlignedByte / sizeof(half);
add_visualization.ascL96-L106
AscendC::DataCopyPadParams padParams;
padParams.isPad = true;
padParams.leftPadding = 0;
// Zero-padding on the right to achieve 32-byte alignment
padParams.rightPadding = this->columnAlignedByte / sizeof(half) - this->column;
padParams.paddingValue = 0;

// Use DataCopyPad to resolve the 32B misalignment of data during GM→UB copy
// DataCopyPad ensures the data is correctly copied from GM->UB based on the `padParams` configuration set above.
AscendC::DataCopyPad<half>(src0Local, src0Global[progress * blockLength / DB_NUMBER], params, padParams);
AscendC::DataCopyPad<half>(src1Local, src1Global[progress * blockLength / DB_NUMBER], params, padParams);
执行流程解析
算子执行过程分为三个核心阶段:

CopyIn阶段(数据输入搬运)

从Global Memory读取输入数据至Unified Buffer
通过 DataCopyPad 完成32字节对齐处理与数据填充
Compute阶段(向量计算)

在Unified Buffer中执行向量加法运算
采用Mask模式处理非对齐边界数据,确保计算正确性
分为主块计算与尾部块计算两个阶段
CopyOut阶段(数据输出搬运)

将计算结果从Unified Buffer搬运回Global Memory
完成最终数据输出
技术价值
本样例通过向量加法算子的完整实现,系统性地展示了Ascend C算子开发中的关键优化技术。双缓冲机制与访存对齐策略的综合应用,为开发者提供了高性能向量算子开发的参考范式,对理解AI Core硬件执行特性与软件优化方法具有重要指导意义。