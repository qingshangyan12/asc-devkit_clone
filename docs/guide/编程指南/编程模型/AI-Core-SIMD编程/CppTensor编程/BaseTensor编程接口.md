LocalTensor原始形态提供基础的Tensor操作接口。

# 头文件引用

```cpp
// 基础API主入口
#include "basic_api/kernel_cube_intf.h"    // Cube计算接口
#include "basic_api/kernel_vec_intf.h"     // Vector计算接口

// 或单独引用
#include "basic_api/kernel_operator_data_copy_intf.h"  // 数据搬运
#include "basic_api/kernel_operator_vec_binary_intf.h" // Vector二元运算
#include "basic_api/kernel_operator_mm_intf.h"         // 矩阵计算
```

# LocalTensor创建

**方式1：通过QueAlloc创建（推荐）**：
```cpp
#include "basic_api/kernel_cube_intf.h"

// 步骤1：定义管道和队列
TPipe pipe;                          // 管道，管理数据流动
TQue<QuePosition::L1, 1> que;        // L1队列，存储位置为L1，队列深度为1

// 步骤2：在Process中分配Tensor
__aicore__ void Process() {
    // 从队列分配一个half类型的LocalTensor
    // AllocTensor会自动分配地址和大小，返回一个指向L1存储区的Tensor
    auto l1Tensor = que.AllocTensor<half>();
    
    // 使用Tensor进行计算...
    // 例如：读取数据、计算等
    
    // 步骤3：释放Tensor（重要！必须释放，否则队列会满）
    que.FreeTensor(l1Tensor);
}
```

**方式2：通过地址创建（手动管理）**：
```cpp
// 步骤1：定义存储区（__cbuf__表示L1存储区）
__cbuf__ half l1Buffer[1024];        // 在L1分配1024个half元素的空间

// 步骤2：创建LocalTensor指向这块存储区
// 方式A：指定位置、地址和大小
auto l1Tensor = LocalTensor<half>(TPosition::L1, l1Buffer, 1024);

// 方式B：只指定地址，后续设置大小
auto l1Tensor = LocalTensor<half>((uint32_t)l1Buffer);
l1Tensor.SetSize(1024);              // 必须手动设置大小
```

# 元素访问

**偏移量访问示例**：
```cpp
// === 读取元素 ===
half value = l1Tensor.GetValue(5);   // GetValue(5)：读取第5个元素（从0开始计数）

// === 写入元素 ===
half& ref = l1Tensor(10);            // operator()(10)：获取第10个元素的引用
ref = 2.0f;                          // 通过引用修改值：第10个元素变为2.0f

// === 创建子Tensor ===
auto subTensor = l1Tensor[16];       // operator[](16)：创建从偏移16开始的子Tensor
// subTensor现在指向l1Buffer[16]开始的数据，大小自动调整
// 例如：原Tensor大小1024，子Tensor大小为1024-16=1008
```

# 数据搬运（DataCopy）

**理解DataCopy**：将数据从GM搬运到内部存储（UB或L1），是算子的第一步。

**接口原型**：
```cpp
// GM → UB搬运（向量计算场景）
template <typename T>
void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, 
              const DataCopyParams& params);

// GM → L1搬运（矩阵计算场景，ND转NZ格式）
template <typename T>
void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src, 
              const Nd2NzParams& params);
```

**使用示例：GM → UB搬运**：
```cpp
// 步骤1：准备源数据（GM）
GlobalTensor<half> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ half*)gmAddr);  // 设置GM地址，指向输入数据

// 步骤2：准备目标存储（UB）
LocalTensor<half> ubTensor = queUB.AllocTensor<half>();  // 从UB队列分配空间

// 步骤3：配置搬运参数
DataCopyParams params;
params.blockCount = 1;      // 搬运块数：1块（一次性搬运）
params.blockLen = 128;      // 每块长度：128个元素
params.srcGap = 0;          // 源间隔：0（连续数据）
params.dstGap = 0;          // 目标间隔：0（连续存储）

// 步骤4：执行搬运
DataCopy(ubTensor, gmTensor, params);  // 将GM的128个half搬运到UB
```

**使用示例：GM → L1搬运（ND转NZ）**：
```cpp
// 场景：矩阵计算需要NZ格式，但输入数据是ND格式
// ND格式：行优先存储，如[0,0][0,1][1,0][1,1]
// NZ格式：分块存储，优化Cube计算效率

LocalTensor<half> l1Tensor = queL1.AllocTensor<half>();

// Nd2NzParams配置ND转NZ的参数
Nd2NzParams nzParams;
nzParams.ndNum = 1;              // ND矩阵数量：1个
nzParams.nValue = 16;            // 矩阵的N维度：16行
nzParams.dValue = 16;            // 矩阵的D维度：16列
nzParams.srcNdMatrixStride = 0;  // ND矩阵间间隔
nzParams.srcDValue = 16;         // ND矩阵的D值
nzParams.dstNzC0Stride = 16;     // NZ格式的C0步长
nzParams.dstNzNStride = 16;      // NZ格式的N步长
nzParams.dstNzMatrixStride = 256;// NZ矩阵总步长

DataCopy(l1Tensor, gmTensor, nzParams);  // 执行搬运+格式转换
```

# Vector计算（Add/Mul等）

**理解Vector计算**：在UB上执行的向量操作，如加法、乘法、激活函数等。

