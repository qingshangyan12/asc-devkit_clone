# MC2、KFC 与 CCU 执行模型

## 1. 三个层次

### 普通 HCCL

上层框架调用 HCCL collective API，Host selector/executor/template准备资源和参数，再直接注册/Launch AICPU或CCU算法。

### MC2 高阶 API

AICore计算Kernel在设备侧调用融合通信接口。Host提前准备通信资源和tiling，设备侧通过消息协议请求通信工作。

### KFC server

设备侧常驻调度循环，接收MC2消息、检查状态、选择算法、装配参数并驱动通信执行。它是调度适配层，不是collective数据面本体。

## 2. 两个正交维度

```text
执行引擎：AICPU / CCU / AIV
启动方式：Host direct / KFC server
```

所以存在：

```text
Host direct + CCU
KFC server + CCU
KFC server + AICPU
```

“使用CCU”不等于“不存在KFC server”；是否需要server由调用场景决定。

## 3. 为什么MC2需要tiling

MC2是算子融合接口，图编译/算子编译阶段需要把通信参数编码进tiling，供AICore Kernel和KFC消息共同解释。普通HCCL Host API直接持有`count、dtype、rank、buffer`等运行参数，因此不需要经过同一套算子tiling ABI。

## 4. 典型链路

```text
Host初始化
  -> 创建通信资源、Channel、CCU instance
  -> 注册CCU Kernel，保存kernelHandle
  -> 生成MC2 tiling

Device运行
  -> AICore高阶API写KFC消息
  -> KFC server读取消息和tiling
  -> 选择CCU算法并生成taskArgs/参数表
  -> 启动CCU Mission
  -> CCU执行数据面
  -> KFC更新完成状态
```

## 5. 组件定位

- `mc2_client`：设备/接口侧消息提交或Host资源接口相关客户端代码，具体位置按当前仓库搜索符号确认。
- `libmc2_compat.so`：MC2可选依赖的兼容动态库，解决接口可装载性，不代表真实KFC算法已经实现。
- `ccu_launch_dl`：运行时解析CCU Launch符号。
- `ccu_primitives_impl_dl`：运行时解析CCU数据面原语符号。
- KFC Server Kernel：消息到CCU算法的适配和调度入口。

## 6. 当前 Devkit ST 如何观察这条链

```text
Host 域：真实 hccl_mc2.cc/hccl_alloc_ctx_res.cc -> OpResCtx/cache/startup assertions
Kernel 域：CaptureXxxKernel -> 真实 collective kernel -> Program -> RuntimeWorld
KFC 域：真实 CcuKfcServerKernel Program + queue/registration
       -> BuildXxxKfcTaskArgs -> same collective Program/RuntimeWorld
```

`CcuKfcSimWorld` 验证控制面后委托内部 `RuntimeWorld` 执行数据面，不是第二套 collective simulator。当前 13-slot `TaskParam` 和单 Mission startup 不能证明多 Mission/SQE 并发。

## 7. 参考资料

```text
MC2_CCU算法适配原理与ST指南.md
PR4261_MC2高阶API支持CCU算法.md
算法组件_对外接口_独立测试_MC2隔离.md
```
