/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

import assert from 'node:assert/strict'
import { test } from 'node:test'

import {
  findKernelLaunch,
  kernelLaunchMatches,
  normalizeKernelLaunchQuery,
} from '../../docs/.vitepress/search-syntax.mjs'

test('normalizes kernel launch syntax queries and prefixes', () => {
  for (const query of [
    '<<<',
    '<<<gridDim',
    '<<<gridDim, blockDim>>',
    '<<<>>>',
    ' <<<...>>> ',
    '<<<gridDim, blockDim>>>',
  ]) {
    assert.equal(normalizeKernelLaunchQuery(query), '<<<>>>')
  }
})

test('does not treat partial or mixed text as a kernel launch syntax query', () => {
  for (const query of ['', '<<', '>>>', 'prefix <<<>>> suffix', 'foo<<<1>>>']) {
    assert.equal(normalizeKernelLaunchQuery(query), '')
  }
})

test('finds complete launch syntax while preferring the canonical spelling', () => {
  assert.deepEqual(kernelLaunchMatches('a <<<1, 2>>> b <<<>>>'), [
    { index: 2, value: '<<<1, 2>>>' },
    { index: 15, value: '<<<>>>' },
  ])
  assert.deepEqual(findKernelLaunch('a <<<1>>> b <<<>>>'), {
    index: 12,
    value: '<<<>>>',
  })
})
