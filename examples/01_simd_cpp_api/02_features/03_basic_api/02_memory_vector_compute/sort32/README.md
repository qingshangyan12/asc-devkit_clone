# Sort32样例

## 概述

本样例基于Sort32实现对128个元素进行排序操作。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── sort32
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── sort32.asc              // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  
  本样例以每32个输入x中的元素和32个输入y中的元素为一次迭代，分4次迭代对128个输入x元素和128个输入y元素进行排序。按输入x进行排序（输入x中大的排前面），排序好的结构体存储在dst中。api详细介绍请参考[Sort32](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0231.html)。
- 算子规格：  
  <table>  
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">sort32</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center"></td></tr>  
  <tr><td align="center">x</td><td align="center">[1,128]</td><td align="center">float</td><td align="center">ND</td><td align="center"></td></tr>  
  <tr><td align="center">y</td><td align="center">[1,128]</td><td align="center">uint32_t</td><td align="center">ND</td><td align="center"></td></tr>  
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">[1,256]</td><td align="center">float</td><td align="center">ND</td><td align="center"></td></tr>  
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">sort32_custom</td></tr>  
  </table>

- 算子实现：  
  - Kernel实现

    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Sort32基础API接口完成计算，得到最终结果，再搬出到外部存储上。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

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