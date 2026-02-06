# 基于gather算子样例实现SIMT时间戳打点功能

## 概述
样例基于gather算子，使用clock进行使能时间戳能力。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构
```
├── simt_gather
│   ├── CMakeLists.txt         # cmake编译文件
│   ├── gather.asc             # Ascend C算子实现gather调用样例
|   └── README.md
```

## 算子描述
参考[gather](../../02_features/03_simt/simt_gather/README.md)

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
  cmake ..; make -j;            # 编译工程
  ./demo                        # 执行样例
  ```
  执行结果如下，说明时间打点功能和精度对比成功。
  ```
  gather excute cycle : 3641003
  ...
  [Success] Case accuracy is verification passed.
  ```
