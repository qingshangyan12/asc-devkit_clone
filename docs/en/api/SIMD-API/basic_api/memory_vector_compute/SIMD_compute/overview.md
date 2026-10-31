# Overview

<!-- md-trans-meta sourceCommit=7f211052f814e6d340cc6bf2dfc44c8393b5424f translatedAt=2026-10-26T10:42:52.266Z pushedAt=2026-10-28T03:14:39.574Z nl=2 -->

Vector computation is performed by the Vector compute unit. Both the source operands and destination operands of vector computation are stored in the Unified Buffer (UB).

Memory vector computation provides two computation modes: [continuous computation](continuous_compute.md#continuous-computing) and [high-dimensional partitioning](high_dim_split.md#high-dimensional-split). You can use a [mask](mask.md#mask) to selectively include data in the computation.

The following figure shows how the two computation modes are invoked, using the Add API as an example. Continuous computation is simple and intuitive, and is suitable for processing continuous data of a one-dimensional tensor. High-dimensional partitioning is flexible and controllable, and supports iterative execution and address interval configuration. You can choose an appropriate computation mode based on your actual service requirements.

![](../../../../figures/multi_level_vector_api.png)
