# CheckLocalMemoryIA 样例

## 概述

本样例基于 KernelAdd（TQue CopyIn/Compute/CopyOut 流水线）模式，演示 `CheckLocalMemoryIA` 接口的使用方法，检测矢量写访问是否在设定的地址范围内。

## 功能说明

`CheckLocalMemoryIA` 用于设置本地内存数据异常检测：

- 监控指定地址范围内的内存访问
- 配置访问类型（Scalar/Vector/MTE 的读写）
- 当检测到符合条件的访问时，触发异常 `ACL_ERROR_RT_VECTOR_CORE_EXCEPTION`

## 参数说明

```cpp
CheckLocalMemoryIAParam {
    uint8_t enableBit;      // SET_DATA_EXP_ZERO(0) ~ THREE(3)
    uint32_t startAddr;     // 监控起始地址（单位：32字节块）
    uint32_t endAddr;       // 监控结束地址（单位：32字节块）
    bool isScalarRead;      // 监控 Scalar 读访问
    bool isScalarWrite;     // 监控 Scalar 写访问
    bool isVectorRead;      // 监控 Vector 读访问
    bool isVectorWrite;     // 监控 Vector 写访问
    bool isMteRead;         // 监控 MTE 读访问
    bool isMteWrite;        // 监控 MTE 写访问
    bool isEnable;          // 启用检测
};
```

## 样例逻辑

基于 KernelAdd（TQue CopyIn/Compute/CopyOut 流水线）模式测试 CheckLocalMemoryIA：

1. **Test 1**：在 `Init` 中对 `outQueueZ` 的 zLocal 地址范围启用 CheckLocalMemoryIA（`isVectorWrite=true`），执行 `Add` 时矢量写触发 `ACL_ERROR_RT_VECTOR_CORE_EXCEPTION`
2. **Test 2**：不启用 CheckLocalMemoryIA，正常执行 `Add`（z = x + y），验证计算精度

## 编译运行

```bash
mkdir -p build && cd build;
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201;
make -j;
./add_check_memory
```

## 预期结果

```
===== Test 1: CheckLocalMemoryIA enabled (monitoring vector write on zLocal) =====
[EXPECTED] Caught ACL_ERROR_RT_VECTOR_CORE_EXCEPTION
CheckLocalMemoryIA successfully detected vector write on zLocal!

===== Test 2: Normal Add without CheckLocalMemoryIA =====
Output: 3.5 3.5 3.5 ...
Golden: 3.5 3.5 3.5 ...
[Success] Case accuracy is verification passed.

===== All tests completed =====
```

## 注意事项

- 地址单位为 32 字节块，需要将实际地址除以 32
- 该接口主要用于调试和异常检测
- 在生产环境中应谨慎使用，避免误触发异常