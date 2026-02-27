# b8/b16/b32数据类型下，基于load2d指令的功能演示

## 概述

本样例展示了9种不同的数据搬运与转置配置场景（暂不支持非对齐场景），主要用于说明在矩阵运算中，使用load2d作为搬运指令，数据如何在不同的存储层级（GM、L1、L0A、L0B）之间流动，以及是否启用了随路转置功能。
<table>
    <thead>
        <tr>
            <th>scenarioNum</th>
            <th>数据类型</th>
            <th>GM->L1</th>
            <th>GM->L0A</th>
            <th>GM->L0B</th>
            <th>L1->L0A</th>
            <th>L1->L0B</th>
            <th>A矩阵使能转置</th>
            <th>B矩阵使能转置</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>1</td>
            <td>b8</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>2</td>
            <td>b8</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>3</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>√</td>
        </tr>
        <tr>
            <td>4</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
        </tr>
        <tr>
            <td>5</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>6</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
        </tr>
        <tr>
            <td>7</td>
            <td>b16</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>8</td>
            <td>b32</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>9</td>
            <td>b32</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
    </tbody>
</table>

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── load2d
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── load2d.asc                  // Ascend C算子实现 & 调用样例
```

## 算子描述

输入A矩阵为Nz数据分形，shape为(m,k)，其中m为外轴，k为内轴
输入B矩阵为Nz数据分形，shape为(k,n)，其中k为外轴，n为内轴
根据传入scenarioNum取值的不同，算子会改变执行的逻辑，具体可参考下表：
<table>
    <thead>
        <tr>
            <th>scenario_num</th>
            <th>介绍</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td><strong>1</strong></td>
            <td><strong>int8_t矩阵乘法</strong>：A、B矩阵均为int8类型，计算结果为int32。数据流路径为GM→L1→L0A/L0B，不启用矩阵转置。适用于需要高精度累积结果的低精度计算场景。</td>
        </tr>
        <tr>
            <td><strong>2</strong></td>
            <td><strong>int8_t直通模式</strong>：与场景1相同的数据类型组合(int8_t→int32_t)，但数据流路径为GM→L0A/L0B直通，跳过L1缓存。适用于对延迟敏感或L1资源受限的场景。</td>
        </tr>
        <tr>
            <td><strong>3</strong></td>
            <td><strong>half精度B矩阵转置</strong>：A、B矩阵均为half类型，计算结果为float。数据流路径GM→L1→L0，仅B矩阵启用转置。适用于输入B矩阵需要转置参与计算的情况。</td>
        </tr>
        <tr>
            <td><strong>4</strong></td>
            <td><strong>half精度A矩阵转置</strong>：A、B矩阵均为half类型，计算结果为float。数据流路径GM→L1→L0，仅A矩阵启用转置。适用于输入A矩阵需要转置参与计算的情况。</td>
        </tr>
        <tr>
            <td><strong>5</strong></td>
            <td><strong>half精度基础模式</strong>：A、B矩阵均为half类型，计算结果为float。数据流路径GM→L1→L0，双矩阵均不转置。是最常用的半精度矩阵乘法配置。</td>
        </tr>
        <tr>
            <td><strong>6</strong></td>
            <td><strong>half精度双转置</strong>：A、B矩阵均为half类型，计算结果为float。数据流路径GM→L1→L0，A和B矩阵同时启用转置。适用于输入A、B都需要转置的特殊计算需求。</td>
        </tr>
        <tr>
            <td><strong>7</strong></td>
            <td><strong>half精度直通模式</strong>：与场景5相同的数据类型组合(half→float)，但数据流路径为GM→L0A/L0B直通，跳过L1缓存。验证直通路径在半精度下的性能表现。</td>
        </tr>
        <tr>
            <td><strong>8</strong></td>
            <td><strong>float精度基础模式</strong>：A、B矩阵均为float类型，计算结果为float。数据流路径GM→L1→L0，不启用转置。适用于单精度浮点矩阵乘法标准场景。</td>
        </tr>
        <tr>
            <td><strong>9</strong></td>
            <td><strong>float精度直通模式</strong>：与场景8相同的数据类型组合(float→float)，但数据流路径为GM→L0A/L0B直通，跳过L1缓存。验证浮点计算在直通路径下的正确性与性能。</td>
        </tr>
    </tbody>
</table>
  

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIO=1 M=64 K=64 N=64
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```