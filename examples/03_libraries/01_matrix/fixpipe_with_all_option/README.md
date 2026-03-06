
# fixpipe_co12gm_quantization_s322s8样例

## 概述

本样例介绍如何使用组合API Fixpipe，支持通过脚本传入参数，控制 Fixpipe 所有功能的组合实现。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

//

## 算子描述

//

## （各类功能分类介绍）

### 随路格式转换

//

### 量化模式

//

### Relu模式

//

### unitFlag

//

### Channel Merge

//

### Channel Split

//

### 卷积格式转换

//

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
  //
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```