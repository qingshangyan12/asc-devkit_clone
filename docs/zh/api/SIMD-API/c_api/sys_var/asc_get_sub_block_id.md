# asc_get_sub_block_id

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

获取AI Core上Vector核的ID。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_sub_block_id()
```

## 参数说明

无

## 返回值说明

返回Vector核ID。

## 流水类型

PIPE_S

## 约束说明

### 通用约束

- 本接口为只读查询接口，不修改任何寄存器或存储状态，可在AIC与AIV上下文中调用。
- 返回值由系统控制在kernel启动前配置完成，kernel运行期间不应依赖返回值发生变化；若运行期间系统侧修改了SUBBLOCKID，本接口读到的新值与kernel启动时的调度假设可能不一致，后续按子块ID路由的逻辑需自行保证一致性。
- 返回值落在16 bit范围内，按`int64_t`返回时高48位为0，调用方无需额外掩码处理。
- 本接口读取的是SUBBLOCKID寄存器（子块级ID），与核ID、Block ID等其他层级标识不同，不可混用。

## 调用示例

无
