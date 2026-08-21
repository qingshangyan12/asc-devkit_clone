# HCCL AllReduce（CCU）直调样例
## 概述
本样例通过Ascend C编程语言实现了基于CCU服务端类型的HCCL AllReduce通信任务下发流程。样例在Kernel侧使用`AscendC::Hccl<HcclServerType::HCCL_SERVER_TYPE_CCU>`完成`InitV2`/`SetCcTilingV2`/`AllReduce`/`Commit`调用，其中`AllReduce`最终转发到CCU版实现的`CommonPrepareImpl`，由该函数完成通信数据量校验、handle分配与CCU消息体填写。使用`<<<>>>`内核调用符完成核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

> **说明：** `HcclServerType::HCCL_SERVER_TYPE_CCU`仅在`dav-3510`架构下可用，因此本样例不支持其他产品。

## 环境依赖

| 依赖项 | 要求 |
|--------|------|
| CANN软件包 | 需包含编译器（`ccec_compiler`）与runtime头文件（`include/acl/acl.h`），编译前须`source`其`set_env.sh` |
| 主机架构 | x86_64 与 aarch64 均可，由CMake根据`CMAKE_SYSTEM_PROCESSOR`自动选择CANN包内对应的`x86_64-linux`/`aarch64-linux`子目录 |
| CMake | >= 3.16 |
| NPU设备 | 仅`npu`模式需要真实的Ascend 950PR/950DT设备；`cpu`模式在主机上运行，无需NPU |

> **注意：** 切换环境或主机架构后，须删除整个`build`目录后重新执行cmake。CMake缓存中保存的是上一环境的绝对路径，仅删除`CMakeCache.txt`不足以清理干净。

## 目录结构
```
├── hccl_allreduce_ccu
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── hccl_allreduce_ccu.asc      // Ascend C算子实现 & 调用样例
│   └── README.md                   // 样例介绍
```

## 功能描述

- 算子实现：
  - kernel实现
    1. `InitV2`绑定通信上下文与初始化Tiling。Tiling中`Mc2InitTilingInner::version`取`INIT_TILING_VERSION`，上下文按`HcclCombineOpParam`布局解析。
    2. `SetCcTilingV2`注册AllReduce的通信Tiling，入参为通信Tiling相对`InitV2`传入地址的偏移，故Tiling结构体中`Mc2InitTilingInner`必须为第一个成员。
    3. `AllReduce`下发一次AllReduce任务，返回handleId。通信数据量为`count * sizeof(dataType)`，需落在`(0, 256MB]`区间内，且handleId需小于`HCCL_MAX_HANDLE_ID`，否则`CommonPrepareImpl`校验失败。
    4. `Commit`通知CCU执行已下发的任务。
    5. 将handleId、rankId、rankDim写回Global Memory供Host侧校验。

  - 调用实现
    使用内核调用符`<<<>>>`调用核函数，通信任务下发在单核（block 0）上完成。Host侧手工构造通信上下文，包含workspace、rankId/rankNum、`windowsOut[0]`（scratch地址）、`xnOffset`（CCU消息队列）、`ckeOffset`（CKE区域）。

> **注意：**
> - 在实际算子工程（aclnn单算子调用）中，通信上下文由框架通过`GetHcclContext()`下发。本样例为Kernel直调形态，上下文由Host侧手工构造，用于演示与验证`CommonPrepareImpl`的调用链路。
> - `Wait`与`Finalize`需要真实通信域驱动CCU写回CKE才会返回，在无真实通信域时会持续轮询，因此样例中使用`ASCENDC_CPU_DEBUG`宏保护。
> - Host侧不能直接使用`AscendC::HcclCombineOpParam`，而需使用样例中与之布局一致的`CcuCombineOpParamHost`。原因是`HcclCombineOpParam`按`__NPU_ARCH__`宏提供两种布局，而该宏仅在编译Device代码时定义，Host代码会取到其他产品的布局（`windowsOut`偏移不同且没有`xnOffset`/`ckeOffset`）；同时其`GM_ADDR`成员会在Host代码中引入`__gm__`地址空间限定的指针，导致编译报`error pointer address space cast`。样例中已通过`static_assert`校验两者成员偏移一致。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  以下命令均在本样例目录（`hccl_allreduce_ccu`）下执行，使用相对路径，不依赖具体的仓库存放位置。
  ```bash
  mkdir -p build && cd build
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510   # 默认npu模式
  make -j
  ./demo
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510   # cpu调试模式
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510   # NPU仿真模式
  ```

  > **注意：**
  > - `-DCMAKE_ASC_ARCHITECTURES=dav-3510` 不可省略。CCU实现仅在`__NPU_ARCH__ == 3510`下参与编译，使用其他架构时`Hccl<HCCL_SERVER_TYPE_CCU>`会匹配到默认兜底实现，各接口直接返回`-1`，无法进入`CommonPrepareImpl`。
  > - 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake，或直接删除整个 build 目录。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

  执行结果如下，说明执行成功。
  ```bash
  AllReduce handleId: 0
  rankId: 0, rankDim: 8
  test pass!
  ```

## 调用链验证

CPU调试模式下会打印Kernel侧日志，可据此确认`AllReduce`是否真正下发到了CCU实现。

```bash
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510
make -j
./demo 2>&1 | grep hccl_ccu_v0
```

正常情况下可依次看到`InitInner`、`CcuPrepareForAllReduceM2M`、`CcuSendMsg`等函数的日志，其中`InitInner`打印的`rankId`/`rankNum`应与样例中设置的通信域参数一致。若需观察`CommonPrepareImpl`内部的执行情况，可在该函数中临时添加`KERNEL_LOG(KERNEL_INFO, ...)`打印。

> **说明：** 编译时使用的头文件来自CANN安装包（`${ASCEND_HOME_PATH}/asc/include`），该路径由编译器内置且优先级高于`-I`/`-isystem`。因此在CANN包之外的位置修改同名头文件不会生效，如需验证修改后的实现，需要替换`ASCEND_HOME_PATH`所指向的目录。
