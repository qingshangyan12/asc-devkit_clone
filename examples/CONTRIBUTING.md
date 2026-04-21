# 华为 Ascend C 样例写作规范

## 1. 概述
本规范旨在为样例编写人员提供统一写作标准，确保样例代码和文档质量。
按照目录、相关文档、内容规范进行总结，覆盖CMake、README、代码内容并针对规范提供便于遵循的示例说明。

## 2. 样例名称规范

样例名称是用户识别样例功能的第一要素。为确保命名清晰、统一，必须遵循以下规则：

```
cube_{大特性}_{小特性}    示例：  cube_matmul_basic    cube_conv_basic		 
vector_{大特性}_{小特性}  示例：  vector_elemwise_add  vector_reduce_sum
mix_{大特性}_{小特性}
```
* cube是表示样例核函数仅在Cube核上执行
* vector是表示样例核函数仅在Vector核上执行
* mix是表示样例核函数同时在Cube核核Vector核上执行

## 3. 目录结构规范

### 3.1 基本目录结构

样例一般包含以下目录和文件：

```
├── CMakeLists.txt        # CMake编译配置文件（必选）
├── data_utils.h          # 数据读写函数文件（可选）
├── *.asc                 # Ascend C 样例实现文件（必选）
├── *.h                   # 头文件（可选）
├── README.md             # 样例说明文档（必选）
├── scripts/              # 测试脚本目录（不推荐，用户需依赖Python环境。简单、小规模样例，无需复杂验证相关内容直接写入*.asc文件，如：sinh样例。数据量大、多场景样例复用数据可继承此内容，如：matmul_high_performance样例）
│   ├── gen_data.py       # 生成测试输入和golden数据
│   └── verify_result.py  # 验证输出结果
├── figure/               # 示意图（可选）
│   ├── *.png             # README.md展示的图片文件
```
样例参考：[sinh](./01_simd_cpp_api/03_libraries/12_math/sinh)、[matmul_high_performance](./01_simd_cpp_api/04_best_practices/01_matrix_compute_practices)。

### 3.2 目录文件说明
#### CMakeLists.txt

`CMakeLists.txt`文件作为工程构建的核心脚本，是确保代码能够跨架构顺利编译的关键。为统一编译行为，该文件必须包含以下规范条目：

```cmake
# 声明（必选，固定写法）
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) xxxx Huawei Technologies Co., Ltd.
# ... 
# ----------------------------------------------------------------------------------------------------------

# CMake 最低版本（必选，固定写法）
cmake_minimum_required(VERSION 3.16)

# 查找并加载文件（必选，固定写法）
find_package(ASC REQUIRED)

# 定义项目名称及包含语言（必选，固定写法）
set(PROJECT_NAME "kernel_samples" CACHE STRING "Project name")
project(${PROJECT_NAME} LANGUAGES ASC CXX)

# 设置多场景参数，默认为1（可选，单个用例可包含多种实现场景。如：当Reduce样例中存在多种场景WholeReduceMin/WholeReduceMax/WholeReduceSum等，代码中分支控制运行场景）
# SCENARIO_NUM: 场景编号
#   1 - WholeReduceMax：每个repeat内求最大值及索引，输入[1, 1024]，输出[1, 16]
#   2 - WholeReduceMin：每个repeat内求最小值及索引，输入[1, 1024]，输出[1, 16]
#   ...
set(SCENARIO_NUM "1" CACHE STRING "Scene number (1-n)")
add_definitions(-DSCENARIO_NUM=${SCENARIO_NUM})

# 构建目标配置文件（必选，固定写法，单个文件单独占一行）
set(TARGET_FILENAME "demo" CACHE STRING "Execution file name")
add_executable(${TARGET_FILENAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/*.asc   （必选）
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp   （可选）
)

# 目标文件的按需链接库列表（可选，要求最小化链接列表，单个链接库独占一行，如下常用链接库）
target_link_libraries(${TARGET_FILENAME} PRIVATE
        tiling_api  # asc，tiling函数库。当代码中需要调用高阶API中Tiling类或接口时需要添加此链接库。
        register    # asc，tiling_api依赖库。
        platform    # asc，tiling_api间接依赖库。代码中需要调用"tiling/platform/platform_ascendc.h"获取平台信息时添加此链接库。
        m           # 系统标准，数学库。当代码中需要<cmath>中函数时添加此链接库。
        dl          # 系统标准，动态链接器库。当需要在程序中使用动态链接库相关的函数（dlopen、dlsym、dlclose、dlerror等）时，需要将 dl 库链接到目标中，以确保程序能够正常编译和运行。
)

# 设置NPU架构，默认为A3架构（必选，固定写法）
set(CMAKE_ASC_ARCHITECTURES "dav-2201" CACHE STRING "NPU ARCH, e.g. dav-2201, dav-3510")

# 设置编译选项（必选）
target_compile_options(${TARGET_FILENAME} PRIVATE
        # 针对 ASC 语言，传入架构参数
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=${CMAKE_ASC_ARCHITECTURES}>
)
```

