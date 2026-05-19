# ReadGmByPassDCache\(ISASI\)<a name="ZH-CN_TOPIC_0000002327311338"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
|------|----------|
| <cann-filter npu-type="950"><term>Ascend 950PR/Ascend 950DT</term> | √</cann-filter> |
| <cann-filter npu-type="A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √</cann-filter> |
| <cann-filter npu-type="910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √</cann-filter> |
| <cann-filter npu-type="310b"><term>Atlas 200I/500 A2 推理产品</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 AI Core</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 Vector Core</term> | x</cann-filter> |
| <cann-filter npu-type="910"><term>Atlas 训练系列产品</term> | x</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为："basic_api/kernel_operator_scalar_intf.h"。

ReadGmByPassDCache接口的功能是：不经过DCache从GM地址上读数据。使用场景：

- 当多核操作GM地址时，如果数据无法对齐到Cache Line，经过DCache的方式下，由于按照Cache Line大小进行读写，会导致多核数据随机覆盖的问题。此时，可以采用不经过DCache直接读写GM地址的方式，从而避免上述随机覆盖的问题。

- 不经过DCache直接读写GM数据，不会导致多核间数据不一致的问题。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T>
__aicore__ inline T ReadGmByPassDCache(__gm__ T* addr)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
|--------|------|
| T | 操作数的数据类型。 |

**表 2**  接口参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| addr | 输入 | 源GM地址。 |

## 数据类型

- 支持的数据类型为int8_t/uint8_t/int16_t/uint16_t/int32_t/uint32_t/int64_t/uint64_t。

## 返回值说明<a name="section640mcpsimp"></a>

源GM地址上的数据。

## 流水类型<a name="section641mcpsimp"></a>

PIPE_S

## 约束说明<a name="section633mcpsimp"></a>

标量单元连续读取相同GM地址时，会造成Bus被长期占用，此时会影响其他核向GM该地址上写入，进而引发卡死现象。在此场景下，建议在标量读指令后插入若干空操作指令（Nop）来释放Bus，指令数量建议为800条，通常按照“需同步的核数 × 200”。

## 调用示例<a name="section6191129670"></a>

ReadGmByPassDCache接口调用示例：

```cpp
__gm__ int32_t *addr = const_cast<__gm__ int32_t *>(srcGlobal.GetPhyAddr());
int32_t value = AscendC::ReadGmByPassDCache<int32_t>(addr);

// 通过Scalar读写相同GM时，需要让读操作，空出总线，避免长时占用
int target = -1;
while (target != 0) {
     target = ReadGmByPassDCache(addr);
     Nop<800>();
}
```

详细请参考[GmByPassDCache类样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/09_scalar_computation/gm_by_pass_dcache)。
