# Language Features Sample Introduction

## Overview

Samples implemented based on Ascend C SIMT, demonstrating key Device-side language features such as Lambda.

## Sample List

| Directory Name | Description |
|---|---|
| [00_lambda_operator_capture](./00_lambda_operator_capture) | Uses a LeakyReLU-Affine fused operator and a Clamp operator as the carrier to demonstrate the practical use of capturing external parameters with Lambdas inside a pure SIMT kernel (capture by value, capture by reference, closure composition, and policy injection). |
