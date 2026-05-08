# Aclrtc运行时编译 + 静态Tensor编程 Add 样例

## 概述

本样例展示了如何使用 Ascend C 的 **Aclrtc（运行时编译）** 模式，在 Host 侧动态编译核函数源码并执行。核函数内部采用 **静态 Tensor 编程范式**（LocalMemAllocator + SetFlag/WaitFlag 事件同步），将核函数源码以字符串形式嵌入 Host 代码中，通过 aclrtc API 在运行时编译并执行。

与传统自定义算子工程不同，Aclrtc 无需提前编译算子包（.run），适合快速验证和原型开发。

## 支持的产品

- Atlas A2 训练系列产品/Atlas A2 推理系列产品（A2，dav-2201）
- Atlas A3 训练系列产品/Atlas A3 推理系列产品（A5，dav-3510）

## 目录结构介绍

```
├── add_custom_template_global
│   ├── scripts
│   │   ├── gen_data.py                  // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py             // 真值对比文件
│   ├── data_utils.h                     // 数据读入写出函数 + CHECK_ACL 宏
│   ├── run.sh                           // 一键编译运行脚本
│   ├── README.md                        // 本文档
│   └── add_custom_template_global.cpp   // Host 侧代码（含 RTC 核函数源码字符串）
```

## 样例描述

Add 计算公式：

```
z = x + y
```

### 样例规格

| 项目 | 值 |
|------|-----|
| 样例类型(OpType) | Add |
| 输入 x | [8, 1024], float, ND |
| 输入 y | [8, 1024], float, ND |
| 输出 z | [8, 1024], float, ND |
| 核函数名 | Kernel::add_custom\<float\> |
| 使用核数 | 8 |
| 数据总量 | 8 \* 1024 |

### Aclrtc 工作流程

```
┌──────────────────────────────────────────────────────┐
│ 1. 构造核函数源码字符串 (src)                          │
│    ↓                                                 │
│ 2. aclrtcCreateProg()   → 创建编译程序                │
│    ↓                                                 │
│ 3. aclrtcAddNameExpr()  → 注册需要导出的核函数名       │
│    ↓                                                 │
│ 4. aclrtcCompileProg()  → 运行时编译，生成设备二进制   │
│    ↓                                                 │
│ 5. aclrtcGetBinData()   → 获取编译产物 (deviceELF)    │
│    ↓                                                 │
│ 6. aclrtcGetLoweredName() → 获取核函数的 mangled name │
│    ↓                                                 │
│ 7. aclrtBinaryLoadFromData() → 加载二进制到设备       │
│    ↓                                                 │
│ 8. aclrtLaunchKernelWithConfig() → 启动核函数         │
│    ↓                                                 │
│ 9. 同步 & 拷回结果                                    │
└──────────────────────────────────────────────────────┘
```

### 核函数实现

核函数以字符串形式嵌入 Host 代码中，采用 **模板 + 命名空间 + `__global__`** 声明方式：

```cpp
namespace Kernel {
template<typename T>
__mix__(0,1) __global__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z)
{
    AscendC::InitSocState();
    KernelAdd op;
    op.Init(x, y, z);
    op.Process();
}
}
```

**KernelAdd 类**：
- `Init()`：通过 `SetGlobalBuffer` 将每个核的 Global Memory 指针偏移到对应分块
- `Process()`：使用 `LocalMemAllocator` 分配三个 LocalTensor，通过 `DataCopy` 将数据从 Global Memory 搬运到 Local Memory，经 `SetFlag/WaitFlag` 事件同步后执行 `Add`，最后将结果拷回 Global Memory

代码片段：

```cpp
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, BLOCK_LENGTH>();
AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, BLOCK_LENGTH>();
AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, BLOCK_LENGTH>();

AscendC::DataCopy(xLocal, xGm, BLOCK_LENGTH);
AscendC::DataCopy(yLocal, yGm, BLOCK_LENGTH);
AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

AscendC::Add(zLocal, xLocal, yLocal, BLOCK_LENGTH);
AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

AscendC::DataCopy(zGm, zLocal, BLOCK_LENGTH);
```

### Host 侧流程

1. **源码构造**：将 Ascend C 核函数源码以字符串形式嵌入 C++ 代码
2. **运行时编译**：调用 `aclrtcCreateProg` → `aclrtcAddNameExpr` → `aclrtcCompileProg` 完成编译
3. **二进制提取**：通过 `aclrtcGetBinData` 获取设备 ELF，通过 `aclrtcGetLoweredName` 获取 mangled name
4. **设备加载与启动**：使用 `aclrtBinaryLoadFromData` 加载二进制，`aclrtLaunchKernelWithConfig` 启动核函数
5. **数据流**：Host 从 `input/` 目录读取输入文件 → 拷贝到 Device → 启动核函数 → 结果拷回 Host → 写入 `output/` 目录

## 编译运行

### 前置条件

- 已安装 CANN 开发套件包并配置环境变量
- 当前环境有可用的 Ascend NPU 设备

### 配置环境变量

请根据当前环境上 CANN 开发套件包的安装方式，选择对应配置环境变量的命令：

```bash
# 默认路径，root 用户安装
source /usr/local/Ascend/cann/set_env.sh

# 默认路径，非 root 用户安装
source $HOME/Ascend/cann/set_env.sh
```

### 方式一：使用 run.sh 一键运行

```bash
# 默认 A2 平台
bash run.sh

# 指定 A5 平台
bash run.sh A5
```

执行成功后输出：

```
================ Aclrtc Sample: add_custom_template_global (A2) ================
test pass!
================ Result: PASS ================
```

### 方式二：手动分步执行

```bash
cd add_custom_template_global

# 1. 生成测试输入数据和 golden
python3 scripts/gen_data.py

# 2. 编译 Host 程序
g++ -DACLRTC_NPU_ARCH="\"dav-2201\"" add_custom_template_global.cpp \
    -I. \
    -I$ASCEND_HOME_PATH/include -L$ASCEND_HOME_PATH/lib64 \
    -lascendcl -lacl_rtc -o main

# 3. 运行核函数
./main

# 4. 精度校验
python3 scripts/verify_result.py output/output.bin output/golden.bin

# 5. 清理
rm -f main && rm -rf input output
```

### 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `ACLRTC_NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构 |

## 与自定义算子工程的对比

| 对比项 | 自定义算子工程 | Aclrtc |
|--------|---------------|--------|
| 编译时机 | 提前编译（msopgen + cmake + make） | 运行时编译（aclrtcCompileProg） |
| 产物形式 | .run 算子包，部署到 CANN 环境 | 内存中的 deviceELF，直接启动 |
| 适用场景 | 生产部署，框架集成 | 快速验证，原型开发，动态算子 |
| 依赖 | op_host 侧 tiling 代码 | 无需 tiling，参数直接传入 |
