# PyTorch Sample Introduction

## Overview

This directory describes different ways to integrate Ascend C custom operators with PyTorch, including direct pybind11 calls, direct torch.library calls, and TorchAir-based lowering from PyTorch custom operators to GE graph execution.

## Sample List

| Directory Name | Description | Supported Products |
| -------------- | ----------- | --- |
| [pybind](./pybind) |  This sample demonstrates how to register custom operators using pybind11 | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series Products/Atlas A3 Inference Series Products<br>Atlas A2 Training Series Products/Atlas A2 Inference Series Products |
| [torch_library](./torch_library) |  This sample demonstrates how to register custom operators using PyTorch's torch.library mechanism | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series Products/Atlas A3 Inference Series Products<br>Atlas A2 Training Series Products/Atlas A2 Inference Series Products |
| [ge_torchair](./ge_torchair) | This sample demonstrates how to lower a PyTorch custom operator to GE graph execution through TorchAir | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series Products/Atlas A3 Inference Series Products<br>Atlas A2 Training Series Products/Atlas A2 Inference Series Products |
