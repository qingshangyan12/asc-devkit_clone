## 概述
本样例基于AddCustom算子工程，介绍了aclnn单算子调用方式进行编译和执行。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── aclnn_invocation           // 使用aclnn单算子调用的方式调用Add算子
│   ├── add_custom             // add_custom自定义算子工程
│   │   ├── framework          // 算子插件实现文件目录
│   │   ├── op_host            // host侧实现文件
│   │   └── op_kernel          // kernel侧实现文件
│   ├── inc                    // 头文件目录
│   │   ├── common.h           // 声明公共方法类，用于读取二进制文件
│   │   ├── op_runner.h        // 算子描述声明文件，包含算子输入/输出，算子类型以及输入描述与输出描述
│   │   └── operator_desc.h    // 算子运行相关信息声明文件，包含算子输入/输出个数，输入/输出大小等
│   ├── scripts                // 真值脚本
│   │   ├── acl.json           // acl配置文件
│   │   ├── gen_data.py        // 输入数据和真值数据生成脚本
│   │   └── verify_result.py   // 真值对比文件
│   ├── src                    // aclnn调用源文件
│   │   ├── CMakeLists.txt     // 编译规则文件
│   │   ├── common.cpp         // 公共函数，读取二进制文件函数的实现文件
│   │   ├── main.cpp           // 单算子调用应用的入口
│   │   ├── op_runner.cpp      // 单算子调用主体流程实现文件
│   │   └── operator_desc.cpp  // 构造算子的输入与输出描述
│   ├── add_custom.json        // add_custom算子的原型定义json文件
│   └── install.sh             // 脚本，调用msOpGen生成自定义算子工程，并编译
│   └── run.sh                 // 脚本，用于编译aclnn调用源文件并执行
```

## 算子描述
Add算子实现了两个数据相加，返回相加结果的功能。对应的数学表达式为：
```
z = x + y
```
## 算子规格描述
<table>
<tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">Add</td></tr>
</tr>
<tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td align="center">y</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
</tr>
<tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">8 * 2048</td><td align="center">float</td><td align="center">ND</td></tr>
</tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">add_custom</td></tr>
</table>

## 算子工程介绍
CANN软件包中提供了工程创建工具msOpGen，add_custom算子工程可通过add_custom.json自动创建，自定义算子工程具体请参考[Ascend C算子开发](https://hiascend.com/document/redirect/CannCommunityOpdevAscendC)>工程化算子开发>创建算子工程 章节。

创建完自定义算子工程后，开发者重点需要完成算子host和kernel文件的功能开发。为简化样例运行流程，本样例已在add_custom目录中准备好了必要的算子实现，install.sh脚本会创建一个custom_op目录，并将算子实现文件复制到对应目录下，再编译算子。

备注：custom_op目录为生成目录，每次执行install.sh脚本都会删除该目录并重新生成，切勿在该目录下编码算子，会存在丢失风险。

## 代码实现介绍
完成自定义算子的开发部署后，可以通过单算子调用的方式来验证单算子的功能。src/main.cpp代码为单算子API执行方式。单算子API执行是基于C语言的API执行算子，无需提供单算子描述文件进行离线模型的转换，直接调用单算子API接口。

自定义算子编译部署后，会自动生成单算子API，可以直接在应用程序中调用。算子API的形式一般定义为“两段式接口”，形如：
   ```cpp
   // 获取算子使用的workspace空间大小
   aclnnStatus aclnnAddCustomGetWorkspaceSize(const aclTensor *x, const aclTensor *y, const aclTensor *out, uint64_t *workspaceSize, aclOpExecutor **executor);
   // 执行算子
   aclnnStatus aclnnAddCustom(void *workspace, int64_t workspaceSize, aclOpExecutor *executor, aclrtStream stream);
   ```
其中aclnnAddCustomGetWorkspaceSize为第一段接口，主要用于计算本次API调用计算过程中需要多少的workspace内存。获取到本次API计算需要的workspace大小之后，开发者可以按照workspaceSize大小申请Device侧内存，然后调用第二段接口aclnnAddCustom执行计算。具体参考[单算子API调用](https://hiascend.com/document/redirect/CannCommunityAscendCInVorkSingleOp)章节。

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

- 编译生成自定义算子安装包  
  指定命令行安装路径来运行脚本
      ```bash
      bash install.sh -v [SOC_VERSION] -i [ASCEND_INSTALL_PATH]
      ```
    参数说明：
    - SOC_VERSION：昇腾AI处理器型号，如果无法确定具体的[SOC_VERSION]，则在安装昇腾AI处理器的服务器执行npu-smi info命令进行查询，在查询到的“Name”前增加Ascend信息，例如“Name”对应取值为xxxyy，实际配置的[SOC_VERSION]值为Ascendxxxyy。支持以下产品型号：
        - Atlas 训练系列产品
        - Atlas 推理系列产品AI Core
        - Atlas A2训练系列产品/Atlas 800I A2推理产品
        - Atlas 200/500 A2推理产品
    - ASCEND_INSTALL_PATH：CANN软件包安装路径

    脚本运行成功后，会在当前目录下创建custom_op目录，编译完成后，会在custom_op/build_out中，生成自定义算子安装包custom_opp_\<target os>_\<target architecture>.run，例如“custom_opp_ubuntu_x86_64.run”。

    备注：如果要使用dump调试功能，需要移除op_host内的Atlas 训练系列产品、Atlas 200/500 A2 推理产品的配置项。

- 部署自定义算子包  
  - 部署自定义算子包前，请确保存在自定义算子包默认部署路径环境变量ASCEND_OPP_PATH
    ```bash
    echo $ASCEND_OPP_PATH
    # 输出示例 /usr/local/Ascend/ascend-toolkit/latest/opp

    # 若没有，则需导出CANN环境变量
    source [ASCEND_INSTALL_PATH]/bin/setenv.bash
    # 例如 source /usr/local/Ascend/ascend-toolkit/latest/bin/setenv.bash
    ```
    参数说明：
    - ASCEND_INSTALL_PATH：CANN软件包安装路径，一般和上一步中指定的路径保持一致

  - 在自定义算子安装包所在路径下，执行如下命令安装自定义算子包
    ```bash
    cd custom_op/build_out
    ./custom_opp_<target os>_<target architecture>.run
    ```
  命令执行成功后，自定义算子包中的相关文件将部署至opp算子库环境变量ASCEND_OPP_PATH指向的的vendors/customize目录中。

- 调用执行算子工程  
  - 样例执行  
    样例执行过程中会自动生成测试数据，然后编译与运行aclnn样例，最后检验运行结果。具体过程可参见run.sh脚本。
    ```bash
    bash run.sh
    ```
