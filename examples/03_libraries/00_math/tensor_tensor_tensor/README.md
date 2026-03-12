# MulAddDst算子直调样例

## 概述


## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── tensor_tensor_tensor
│   ├── scripts
│   │   ├── gen_data.py                  // 输入数据和真值数据生成脚本
│   │   └── verify_result.py             // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── data_utils.h                     // 数据读入写出函数
│   └── tensor_tensor_tensor.asc         // Ascend C算子实现 & 调用样例
```

## 算子描述

- 算子功能：  


- 算子实现：  


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
  SIZE=1024
  mkdir -p build && cd build;                                                                      # 创建并进入build目录
  cmake .. -DSIZE=$SIZE;make -j;                                             # 编译工程
  python3 ../scripts/gen_data.py  -size=$SIZE                                # 生成测试输入数据
  ./demo                                                                     # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py  output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 测试场景说明
说明如何进行测试，描述具体的测试步骤

## 实测性能说明
说明测试数据，结论