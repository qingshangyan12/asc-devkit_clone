# b16数据类型下，基于load2d/load_data_with_transpose（待补充）/load2d（待补充）指令的性能测试样例

## 概述

下表描述了 12个测试场景，所有场景均使用 b16（16位）数据类型 和 64×64×64的矩阵尺寸，主要测试不同数据搬运路径和转置配置下的三种API实现方式的性能。

核心内容概括：
1. 数据搬运路径分类
    - 场景1-3: GM（全局内存）→ L1/L0A/L0B（直接加载）
    - 场景4-8: L1 → L0A（加载到A缓冲区）
    - 场景9-12: L1 → L0B（加载到B缓冲区）
2. 转置配置
    - 无转置: 场景1-5、9
    - 使能转置: 场景6-8、10-12

3. 三种API实现方式对比
    - load2d（场景1,2,3,4,6,9,10）: 基础2D数据搬运
    - load_data_with_transpose（场景7,11）: 带转置功能的专用API
    - load3d（场景5,8,12）: 3D数据搬运指令
<table>
    <tr>
        <td>scenario_num</td>
        <td>描述</td>
    </tr>
    <tr>
        <td>1</td>
        <td>M=64，N=64，K=64，b16，GM-&gt;L1，load2d</td>
    </tr>
    <tr>
        <td>2</td>
        <td>M=64，N=64，K=64，b16，GM-&gt;L0A，load2d</td>
    </tr>
    <tr>
        <td>3</td>
        <td>M=64，N=64，K=64，b16，GM-&gt;L0B，load2d</td>
    </tr>
    <tr>
        <td>4</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0A，load2d</td>
    </tr>
    <tr>
        <td>5</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0A，load3d</td>
    </tr>
    <tr>
        <td>6</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0A（使能转置），load2d</td>
    </tr>
    <tr>
        <td>7</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0A（使能转置），load_data_with_transpose</td>
    </tr>
    <tr>
        <td>8</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0A（使能转置），load3d</td>
    </tr>
    <tr>
        <td>9</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0B，load2d</td>
    </tr>
    <tr>
        <td>10</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0B（使能转置），load2d</td>
    </tr>
    <tr>
        <td>11</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0B（使能转置），load_data_with_transpose</td>
    </tr>
    <tr>
        <td>12</td>
        <td>M=64，N=64，K=64，b16，L1-&gt;L0B（使能转置），load3d</td>
    </tr>
</table>

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── loaddata
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── loaddata.asc                // Ascend C算子实现 & 调用样例
```

## 算子描述

输入A矩阵为Nz数据分形，shape为(m,k)，其中m为外轴，k为内轴
输入B矩阵为Nz数据分形，shape为(k,n)，其中k为外轴，n为内轴
根据传入scenarioNum取值的不同，算子会改变执行的逻辑。
12个测试场景（由宏 SCENARIO_NUM 控制）：

- 场景1-3：GM直连L1/L0A/L0B（基础加载）
- 场景4-8：L1→L0A路径（含转置测试）
- 场景9-12：L1→L0B路径（含转置测试）

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
  SCENARIO=1 SHAPEKEY=0 APIKEY=0
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO -DSHAPE_KEY=$SHAPEKEY -DAPI_KEY=$APIKEY;make -j;    # 编译工程
  ./demo                           # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```