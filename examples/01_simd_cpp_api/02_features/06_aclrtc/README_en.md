# aclrtc Example Introduction

## Overview

This example demonstrates using Aclrtc (runtime compilation) interfaces to compile Ascend C kernel functions in string form on the Host side and complete the loading and execution of compilation results.

## Example List

| Directory Name                         | Description                                                                           | Supported Products                                                                      |
| -------------------------------------- | ------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| [rtc_hello_world](./rtc_hello_world)   | This example demonstrates calling HelloWorld kernel function using aclrtc interfaces. | Ascend 950PR/Ascend 950DT, Atlas A3训练系列产品/Atlas A3推理系列产品, Atlas A2训练系列产品/Atlas A2推理系列产品 |
| [rtc_template_add](./rtc_template_add) | Implements Add using Aclrtc runtime compilation + LocalMemAllocator (static Tensor)   | Ascend 950PR/Ascend 950DT, Atlas A3训练系列产品/Atlas A3推理系列产品, Atlas A2训练系列产品/Atlas A2推理系列产品 |