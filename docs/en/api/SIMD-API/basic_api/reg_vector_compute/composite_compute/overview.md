# Overview

<!-- md-trans-meta sourceCommit=7f211052f814e6d340cc6bf2dfc44c8393b5424f translatedAt=2026-09-23T16:09:37.528Z pushedAt=2026-10-26T07:41:44.503Z nl=2 -->

The compound computation supports the following APIs:

**Table 1** Compound computation APIs

| API | Description |
| :--- | :--- |
| [AbsSub](AbsSub.md) | $dstReg_i = \lvert srcReg0_i - srcReg1_i \rvert$ |
| [Axpy](Axpy.md) | $dstReg_i = scalarValue \times srcReg_i + dstReg_i$ |
| [ExpSub](ExpSub.md) | • When the srcReg data type is float:<br>$dstReg_i = e^{srcReg0_i - srcReg1_i}$<br>• When the srcReg data type is half:<br>$dstReg_i = e^{cast\_f16\_to\_f32(srcReg0_i) - cast\_f16\_to\_f32(srcReg1_i)}$ |
| [MulDstAdd](MulDstAdd.md) | $dstReg_i = dstReg_i \times srcReg0_i + srcReg1_i$ |
| [MulAddDst](MulAddDst.md) | $dstReg_i = srcReg0_i \times srcReg1_i + dstReg_i$ |
| [MulsCast](MulsCast.md) | $dst_i = cast\_round\_to\_f16(src_i \times scalarValue)$ |
