# GELU样例

## 概述

本样例基于 `gelu_high_performance` 样例 Case 1 的思路，展示一个简化版的 GELU 向量计算实现。样例使用 RegBase API 和 VF 融合调用方式完成 GELU 计算，并固定使用 2 个 Vector 核，仅按 M 方向分核。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

``` 
├── gelu
│   ├── scripts
│   │   ├── gen_data.py     // 输入数据和真值数据生成脚本
│   │   └── verify_result.py // 输出结果和真值数据校验脚本
│   ├── CMakeLists.txt      // 编译工程文件
│   ├── data_utils.h        // 数据读入写出函数
│   └── gelu.asc            // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  本样例实现 GELU 近似计算，采用 `gelu_high_performance` 样例 README 中的方式2公式：
  $$
  GELU(x) \approx \frac{x}{1 + e^{-1.595769 \cdot x - 0.071405 \cdot x^3}}
  $$

- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">GELU</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[256, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[256, 1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">gelu_custom</td></tr>
  </table>

- 分核逻辑：
  - 总核数固定为 2 个 Vector 核
  - 仅按 M 方向分核，`totalM = 256`，`singleCoreM = 128`
  - 每个核负责 `128 x 1024` 大小的数据块，不做 N 方向切分

- 样例实现：
  - Kernel 实现  
    本样例参考 `gelu_high_performance` 的 Case 1，使用 RegBase API 和 `asc_vf_call` 调用 VF 函数，在寄存器中完成方式2的 8 步 GELU 计算。

    GELU 计算步骤如下：
    1. `y = x^2`
    2. `y = x^3`
    3. `y = -0.071405 * x^3`
    4. `tmp = -1.595769 * x`
    5. `y = tmp + y`
    6. `y = exp(y)`
    7. `y = 1 + y`
    8. `y = x / y`

    单核内部按 tile 分块处理，每次从 GM 搬运 `tileLen = 4096` 个元素到 UB，完成 RegBase 计算后再写回 GM。

- 调用实现  
  使用内核调用符 `<<<>>>` 调用核函数，启动 2 个 Vector 核执行计算。

  Host 侧不再内联生成输入和 golden，而是从 `./input/input_x.bin` 读取输入数据，执行完成后把结果写入 `./output/output.bin`，再由校验脚本与 `./output/golden.bin` 做精度比对。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上 CANN 开发套件包的安装方式，选择对应配置环境变量的命令。

  - 默认路径，root 用户安装 CANN 软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非 root 用户安装 CANN 软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径 `install_path`，安装 CANN 软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  使用 NPU 仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：`dav-3510` 对应 Ascend 950PR/Ascend 950DT |


- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