#### README.md

为确保文档的可读性与一致性，`README.md`必须严格遵循标准化模板，文件必须按顺序包含以下章节：

- 名称（必选）
```
示例1：归约类接口多场景示例
示例2：Matmul最佳实践样例
```
- 概述（必选）  
 简洁说明样例的主要功能、特点，或是要解决的问题，达成的目的，1-2段即可。
```
示例1：
本样例介绍归约类接口在多种场景下的使用方法，包括WholeReduceMax、WholeReduceMin、WholeReduceSum、RepeatReduceSum，以及WholeReduceMax/Min配合GetReduceRepeatMaxMinSpr获取全局极值及索引的使用方法。 这些接口用于对LocalTensor中每个repeat内所有元素进行归约运算（求最大值、最小值或求和），归约结果存放到目的LocalTensor中。

注：GetReduceRepeatMaxMinSpr为 CANN 9.0.0 重命名后的 API。CANN 8.5.0 及之前版本请使用GetReduceMaxMinCount。

示例2：
本样例基于Matmul 高阶API实现矩阵乘法运算，通过9个递进式的优化case展示从基础实现到高性能优化的完整调优路径，包括单核基础版本、Tiling优化、多核并行切分、MDL模式、L1Cache/L2Cache优化、常量Tiling、UnitFlag优化等多种优化手段。
```
- 支持产品（必选）  
  列出所有支持的硬件产品，按产品系列分类。
```
示例：
 - Ascend 950PR/Ascend 950DT
 - Atlas A3 训练系列产品/Atlas A3 推理系列产品
 - Atlas A2 训练系列产品/Atlas A2 推理系列产品
```
- 目录结构介绍（必选）  
  使用树状结构展示目录，并为每个文件/目录添加注释说明（README文件排除）。
```
示例：
├── sinh
│   ├── CMakeLists.txt      // 编译工程文件
│   └── sinh.asc            // Ascend C算子实现 & 调用样例
```
- 样例描述（必选）
  - 样例功能   
    描述样例实现的具体功能、对应场景、实现原理（需要添加实现原理图示）。
    ```
    示例1：
        计算公式：C = A * B
            A、B为源操作数，A为左矩阵，形状为[M, K]；B为右矩阵，形状为[K, N]
            C为目的操作数，存放矩阵乘结果的矩阵，形状为[M, N]
    示例2：
        场景1：WholeReduceMax
            输入：[1, 1024]个half元素，mask=128，repeat=8（1024/128）
            输出：[1, 16]个half元素（8个最大值 + 8个索引，交错存储）
            实现：WholeReduceMax<half>(dstLocal, srcLocal, mask=128, repeat=8, 1, 1, 8)
            说明：使用默认order（ORDER_VALUE_INDEX），输出按 [max0, idx0, max1, idx1, ...] 交错存储
    
        场景2：WholeReduceMin
            输入：[1, 1024]个half元素，mask=128，repeat=8（1024/128）
            输出：[1, 16]个half元素（8个最小值 + 8个索引，交错存储）
            实现：WholeReduceMin<half>(dstLocal, srcLocal, mask=128, repeat=8, 1, 1, 8)
            说明：使用默认order（ORDER_VALUE_INDEX），输出按 [min0, idx0, min1, idx1, ...] 交错存储
    ```
  - 样例规格（必选）  
    使用表格展示输入输出规格 。 
    示例：
      <table border="2" align="center">
      <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="5" align="center">MatmulCustom</td></tr>
      <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">isTrans</td></tr>
      <tr><td align="center">a</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">false</td></tr>
      <tr><td align="center">b</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">true</td></tr>
      <tr><td rowspan="1" align="center">样例输出</td><td align="center">c</td><td align="center">[8192, 8192]</td><td align="center">float16</td><td align="center">ND</td><td align="center">-</td></tr>
      </table>
  - 样例实现（必选）
    * 实现流程：说明样例实现过程（必选，可通过流程图或者代码架构图说明）。
    * 约束条件：列出需要满足的条件（可选）。
    ```
    示例：
    实现流程
        1、通过GenerateTiling实现host侧的Tiling计算
        2、通过CalcGMOffset完成分核计算
        3、通过Iterate接口完成矩阵乘计算
        4、通过LeakyRelu接口完成激活函数计算
    ```
