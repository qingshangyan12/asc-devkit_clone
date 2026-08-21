# SSBuffer核间通信样例

## 概述

本样例演示Ascend 950PR/Ascend 950DT上SSBuffer核间通信的使用方法。SSBuffer是AI Core内用于AIC和AIV核间小数据通信的片上存储，适用于在同一个任务内传递控制信息、地址或少量标量数据；大块数据搬运应使用相应的数据通路，而不应将SSBuffer作为通用数据缓存。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── ssbuffer
│   ├── scripts
│   │   ├── gen_data.py              // 真值数据生成脚本
│   │   └── verify_result.py         // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── data_utils.h                 // 数据读入写出函数
│   ├── ssbuffer.h                   // SSBuffer通信消息定义与常量
│   ├── ssbuffer.asc                 // 调用样例以及结果校验
│   └── README.md                    // 样例说明文档
```

## 样例描述

<table border="1" style="text-align: center;">
  <tr>
    <td>SCENARIO_NUM取值</td>
    <td>业务场景</td>
    <td>验证方式</td>
  </tr>
  <tr>
    <td>0</td>
    <td>SSBuffer基址验证：以单个AIV Block启动核函数，验证NPU域能够调用GetSsbufBaseAddr并获得SSBuffer的特殊基址</td>
    <td>检查打印日志中GetSsbufBaseAddr的输出</td>
  </tr>
  <tr>
    <td>1</td>
    <td>AIV到AIC消息验证：在AIC:AIV为1:2的分离模式下，AIV0向SSBuffer写入32字节消息，AIC经核间同步后读取</td>
    <td>对比output.bin与golden.bin</td>
  </tr>
</table>

### SCENARIO_NUM=0（基址验证）

本场景只调用`GetSsbufBaseAddr`读取基址，不读写SSBuffer中的消息。在NPU域以单个AIV Block启动核函数并完成流同步后，日志中应包含`GetSsbufBaseAddr`的输出。NPU域中该特殊基址可能显示为`0`或`nil`，不应将其作为普通空指针解引用。

### SCENARIO_NUM=1（AIV到AIC消息验证）

本场景验证AIC:AIV为1:2时的单向通信，代码仅包含Device侧实现；Host侧为`output`分配8个`uint64_t`，以单个Block启动核函数、完成流同步后将`output`拷回并校验。

- 消息定义：32字节对齐的`SsbufferMessage`（`value[4]`，共32字节）。
- 消息布局：消息起始偏移为256字节（`VERIFY_MESSAGE_OFFSET`），须32字节对齐且不越过SSBuffer末端。
- 通信流程：
  1. AIV0将完整的32字节消息写入SSBuffer，并将消息回写GM（`output[4..7]`供Host校验AIV侧写入结果）；
  2. AIV0先建立Scalar到Vector的核内顺序（`SetFlag<HardEvent::S_V>`/`WaitFlag<HardEvent::S_V>`），再以模式2调用`CrossCoreSetFlag<2, PIPE_V>`发布AIV到AIC的就绪标记；
  3. AIC调用`CrossCoreWaitFlag<2, PIPE_M>`等待同一逻辑AI Core上的两个AIV均发布完成，先建立Cube流水到Scalar读取的顺序（`SetFlag<HardEvent::M_S>`/`WaitFlag<HardEvent::M_S>`），再从SSBuffer读取消息写入`output[0..3]`。

> **说明：** `AIV_TO_AIC_FLAG`（flagId=14）必须避开当前算子中Matmul高阶API或SyncAll等接口占用的flagId。
> **说明：** 在1:2场景中仅AIV0生产本示例的消息，AIV1仍须参与模式2同步（两个AIV均需调用`CrossCoreSetFlag`，否则AIC将一直等待）。

## 样例实现

### 1. 获取基地址

通过`GetSsbufBaseAddr`获取SSBuffer基地址，返回指针必须使用`__ssbuf__`地址空间限定符访问。

```cpp
__ssbuf__ void* ssbuf = AscendC::GetSsbufBaseAddr();
AscendC::printf("GetSsbufBaseAddr: %p\n", ssbuf);
```

### 2. AIV写入并发布

```cpp
if ASCEND_IS_AIV {
    if (AscendC::GetSubBlockIdx() == 0) {
        message->value[0] = EXPECTED_VALUE[0];
        message->value[1] = EXPECTED_VALUE[1];
        message->value[2] = EXPECTED_VALUE[2];
        message->value[3] = EXPECTED_VALUE[3];
        output[4] = message->value[0];
        output[5] = message->value[1];
        output[6] = message->value[2];
        output[7] = message->value[3];
    }
    AscendC::SetFlag<AscendC::HardEvent::S_V>(0);
    AscendC::WaitFlag<AscendC::HardEvent::S_V>(0);
    AscendC::CrossCoreSetFlag<2, PIPE_V>(AIV_TO_AIC_FLAG);
}
```

### 3. AIC等待并读取

```cpp
if ASCEND_IS_AIC {
    AscendC::CrossCoreWaitFlag<2, PIPE_M>(AIV_TO_AIC_FLAG);
    AscendC::SetFlag<AscendC::HardEvent::M_S>(0);
    AscendC::WaitFlag<AscendC::HardEvent::M_S>(0);
    output[0] = message->value[0];
    output[1] = message->value[1];
    output[2] = message->value[2];
    output[3] = message->value[3];
}
```

## 注意事项

1. SSBuffer只提供共享存储，不提供数据就绪保证。生产者和消费者必须使用核间同步接口（`CrossCoreSetFlag`/`CrossCoreWaitFlag`）建立顺序；`CrossCoreSetFlag`与`CrossCoreWaitFlag`必须成对使用。
2. SSBuffer初始存在脏数据，读取内容不保证全0；每次通信均由生产者完整写入数据，消费者不得依赖历史内容。
3. 仅支持32字节对齐的读写访问，通信数据块的起始地址和访问粒度均须满足32字节对齐，不要对SSBuffer执行小于32字节的成员读写。
4. 非MIX模式下，AIC、AIV0和AIV1各自独立占用1KB空间；MIX模式下（AIC:AIV=1:2）共享整个3KB空间。数据布局必须在通信双方保持一致，且不得越过SSBuffer末端。
5. 当前任务需使能SSBuffer能力（编译选项`ENABLE_CV_COMM_VIA_SSBUF=true`）后方可访问SSBuffer。
6. AIC和AIV启动不同任务时不能访问SSBuffer。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=1  # 设置场景编号（取值为0、1）
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM   # 生成真值数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py ./output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```

  场景0（基址验证）无输出数据文件，执行后请检查日志中`GetSsbufBaseAddr`的打印输出。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU 架构：Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`（默认）、`0` | 场景编号：1（AIV到AIC消息验证）、0（基址验证） |

- 执行结果

  场景1执行结果如下，说明SSBuffer通信结果正确。
  ```bash
  test pass!
  ```