# AICPU FIFO printf Samples

## Overview

This collection of samples demonstrates the AICPU FIFO-based printf feature, including multi-threaded concurrent printf and high-volume printf triggering ringbuf wrap-around.

## Prerequisites

FIFO printf depends on the rts providing the `AicpuGetDfxInfo` weak symbol and ringbuf memory management. These samples require joint debugging with the relevant modules to be completed before they can run properly.

## Sample List

| Directory | Description | Supported Products |
| --------- | ----------- | --- |
| [01_multithread_printf](./01_multithread_printf) | Demonstrates multiple threads in an AICPU kernel calling printf concurrently | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training/Inference<br>Atlas A2 Training/Inference |
| [02_wraparound_printf](./02_wraparound_printf) | Demonstrates high-volume printf filling the 32KB ringbuf and triggering wrap-around (split-write) | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training/Inference<br>Atlas A2 Training/Inference |