**接口原型**：
```cpp
// Level 2接口（简化版，推荐入门使用）
template <typename T>
void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src0, 
         const LocalTensor<T>& src1, const int32_t& count);

// Level 0接口（完整参数控制，用于性能优化）
template <typename T>
void Add(const LocalTensor<T>& dst, const LocalTensor<T>& src0, 
         const LocalTensor<T>& src1, uint64_t mask, uint8_t repeatTime, 
         const BinaryRepeatParams& params);
```

**使用示例：Level 2简化接口**：
```cpp
// 步骤1：准备三个Tensor：dst、src0、src1
LocalTensor<half> dstTensor = queUB.AllocTensor<half>();   // 结果存储
LocalTensor<half> src0Tensor = queUB.AllocTensor<half>();  // 第一个加数
LocalTensor<half> src1Tensor = queUB.AllocTensor<half>();  // 第二个加数

// 步骤2：执行加法
// dstTensor[i] = src0Tensor[i] + src1Tensor[i]，共计算128个元素
Add(dstTensor, src0Tensor, src1Tensor, 128);
```

**使用示例：Level 0完整参数控制**：
```cpp
// 场景：需要精确控制计算模式（如跨步计算、重复计算等）

// BinaryRepeatParams配置计算参数
BinaryRepeatParams params;
params.dstBlkStride = 1;      // 目标块内步长：每次移动1个元素
params.src0BlkStride = 1;     // 源0块内步长：每次移动1个元素
params.src1BlkStride = 1;     // 源1块内步长：每次移动1个元素
params.dstRepStride = 8;      // 目标重复步长：每次重复间隔8个块
params.src0RepStride = 8;     // 源0重复步长
params.src1RepStride = 8;     // 源1重复步长

// mask：掩码，控制哪些元素参与计算（0xFFFFFFFF表示全部参与）
// repeatTime：重复次数，4表示执行4次计算
Add(dstTensor, src0Tensor, src1Tensor, 0xFFFFFFFF, 4, params);
```
// Level 2简化接口
LocalTensor<half> dstTensor = queUB.AllocTensor<half>();
LocalTensor<half> src0Tensor = queUB.AllocTensor<half>();
LocalTensor<half> src1Tensor = queUB.AllocTensor<half>();

Add(dstTensor, src0Tensor, src1Tensor, 128);  // 计算128个元素

// Level 0完整参数控制
BinaryRepeatParams params;
params.dstBlkStride = 1;
params.src0BlkStride = 1;
params.src1BlkStride = 1;
params.dstRepStride = 8;
params.src0RepStride = 8;
params.src1RepStride = 8;

Add(dstTensor, src0Tensor, src1Tensor, 0xFFFFFFFF, 4, params);
```

# Cube计算（LoadData/Mmad）

**理解Cube计算**：矩阵乘加操作，数据流为 L1 → L0A/B → L0C → GM/UB。

**LoadData（L1 → L0A/L0B）**：
```cpp
// 场景：将矩阵数据从L1加载到L0A（或L0B），准备矩阵乘加

// 步骤1：准备源Tensor（L1）和目标Tensor（L0A）
LocalTensor<half> l1Tensor;         // L1中的矩阵数据（NZ格式）
LocalTensor<half> l0ATensor;        // L0A存储区（矩阵A输入）

// 步骤2：配置加载参数
LoadData2DParams params;
params.startIndex = 0;       // 起始索引：从第0个元素开始
params.repeatTimes = 1;      // 重复次数：加载1次
params.srcStride = 16;       // 源步长：每次跳16个元素
params.sid = 0;              // SID标识：0
params.dstGap = 0;           // 目标间隔：0
params.ifTranspose = false;  // 是否转置：不转置

// 步骤3：执行加载
LoadData(l0ATensor, l1Tensor, params);  // 将L1数据加载到L0A
```

**Mmad（矩阵乘加）**：
```cpp
// 场景：执行矩阵乘加 C = A * B

// 步骤1：准备三个Tensor
LocalTensor<float> l0CTensor;       // L0C：存储结果（float累加）
LocalTensor<half> l0ATensor;        // L0A：矩阵A（half）
LocalTensor<half> l0BTensor;        // L0B：矩阵B（half）

// 步骤2：配置矩阵乘加参数
MMadParams params;
params.m = 16;              // M维度：16行
params.k = 16;              // K维度：16列（A的列=B的行）
params.n = 16;              // N维度：16列
params.unitFlag = 0;        // 单元标志：0
params.cmatrixInitVal = 0;  // C矩阵初始值：0（从0开始累加）

// 步骤3：执行矩阵乘加
// 计算：l0CTensor = l0ATensor * l0BTensor（16x16矩阵乘法）
Mmad(l0CTensor, l0ATensor, l0BTensor, params);
```

**Fixpipe（L0C → GM/UB）**：
```cpp
// 场景：将L0C的矩阵乘加结果输出到GM或UB

// 步骤1：准备源Tensor（L0C）和目标Tensor（GM）
LocalTensor<float> l0CTensor;       // L0C中的结果
GlobalTensor<float> gmTensor;       // GM输出目标

// 步骤2：配置输出参数
FixpipeParams params;
params.nValue = 16;         // N维度：16
params.mValue = 16;         // M维度：16
params.kValue = 16;         // K维度：16
params.nz2ndEn = true;      // NZ转ND：开启（将NZ格式转为ND格式输出）

// 步骤3：执行输出
Fixpipe(gmTensor, l0CTensor, params);  // L0C → GM，同时格式转换
```

---
