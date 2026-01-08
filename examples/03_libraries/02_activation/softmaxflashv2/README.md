# SoftmaxFlashV2样例
## 概述
本样例基于Kernel直调算子工程，介绍了调用SoftmaxFlashV2高阶api实现softmaxflashv2单算子，SoftmaxFlash增强版本，对应FlashAttention-2算法。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── softmaxflashv2
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── softmaxflashv2.asc      // Ascend C算子实现 & 调用样例
```

## 算子描述
- 算子功能：  
  softmaxflashv2单算子，将输入tensor[m0, m1, ...mt, n]（t大于等于0）的非尾轴长度相乘的结果看作m，则输入tensor的shape看作[m, n]。对输入tensor[m,n]按行做如下计算，不同的update值对应不同的计算公式，其中x、inmax和insum为输入，M、S、E均为输出。
  update为false：  
  $$
  \begin{align*}
  M&=\text{rowmax}(x_i)\\
  \text{SoftmaxFlashV2}(z_i)&=\exp(x_i - M)\\
  S&=\sum_{i}^{n} \exp(x_i - M)
  \end{align*}
  $$
  update为true：  
  $$
  \begin{align*}
  M&=\max(\text{rowmax}(x_i), \text{inmax})\\
  \text{SoftmaxFlashV2}(z_i)&=\exp(x_i - M)\\
  E&=\exp(\text{inmax}_i - M)\\
  S&=\sum_{i}^{n} \exp(x_i - M)+E \cdot \text{insum}
  \end{align*}
  $$
- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">  </td></tr>

  <tr><td rowspan="5" align="center">算子输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">64*128 </td><td align="center">float</td><td align="center">ND</td></tr>
   <tr><td align="center">max</td><td align="center"> 64*8 </td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">sum</td><td align="center"> 64*8 </td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="2" align="center">算子输出</td></tr>
  <tr><td align="center">dst</td><td align="center"> 64*128 </td><td align="center">float</td><td align="center">ND</td></tr>


  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">softmaxflashv2_custom</td></tr>
  </table>

- 算子实现：  
  本样例中实现的是固定shape为输入src[64, 128]，max[64, 8]，sum[64, 8] 输出dst[64, 128]的softmaxflashv2算子。

  - kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用SoftmaxFlashV2高阶API接口完成softmaxflashv2计算，得到最终结果，再搬出到外部存储上。

    softmaxflashv2算子的实现流程：先将Global Memory上的输入Tensor src1Gm、inMaxGm、inSumGm 搬运至Local Memory，存储在srcLocal、insumLocal、inmaxLocal，然后执行softmaxflashv2计算，计算结果存储在dstLocal中，最后将输出数据dstLocal搬运至Global Memory上的输出Tensor dstGm中。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/ascend-toolkit/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/ascend-toolkit/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/ascend-toolkit/set_env.sh
    ```
    
- 样例执行
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass
  ```