- 编译运行（必选）
  - 编译选项说明（必选）  
    使用表格说明可用的编译选项及其含义。
    ```
    示例：
    |          选项               | 可选值                        | 说明                                    |
    | `CMAKE_ASC_RUN_MODE`       | `npu`（默认）、`sim`、`cpu`    | 运行模式：NPU运行、simulator仿真、CPU Debug |
    | `CMAKE_ASC_ARCHITECTURES`  | `dav-2201`（默认）、`dav-3510` | NPU 架构，dav-2201对应 Atlas A2/A3系列，dav-3510 对应Ascend 950PR/Ascend 950DT |
    ```
  - 配置环境变量（必选）  
    提供不同安装路径的环境变量配置命令。
    ```
    示例：
    请根据当前环境上CANN开发套件包的[安装方式]（相对路径），选择对应配置环境变量的命令。
    - 默认路径，root用户安装CANN软件包
      source /usr/local/Ascend/cann/set_env.sh

    - 默认路径，非root用户安装CANN软件包
      source $HOME/Ascend/cann/set_env.sh

    - 指定路径install_path，安装CANN软件包
      source ${install_path}/cann/set_env.sh
    ```
  - 样例执行（必选）  
    提供完整的编译运行步骤，包括以下内容：
      - 创建并进入 build 目录（必选）
      - cmake 编译命令（必选）
      - 生成测试数据（可选）
      - 执行可执行程序（必选）
      - 验证结果（必选）
      - 对于多场景样例，说明如何设置场景参数(可选)
      - cpu运行模式说明（必选）
      - sim运行模式说明（必选）
      - 框架执行日志查看链接到utilities的log样例（必选）
    ```
    示例1（带场景选项）：
    样例执行
      SCENARIO_NUM=1
      mkdir -p build && cd build;      # 创建并进入build目录
      cmake .. -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
      python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据
      ./demo                           # 执行编译生成的可执行程序，执行样例
      python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
    执行结果如下，说明精度对比成功。
      test pass!
    框架执行日志：
      如果在执行过程中遇到问题，可以通过查看框架日志获取详细的错误信息或运行追踪。关于日志的配置、级别设置以及获取方式，请参考[log]样例指南。
    
    
    示例2（cpu运行模式选项）：
    样例执行
      mkdir -p build && cd build;          # 创建并进入build目录
      cmake -DRUN_MODE=cpu ..; make -j;    # 编译工程
      ./demo                               # 执行样例
    执行结果如下，说明精度对比成功。
      [Success] Case accuracy is verification passed.
    进入GDB模式调试 在上述指令中"./demo"前加入"gdb --args"，再次执行指令即可进入GDB模式。调试时需要选择子进程进行调试。
      mkdir -p build && cd build;
      cmake -DRUN_MODE=cpu ..; make -j;
      gdb --args ./demo
    ```
    
