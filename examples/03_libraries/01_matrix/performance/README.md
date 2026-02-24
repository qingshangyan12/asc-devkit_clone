# LoadData性能样例

## 概述

本样例包含了以下场景:
1. 输入数据类型为half, 调用Load2D指令进行GM->L1搬运
2. 输入数据类型为half，调用Load2D指令进行GM->L0A，GM->L0B搬运
3. 输入数据类型为half，调用Load2D指令进行L1->L0A搬运
4. 输入数据类型为half，调用Load2D指令进行L1->L0B搬运
5. 输入数据类型为int8，调用LoadDataWithTranspose指令进行L1->L0A搬运
6. 输入数据类型为int8，调用LoadDataWithTranspose指令进行L1->L0B搬运

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── performance
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── performance.asc            // Ascend C算子实现 & 调用样例
```

## 算子描述

<table border="2" align="center">
<caption>表1：scenarioNum不同取值的含义</caption>
  <tr>
    <td>scenarioNum</td>
    <td>指令</td>
    <td>通路</td>
    <td>输入数据类型</td>
    <td>是否使能转置</td>
  </tr>
  <tr>
    <td>1</td>
    <td>Load2D</td>
    <td>GM->L1</td>
    <td>half</td>
    <td>不转置</td>
  </tr>
  <tr>
    <td>2</td>
    <td>Load2D</td>
    <td>GM->L0A + GM->L0B</td>
    <td>half</td>
    <td>不转置</td>
  </tr>
  <tr>
    <td>3</td>
    <td>Load2D</td>
    <td>L1->L0A</td>
    <td>half</td>
    <td>不转置</td>
  </tr>
  <tr>
    <td>4</td>
    <td>Load2D</td>
    <td>L1->L0B</td>
    <td>half</td>
    <td>不转置</td>
  </tr>
  <tr>
    <td>5</td>
    <td>LoadDataWithTranspose</td>
    <td>L1->L0A</td>
    <td>int8_t</td>
    <td>转置</td>
  </tr>
  <tr>
    <td>6</td>
    <td>LoadDataWithTranspose</td>
    <td>L1->L0B</td>
    <td>int8_t</td>
    <td>转置</td>
  </tr>
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
  SCENARIO=1 M=64 K=64 N=64        # 设置变量
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;                # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N  # 生成测试输出数据
  msprof ./demo                    # 执行编译生成的可执行程序，执行样例并进行性能测试
  ```
