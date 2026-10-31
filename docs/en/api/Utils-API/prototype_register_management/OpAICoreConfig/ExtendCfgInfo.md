# ExtendCfgInfo<a name="ZH-CN_TOPIC_0000002114097093"></a>

<!-- md-trans-meta sourceCommit=f83e857605e3844db8b9de0442007cc115278a47 translatedAt=2026-10-23T03:18:55.381Z pushedAt=2026-10-23T07:01:40.575Z nl=2 -->

## Description<a name="zh-cn_topic_0000001600884192_zh-cn_topic_0000001576875005_zh-cn_topic_0000001525424352_section36583473819"></a>

Extends operator-related parameter configuration to provide more flexible parameter configuration capabilities.

## Prototype<a name="zh-cn_topic_0000001600884192_zh-cn_topic_0000001576875005_zh-cn_topic_0000001525424352_section13230182415108"></a>

```
OpAICoreConfig &OpAICoreConfig::ExtendCfgInfo(const char *key, const char *value) 
```

## Parameters<a name="zh-cn_topic_0000001600884192_zh-cn_topic_0000001576875005_zh-cn_topic_0000001525424352_section75395119104"></a>

**Table 1**  Parameters

| Parameter | Description |
|:-------|:-----|
| key | Configuration item, for example, "aclnnSupport.value". |
| value | Value corresponding to the configuration item key. For example, for "aclnnSupport.value", you can set it to "support_aclnn" or "aclnn_only". |

The parameters supported by ExtendCfgInfo are listed in the following table:

**Table 2** Parameters configurable through ExtendCfgInfo

| parameter name | Description |
|:-------|:---------|
| aclnnSupport.value | - support_aclnn: In this mode, the operator is executed through model offloading in static shape scenarios, while in dynamic shape scenarios, the fallback function is called on the host side to deliver the operator. This mode is used by default if [EnableFallBack](../OpDef/EnableFallBack.md) is called.<br>- aclnn_only: In this mode, the operator is delivered in fallback form in both dynamic and static shape scenarios. This mode is not recommended and will be deprecated in later versions.<br>For details about delivering operators through fallback, see [Graph Development](https://gitcode.com/cann/ge/blob/9.2.0/docs/en/user_guides/graph_dev/README.md). |

Example of using support_aclnn:

```cpp
// Example of a dynamic shape scenario.
OpAICoreConfig aicore_config;
aicore_config.DynamicShapeSupportFlag(true)   // Set DynamicShapeSupportFlag to true in dynamic shape scenarios.
             .ExtendCfgInfo("aclnnSupport.value", "support_aclnn");
this->AICore().AddConfig("ascendxxx", aicore_config);

// Example of a static shape scenario.
OpAICoreConfig aicore_config;
aicore_config.DynamicCompileStaticFlag(true)  // Set DynamicCompileStaticFlag to true in static shape scenarios.
             .ExtendCfgInfo("aclnnSupport.value", "support_aclnn");
this->AICore().AddConfig("ascendxxx", aicore_config);
```

Example of using aclnn_only:

```cpp
// Example of a dynamic shape scenario.
OpAICoreConfig aicore_config;
aicore_config.DynamicShapeSupportFlag(true) // Set DynamicShapeSupportFlag to true for a dynamic shape scenario.
			 .ExtendCfgInfo("aclnnSupport.value", "aclnn_only");
this->AICore().AddConfig("ascendxxx", aicore_config);
// Example of a static shape scenario.
OpAICoreConfig aicore_config;              // No need to configure DynamicCompileStaticFlag/DynamicShapeSupportFlag. Operators are dispatched in fallback mode, which is the dispatch method for dynamic shape models.
aicore_config.ExtendCfgInfo("aclnnSupport.value", "aclnn_only");
this->AICore().AddConfig("ascendxxx", aicore_config);
```

## Return Value<a name="zh-cn_topic_0000001600884192_zh-cn_topic_0000001576875005_zh-cn_topic_0000001525424352_section25791320141317"></a>

For the OpAICoreConfig operator definition, see [OpAICoreConfig](OpAICoreConfig.md).

## Constraints<a name="zh-cn_topic_0000001600884192_zh-cn_topic_0000001576875005_zh-cn_topic_0000001525424352_section19165124931511"></a>

None
