# Predicate Optimizations Sample Introduction

## Overview

Predicate optimization samples, implemented through direct `<<<>>>` invocation, introduce predicate-based branch divergence tuning approaches in SIMT programming. Currently, a predicate optimization case based on mathematical function branch chains is provided, demonstrating optimization methods that trade off between predication and real branches according to branch body length and hit rate.

## Sample List

| Directory Name                                              | Description                                                                                        |
| ------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| [predicate_optimization](./predicate_optimization)   | Using the branch chains of the mathematical functions `hlog10` and `j0f` as examples, this sample compares branch organizations such as "early return upon hit" and "unified return with predicated straight-line execution", demonstrating the performance gains and applicable scenarios of predicate optimization. |
