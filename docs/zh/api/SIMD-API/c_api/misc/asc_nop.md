# asc_nop

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

生成一条空操作指令，不执行实际计算。每调用一次阻塞一拍流水。

## 函数原型

```c
__aicore__ inline void asc_nop()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

如果需要在流水间精确插入n拍空操作，需要在n个`asc_nop`接口调用前后插入[`asc_sync`](../sync/asc_sync.md)。

假设不插入`asc_sync`，参考以下伪代码：

```cpp                    
vf(dst_ub, src_ub);                  
asc_nop();
asc_nop(); 
asc_nop(); 
asc_nop();                           
asc_copy_ub2gm(dst_gm, dst_ub);         
```

对应的流水排布如下，nop指令会在VF指令发射的下一拍立即发射并执行4拍空操作，由于nop指令不会打断正在执行的流水，此时VF函数仍在执行，在nop指令都执行完成后搬运指令才会发射执行，所以计算和搬运之间的间隔不足4拍：
```txt
S:       |==nop==||==nop==||==nop==||==nop==|
V:    |========计算========|
MTE3:                                       |======搬运======|
```

假设插入`asc_sync`，参考以下伪代码：

```cpp
vf(dst_ub, src_ub);               
asc_sync();   
asc_nop();
asc_nop(); 
asc_nop(); 
asc_nop(); 
asc_sync();                          
asc_copy_ub2gm(dst_gm, dst_ub);
```

对应的流水排布如下，nop指令会在VF执行完成后发射并执行4拍空操作，在nop指令都执行完成后搬运指令才会发射执行，此时，计算和搬运之间的间隔为4拍。：

```
S:                         |==nop==||==nop==||==nop==||==nop==|
V:    |========计算========|
MTE3:                                                         |======搬运======|
```

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id8 -->

```cpp
#include <iostream>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
__global__ __vector__ void asc_nop_kernel()
{
    asc_init();
    asc_sync();
    asc_nop();
    asc_nop();
    asc_nop();
    asc_sync();
}
} // namespace

int main()
{
    aclInit(nullptr);
    aclrtSetDevice(0);
    asc_nop_kernel<<<1, 0>>>();
    const aclError ret = aclrtSynchronizeDevice();
    const bool passed = ret == ACL_SUCCESS;
    std::cout << (passed ? "[Success] asc_nop passed." : "[Failed] asc_nop failed.") << std::endl;
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
