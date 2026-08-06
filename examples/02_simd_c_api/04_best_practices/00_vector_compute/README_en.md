# Vector Compute Practices Sample Introduction

## Overview

This directory provides Vector computation performance optimization samples based on the Ascend C C API, focusing on pipelining GM/UB data movement with Vector computation.

## Sample List

| Directory Name | Description | Supported Products |
| --- | --- | --- |
| [add_high_performance](./add_high_performance) | An Add performance sample that compares a single-buffer baseline with Ping-Pong double buffering under identical computation specifications to demonstrate MTE2, Vector, and MTE3 pipeline overlap. | Ascend 950PR/Ascend 950DT |
