# 新同构编程实现gather算子样例

## 概述
样例基于Ascend新同构编程模型SIMT方式实现gather算子，从100000×128的输入张量中采集12288行数据，展示离散内存访问类算子的开发方法。

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
参考[gather](../../02_features/03_simt/simt_gather)

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
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
  
## 性能调优
算子调优工具支持上板调优和仿真调优两种模式，可分别获取算子在实际硬件/仿真环境下的性能数据，用于定位性能瓶颈、优化算子实现。
### 上板调优
基于编译生成的可执行文件，直接在NPU硬件上采集算子性能数据，数据为算子预热后的真实运行指标。

**操作步骤**

**1.执行调优命令**

基于编译得到的demo文件，运行算子调优工具。
```bash
msprof op ./demo
```