# 静态库集成和使用样例
## 概述
本样例基于add_custom算子工程和matmul_custom算子工程，介绍了自定义算子工程静态库的集成和使用。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
## 目录结构介绍
```
├── static_library             // 使用自定义算子工程静态库方式调用add_custom算子和matmul_custom算子
    ├── aclnn_invocation       // 基于add_custom算子工程和matmul_custom算子工程，介绍自定义算子工程静态库的集成和使用
    ├── op_runner              // 对多个自定义算子工程的aclnn接口进行二次封装
    ├── add_custom             // add_custom算子工程
    |  ├── op_host             // host侧实现文件
    |  └── op_kernel           // kernel侧实现文件
    ├── matmul_custom          // matmul_custom算子工程
    |  ├── op_host             // host侧实现文件
    |  └── op_kernel           // kernel侧实现文件
    ├── add_custom.json        // add_custom算子的原型定义json文件
    ├── matmul_custom.json     // matmul_custom算子的原型定义json文件
    ├── install_matmul.sh      // 脚本，调用msOpGen生成matmul_custom自定义算子工程
    └── install_add.sh         // 脚本，调用msOpGen生成add_custom自定义算子工程
```

## 算子工程介绍
CANN软件包中提供了工程创建工具msOpGen，add_custom算子工程和Matmul算子工程可通过add_custom.json和matmul_custom.json自动创建，自定义算子工程具体请参考[Ascend C算子开发](https://hiascend.com/document/redirect/CannCommunityOpdevAscendC)>工程化算子开发>创建算子工程 章节。

创建完自定义算子工程后，开发者重点需要完成算子host和kernel文件的功能开发。为简化样例运行流程，本样例已在add_custom目录和matmul_custom目录中准备好了必要的算子实现，install_add.sh脚本会创建一个custom_op_add目录，install_matmul.sh脚本会创建一个custom_op_matmul目录，并将对应的算子实现文件复制到对应目录下。之后可以修改配置文件再编译算子。

备注：custom_op_add和custom_op_matmul目录为生成目录，每次执行对应脚本都会删除该目录并重新生成，切勿在该目录下编码算子，会存在丢失风险。

## 编译运行
在本样例根目录下执行如下步骤，编译生成自定义算子静态库、链接到公共动态库并调用执行自定义算子。
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

- 编译生成自定义算子动态库文件  
    指定命令行安装路径运行脚本：
    ```bash
    bash install_add.sh -v [SOC_VERSION] -i [ASCEND_INSTALL_PATH]
    bash install_matmul.sh -v [SOC_VERSION] -i [ASCEND_INSTALL_PATH]
    ```
    参数说明：
    - SOC_VERSION：昇腾AI处理器型号，如果无法确定具体的[SOC_VERSION]，则在安装昇腾AI处理器的服务器执行npu-smi info命令进行查询，在查询到的“Name”前增加Ascend信息，例如“Name”对应取值为xxxyy，实际配置的[SOC_VERSION]值为Ascendxxxyy。支持以下产品型号：
        - Atlas 推理系列产品AI Core
        - Atlas A2训练系列产品/Atlas 800I A2推理产品
    - ASCEND_INSTALL_PATH：CANN软件包安装路径

    脚本运行成功后，会在当前目录下创建custom_op_add和custom_op_matmul目录。
    进入custom_op_add目录，修改CMakePresets.json文件中的vendor_name字段的value修改为add_custom，将ASCEND_PACK_SHARED_LIBRARY字段的value设置为True，从而开启动态库和静态库编译。
    进入custom_op_matmul目录，修改CMakePresets.json文件中的vendor_name字段的value修改为matmul_custom，将ASCEND_PACK_SHARED_LIBRARY字段的value设置为True，从而开启动态库和静态库编译。
    修改完成后，分别在对应目录下执行bash build.sh命令，进行编译。
    ```bash
      cd custom_op_add
      bash build.sh
      cd ..
      cd custom_op_matmul
      bash build.sh
      cd ..
    ```
    编译完成后，会在custom_op_add/build_out和custom_op_matmul/build_out中，生成自定义算子动态库和静态库存放目录op_api。

- 链接静态库文件  
  在当前static_library目录下，执行如下命令将编译生成的算子静态库存放到同一目录中，方便后续链接到动态库时指定链接目录。
  ```bash
  rm -rf package; mkdir package
  cp -r custom_op_add/build_out/op_api  ./package/add_custom
  cp -r custom_op_matmul/build_out/op_api  ./package/matmul_custom
  ```

  之后，进入op_runner目录，执行命令，将两个静态库链接到同一个动态库中。详见：[对多个静态库的集成和使用](./op_runner/README.md)

- 调用执行算子工程  
  详见：[aclnn调用add_custom和matmul_custom算子工程](./aclnn_invocation/README.md)