#### 性能优化样例特别规范

性能优化样例在README.md`样例实现`内容，采用递进式说明（参考：[matmul_high_performance](./01_simd_cpp_api/04_best_practices/01_matrix_compute_practices)）。  
从基础算子功能实现开始到最后一步优化步骤结束，逐步引入优化手段。
- 每个优化步骤都是单独的场景Case，每个Case包含如下内容：
  - 优化目标：说明该优化步骤的目标
  ```
  示例：
  
  优化目标：启用UnitFlag优化，并行化计算与搬运流水
  ```
  - 核心实现：说明具体的实现方法
  ```
  示例：
  
  核心实现：
    - 使用24核并行计算，切分策略同上（4x6）
    - 使用MDL模板并优化L1 Cache参数同上
    - L2 Cache策略同上
    - 使能Tiling全量常量化同上
    - 启用UnitFlag，优化计算与搬运的并行度
  ```
  - 关键代码：展示核心代码片段（建议10行内）
  ```
  示例：
  
  关键代码：
    // 使用constexpr定义编译期常量，并启用UnitFlag
    MatmulConfig mmCFG = GetMMConfig<MatmulConfigMode::CONFIG_MDL>(shapeParams);
    mmCFG.enUnitFlag = true;
  ```
  - 优化手段：详细说明优化策略，尽可能添加示意图将原理说明白
  ```
  示例：
  
  优化手段：
  - 启用UnitFlag优化：
    - 优化计算与搬运的并行度：未开启UnitFlag功能时，AIC核的MMAD计算指令和FIXPIPE数据搬运指令是指令级别的同步，FIXPIPE指令需要等MMAD指令执行完才进行结果搬出，MMAD和FIXPIPE之间流水串行；开启UnitFlag功能后，MMAD和FIXPIPE是512B大小的细粒度同步，在一条MMAD指令执行过程中，每当完成一个512B数据结果的计算，FIXPIPE立即搬出该512B的数据，从而实现Cube计算单元和FIXPIPE搬运单元流水并行
  
  本示例示意图参考：matmul_high_performance README.md 内容
  ```
  - 性能数据：提供性能指标数据表格
  ```
  示例：
  
  性能数据：
  | Task Duration(μs) | Block Num | aicore_time(μs) | aic_mac_time(μs) | aic_mac_ratio | aic_scalar_time(μs) | aic_scalar_ratio | aic_mte1_time(μs) | aic_mte1_ratio | aic_mte2_time(μs) | aic_mte2_ratio | aic_fixpipe_time(μs) | aic_fixpipe_ratio |
  |-------------------|-----------|-----------------|------------------|---------------|---------------------|------------------|-------------------|----------------|-------------------|----------------|----------------------|-------------------|
  | 4012.44           | 24        | 3559.06         | 3076.396         | 0.864         | 1026.069            | 0.288            | 2533.512          | 0.712          | 3435.584          | 0.965          | 272.576              | 0.077             |
  
  ```
  - 分析：解释以上性能数据结果，可分不同性能指标说明优化效果，同时可给出下一步优化方向
  ```
  示例：
  
  分析：
    - 相比Case 7，Task Duration从4053.44μs降低到4012.44μs
    - aic_fixpipe_time从45.76μs提高到272.576μs，原因为开启Unitflag后，aic_fixpipe_time会包含FIXPIPE指令的等待时间，并不是真正流水的耗时，用户可关注端到端性能是否提升
    - 当前性能收益较小的原因为制约性能的仍为MTE2 bound，算子的MMAD、FIXPIPE流水被MTE2 bound掩盖。当样例因FIXPIPE阻塞性能较差时，用户可启用UnitFlag功能
  ```
