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
import test from 'node:test'
import {
  classifyTableColumns,
  measureTextUnits,
  placeTableCells,
} from '../../docs/.vitepress/theme/table_layout.mjs'

const cell = (text, options = {}) => ({
  text,
  colSpan: 1,
  rowSpan: 1,
  hasComplexContent: false,
  ...options,
})

test('measures CJK labels more widely than ASCII identifiers', () => {
  assert.equal(measureTextUnits('数据类型'), 4)
  assert.ok(measureTextUnits('srcNzMatrixStride') < 12)
})

test('keeps short label columns compact and gives prose or code columns room', () => {
  const columns = classifyTableColumns([
    [cell('参数名称'), cell('数据类型'), cell('含义')],
    [cell('srcNzMatrixStride'), cell('必选输入'), cell('源矩阵中相邻地址之间的偏移，单位为数据块。')],
    [cell('quantPre'), cell('可选输入'), cell('enum QuantMode_t', { hasComplexContent: true })],
  ])

  assert.deepEqual(columns.map(column => column.compact), [true, true, false])
  assert.ok(columns[0].preferredUnits > columns[1].preferredUnits)
})

test('does not force long inline-only columns onto one line', () => {
  const columns = classifyTableColumns([
    [cell('名称'), cell('说明')],
    [cell('short'), cell('This inline description is intentionally longer than the compact column limit.')],
  ])

  assert.deepEqual(columns.map(column => column.compact), [true, false])
})

test('places cells correctly across row and column spans', () => {
  const rows = [
    [cell('group', { rowSpan: 2 }), cell('wide', { colSpan: 2 })],
    [cell('value'), cell('description')],
  ]
  const placements = placeTableCells(rows)

  assert.deepEqual(placements[0].map(item => [item.column, item.colSpan]), [[0, 1], [1, 2]])
  assert.deepEqual(placements[1].map(item => [item.column, item.colSpan]), [[1, 1], [2, 1]])
})

test('ignores spanning cells when classifying individual columns', () => {
  const columns = classifyTableColumns([
    [cell('A'), cell('B'), cell('C')],
    [cell('a'), cell('a very long merged description that spans two columns', { colSpan: 2 })],
    [cell('b'), cell('x'), cell('y')],
  ])

  assert.deepEqual(columns.map(column => column.compact), [true, true, true])
})
