# MrgSort样例

## 概述

本样例展示接口[Sort32](../../../../../../docs/api/context/Sort32.md)与[MrgSort](../../../../../../docs/api/context/MrgSort.md)在排序场景下的配合使用。首先调用Sort32将数据并行地预处理为多个有序子序列；随后调用MrgSort，将这些子序列合并为一个全局有序的结果。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── mrg_sort
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── mrg_sort.asc            // Ascend C 样例实现 & 调用
```

## 样例描述

- 样例功能：  

  本样例使用Sort32与MrgSort实现输入数据的排序。样例规格如下表所示：

  <table border="2" align="center">
  <caption>表1：MrgSort样例规格</caption>
  <tr><th align="center">样例类型(OpType)</th><th colspan="5" align="center">MrgSort</th></tr>  
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">default</td></tr>  
  <tr><td align="center">x</td><td align="center">[1, 128]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>  
  <tr><td align="center">y</td><td align="center">[1, 128]</td><td align="center">uint32</td><td align="center">ND</td><td align="center">\</td></tr>  
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">out</td><td align="center">[1, 256]</td><td align="center">float</td><td align="center">ND</td><td align="center">\</td></tr>  
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">vec_mrgsort_kernel</td></tr>  
  </table>

- 样例实现：  

  本样例中实现的是固定shape为输入x[1, 128]、y[1, 128]，输出out[1, 256]的MrgSort样例。

  Compute任务分为两个阶段：首先调用Sort32接口将128个元素按每32个一组进行降序排序（共4组），排序结果以(score, index)交替结构存储在workLocal中，形成4条已排序的队列；然后构造MrgSort4Info参数和MrgSortSrcList源列表，调用MrgSort接口将4条队列合并为1条全局有序的队列，结果存储在dstLocal中。


## 编译运行

在本样例根目录下执行如下步骤，编译并运行样例。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
