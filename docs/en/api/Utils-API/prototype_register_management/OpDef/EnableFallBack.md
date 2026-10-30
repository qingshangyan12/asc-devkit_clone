# EnableFallBack<a name="ZH-CN_TOPIC_0000002190399885"></a>

<!-- md-trans-meta sourceCommit=f83e857605e3844db8b9de0442007cc115278a47 translatedAt=2026-10-23T03:37:20.993Z pushedAt=2026-10-23T07:01:40.640Z nl=2 -->

## Description<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001525424352_section36583473819"></a>

This API enables the fallback configuration. Once enabled, a fallback function is automatically generated and registered with GE. The core logic of the fallback function converts GE inputs, outputs, and attributes into the parameter format required by the aclnn single-operator API, and then calls the aclnn API. In dynamic graph scenarios, GE can directly call the fallback function (which calls the aclnn API), thereby simplifying the scheduling process. For details about how fallback dispatches operators, see [Graph Development](https://gitcode.com/cann/ge/blob/9.2.0/docs/en/user_guides/graph_dev/README.md).

## Prototype<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001525424352_section13230182415108"></a>

```
OpDef &EnableFallBack(void)
```

## Parameters<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001525424352_section75395119104"></a>

None

## Return Value<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001525424352_section25791320141317"></a>

OpDef operator definition. For details about OpDef, see [OpDef](OpDef.md).

## Constraints<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001525424352_section19165124931511"></a>

-   The operator must be registered and implement the InferShape function.
-   The operator must be registered and implement the InferDataType function.

## Examples<a name="zh-cn_topic_0000001600267337_zh-cn_topic_0000001526112350_zh-cn_topic_0000001575944081_section320753512363"></a>

```
class AddCustom : public OpDef {
public:
    AddCustom(const char* name) : OpDef(name)
    {
        this->Input("x").ParamType(REQUIRED).DataType({ge::DT_FLOAT}).Format({ge::FORMAT_ND});
        this->Input("y").ParamType(REQUIRED).DataType({ge::DT_FLOAT}).Format({ge::FORMAT_ND});
        this->Output("z").ParamType(REQUIRED).DataType({ge::DT_FLOAT}).Format({ge::FORMAT_ND})
        this->AICore().AddConfig("ascendxxx");
        this->SetInferShape(ge::InferShapeFunc);
        this->SetInferDataType(ge::InferDataTypeFunc);
        this->EnableFallBack();
    }
};
OP_ADD(AddCustom);
```

