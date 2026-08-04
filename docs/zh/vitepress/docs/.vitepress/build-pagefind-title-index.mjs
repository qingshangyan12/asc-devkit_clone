/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

import { readFile } from 'node:fs/promises'
import { join, resolve } from 'node:path'
import { gunzipSync } from 'node:zlib'

import { createIndex } from 'pagefind'

const CJK_PHRASE_REGEX = /[\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff]{2,}/g

function cjkAlias(value) {
  return `cjk${[...value].map(character => character.codePointAt(0).toString(16)).join('')}x`
}

function cjkAliases(values) {
  const aliases = new Set()
  for (const value of values) {
    for (const phrase of value.match(CJK_PHRASE_REGEX) || []) {
      aliases.add(cjkAlias(phrase))
      for (let index = 0; index < phrase.length - 1; index++) {
        aliases.add(cjkAlias(phrase.slice(index, index + 2)))
      }
    }
  }
  return [...aliases]
}

function routeSegments(route) {
  return route.split('/').filter(Boolean).map((segment) => {
    try {
      return decodeURIComponent(segment)
    }
    catch {
      return segment
    }
  })
}

function createRecord(route, rawTitle) {
  const title = String(rawTitle || '').replace(/\s+/g, ' ').trim()
    || routeSegments(route).at(-1) || 'Ascend C'
  const segments = routeSegments(route)
  return {
    url: route,
    content: [...new Set([title, ...segments, ...cjkAliases([title, ...segments])])].join(' '),
    language: 'zh',
    meta: { title, ascSearchSource: 'title-route' },
  }
}

function decodeFragment(buffer) {
  let decoded = buffer
  if (!buffer.subarray(0, 12).toString().startsWith('pagefind_dcd')) decoded = gunzipSync(buffer)
  const content = decoded.subarray(12)
  if (content[0] === 0x7B) return JSON.parse(content.toString('utf8'))
  return content
}

async function readPrimaryRecords(distDirectory) {
  const pagefindDirectory = resolve(distDirectory, 'pagefind')
  const fragmentDirectory = resolve(pagefindDirectory, 'fragment')
  const entry = JSON.parse(await readFile(resolve(pagefindDirectory, 'pagefind-entry.json'), 'utf8'))
  const language = Object.keys(entry.languages || {})[0]
  const metadataHash = entry.languages?.[language]?.hash
  if (!metadataHash) throw new Error('Primary Pagefind entry does not contain a language index')
  const metadata = decodeFragment(await readFile(resolve(pagefindDirectory, `pagefind.${metadataHash}.pf_meta`)))
  const fragmentFiles = [...new Set(
    metadata.toString('latin1').match(/[a-z]{2}_[0-9a-f]{7}/g) || []
  )].map(hash => `${hash}.pf_fragment`)
  const records = (await Promise.all(fragmentFiles.map(async (file) => {
    try {
      const fragment = decodeFragment(await readFile(join(fragmentDirectory, file)))
      return createRecord(fragment.url, fragment.meta?.title)
    }
    catch (error) {
      if (error?.code === 'ENOENT') return null
      throw error
    }
  }))).filter(Boolean)
  if (!records.some(record => record.url === '/')) records.push(createRecord('/', 'Ascend C'))
  return records
}

async function buildTitleIndex(distDirectory) {
  const response = await createIndex({ forceLanguage: 'zh', includeCharacters: '_:' })
  if (!response.index || response.errors.length > 0) {
    throw new Error(`Unable to create Pagefind title index: ${response.errors.join('; ')}`)
  }

  const records = await readPrimaryRecords(distDirectory)
  let indexed = 0
  const batchSize = 64
  for (let offset = 0; offset < records.length; offset += batchSize) {
    const results = await Promise.all(
      records.slice(offset, offset + batchSize).map(record => response.index.addCustomRecord(record))
    )
    const errors = results.flatMap(result => result.errors)
    if (errors.length > 0) throw new Error(errors.join('; '))
    indexed += results.length
  }

  const outputPath = resolve(distDirectory, 'pagefind-titles')
  const written = await response.index.writeFiles({ outputPath })
  await response.index.deleteIndex()
  if (written.errors.length > 0) throw new Error(written.errors.join('; '))
  console.log(`[pagefind-titles] Indexed ${indexed} page titles and routes into ${outputPath}`)
}

const distDirectory = process.argv[2]
if (!distDirectory) throw new Error('Usage: node build-pagefind-title-index.mjs <dist-directory>')
await buildTitleIndex(resolve(distDirectory))
