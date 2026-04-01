# Sum/Mean 合并样例

## 概述

本样例演示了归约接口在多场景下的使用方法，包括 [Sum](../../../../../../docs/api/context/Sum.md) 和 [Mean](../../../../../../docs/api/context/Mean.md) 高阶API。这些接口对LocalTensor的每行元素进行归约计算（求和、求均值），支持通过编译时宏切换不同场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构说明

```
样例目录 sum-mean
├── scripts
│   ├── gen_data.py                    // 生成数据和真值数据的生成脚本
│   └── verify_result.py              // 验证输出数据和真值数据是否一致的验证脚本
├── CMakeLists.txt                    // 编译工程文件
├── data_utils.h                      // 数据读写工具函数
└── sum-mean.asc                      // Ascend C算子实例 & 主程序
```

## 场景详细说明

本样例通过编译时宏 `SCENARIO_NUM` 选择不同的归约计算场景，数据格式为 ND，核函数为 `sum_mean_custom`。

**场景1：Sum**
- 输入：[3, 32]，float元素，每行32个元素
- 输出：[8]，float元素，3个和值 + 5个padding（32字节对齐）
- 实现：`AscendC::Sum<float>(dstLocal, srcLocal, meanSumParams)`
- 说明：对每行元素求和，输出 dst[i] = sum(src[i,:])

**场景2：Mean**
- 输入：[3, 32]，float元素，每行32个元素
- 输出：[8]，float元素，3个均值 + 5个padding（32字节对齐）
- 实现：`AscendC::Mean<float, float>(dstLocal, srcLocal, tmplocalTensor, meanSumParams)`
- 说明：对每行元素求均值，输出 dst[i] = sum(src[i,:]) / n

## 算子规格

<table border="2" align="center">
<caption>表1：算子规格</caption>
<tr><td rowspan="2" align="center">输入张量</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">src</td><td align="center">[3, 32]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="2" align="center">输出张量</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">dst</td><td align="center">[8]</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sum_mean_custom</td></tr>
</table>

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

- 样例执行
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  执行结果如下，说明样例对比成功：
  ```bash
  test pass!
  ```
