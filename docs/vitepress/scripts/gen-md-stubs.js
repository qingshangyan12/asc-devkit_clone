/**
 * Copyright (c) 2026 Example Corp. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

const { readdirSync, writeFileSync, existsSync, readFileSync, mkdirSync } = require('node:fs')
const { join, resolve } = require('node:path')

const docsDir = resolve(__dirname, '..', 'docs')
const manifestFile = resolve(__dirname, '..', '.stubs-manifest.json')
const figuresDir = join(docsDir, 'api', 'figures')
const placeholderSrc = resolve(__dirname, 'placeholder.png')

function findAllHtml(dir) {
  const results = []
  const entries = readdirSync(dir, { withFileTypes: true })
  for (const entry of entries) {
    const fullPath = join(dir, entry.name)
    if (entry.isDirectory()) {
      results.push(...findAllHtml(fullPath))
    } else if (entry.name.endsWith('.html')) {
      results.push(fullPath)
    }
  }
  return results
}

function genStubs(subdir) {
  const dir = join(docsDir, subdir)
  if (!existsSync(dir)) {
    console.log(`Directory ${subdir}/ not found, skipping`)
    return
  }
  const htmlFiles = findAllHtml(dir)
  const stubs = []
  for (const htmlPath of htmlFiles) {
    const mdPath = htmlPath.replace(/\.html$/, '.md')
    if (!existsSync(mdPath)) {
      writeFileSync(mdPath, '', 'utf-8')
      stubs.push(mdPath)
    }
  }
  allStubs.push(...stubs)
  console.log(`  ${subdir}/: generated ${stubs.length} .md stubs (${htmlFiles.length} .html files)`)
}

const allStubs = []
genStubs('api')
genStubs('guide')

writeFileSync(manifestFile, JSON.stringify(allStubs, null, 2), 'utf-8')
console.log(`Total: ${allStubs.length} .md stubs generated`)

if (!existsSync(figuresDir)) {
  mkdirSync(figuresDir, { recursive: true })
}
if (!existsSync(join(figuresDir, 'placeholder.png')) && existsSync(placeholderSrc)) {
  writeFileSync(join(figuresDir, 'placeholder.png'), readFileSync(placeholderSrc))
  console.log('Created figures/placeholder.png')
}