- 性能对比总结：说明总体优化效果、计算和带宽分析说明，展示所有Case性能指标表格
```
示例：

Atlas A2训练系列芯片性能数据
综合优化效果：
- 本样例cube利用率提升了67.7%(18.7% → 86.4%)，已达到芯片峰值算力的86.4%
- 通过Case 0到Case 8的递进优化，样例耗时降幅达99.47%(759363.98μs → 4012.44μs)

*本示例详情参考：matmul_high_performance README.md 内容
```
- 调优建议：总结样例调优过程
```
示例：
调优建议：
1、从小规模开始：先使用单核基础版本验证功能正确性
2、逐步优化：按照case顺序逐步引入优化手段，观察性能提升
3、多核切分策略：合理设置多核切分策略，避免同地址访问
4、利用MDL模式：MDL模式提供了高度优化的实现，优先使用
5、L2Cache：L2Cache能进一步缓解MTE2 bound，对于需重复读取的数据推荐使用
6、常量Tiling：对于固定shape的场景，使用常量Tiling减少Scalar运行时开销
7、UnitFlag优化：启用UnitFlag可以并行化计算与搬运流水
```

#### .asc 实现文件
```c++
/** 1、单文件版权声明信息（必选）
 * Copyright (c) xxxx Huawei Technologies Co., Ltd.
 * ......
*/
 
/** 2、头文件包含顺序及内容（必选）
 * - 标准库头文件
 * - Ascend C头文件 —— ASC头文件包含精细化，禁止使用大范围通用头文件，只包含实际使用的具体头文件，如：kernel_operator.h、tiling/tiling_api.h，ASC API资料会添加所在头文件，编码人员可据此按需添加
 * - rts 头文件
 * - 其他类、辅助函数，utils、cpu_debug等
 * - 项目内头文件
 *  示例：
*/
// c/c++标准库
#include <random> 
// asc api头文件（最小引用原则）
#include "kernel_tensor.h"
// rts api头文件
#include "acl/acl.h"
// 其他类、辅助函数，utils、cpu_debug等
#ifdef ASCENDC_CPU_DEBUG
 #include "cpu_debug_launch.h"
#endif
// 项目内头文件
#include "*.h"

/** 3、常量定义。必须使用 `constexpr` 定义编译期常量，命名使用 全大写+下划线。
 * 示例：
*/
 constexpr uint32_t CUBE_BLOCK = 16;
 constexpr uint32_t TILE_LENGTH = 256;

/** 4、Kernel类定义
 * 
 * - 类名使用大驼峰形式
 * - 构造函数和关键方法使用 `__aicore__ inline` 限定符
 * - 成员顺序：public方法 → private方法 → private方法和数据成员
 * 示例：
*/
template <uint32_t M, uint32_t K, uint32_t N>
class KernelMatmul {
public:
    __aicore__ inline KernelMatmul() {}
    __aicore__ inline void Init(GM_ADDR a, GM_ADDR b, GM_ADDR c)
    {
        AscendC::InitSocState();
        aGM.SetGlobalBuffer((__gm__ half*)a);
        bGM.SetGlobalBuffer((__gm__ half*)b);
        cGM.SetGlobalBuffer((__gm__ half*)c);
    }
    __aicore__ inline void Process() { /* ... */ }
private:
    __aicore__ inline void CopyIn() { /* ... */ }
    __aicore__ inline void CopyOut() { /* ... */ }
    /* ... */

    AscendC::GlobalTensor<half> aGM;
    AscendC::GlobalTensor<half> bGM;
    AscendC::GlobalTensor<half> cGM;
};

/** 5、核函数入口定义
 * - 使用 `__global__` 限定符
 * - 函数名使用小写字母 + 下划线，建议添加 `_custom` 后缀
 * 示例：
*/
__global__ __cube__ void matmul_custom(GM_ADDR a, GM_ADDR b, GM_ADDR bias, GM_ADDR c, GM_ADDR workspace, GM_ADDR tilingGm)
{
    // 实现代码
}

/** 5、Golden data
 * 示例：
*/
struct GoldenData {
    std::vector<float> src;
    std::vector<float> golden;
};
GoldenData gen_golden_data()
{
    // 实现代码，返回golden data结构体数据
}

/** 6、Compare result
 * 示例：
*/
static bool CompareResult(const void* outputData, const void* goldenData, uint32_t outSize)
{
    // 实现代码，返回比较结果
}
```