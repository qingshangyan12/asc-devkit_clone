# 基础知识

>[!NOTE]说明
>学习本节前，请先了解[CV融合算子](../cv_fusion/basics.md)、[矩阵编程](../../matrix_advanced_api/basics.md)和[地址空间限定符](../../../../programming_guide/language_extension/simd_builtin_keywords.md#section1624210295308)。
>本节内容适用于如下产品型号：
><!-- npu="950" id1 -->
>- Ascend 950PR/Ascend 950DT（NPU架构版本3510）
><!-- end id1 -->

## SSBuffer

SSBuffer是AI Core内部的物理存储单元，具有独立的地址空间。在分离模式下，同一个Mix任务中的AIC与对应的AIV可以通过Scalar单元读写SSBuffer，交换控制信息、地址、长度和少量标量数据。硬件架构及与其他存储单元的关系请参见[NPU架构版本3510](../../../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。

CV融合算子中的通信包含消息交换和业务数据搬运。例如，AIV将向量计算结果作为Cube计算的输入时，需要告诉AIC输入位置、矩阵形状等信息，还需要将输入数据搬运至L1 Buffer。SSBuffer可以保存这些消息，矩阵数据本身则通过UB到L1 Buffer等数据通路搬运。SSBuffer容量有限，不用于保存大块中间计算结果，也不能替代GM、UB或L1 Buffer。

SSBuffer读写与核间同步是两个相关但不同的操作：写入消息用于传递内容，核间同步用于保证另一侧在消息或数据准备完成后再读取。SSBuffer不会自动生成消息队列、有效标记或完成通知，开发者需要通过接口或通信规则建立这些关系。

## 使用场景和优势

### 使用场景

SSBuffer适用于同一AI Core内的小数据量通信，常见场景如下。

**表1**  SSBuffer通信场景

| 场景 | SSBuffer中传递的内容 | 需要配合的操作 |
| --- | --- | --- |
| AIV向AIC提交计算任务 | 功能号、输入地址、矩阵形状、任务序号等。 | AIV准备输入，AIC等待相应通知后读取消息并执行计算。 |
| AIC向AIV反馈信息 | 目标L1 Buffer地址、下一轮任务参数、处理结果等。 | AIV等待消息准备完成通知，再使用地址或参数。 |
| AIC与单个AIV交换消息 | 一侧产生、另一侧消费的控制信息。 | 使用模式4分别协调AIC与AIV0、AIC与AIV1。 |
| AIC与两个AIV协同处理 | 两个AIV分别产生的参数，或AIC提供给两个AIV的参数。 | 根据依赖关系选择模式2的共同同步，或模式4的分别同步。 |
| 同一AI Core内两个AIV交换信息 | 放在约定区域的少量标量数据。 | 在共享SSBuffer的Mix任务中，使用模式1等匹配的同步方式保证读写顺序。 |
| 连续任务通信 | 按顺序产生的多条消息。 | 使用消息队列、处理完成确认和队列回绕，在有限空间中重复收发。 |

非MIX模式下每个核独占的SSBuffer空间不能按上述共享方式使用。不同AI Core之间、不同任务之间以及Host与Device之间的数据交换，也不应使用本节介绍的SSBuffer共享布局。

### 使用优势

相较于通过GM交换小消息，SSBuffer通信不需要将消息先写入GM再由另一侧读取，可以减少GM访问和相关带宽占用，降低小数据量核间通信的开销。与UB到L1 Buffer、L0C Buffer到UB等数据通路配合，可以减少融合算子的中间数据经过GM的次数。

### 使用方式选择

本节以开发者显式访问SSBuffer的通信方式为主。选择使用方式时，应先确定核间通信由开发者管理还是由Matmul框架管理。

**表2**  通信方式及接口边界

| 使用方式 | 适用场景 | 开发者需要处理的内容 |
| --- | --- | --- |
| 显式SSBuffer通信 | AIC/AIV需要交换自定义控制信息或少量标量数据。 | 获取SSBuffer基地址，自行规划消息布局、初始化、核间同步、空间复用及退出条件。 |
| 基于Matmul高阶API的CV融合 | AIV发起Matmul计算，并与Vector计算组合。 | 使用[REGIST_MATMUL_OBJ](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/REGIST_MATMUL_OBJ.md)等公开接口，由框架管理相应的KFC调用及消息。SSBuffer路径下不要直接修改内部消息结构。 |
| 基于资源管理API的核间通信 | 需要按资源管理API的编程方式组织Cube资源和通信队列。 | 遵守[KfcWorkspace](../../../../../api/SIMD-API/basic_api/cube_group_mgmt_ISASI/KfcWorkspace/constructor_and_destructor.md)及[CubeResGroupHandle](../../../../../api/SIMD-API/basic_api/cube_group_mgmt_ISASI/CubeResGroupHandle/CubeResGroupHandle_usage.md)的接口约束；KfcWorkspace接收的是GM消息空间，不是SSBuffer分配接口。 |

KFC（kernel function call）表示核间通信调用，并不专指SSBuffer通信。KfcWorkspace的消息空间需要满足GM地址对齐和清零要求，且不能与REGIST_MATMUL_OBJ同时使用。显式SSBuffer通信也不应直接使用框架内部的KFC消息类型或清理函数。

## 规格和约束

### 存储范围和访问约束

使用前应确认以下规格和约束：

**表3**  SSBuffer规格和约束

| 项目 | 说明 |
| --- | --- |
| 支持产品 | Ascend 950PR/Ascend 950DT，对应编译目标`dav-3510`。 |
| 总容量 | 3KB。 |
| 非MIX模式 | AIC、AIV0、AIV1各自独立占用1KB，不构成供三者共同访问的3KB工作区。 |
| `__mix__(1, 2)`模式 | AIC:AIV为1:2时，AIC、AIV0、AIV1共享整个3KB空间。 |
| 地址空间 | 使用`__ssbuf__`限定符。 |
| 访问方式 | 通过Scalar单元读写。 |
| 对齐要求 | SSBuffer只支持通过读写指令32字节的对齐访问。通信区域起始偏移、消息布局和访问范围需要满足该要求。 |
| 初始内容 | 可能存在脏数据，不保证为0。读取消息之前必须完成本次消息写入或必要的状态初始化。 |
| 任务约束 | AIC和AIV启动不同任务时，不能访问同一SSBuffer进行通信；任务配置应开启所需能力。 |
| 越界访问 | 访问超过可用空间的最末端地址会产生异常。除消息内容外，还需计入结构体填充、对齐、队列和反馈信息的空间。 |
| 并发写入 | 不同生产者应使用互不重叠的消息区域。同一消息空间在交给消费者后，生产者不能在消费完成前覆盖。 |

### 核函数与编译配置

使用[函数执行空间限定符](../../../../programming_guide/language_extension/simd_builtin_keywords.md#section1074418132518)明确核函数的执行方式。`__mix__(1, 2)`表示每组AIC/AIV按1:2配比运行，多组执行时，各组访问其各自的SSBuffer。工程化算子应配置与`__mix__(1, 2)`对应的[核函数类型](../../../../../api/SIMD-API/basic_api/Kernel-Tiling/set_Kernel_type.md)，即`KERNEL_TYPE_MIX_AIC_1_2`。

基础API及Matmul相关路径通过[ENABLE_CV_COMM_VIA_SSBUF](../../../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#内置编译宏开关)选择SSBuffer通信和UB到L1 Buffer硬通道，该开关默认关闭。编译配置示意如下：

```bash
bisheng <source_file>.asc -o <output_file> --npu-arch=dav-3510 -DENABLE_CV_COMM_VIA_SSBUF=true
```

需要区分以下情况：

- 使用基础API/Matmul的SSBuffer路径时，应配置该开关并使用匹配的Mix任务，不能只打开宏而忽略核函数类型。
- C API的`asc_copy_ub2l1`直接使用硬件通道，不通过该宏选择GM软件仿真路径。显式`__ssbuf__`指针访问和C API核间同步也不依赖Matmul注册来分配消息空间。
- 该开关不会为自定义消息划分空间、清理状态或插入完整收发协议，也不表示所有数据搬运API都需要打开它。

版本差异及数据通路的选择请参见[3510新特性使用说明](../../../../cross_gen_migration_guide/instructions_for_new_features/3510_new_features.md#section_ssbuf)。

### 基础API与C API的地址获取

在基础API中，[GetSsbufBaseAddr](../../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md)用于获取SSBuffer基地址，头文件为`"basic_api/kernel_operator_sys_var_intf.h"`，也可通过`"kernel_operator.h"`引入。基地址获取方式如下：

```cpp
// __ssbuf__表示指针指向SSBuffer地址空间，后续偏移计算保留该限定符。
__ssbuf__ void* base = AscendC::GetSsbufBaseAddr();
__ssbuf__ uint8_t* bytes = reinterpret_cast<__ssbuf__ uint8_t*>(base);
```

在NPU域中，该接口返回`__ssbuf__`地址空间的零地址。这里的零地址是SSBuffer的特殊基地址，不能按普通GM空指针判断是否获取失败。CPU调试域中，接口返回CPU模拟分配的地址，因此不要在需要CPU调试的基础API代码中将基地址固定为0。

C API当前没有与GetSsbufBaseAddr对应的基地址获取接口。在`dav-3510` NPU环境下，可以按以下示例的方式，将零地址转换为带`__ssbuf__`限定的结构体指针。

```cpp
// SsbufWorkspace的定义见下文；这里只取得地址，不进行动态内存分配。
__ssbuf__ SsbufWorkspace* workspace = reinterpret_cast<__ssbuf__ SsbufWorkspace*>(0);
```

### 消息结构与空间规划

开发者可以使用普通结构体描述消息布局，再用`__ssbuf__`结构体指针访问对应区域。结构体声明只定义布局，不额外分配SSBuffer。下面的结构与后续通信算子的消息队列相对应：

```cpp
struct alignas(128) CommMessage {
    volatile uint32_t head;
    volatile uint32_t reserved0;
    volatile uint64_t sequence;
    volatile uint64_t value;
    volatile uint64_t checksum;
    uint64_t reserved[12];
};

struct SsbufWorkspace {
    CommMessage queue[2][8]; // 两个AIV各使用一条深度为8的消息队列。
};
```

消息头用于保存功能号、有效标记和发送方编号；消息体保存序号、数据及校验值。`reserved`用于补齐消息长度。

上述布局中，每个AIV有一条深度为8的消息队列，每条队列占1024B，空间分布如下。

```text
SSBuffer字节偏移
0                         1024                      2048                 3072
| AIV0：8条消息，每条128B   | AIV1：8条消息，每条128B   | 本通信算子未使用     |
```

消息地址相对基址的偏移为`(sender * 8 + queueIndex) * sizeof(CommMessage)`。例如AIV1的第一个队列位置位于1024B处。SSBuffer没有规定消息必须为128B；改变消息大小或深度时，应重新检查起始地址对齐、每条消息的范围和全部区域的总占用。跨核轮询的状态应使用`volatile`等适合该访问方式的声明，确保重新读取共享状态。`volatile`不保证复合消息的原子性，也不能替代核间同步或保证不同流水的数据搬运已经完成。

不要用声明`__ssbuf__`数组的方式申请工作区。在后续样例采用的编译方式下，直接声明带`__ssbuf__`限定的数组对象或指向数组类型的指针不受支持，采用结构体成员数组描述布局。不支持的使用方式示例如下：

```cpp
__ssbuf__ uint8_t ssbuffer[3 * 1024]; // 不支持：数组对象只能位于默认的Local Memory。

using SsbufByteArray = uint8_t[3 * 1024];
__ssbuf__ SsbufByteArray* ssbuffer; // 不支持：指向数组类型的指针不能带该地址空间限定符。
```

如果希望用数组统一管理多条消息队列，可以声明一个位于默认Local Memory的**指针数组**，数组中的每个元素分别指向SSBuffer中的一段`CommMessage`空间。这种写法可以编译，但消息队列起始偏移需要开发者自行计算。

```cpp
__ssbuf__ CommMessage* queues[2] = {
    reinterpret_cast<__ssbuf__ CommMessage*>(0),
    reinterpret_cast<__ssbuf__ CommMessage*>(8 * sizeof(CommMessage))
};

queues[sender][queueIndex].head = make_head(sender);
```

这里的`queues`数组本身不在SSBuffer中，只有数组元素指向的`CommMessage`位于SSBuffer。与这种写法相比，`SsbufWorkspace::queue[2][8]`把结构体字段和队列偏移放在同一份类型定义中，不需要手动维护各消息队列的起始偏移，本样例采用结构体成员数组方式。

### 同步方式选择

基础API使用[CrossCoreSetFlag](../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md)和[CrossCoreWaitFlag](../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)配对进行核间同步，并通过模板参数选择同步模式。

C API同步方式见下表：

**表4**  C API中与SSBuffer共享访问相关的同步模式

| 模式 | 同步范围 | 在本节1:2场景中的使用方式 | 对应C API |
| --- | --- | --- | --- |
| 模式1 | 同一AI Core内的两个AIV。 | 两个AIV都上报后，各AIV再读取对方放入共享区域的数据。 | [asc_sync_subblock_arrive](../../../../../api/SIMD-API/c_api/sync/asc_sync_subblock_arrive.md)、[asc_sync_subblock_wait](../../../../../api/SIMD-API/c_api/sync/asc_sync_subblock_wait.md)。 |
| 模式2 | 同一AI Core内的AIC与所有AIV。 | AIC等待两个AIV都上报；或AIC上报后让两个AIV继续。 | [asc_sync_block_arrive](../../../../../api/SIMD-API/c_api/sync/asc_sync_block_arrive.md)、[asc_sync_block_wait](../../../../../api/SIMD-API/c_api/sync/asc_sync_block_wait.md)。 |
| 模式4 | 同一AI Core内的AIC与单个AIV。 | AIC与AIV0、AIC与AIV1分别通知和等待，适合独立队列收发。 | [asc_sync_intra_arrive](../../../../../api/SIMD-API/c_api/sync/asc_sync_intra_arrive.md)、[asc_sync_intra_wait](../../../../../api/SIMD-API/c_api/sync/asc_sync_intra_wait.md)。 |

## 编程范式

### 一次消息收发

显式SSBuffer通信需要明确：消息空间属于谁、谁写谁读、何时消息有效、何时可以复用，以及双方何时退出。推荐先建立单条消息的正确交接，再扩展到多条消息队列。

以下代码片段以使用基础API实现AIV0向AIC传递32B消息为例，展示模式4下的写入、读取和ACK。该片段位于同一个`__mix__(1, 2)`核函数内，`output`指向本组独占的4个`uint64_t` GM输出元素；AIV1不参与这次模式4通信。消息在通知前被完整写入，不依赖初始内容。

```cpp
struct alignas(32) SsbufferMessage { uint64_t value[4]; };
// __ssbuf__表示SSBuffer地址空间；示例独占从基址开始的32B区域。
auto message = reinterpret_cast<__ssbuf__ SsbufferMessage*>(AscendC::GetSsbufBaseAddr());
if ASCEND_IS_AIV {
    if (AscendC::GetSubBlockIdx() == 0) {
        for (uint32_t i = 0; i < 4; ++i) {
            message->value[i] = i + 1;
        }
        AscendC::CrossCoreSetFlag<4, PIPE_S>(0);
        AscendC::CrossCoreWaitFlag<4, PIPE_S>(1); // 等待AIC读完，之后才能覆盖该区域。
    }
}
if ASCEND_IS_AIC {
    AscendC::CrossCoreWaitFlag<4, PIPE_S>(0);
    for (uint32_t i = 0; i < 4; ++i) {
        output[i] = message->value[i];
    }
    AscendC::CrossCoreSetFlag<4, PIPE_S>(1); // 通知AIV0消息已读取。
}
```

该片段只表达消息交接；完整Kernel还需进行控制状态初始化、输出区域分配及必要的收尾同步。完整工程组织与多消息通信可参考[SSBuffer AIV/AIC通信基础 API样例](../../../../../../../examples/01_simd_cpp_api/05_best_practices/03_fusion_compute/ssbuf_aiv_aic_comm/README.md)，使用C API的实现可参考[SSBuffer AIV/AIC通信C API样例](../../../../../../../examples/02_simd_c_api/05_best_practices/03_fusion_compute/ssbuf_aiv_aic_comm)

### 连续消息的初始化、发布和确认

对于后续使用消息头和环形队列的算子，按以下步骤实现：

1. **规划空间**：为两个AIV划分互不重叠的队列，统一消息类型、队列深度、消息数量、同步ID和输出偏移。
2. **初始化状态**：由AIC作为唯一初始化方清理消息头，避免将旧有效位当成新消息。通过[InitSocState](../../../../../api/SIMD-API/basic_api/tool_interface/system_init/InitSocState.md)或C API的[asc_init](../../../../../api/SIMD-API/c_api/utils/sys_init/asc_init.md)恢复控制状态，不能代替自定义SSBuffer队列初始化。
3. **建立启动顺序**：AIC清理完成后分别通知AIV0、AIV1。两个AIV收到各自的启动通知后，才能开始向队列写入，避免初始化覆盖新消息。
4. **发布消息**：生产者先确保当前位置可写，写入本次消息体，再设置消息头中的有效标记，最后发送消息准备完成通知。
5. **消费消息**：AIC等待所选AIV的通知，读取消息头和消息体，校验功能号、发送方和序号，再处理消息。不能只看到有效标记就忽略协议要求的通知。
6. **确认完成**：AIC完成该消息所需的读取后清理消息头，向原发送方返回ACK。返回ACK前，必须保证后续操作不再依赖即将被覆盖的消息内容。
7. **复用或退出**：AIV在相应ACK到达后复用消息空间；发送结束后等待所有尚未确认的消息。AIC按双方约定的数量或结束消息退出，不能自行提前停止服务。

```text
AIC：清理消息头 → 发送启动通知 → 等待消息通知 → 读取并处理 → 清消息头 → 返回ACK
                         │             ↑                              │
                         ↓             │                              ↓
AIV：               等待启动通知 → 写消息并通知 → 使用下一位置/等待ACK → 复用
```

消息头有效位表示消息的协议状态，启动、消息准备完成和ACK是核间同步通知，两者需要按同一套规则使用。软件轮询消息头是另一种实现方式，读取循环必须持续访问共享状态，并处理发布顺序和退出条件；不能只删除通知和ACK等待，就把模式4通信算子改成正确的软件轮询方案。

### 队列回绕与退出条件

深度为8时，位置可以用`sequence % 8`计算；由于8是2的幂，也可以用`sequence & (8 - 1)`计算。队列回绕只复用空间，消息序号仍然递增。深度不是2的幂时不能沿用上述位与计算。

**表6**  每个AIV发送12条消息时的空间复用

| 消息序号 | 使用位置 | 所需条件 |
| --- | --- | --- |
| 0～7 | 0～7 | 队列已经初始化，可以首次写入。 |
| 8～11 | 0～3 | 分别等待序号0～3的ACK，再覆盖相应位置。 |
| 发送结束 | 无新写入 | 前4个ACK已经在回绕时消费，再等待剩余8个ACK后退出。 |

在该顺序收发方案中，每侧最多保留8条尚未确认的消息，消息准备完成及ACK通知的未消费数量也随收发节奏受到限制，不会通过无限发送来绕过计数器上限。

这一规则依赖AIC按同一队列的消息顺序处理并返回ACK。若允许乱序消费，一个没有位置编号的ACK不能直接证明“即将复用的这个位置”已经释放，需要重新设计确认方式。

发送数量小于队列深度时，只等待实际发送消息对应的ACK。对于上述每回绕一次先等待一个ACK的顺序收发方式，发送结束时需要等待的ACK数量为`min(消息总数, 队列深度)`。生产速度快于消费速度时，等待ACK是防止覆盖的正常行为；若双方使用了不一致的消息数量、ID映射或退出条件，则可能一直等待。

ACK在本协议中表示消息空间可复用，不默认表示消息指向的UB、L1 Buffer、GM数据也都可以覆盖。如果消息触发了异步搬运或矩阵计算，需要另外满足对应的输入消费和输出完成条件。

## 与数据搬运和Matmul高阶API配合

### 业务数据搬运

SSBuffer传递控制信息，大块数据使用相应搬运接口。根据CV融合算子的数据流选择如下通路，具体数据类型、格式、长度和对齐要求参见接口说明。

**表7**  相关数据通路

| 数据流 | 基础API | C API | 与通信的关系 |
| --- | --- | --- | --- |
| AIV UB → AIC L1 Buffer | [DataCopy连续搬运](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md)、[高维切分搬运](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_highdim_split.md)、[ND2NZ搬运](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_ND2NZ.md)、[DataCopyPad](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md)。 | [asc_copy_ub2l1](../../../../../api/SIMD-API/c_api/vector_datamove/asc_copy_ub2l1.md)。 | 可以先交换L1地址等参数，再通过硬通道搬运；消息已发布不表示输入已搬完。 |
| AIC L1 Buffer → AIV UB | [DataCopyL1ToUB](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md)。 | [asc_copy_l12ub](../../../../../api/SIMD-API/c_api/cube_datamove/asc_copy_l12ub.md)。 | 用于L1数据提供给Vector侧，仍需满足目的UB及读写同步约束。 |
| AIC L0C Buffer → AIV UB | [Fixpipe](../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)。 | [asc_copy_l0c2ub](../../../../../api/SIMD-API/c_api/cube_datamove/asc_copy_l0c2ub.md)。 | 将矩阵输出直接交给Vector处理，需要保证FIX输出完成后再消费。 |

基础API的UB到L1 Buffer硬通道路径与GM软件仿真路径不同：前者在相应配置下直接搬运，后者需要借助Matmul注册及GM中转空间。C API直接使用对应硬件接口，不能套用基础API兼容路径的注册要求。数据搬运能力也不意味着每次调用都要求用户另写一条SSBuffer消息，是否交换地址和参数取决于算子设计。

对应的实际数据流为：

```text
控制信息：AIV Scalar ↔ SSBuffer ↔ AIC Scalar
Vector作为Cube输入：UB → L1 Buffer → L0A/L0B Buffer → Cube计算
Cube作为Vector输入：Cube计算 → L0C Buffer → UB → Vector计算
```

### Matmul使用SSBuffer的方式

在普通的Matmul混合计算模式下，AIV侧客户端将调用信息交给AIC侧服务。使用SSBuffer路径时，框架会在SSBuffer中管理以下信息：

- **调用消息**：函数编号、实例编号、输入输出地址及形状等参数，通过消息队列传给AIC。
- **Tiling信息**：AIV通过专用区域传递矩阵分块参数，AIC在处理初始化消息时取得副本，再允许该区域复用。
- **L1 Buffer地址**：AIC在适用输入类型下发布A、B、bias和scale等资源的L1地址，AIV据此准备UB到L1 Buffer的数据搬运。

框架还通过核间事件协调L1空间复用、输入就绪和矩阵输出完成。消息区域释放、Tiling区域可复用、L1地址可用和输出计算完成是不同条件。开发者应按[SetTensorA](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorA.md)、[GetTensorC](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/GetTensorC.md)及[WaitIterateAll](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/WaitIterateAll.md)等接口的同步/异步约束使用结果，不能以某个共享标志代替这些接口的完成条件。

配套的搬运服务也会传递GM到L1 Buffer搬运所需的地址、块长和步长等描述，实际矩阵内容仍在数据通路上搬运。

SSBuffer相关路径还配合UB/TSCM输入、MX矩阵计算及scale输入等能力。例如[SetTensorScaleA](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleA.md)、[SetTensorScaleB](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleB.md)需要匹配支持该能力的配置。TSCM表示相关队列使用的L1 Buffer位置，不是SSBuffer中的矩阵存储区。设置UB/TSCM、AB共享、批量迭代或双主模式时，还需遵守对应Matmul配置的支持范围，不能认为开启SSBuffer后所有组合都适用。当前SSBuffer通信实现每个AIV使用8个命令位置，支持的Matmul对象数上限为4；命令位置数不等于对象数。UB来源的TSCM队列还会占用核间同步资源，当前框架对所用TSCM Buffer数与Matmul对象数之和检查上限为10，不能分别按各自最大值独立配置。

可结合以下样例了解不同用法：

- [MatmulLeakyRelu](../../../../../../../examples/01_simd_cpp_api/00_introduction/03_fusion_operation/matmul_leakyrelu_advanced_api)：矩阵计算与Vector激活融合，3510构建配置开启SSBuffer相关路径。
- [matmul_vecout](../../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_vecout)：以Vector侧VECOUT作为矩阵输入。
- [matmul_mx](../../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_mx)、[matmul_mx_scale_cache](../../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_mx_scale_cache)：MX计算及scale相关场景。
- [matmul_mx_ub_tscm_nz](../../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_mx_ub_tscm_nz)：UB、TSCM和NZ格式的MX矩阵输入。

下一节将以[基于SSBuffer的AIV/AIC通信算子](./operator_impl.md)为例，按算子分析、数据流分析、SSBuffer空间规划、通信规则设计和算子实现，说明两条环形队列及模式4通知如何配合使用。
