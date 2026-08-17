/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

const FILTER_OR_CONTAINER_TOKEN_RE = /<\/?(?:p|td|th)\b[^>]*>|<!--\s*(?:npu\s*=\s*"[^"]*"\s+\w+|end\s+\w+)\s*-->|<\/?cann-filter\b[^>]*>/gi
const NPU_OPEN_RE = /^<!--\s*npu\s*=\s*"([^"]*)"\s+(\w+)\s*-->$/i
const NPU_CLOSE_RE = /^<!--\s*end\s+(\w+)\s*-->$/i
const CANN_OPEN_RE = /^<cann-filter\b([^>]*)>$/i
const CANN_CLOSE_RE = /^<\/cann-filter\b[^>]*>$/i

function classifyFilterToken(value) {
  let match = value.match(NPU_OPEN_RE)
  if (match) {
    return match[1]
      ? { direction: 'open', syntax: 'npu', type: match[1], id: match[2] }
      : null
  }

  match = value.match(NPU_CLOSE_RE)
  if (match) {
    return { direction: 'close', syntax: 'npu', id: match[1] }
  }

  match = value.match(CANN_OPEN_RE)
  if (match) {
    const typeMatch = match[1].match(/npu[_-]type\s*=\s*"([^"]+)"/i)
    return typeMatch
      ? { direction: 'open', syntax: 'cann-filter', type: typeMatch[1] }
      : null
  }

  if (CANN_CLOSE_RE.test(value)) {
    return { direction: 'close', syntax: 'cann-filter' }
  }

  return null
}

function isMatchingPair(opener, closer) {
  if (opener.syntax !== closer.syntax) return false
  return opener.syntax !== 'npu' || opener.id === closer.id
}

export function replaceProductFilterTags(html) {
  const replacements = []
  const stack = []
  const pairs = []
  let paragraphId = null
  let nextParagraphId = 0
  let currentCell = null
  let nextCellId = 0
  let match

  FILTER_OR_CONTAINER_TOKEN_RE.lastIndex = 0
  while ((match = FILTER_OR_CONTAINER_TOKEN_RE.exec(html)) !== null) {
    const value = match[0]

    if (/^<p\b/i.test(value)) {
      paragraphId = ++nextParagraphId
      continue
    }
    if (/^<\/p\b/i.test(value)) {
      paragraphId = null
      continue
    }
    if (/^<(?:td|th)\b/i.test(value)) {
      currentCell = {
        id: ++nextCellId,
        contentStart: match.index + value.length,
        contentEnd: null,
      }
      continue
    }
    if (/^<\/(?:td|th)\b/i.test(value)) {
      if (currentCell) currentCell.contentEnd = match.index
      currentCell = null
      continue
    }

    const token = classifyFilterToken(value)
    const replacement = {
      start: match.index,
      end: match.index + value.length,
      value: '',
    }
    replacements.push(replacement)

    if (!token) continue

    if (token.direction === 'open') {
      replacement.value = `<div data-filter="${token.type}">`
      stack.push({ ...token, paragraphId, cell: currentCell, replacement })
      continue
    }

    const opener = stack[stack.length - 1]
    if (!opener || !isMatchingPair(opener, token)) continue

    stack.pop()
    pairs.push({
      opener,
      closer: replacement,
      paragraphId,
      cell: currentCell,
    })
  }

  for (const pair of pairs) {
    const { opener, closer } = pair
    const isParagraphInline = pair.paragraphId !== null && opener.paragraphId === pair.paragraphId
    let isPartialTableCell = false

    if (!isParagraphInline && opener.cell && opener.cell === pair.cell && opener.cell.contentEnd !== null) {
      const before = html.slice(opener.cell.contentStart, opener.replacement.start).trim()
      const after = html.slice(closer.end, opener.cell.contentEnd).trim()
      isPartialTableCell = before.length > 0 || after.length > 0
    }

    const tagName = isParagraphInline || isPartialTableCell ? 'span' : 'div'
    opener.replacement.value = `<${tagName} data-filter="${opener.type}">`
    closer.value = `</${tagName}>`
  }

  const parts = []
  let lastIndex = 0
  for (const replacement of replacements) {
    parts.push(html.slice(lastIndex, replacement.start), replacement.value)
    lastIndex = replacement.end
  }
  parts.push(html.slice(lastIndex))

  while (stack.length > 0) {
    stack.pop()
    parts.push('</div>')
  }

  return parts.join('')
}
