# aclnn调用MatmulCustom算子工程
## 概述
本样例基于MatmulCustom算子工程，介绍了aclnn调用方式。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
## 目录结构介绍
```
└── AclNNInvocation             // 通过aclnn调用的方式调用MatmulCustom算子
    ├── inc                     // 头文件目录
    │   ├── common.h            // 声明公共方法类，用于读取二进制文件
    │   ├── operator_desc.h     // 算子描述声明文件，包含算子输入/输出，算子类型以及输入描述与输出描述
    │   └── op_runner.h         // 算子运行相关信息声明文件，包含算子输入/输出个数，输入/输出大小等
    ├── input                   // 存放脚本生成的输入数据目录
    ├── output                  // 存放算子运行输出数据和真值数据的目录
    ├── scripts
    │   ├── verify_result.py    // 真值对比文件
    │   └── gen_data.py         // 输入数据和真值数据生成脚本文件
    ├── src
    │   ├── CMakeLists.txt      // 编译规则文件
    │   ├── common.cpp          // 公共方法类的实现，用于读取二进制文件
    │   ├── main.cpp            // 单算子调用应用的入口
    │   ├── op_runner.cpp       // 算子运行相关信息实现，包含算子输入/输出个数，输入/输出大小等
    │   └── operator_desc.cpp   // 算子描述实现，包含算子输入/输出，算子类型以及输入描述与输出描述
    └── run.sh                  // 执行命令脚本
```

## 算子实现
完成自定义算子的开发部署后，可以通过单算子调用的方式来验证单算子的功能。src/main.cpp代码为单算子API执行方式。单算子API执行是基于C语言的API执行算子，无需提供单算子描述文件进行离线模型的转换，直接调用单算子API接口。

自定义算子编译部署后，会自动生成单算子API，可以直接在应用程序中调用。算子API的形式一般定义为“两段式接口”，形如：

```cpp
// 获取算子使用的workspace空间大小
aclnnStatus aclnnMatmulCustomGetWorkspaceSize(const aclTensor *a, const aclTensor *b, const aclTensor *bias, const aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);
// 执行算子
aclnnStatus aclnnMatmulCustom(void *workspace, uint64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream);
```
其中aclnnMatmulCustomGetWorkspaceSize为第一段接口，主要用于计算本次API调用计算过程中需要多少的workspace内存。获取到本次API计算需要的workspace大小后，按照workspaceSize大小申请Device侧内存，然后调用第二段接口aclnnMatmulCustom执行计算。具体请参考[单算子API调用](https://hiascend.com/document/redirect/CannCommunityAscendCInVorkSingleOp)章节。

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
  样例执行过程中会自动生成测试数据，然后编译与运行aclnn样例，最后检验运行结果。具体过程可参见run.sh脚本。
  ```bash
  bash run.sh
  ```
