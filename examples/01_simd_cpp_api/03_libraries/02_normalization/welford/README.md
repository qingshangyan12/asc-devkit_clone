# Welford融合算子样例

## 概述

本样例基于Kernel直调算子工程，介绍了在一个核函数中连续调用WelfordUpdate和WelfordFinalize高阶API，实现完整的Welford在线算法。Welford是一种在线计算均值和方差的方法，可以在单次数据遍历中逐步计算均值和方差，减少访存次数，提高计算性能。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── welford
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── welford.asc            // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  本样例在一个核函数中依次调用WelfordUpdate和WelfordFinalize高阶API，实现完整的Welford在线算法。Welford算法优势：
  - 可以在不存储所有样本的情况下，逐步计算所有样本的均值和方差
  - 只需对数据进行一次遍历，减少访存次数，提高计算性能
  - WelfordUpdate：在线更新均值和方差，计算公式为 $M_t = M_{t-1} + (x_t - M_{t-1}) / n$, $S_t = S_{t-1} + (x_t - M_{t-1}) \times (x_t - M_t)$
  - WelfordFinalize：汇总多个块的结果得到最终均值和方差，计算公式为 $Mean = \frac{\sum(M_i \cdot n_i)}{\sum n_i}$, $Var = \frac{\sum(S_i + (Mean_i - Mean)^2 \cdot n_i)}{\sum n_i}$

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center"> welford </td></tr>

  <tr><td rowspan="4" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">srcGm</td><td align="center">[RN, AB]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">inMeanGm</td><td align="center">[RN, AB]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">inVarGm</td><td align="center">[RN, AB]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="6" align="center">算子输出</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">outMeanGm</td><td align="center">[RN, AB]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">outVarGm</td><td align="center">[RN, AB]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">finalMeanGm</td><td align="center">[8]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">finalVarGm</td><td align="center">[8]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">welford_coop_custom</td></tr>
  </table>

- 算子实现：  
  本样例实现固定shape的融合Welford算子（RN=1, AB=64）。

  本样例使用 **融合计算**模式，在单个核函数中调用两个API：
  - 1. WelfordUpdate：对每个数据块计算局部均值和方差
  - 2. WelfordFinalize：汇总所有块的均值和方差，得到全局均值和方差

  Welford算法公式已在算子功能中描述，详细说明请参考 [WelfordUpdate API文档](../../../../../docs/api/context/WelfordUpdate.md) 和 [WelfordFinalize API文档](../../../../../docs/api/context/WelfordFinalize.md)。

  - Kernel实现

    本样例在一个核函数内部实现了完整的Welford在线算法：
    - 1. CopyIn：将全局内存数据搬运到片上
    - 2. WelfordUpdate计算：对输入数据计算局部统计量
    - 3. WelfordFinalize计算：汇总所有块的统计量，得到最终均值和方差
    - 4. CopyOut：将结果搬出到全局内存

  - Tiling实现

    算子的tiling实现流程如下：
    1. 分别获取WelfordUpdate和WelfordFinalize接口能完成计算所需的最大/最小临时空间大小
    2. 根据该范围结合实际的内存使用情况设置合适的空间大小
    3. 将两个tiling参数整合到一个融合tiling结构体中

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- 样例执行
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DNPU_ARCH=dav-2201 ..;make -j;             # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 模式时，添加 `-DRUN_MODE=cpu` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j; # cpu调试模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```