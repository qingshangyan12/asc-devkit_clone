/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


const { existsSync, unlinkSync, readFileSync, readdirSync, statSync } = require('node:fs')
const { isAbsolute, relative, resolve } = require('node:path')

const root = resolve(__dirname, '..')
const docsDir = resolve(root, 'docs')
const manifestFile = resolve(root, '.stubs-manifest.json')
const generatedFiles = new Set()
const generatedHeaders = new Set()

function isInsideRoot(file) {
  const rel = relative(root, resolve(file))
  return rel !== '' && !rel.startsWith('..') && !isAbsolute(rel)
}

function discoverGeneratedFiles(directory) {
  if (!existsSync(directory)) return
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    const fullPath = resolve(directory, entry.name)
    if (entry.isDirectory()) {
      discoverGeneratedFiles(fullPath)
      continue
    }
    if (!entry.isFile()) continue

    if (entry.name.endsWith('.md.header')) {
      const htmlPath = fullPath.slice(0, -'.md.header'.length) + '.html'
      if (existsSync(htmlPath)) generatedHeaders.add(fullPath)
      continue
    }

    if (entry.name.endsWith('.md') && statSync(fullPath).size === 0) {
      const htmlPath = fullPath.slice(0, -'.md'.length) + '.html'
      if (existsSync(htmlPath)) generatedFiles.add(fullPath)
    }
  }
}

if (existsSync(manifestFile)) {
  try {
    for (const file of JSON.parse(readFileSync(manifestFile, 'utf-8'))) {
      if (isInsideRoot(file)) generatedFiles.add(resolve(file))
    }
  } catch {}
}

discoverGeneratedFiles(resolve(docsDir, 'api'))
discoverGeneratedFiles(resolve(docsDir, 'guide'))

let removedFiles = 0
let removedHeaders = 0
for (const file of generatedFiles) {
  if (existsSync(file)) {
    unlinkSync(file)
    removedFiles++
  }
  const headerFile = file + '.header'
  if (existsSync(headerFile)) generatedHeaders.add(headerFile)
}
for (const headerFile of generatedHeaders) {
  if (existsSync(headerFile)) {
    unlinkSync(headerFile)
    removedHeaders++
  }
}

if (existsSync(manifestFile)) unlinkSync(manifestFile)
console.log(`Cleaned up ${removedFiles} generated files and ${removedHeaders} header caches`)
