# Kernel 接口与代码复用

## 1. 当前两个产品 kernel 的角色

```text
collective kernel
  CcuAlltoallVMesh1DKernel
  -> 描述真实 peer、地址、Write/LocalCopy 和同步数据面

KFC server kernel
  CcuKfcServerKernel
  -> 描述常驻消息读取、opId dispatch、queue wrap、finalize 和完成回写
```

当前 Device ST 分别由 `CaptureAllToAllVMesh1DKernel` 和 `CaptureKfcServerKernel` 类型安全调用并捕获 Program。不存在需要为每个 collective 复制的 `CcuXxxKfcServerKernel` 数据面副本。

## 2. 普通启动与 KFC 启动如何汇合

```text
普通启动
  PrepareXxxTaskArgs(launchConfig)
    -> RuntimeWorld(collective Program)

KFC 启动
  message + startup
    -> BuildXxxKfcTaskArgs
    -> RuntimeWorld(same collective Program)
```

两条入口必须复用：

- 同一个 collective Program/kernel handle；
- 同一 taskArg/LoadArg 顺序；
- 同一 peer 参数表布局；
- 同一 RankMemory 地址和最终 collective 语义 Checker。

KFC 专属内容仅包括 queue/CKE、opId、registration、message mapping、dispatch、completion 和 finalize。

## 3. adapter 的边界

```cpp
Result CaptureXxxKernel(void *kernelArg);
std::vector<uint64_t> PrepareXxxTaskArgs(const XxxLaunchConfig &config);
Result BuildXxxKfcTaskArgs(const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup, std::vector<uint64_t> &taskArgs);
```

- Capture 解决产品函数签名到统一 `KernelFunction(void *)` 的桥接。
- Prepare 集中普通 Launch ABI。
- Build 只做 KFC message/startup 到相同 ABI 的映射。

adapter 不实现 peer/rank 映射、LocalAddr/RemoteAddr、LocalCopy/Write/Reduce、PreSync/PostSync、tail/repeat 或 LoopGroup 算法。

## 4. 接入第二种 collective

复用同一个 `CcuKfcServerKernel` 和 `CcuKfcSimWorld`。新增：

- `CCU_KFC_XXX` identity；
- collective product kernel 与 `CaptureXxxKernel`；
- `PrepareXxxTaskArgs/BuildXxxKfcTaskArgs`；
- `opId -> collectiveHandle + taskArgBuilder` registration；
- collective Program/MC2 Checker 和 testcase。

同时把当前 `CaptureKfcServerKernel` 的 AllToAllV `opType` 硬编码改成由 `KfcServerKernelConfig.opType` 提供，每个 Server 仍绑定单个 primitive。不要在 DispatchOne 中追加算法 `if/else`。

## 5. ABI 核对

| 语义 | MC2 tiling/Host资源 | KFC message/startup | CCU taskArg/参数表 | Checker |
|---|---|---|---|---|
| input/output | 地址和 buffer | message slot | ArgId/Address | CheckMessage/CheckTaskArgs |
| token | queue/资源准备 | startup.token | ArgId/Variable | KFC registration + TaskArg |
| Rank/Channel | tiling/comm | registration | static kernelArg/Program | CheckRegistration |
| peer count/offset | 参数表 writer | table address | Load byteOffset | CheckPeerTable |
| goSize/GSA offset | tiling/launch config | message field | ArgId/AddressAdd | collective Checker |

任一列变化时沿整行更新，不能只修改产品 kernel。
