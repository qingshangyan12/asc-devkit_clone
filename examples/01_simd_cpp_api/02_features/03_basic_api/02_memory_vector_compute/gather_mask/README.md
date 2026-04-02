# GatherMask 样例

## 概述

本样例介绍了GatherMask接口在多场景下的使用方式，包括内置固定模式和用户自定义模式来生成gather mask（数据收集的掩码），并从源操作数中选取元素写入目的操作数。样例支持通过编译参数切换不同场景，便于开发者理解GatherMask接口的使用方法和实现差异。

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── gather_mask
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── gather_mask.asc         // Ascend C样例实现 & 调用样例
```

## 场景详细说明
本样例通过编译参数`SCENARIO_NUM` 来切换不同的掩码生成场景：

**场景1：内置固定模式**
- 实现：
    ```cpp
    AscendC::GatherMask(dstLocal, src0Local, src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
    ```
- 说明：通过`src1Pattern`选择对应的二进制作为掩码，来获取数据

**场景2：用户自定义模式**
- 实现：
    ```cpp
    AscendC::GatherMask (dstLocal, src0Local, src1Local, reduceMode, mask, gatherMaskParams, rsvdCnt);
    ```
- 说明：通过用户输入的`src1Local`对应的二进制作为掩码，来获取数据

 ## 样例规格
 详细的场景与对应的样例规格如下表：
  <table border="2" align="center">
  <caption>表1：样例规格</caption>
  <tr>
    <th>场景名称</th>
    <th>数据类型</th>
    <th>输入规格</th>
    <th>输出规格</th>
    <th>参数说明</th>
  </tr>
  <tr>
    <td>内置固定模式</td>
    <td>uint16</td>
    <td>[1, 128]</td>
    <td>[1, 128]</td>
    <td>使用内置固定模式src1Pattern=2进行元素选取，reduceMode=false（Normal模式），mask=0, gatherMaskParams={1, 1, 0, 0}。详见表2</td>
  </tr>
  <tr>
    <td>用户自定义模式</td>
    <td>uint32</td>
    <td>[1, 256], [1, 32]</td>
    <td>[1, 256]</td>
    <td>使用用户提供的mask模式进行元素选取，reduceMode=true（Counter模式），mask=70, gatherMaskParams={1, 2, 4, 0}。详见表2</td>
  </tr>
  </table>

详细的参数说明见表2：
  <table border="2" align="center">
  <caption>表2：参数说明</caption>
  <tr>
    <th>参数</th>
    <th>说明</th>
  </tr>
  <tr>
    <td>dstLocal</td>
    <td>目的操作数</td>
  </tr>
  <tr>
    <td>src0Local</td>
    <td>源操作数</td>
  </tr>
  <tr>
    <td>src1Pattern/src1Local</td>
    <td>src1Pattern：内置固定模式使用，取值范围[1,7]，详见表3<br>src1Local：用户自定义模式使用，LocalTensor类型</td>
  </tr>
  <tr>
    <td>reduceMode</td>
    <td>false表示Normal模式，true表示Counter模式<br>Normal模式：每次repeat操作256Bytes数据，总的数据计算量为repeatTimes * 256Bytes，mask参数无效<br>Counter模式：每次repeat操作mask个元素，总的数据计算量为repeatTimes * mask个元素</td>
  </tr>
  <tr>
    <td>mask</td>
    <td>Counter模式下表示每次迭代内参与计算的元素个数，Normal模式下建议设置为0</td>
  </tr>
  <tr>
    <td>gatherMaskParams</td>
    <td>控制操作数地址步长的数据结构，包含以下参数：<br>src0BlockStride：单次迭代内数据间隔<br>repeatTimes：重复迭代次数<br>src0RepeatStride：源操作数迭代间数据间隔<br>src1RepeatStride：src1迭代间数据间隔</td>
  </tr>
  <tr>
    <td>rsvdCnt</td>
    <td>筛选后保留下来的元素计数，对应dstLocal中有效元素个数</td>
  </tr>
  </table>

内置固定模式取值范围为[1,7]，其具体含义如下表：
  <table border="2" align="center">
  <caption>表3：内置固定模式说明</caption>
  <tr>
    <th>模式值</th>
    <th>二进制表示</th>
    <th>说明</th>
  </tr>
  <tr>
    <td>1</td>
    <td>01010101…0101</td>
    <td>每个repeat取偶数索引元素</td>
  </tr>
  <tr>
    <td>2</td>
    <td>10101010…1010</td>
    <td>每个repeat取奇数索引元素</td>
  </tr>
  <tr>
    <td>3</td>
    <td>00010001…0001</td>
    <td>每个repeat内每四个元素取第一个元素</td>
  </tr>
  <tr>
    <td>4</td>
    <td>00100010…0010</td>
    <td>每个repeat内每四个元素取第二个元素</td>
  </tr>
  <tr>
    <td>5</td>
    <td>01000100…0100</td>
    <td>每个repeat内每四个元素取第三个元素</td>
  </tr>
  <tr>
    <td>6</td>
    <td>10001000…1000</td>
    <td>每个repeat内每四个元素取第四个元素</td>
  </tr>
  <tr>
    <td>7</td>
    <td>11111111…1111</td>
    <td>每个repeat内取全部元素</td>
  </tr>
  </table>

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
  DSCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DRUN_MODE=npu -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```
  其中编译参数使用说明如下表所示：
  <table border="2" align="center">
  <caption>表2：编译参数说明</caption>
  <tr>
    <th>参数</th>
    <th>说明</th>
    <th>默认值</th>
  </tr>
  <tr>
    <td>SCENARIO_NUM</td>
    <td>场景编号：1（内置固定模式）、2（用户自定义模式）</td>
    <td>1</td>
  </tr>
  </table>

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```