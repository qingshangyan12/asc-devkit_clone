# mul 样例

## 概述

本样例演示了在 SIMD（单指令多数据）计算场景下，基于 Ascend C RegBase 编程范式的向量乘法实现方法。RegBase 是 Ascend C 提供的一种底层编程接口，允许开发者直接操作寄存器级别的计算资源，适用于需要精细控制计算流程和高性能的场景。

**应用背景**：在矩阵运算、信号处理等场景中，经常需要对数据进行逐元素的累乘操作。例如，在矩阵行方向累乘场景中，每一行的计算依赖于上一行的结果，需要在计算过程中插入同步操作来保证数据依赖关系的正确性。

**技术特点**：
- 使用 `AscendC::Reg` 命名空间下的寄存器操作接口
- 支持向量数据的对齐加载和存储（`LoadAlign`/`StoreAlign`）
- 提供写-读同步机制（`LocalMemBar`）保证数据依赖

**本样例功能**：输入一个形状为 [64, 64] 的矩阵，沿行方向进行逐行累乘操作。由于每一行的计算依赖于上一行的结果，代码中通过 `LocalMemBar` 插入写-读同步，确保前一行结果写入后再进行下一行计算。

## 支持的产品

- Ascend 950PR
- Ascend 950DT

## 目录结构介绍

```
├── mul
│   ├── scripts
│   │   └── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   └── mul.asc                        // Ascend C 样例实现 & 调用代码
```

## 样例描述

### 样例功能

本样例输入一个形状为 [64, 64] 的矩阵，沿行方向执行逐行累乘操作。计算逻辑为：

```
第0行：保持不变
第1行：第1行 = 第0行 * 第1行
第2行：第2行 = 第1行(已更新) * 第2行
...
```

### 样例规格

<table>
<tr><td rowspan="1" align="center">样例类型</td><td colspan="3" align="center">AIV（AI Vector）样例</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">x</td><td align="center">[64, 64]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[64, 64]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mul</td></tr>
</table>

### 实现原理

样例实现主要包括以下步骤：

1. **初始化参数**：设置矩阵的行数（outer=64）和列数（inner=64）
2. **数据搬运**：将数据从全局内存（GM）搬运到统一缓冲区（UB）
3. **寄存器计算**：调用 `MulVF` 函数进行逐行累乘，结果写回 UB
4. **结果搬运**：将计算结果从 UB 搬运回 GM

**关键点 - 写-读同步**：

由于每一行的累乘依赖于前一行的计算结果，在代码中通过 `AscendC::Reg::LocalMemBar<VEC_STORE, VEC_LOAD>()` 插入同步屏障。该同步操作的含义是：等待所有向量存储操作完成后，才能执行后续的向量加载操作，确保数据依赖关系的正确性。

**寄存器操作流程**：

```cpp
// 1. 创建掩码寄存器（控制参与计算的元素）
AscendC::Reg::MaskReg mask = AscendC::Reg::CreateMask<T, MaskPattern::ALL>();

// 2. 创建寄存器张量
AscendC::Reg::RegTensor<T> srcReg, dstReg;

// 3. 从 UB 对齐加载数据到寄存器
AscendC::Reg::LoadAlign(srcReg, srcAddr);
AscendC::Reg::LoadAlign(dstReg, dstAddr);

// 4. 执行乘法计算
AscendC::Reg::Mul(dstReg, dstReg, srcReg, mask);

// 5. 将结果对齐存储回 UB
AscendC::Reg::StoreAlign(dstAddr, dstReg, mask);

// 6. 插入写-读同步
AscendC::Reg::LocalMemBar<VEC_STORE, VEC_LOAD>();
```

### 调用方式

使用内核调用符 `<<<>>>` 调用核函数：

```cpp
mul<<<numBlocks, nullptr, stream>>>(xDevice, yDevice);
```

## 详细描述

相关 API 参考手册：
- [AscendC::Reg::Mul](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2/apisearch/aclpubdev/topics/api_cann_reg_mul.html) - 寄存器级向量乘法
- [AscendC::Reg::LoadAlign](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2/apisearch/aclpubdev/topics/api_cann_reg_loadalign.html) - 对齐加载
- [AscendC::Reg::StoreAlign](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2/apisearch/aclpubdev/topics/api_cann_reg_storealign.html) - 对齐存储
- [AscendC::Reg::LocalMemBar](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2/apisearch/aclpubdev/topics/api_cann_reg_localmembar.html) - 内存屏障同步
- [AscendC::Reg::CreateMask](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/80RC2/apisearch/aclpubdev/topics/api_cann_reg_createmask.html) - 创建掩码

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

### 配置环境变量

请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

- 默认路径，root 用户安装 CANN 软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非 root 用户安装 CANN 软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径 install_path，安装 CANN 软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

```bash
# 创建并进入 build 目录
mkdir -p build && cd build

# 编译工程
cmake .. && make -j

# 生成测试输入数据（数据量为 64 * 64 = 4096 个 float 元素）
python3 ../scripts/gen_data.py

# 执行编译生成的可执行程序
./demo
```

### 执行结果

执行成功后，输出如下结果表示精度对比通过：

```bash
ReadFile golden.bin success!
CompareResult golden.bin success!
test pass!
```

## 注意事项

1. **数据对齐要求**：RegBase 接口中的 `LoadAlign` 和 `StoreAlign` 要求数据地址必须对齐，不满足对齐要求时需使用非对齐版本的接口
2. **同步机制**：当存在数据依赖时，必须通过 `LocalMemBar` 插入同步，否则可能导致计算结果错误
3. **数据量限制**：本样例的数据量为 4096 个元素（64 * 64），实际使用时需根据 UB 容量合理设置
