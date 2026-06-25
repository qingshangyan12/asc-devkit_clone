# ClearSpr<a name="ZH-CN_TOPIC_0000002537397560"></a>

## 产品支持情况<a name="section364964311912"></a>

| 产品 | 是否支持 |
|------|:------:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明<a name="section11971757181915"></a>

接口头文件定义路径为：basic\_api/kernel\_operator\_sys\_var\_intf.h。

对指定的特殊寄存器进行清零。当前支持[表SpecialPurposeReg模板参数说明](GetSpr.md#table37531617424)。

## 函数原型<a name="section113251712205"></a>

```cpp
template <SpecialPurposeReg spr>
__aicore__ inline void ClearSpr()
```

## 参数说明<a name="section1116018170208"></a>

**表1** 模板参数说明

| 参数名 | 描述 |
|--------|------|
| spr | 特殊寄存器，类型为SpecialPurposeReg枚举类，具体的取值请参考[表SpecialPurposeReg模板参数说明](GetSpr.md#table37531617424)。 |

## 数据类型<a name="section3853852112218"></a>

无

## 返回值说明<a name="section16895132314202"></a>

无

## 约束说明<a name="section162221734202016"></a>

本接口只能在VF函数外调用，命名空间为AscendC，函数标记符为\_\_aicore\_\_。如果需要在VF内调用，命名空间为AscendC::Reg，函数标记符为\_\_simd\_callee\_\_。

## 调用示例<a name="section849174212202"></a>

无
