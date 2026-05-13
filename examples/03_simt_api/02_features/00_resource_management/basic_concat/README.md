# SIMT编程模式实现Concat算子样例

## 概述

样例基于Ascend C SIMT编程方式实现简单场景的Concat算子，将两个输入张量沿指定维度拼接为一个输出张量，展示连续/分段内存访问类算子的SIMT开发方法。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── basic_concat
│   ├── CMakeLists.txt
│   ├── concat.asc             # SIMT实现concat调用样例
|   └── README.md
```

## 算子描述

- 算子功能：  
  concat算子将两个同rank输入张量沿指定dim拼接，除拼接维度外，其他维度长度必须一致。以二维张量为例：

  当`dim = 0`时：
  
  ```
  output[i][j] = input0[i][j], i < input0_shape[0]
  output[i][j] = input1[i - input0_shape[0]][j], i >= input0_shape[0]
  ```

  当`dim = 1`时：
  
  ```
  output[i][j] = input0[i][j], j < input0_shape[1]
  output[i][j] = input1[i][j - input0_shape[1]], j >= input0_shape[1]
  ```

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">concat</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input0</td><td align="center">2 * 4 / 3 * 2</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">input1</td><td align="center">3 * 4 / 3 * 4</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">5 * 4 / 3 * 6</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">concat_custom</td></tr>
  </table>

- 数据切分：  
  * 最大线程数：2048线程/Block
  * Block数量：根据输出元素数量和AI Vector Core数量自动计算
  * 单线程处理：通过grid-stride循环处理一个或多个输出元素

- 算子实现：  
  concat算子的实现流程为从Global Memory读取input0和input1。核函数根据当前线程处理的输出线性位置反推多维坐标，判断该坐标沿dim轴落在input0还是input1范围内，计算对应输入偏移后写入output。

- 调用实现：  
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
  cmake ..; make -j;            # 编译工程
  ./concat                      # 执行样例
```
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
