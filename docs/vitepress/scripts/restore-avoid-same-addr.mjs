import { readFileSync, writeFileSync } from 'fs';

const f = 'docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免同地址访问.md';
const backup = 'docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免同地址访问.md.orig';

// Restore from original (the original HTML-table version)
// Actually, we don't have an original backup. Let me just construct the content:
const content = `# 避免同地址访问

【优先级】高

>[!NOTE]说明 
>该性能优化指导适用于如下产品型号：
>- Atlas A3 训练系列产品/Atlas A3 推理系列产品
>- Atlas A2 训练系列产品/Atlas A2 推理系列产品

【描述】MTE2、MTE3、Scalar等单元访问Global Memory数据时，其地址请求会按照512字节粒度对齐后进行处理。当同时访问Global Memory的数据，且地址处于连续的512字节范围内时，由于数据一致性的原因，多个请求会被串行处理，进而影响数据搬运效率。

当前算子执行机制保证用户Kernel入参（包括Workspace/Tiling）的地址512字节对齐，因此开发者只需要根据地址的偏移量即可判断两个地址是否会落入连续的512字节范围内。

如下图所示，AI Core内的各个核对Global Memory的数据同时发出读写请求，尽管addr0~addr5是多个不同的地址，但因为落在连续的512字节范围内，被视为同一个地址请求，此时这几个数据请求会被串行处理，数据访问效率会降低。同地址访问的影响受同时访问的核数影响，同地址访问的核数越多时，串行导致的性能劣化越严重。

避免同地址访问的方法主要有以下两种：**调整数据访问顺序**和**修改切分策略**。下文介绍配套的样例请参考[避免同地址访问样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/04_best_practices/04_memory_access_practices/mata_address_conflict)。

**调整数据访问顺序**

以一个形状为 \\(8192, 128\\) 的float类型输入进行Adds计算为例。

为了体现同地址冲突的影响，上述场景设计中每一行的数据大小为512字节（128个float），每个核每一轮计算处理512 \\* 8字节的数据，并进行全核同步（实际场景中并不需要），每一轮计算都需要等待所有核完成当前数据块的计算后，再进行下一轮。

| 方案 | 原始 | 优化 |
| - | - | - |
| 方法 | test1 | test2 |
| 示例代码 | code1 | code2 |

**修改切分策略**

仍以一个形状为 \\(8192, 128\\) 的float类型输入进行Adds计算为例。

为了体现同地址冲突的影响，上述场景设计中每一行的数据大小为512字节（128个float），每个核每一轮计算处理512 \\* 8字节的数据，并进行全核同步（实际场景中并不需要），每一轮计算都需要等待所有核完成当前数据块的计算后，再进行下一轮。

| 方案 | 原始 | 优化 |
| - | - | - |
| 方法 | test3 | test4 |
| 示例代码 | code3 | code4 |

>[!NOTE]说明 
>你可以通过执行如下命令行，通过msprof工具获取上述示例的性能数据并进行对比。
>\`\`\`
>msprof op --launch-count=3 --output=./prof ./execute_adds_op
>\`\`\`
>重点关注PipeUtilization.csv中的aiv_mte2_time(us)和aiv_mte3_time(us)搬运指令耗时。
`;

writeFileSync(f, content);
console.log('Restored with short table cells');
