# 单算子 API 调用

> 类型：基础

单算子 API 调用方式，是指直接调用单算子 API 接口，基于 C 语言的 API 执行算子。算子工程创建完成后，基于工程代码框架完成算子原型定义、kernel 侧算子实现、host 侧 tiling 实现，通过工程编译脚本完成算子的编译部署，之后再进行单算子 API 的调用。

## 基本原理

完成自定义算子编译后，会自动生成单算子 API，可以直接在应用程序中调用。

单算子 API 的形式一般定义为"两段式接口"，形如：

```c
aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *src, ..., aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);
aclnnStatus aclnnXxx(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream);
```

其中 `aclnnXxxGetWorkspaceSize` 为第一段接口，用于计算 workspace 内存需求；`aclnnXxx` 为第二段接口，执行实际计算。Xxx 代表算子原型注册时传入的算子类型。

## API 命名规则对照表

**表 1** API 命名规则对照表

| 场景 | 第一段接口参数 | 示例 |
|------|--------------|------|
| 可选输入 | name + Optional 后缀 | xOptional |
| 输入输出同名 | 去掉 const + Ref 后缀 | xRef（输入输出指向同一块 Device 内存时，输出需要去掉 const 修饰以允许写入） |
| 单个输出 | out | out |
| 多个输出 | name + Out 后缀 | yOut, y1Out |
| 含属性 | 介于输入输出之间 | negativeSlope |

**示例代码**：

```c
// 可选输入
aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *xOptional, ..., aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);

// 输入输出同名
aclnnStatus aclnnXxxGetWorkspaceSize(aclTensor *xRef, ..., uint64_t *workspaceSize, aclOpExecutor **executor);

// 单个输出
aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *src, ..., aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);

// 多个输出
aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *src, ..., aclTensor *yOut, aclTensor *y1Out, ..., uint64_t *workspaceSize, aclOpExecutor **executor);

// 含属性
aclnnStatus aclnnXxxGetWorkspaceSize(const aclTensor *x, double negativeSlope, aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);
```

## 前置步骤

在调用单算子 API 之前，需要完成算子的设计、实现、编译和部署。完整流程请参考[快速入门](../快速入门.md)或按[概述](../概述.md)中的阅读路径逐步完成。

编译部署时需要确保开启了二进制编译功能（`ENABLE_BINARY_PACKAGE` 为 `True`，默认已开启）。

算子编译部署后，会在算子包安装目录下生成单算子调用的头文件（`op_api/include/aclnn_<xx>.h`）和动态库（`op_api/lib/libcust_opapi.so`）。部署后的完整目录结构请参考[编译与部署基本流程 — 部署后的目录结构](../编译与部署/基本流程.md#部署后的目录结构)。

## 准备验证代码工程

代码工程目录结构如下：

```
├── CMakeLists.txt    // 编译规则文件
├── main.cpp          // 单算子调用主体流程实现文件
```

您可以单击 [LINK](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/01_invocation/aclnn_invocation)，获取样例工程的完整样例。

## 单算子调用流程

单算子 API 执行流程如下：

**图 1** 单算子 API 执行接口调用流程
![](../../../../../figures/单算子API执行接口调用流程.png "单算子API执行接口调用流程")

调用流程分为以下关键步骤（以 AddCustom 算子为例）：

> [!NOTE] 说明
> 算子工程编译时，会自动在 `build_out/autogen` 目录下生成对应的 API 头文件，编写调用代码时需要包含该头文件。例如：
> ```c
> #include "aclnn_add_custom.h"
> ```

### 1. 初始化与资源准备

```cpp
// 初始化 ACL
CHECK_ACL(aclnnInit(nullptr));

// 设置设备并创建 Stream
CHECK_RTL(aclrtSetDevice(deviceId));
aclrtStream stream;
CHECK_RTL(aclrtCreateStream(&stream));
```

### 2. 准备数据与创建 Tensor

```cpp
// 申请 device 内存并拷贝输入数据
void *inputDeviceMem = nullptr;
CHECK_RTL(aclrtMalloc(&inputDeviceMem, bufferSize, ACL_MEM_MALLOC_HUGE_FIRST));
CHECK_RTL(aclrtMemcpy(inputDeviceMem, bufferSize, hostData.data(),
                      bufferSize, ACL_MEMCPY_HOST_TO_DEVICE));

// 创建 Tensor 描述
aclTensor *input = aclCreateTensor(shape, numDims, aclDataType::ACL_FLOAT,
                                   nullptr, 0, aclFormat::ACL_FORMAT_ND,
                                   aclFormat::ACL_FORMAT_ND,
                                   inputDeviceMem, bufferSize);
```

### 3. 两段式接口调用（核心步骤）

```cpp
// 第一段：计算 workspace 大小
uint64_t workspaceSize = 0;
aclOpExecutor *executor = nullptr;
CHECK_ACL(aclnnAddCustomGetWorkspaceSize(input0, input1, output,
                                         &workspaceSize, &executor));

// 按 workspaceSize 申请内存（如有需要）
void *workspaceDeviceMem = nullptr;
if (workspaceSize > 0) {
    CHECK_RTL(aclrtMalloc(&workspaceDeviceMem, workspaceSize, ACL_MEM_MALLOC_HUGE_FIRST));
}

// 第二段：执行算子
CHECK_ACL(aclnnAddCustom(workspaceDeviceMem, workspaceSize, executor, stream));

// 同步等待执行完成
CHECK_RTL(aclrtSynchronizeStream(stream));
```

### 4. 结果获取与资源释放

```cpp
// 将输出拷贝回 host 并验证结果
CHECK_RTL(aclrtMemcpy(hostOutput.data(), bufferSize, outputDeviceMem,
                      bufferSize, ACL_MEMCPY_DEVICE_TO_HOST));

// 释放资源
aclDestroyTensor(input0);
aclDestroyTensor(input1);
aclDestroyTensor(output);
aclDestroyAclOpExecutor(executor);
// ... 释放 device 内存、stream 等
aclnnFinalize();
```

> 如需完整可运行的 main.cpp 和 CMakeLists.txt 代码，请参考[快速入门 - 步骤7](../快速入门.md)。

## 编译与运行

### CMakeLists.txt 关键配置

```cmake
# 头文件搜索路径：CANN 基础头文件 + 算子 API 头文件
target_include_directories(execute_add_op PRIVATE
    $ENV{ASCEND_HOME_PATH}/include
    $ENV{ASCEND_OPP_PATH}/vendors/customize/op_api/include
)

# 链接库
target_link_libraries(execute_add_op PRIVATE
    cust_opapi    # 算子动态库
    nnopbase
    acl_rt
)
```

### 编译运行步骤

```bash
# 加载 CANN 环境变量（ASCEND_HOME_PATH、ASCEND_OPP_PATH 等会自动设置）
source /usr/local/Ascend/cann/set_env.sh

# 编译
mkdir -p build && cd build
cmake .. -DCMAKE_SKIP_RPATH=TRUE
make

# 运行
./output/execute_add_op
```

程序输出 `test pass` 表示调用成功。

## 相关文档

- [快速入门](../快速入门.md) - 创建算子工程
- [算子工程编译](../编译与部署/基本流程.md) - 编译算子
- [算子包部署](../编译与部署/基本流程.md) - 部署算子包
