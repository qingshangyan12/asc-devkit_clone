# Introduction<a name="ZH-CN_TOPIC_0000002114097953"></a>

<!-- md-trans-meta sourceCommit=4c68db07c483e21d3d63ceb9ef38c2963da2162f translatedAt=2026-10-23T07:34:33.468Z pushedAt=2026-10-26T11:12:09.518Z nl=2 -->

The ContextBuilder class provides a series of APIs for manually constructing a TilingContext class to verify the Tiling function, and a KernelContext class for verifying the TilingParse function.

## Examples<a name="zh-cn_topic_0000001819028992_zh-cn_topic_0000001389787297_section320753512363"></a>

```
// Construct KernelContext.
auto kernelContextHolder = context_ascendc::ContextBuilder()
    .Inputs(...)
    .Outputs(...)
    .BuildKernelRunContext();
gert::KernelContext* tilingParseContext = kernelContextHolder->GetContext<gert::KernelContext>();

// Construct TilingContext.
auto tilingContextHolder = context_ascendc::ContextBuilder()
    .SetOpNameType(...,...)
    .NodeIoNum(...)
    .IrInstanceNum(...)
    .AddInputTd(...)
    .AddOutputTd(...)
    .AddAttr(...)
    .BuildTilingContext(...);
gert::TilingContext* tilingContext = tilingContextHolder->GetContext<gert::TilingContext>();
```

