# Brcb样例
## 概述
本样例基于Brcb实现数据填充的功能，可用于输入一个张量，每一次取输入张量中的8个数填充到结果张量的8个DataBlock中。

本样例通过Ascend C编程语言实现了Brcb算子，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。

下面主要记录了5种不同的测试场景（scenario），用于评估某种计算系统或处理器在不同条件下的性能表现。

<table>
    <tr>
        <td>scenarioNum</td>
        <td>数据类型</td>
        <td>描述</td>
    </tr>
    <tr>
        <td>1</td>
        <td>half</td>
        <td>【基础半精度功能测试】目的操作数连续写出模式</td>
    </tr>
    <tr>
        <td>2</td>
        <td>half</td>
        <td>【基础半精度功能测试】目的操作数非连续写出模式</td>
    </tr>
    <tr>
        <td>3</td>
        <td>half</td>
        <td>【半精度无业务性能测试】目的操作数连续写出模式</td>
    </tr>
    <tr>
        <td>4</td>
        <td>half</td>
        <td>【半精度无业务性能测试】bank conflict的场景</td>
    </tr>
    <tr>
        <td>5</td>
        <td>float</td>
        <td>【单精度无业务性能测试】目的操作数连续写出模式</td>
    </tr>
</table>

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
## 目录结构介绍
```
├── brcb
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── brcb.asc      // Ascend C算子实现 & 调用样例
```

## 算子描述  
- 算子功能：  	 
   BrcbCustom算子给定一个输入张量，每一次取输入张量中的8个数填充到结果张量的8个DataBlock（32Bytes）中去，每个数对应一个DataBlock。	 

 
 - 算子实现：  	 

   - Kernel实现	 
 
     计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Brcb基础API接口完成计算，得到最终结果，再搬出到外部存储上。	 
 
     BrcbCustom算子的实现流程分为3个基本任务：CopyIn，Compute，CopyOut。CopyIn任务负责将Global Memory上的输入Tensor srcGm存储在srcLocal中，Compute任务负责从srcLocal每一次取输入张量中的8个数填充到dstLocal的8个DataBlock（32Bytes）中去，CopyOut任务负责将输出数据从dstLocal搬运至Global Memory上的输出Tensor dstGm。	 
 
 
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
  SCENARIO=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;             # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO  # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```