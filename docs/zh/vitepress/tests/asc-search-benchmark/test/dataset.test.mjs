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
import { readFile } from 'node:fs/promises'
import { test } from 'node:test'

const dataset = JSON.parse(await readFile(new URL('../cases/ascendc.json', import.meta.url), 'utf8'))

test('dataset has unique and complete cases', () => {
  assert.equal(dataset.cases.length, 155)
  assert.equal(new Set(dataset.cases.map(item => item.id)).size, dataset.cases.length)
  for (const item of dataset.cases) {
    assert.ok(item.id)
    assert.ok(item.query)
    assert.ok(item.category)
    assert.ok(Array.isArray(item.expect.targets))
    if (item.expect.descriptionIncludes !== undefined) {
      assert.ok(Array.isArray(item.expect.descriptionIncludes))
      assert.ok(item.expect.descriptionIncludes.length > 0)
      assert.ok(item.expect.descriptionIncludes.every(value => typeof value === 'string' && value))
    }
    if (item.expect.highlightIncludes !== undefined) {
      assert.ok(Array.isArray(item.expect.highlightIncludes))
      assert.ok(item.expect.highlightIncludes.length > 0)
      assert.ok(item.expect.highlightIncludes.every(value => typeof value === 'string' && value))
    }
    if (item.expect.sectionHash !== undefined) {
      assert.match(item.expect.sectionHash, /^#./)
    }
    for (const target of item.expect.targets) {
      assert.equal(['exact', 'prefix', 'contains'].filter(key => key in target).length, 1)
      assert.ok([1, 2, 3].includes(target.grade))
    }
  }
})

test('dataset covers positive and negative search intents', () => {
  assert.ok(dataset.cases.some(item => item.expect.targets.length === 0))
  assert.ok(dataset.cases.some(item => item.expect.targets.length > 0))
  assert.ok(dataset.cases.some(item => item.category === 'api-case-sensitive'))
  assert.ok(dataset.cases.some(item => item.category === 'zh-content'))
})

test('dataset covers compiler option identifier regressions', () => {
  const target = '/guide/编程指南/编译与运行/算子编译/AI-Core算子编译基本用法'
  for (const query of ['NO_OVER', 'NO_OVERLAP_IN_MULTI_REPEAT', 'no over', 'no-over', 'NO_O', 'NO_OV', 'NO_OVE']) {
    const item = dataset.cases.find(candidate => candidate.query === query)
    assert.ok(item, `missing case for ${query}`)
    assert.equal(item.expect.minResults, 1)
    assert.equal(item.expect.maxResults, 1)
    assert.deepEqual(item.expect.targets, [{ exact: target, grade: 3 }])
    assert.deepEqual(item.expect.descriptionIncludes, ['NO_OVERLAP_IN_MULTI_REPEAT', '内存同步指令'])
  }
  assert.equal(dataset.cases.find(item => item.query === 'NO_OVER').expect.sectionHash, '#内置编译宏开关')
  assert.deepEqual(dataset.cases.find(item => item.query === 'NO_OVER').expect.highlightIncludes, ['NO_OVER'])
})

test('dataset covers existing section navigation', () => {
  const item = dataset.cases.find(candidate => candidate.id === 'zh-section-memory-lifecycle')
  assert.ok(item)
  assert.equal(item.expect.sectionHash, '#内存生命周期管理：AllocTensor/FreeTensor解决读后写冲突')
})

test('dataset covers humanized identifiers and incomplete prefixes', () => {
  for (const category of ['identifier-separator', 'identifier-prefix', 'datatype-separator', 'parameter-separator']) {
    assert.ok(dataset.cases.some(item => item.category === category), `missing category ${category}`)
  }
  for (const query of ['uint32 t', 'fp8 e4m3fn t', 'dst repeat stride', 'GET_TIL', 'vector_uint']) {
    assert.ok(dataset.cases.some(item => item.query === query), `missing case for ${query}`)
  }
})
