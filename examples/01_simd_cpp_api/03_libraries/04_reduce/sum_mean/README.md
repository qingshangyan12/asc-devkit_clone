# Sum/Mean 合并样例

## 概述

本样例演示了[Sum](../../../../../../docs/api/context/Sum.md) 和 [Mean](../../../../../../docs/api/context/Mean.md) 高阶API，对输入数据最后一个维度的元素（求和、求均值）。如果输入是向量，则对向量中各元素进行（求和、求均值）；如果输入是矩阵，则沿最后一个维度对每行中元素（求和、求均值），支持通过编译时宏切换不同场景。mean接口最多支持输入为二维数据，不支持更高维度的输入。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构说明

```
样例目录 sum_mean
├── scripts
│   ├── gen_data.py                    // 生成数据和真值数据的生成脚本
│   └── verify_result.py              // 验证输出数据和真值数据是否一致的验证脚本
├── CMakeLists.txt                    // 编译工程文件
├── data_utils.h                      // 数据读写工具函数
└── sum_mean.asc                      // Ascend C样例实例 & 主程序
```

## 场景详细说明

本样例通过编译时宏 `SCENARIO_NUM` 选择不同的计算场景，数据格式为 ND，核函数为 `sum_mean_custom`。

**场景1：Sum**
- 输入：[3, 32]，float元素，每行32个元素
- 输出：[8]，float元素，3个和值 + 5个padding（32字节对齐）
- 实现：`AscendC::Sum<float>(dstLocal, srcLocal, meanSumParams)`
- 说明：对每行元素求和，输出 dst[i] = sum(src[i,:])
- 对齐要求：输出张量在Global Memory上需要32字节对齐，因此输出shape从[3]对齐到[8]

**场景2：Mean**
- 输入：[3, 32]，float元素，每行32个元素
- 输出：[8]，float元素，3个均值 + 5个padding（32字节对齐）
- 实现：`AscendC::Mean<float, float>(dstLocal, srcLocal, tmplocalTensor, meanSumParams)`
- 说明：对每行元素求均值，输出 dst[i] = sum(src[i,:]) / n
- 对齐要求：输出张量在Global Memory上需要32字节对齐，因此输出shape从[3]对齐到[8]

**术语说明：**
- padding：为满足硬件对齐要求，在有效数据后填充的额外元素，padding值为0，不影响计算结果

## 样例规格

- 样例功能：  
   
  sum_mean_custom核函数对矩阵的每行元素求和（均值）。

<table border="2" align="center">
<caption>表1：样例规格</caption>
<tr><td rowspan="2" align="center">输入张量</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">src</td><td align="center">[3, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="2" align="center">输出张量</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">dst</td><td align="center">[1，8]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sum_mean_custom</td></tr>
</table>

- 样例实现：  
  本样例中实现的是固定shape为输入x[3, 32]，输出y[1，8]，其中y中的有效值数量为3，对输入x的每行元素求和（均值）后，输出y的有效数据为前3位，最后5位为padding填充的数据。。
  - Kernel实现

    计算逻辑是： 本样例将输入数据搬运进片上存储，然后使用Sum(mean)高阶API接口完成求和（均值）计算，得到最终结果，再搬出外部存储上。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 运行示例

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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

- 样例执行（NPU模式）
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO -DNPU_ARCH=dav-2201;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DRUN_MODE=cpu` 或 `-DRUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j; # cpu调试模式
  cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j; # NPU仿真模式
  ```

  若需详细了解CPU调试相关内容，请参考[03_cpudebug样例](../../../01_utilities/03_cpudebug)。

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构，dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |
  
- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```