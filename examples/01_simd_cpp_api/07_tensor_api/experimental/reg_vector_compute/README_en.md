# Register Vector Compute Tensor API Samples

## Overview

This directory contains samples for the experimental register Tensor APIs. The samples demonstrate how to move tensors between GM and UB and then perform vector processing through register load, compute, and store interfaces.

## Sample List

| Directory | Description | Supported Products |
| --- | --- | --- |
| [copy_vector](./copy_vector) | Demonstrates GM-to-UB, UB-to-GM, and UB-to-UB Tensor copies, including one-dimensional copies and left/right constant padding. | Ascend 950PR/Ascend 950DT |
| [cast](./cast) | Performs element-wise conversions between multiple data types by using the experimental register Tensor load, cast, and store interfaces. | Ascend 950PR/Ascend 950DT |
| [trunc](./trunc) | Uses the experimental register Tensor APIs to truncate FP16, BF16, and FP32 values toward zero element by element. | Ascend 950PR/Ascend 950DT |
