# Convolution Format Conversion

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-22T12:53:23.211Z pushedAt=2026-10-28T12:45:45.422Z nl=2 -->

In convolution operations, the data stored in the L0C Buffer is in the NC1HWC0 format. Through the [Nz2ND](NZ2ND.md) capability, the Fixpipe instruction supports converting the data in the L0C Buffer to the NHWC format for output. The conversion diagram is as follows:

**Figure 1** Converting NC1HWC0 to NHWC format

![](../../../../figures/Fixpipe_NC1HWC0_NHWC.png)

<!-- npu="950" id1 -->
In particular, for Ascend 950PR&950DT products, the Fixpipe instruction also supports converting the data in the L0C Buffer to the NCHW format for output through the [Nz2DN](NZ2DN.md) capability. The conversion diagram is as follows:

**Figure 2** Converting NC1HWC0 to NCHW format

![](../../../../figures/Fixpipe_NC1HWC0_NCHW.png)
<!-- end id1 -->
