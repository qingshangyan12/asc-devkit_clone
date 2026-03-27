# Sort32样例

## 概述

本样例在排序场景下，基于Sort32 API实现32位元素的排序组合功能，支持将score数据与对应的index数据按score值进行降序排序，并将排序结果以(score, index)的组合形式输出。Sort32 API支持每次迭代完成32个元素的排序，适用于需要对评分和索引同时进行排序的场景，例如Top-K选择、推荐系统排序等。

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
│   └── sort32.asc              // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  本样例展示了使用Sort32 API实现32位元素的排序组合功能，将score数据与对应的index数据按score值进行降序排序，并将排序结果以(score, index)的组合形式输出。Sort32 API适用于需要对数据进行局部排序的场景，例如TopK计算、数据筛选、排序索引等。通过repeatTime参数控制排序迭代次数，每次迭代完成32个元素的独立排序。所用API详细介绍请参考[Sort32 API文档](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900beta1/API/ascendcopapi/atlasascendc_api_07_0231.html)。

- 样例规格：  
  <table border="2" align="center">
  <caption>表1：样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型</td><td colspan="4" align="center">Sort32</td></tr>
  <tr><td rowspan="5" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x (score)</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y (index)</td><td align="center">[128]</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">z</td><td align="center">[256]</td><td align="center">float</td><td align="center">ND</td></tr>
  
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sort32_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是对128个score值和128个index值进行排序组合，输出256个元素的排序结果。
  
  Sort32 API参数说明：
  - dst：目标张量，存储排序结果，每个(score, index)组合占8字节
  - src0：源操作数0，存储待排序的score数据，本样例中为128个float元素
  - src1：源操作数1，存储对应的index数据，本样例中为128个uint32_t元素
  - repeatTime：重复迭代次数，本样例中为4，表示对128个元素分4组进行排序，每组32个元素

  - Kernel实现
    计算逻辑是：Ascend C提供的矢量计算接口的操作元素都为LocalTensor，输入数据需要先搬运进片上存储，然后使用Sort32基础API接口将score和index进行排序组合，得到排序结果，再搬出到外部存储上。Sort32 API每次迭代完成32个元素的独立排序，不同迭代间的数据不会相互影响。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

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