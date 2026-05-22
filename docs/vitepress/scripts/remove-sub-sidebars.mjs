import { readFileSync, writeFileSync } from 'node:fs'
import { resolve } from 'node:path'

const root = resolve(import.meta.dirname, '..')
const sidebarPath = resolve(root, 'docs', '.vitepress', 'sidebar.mjs')

let content = readFileSync(sidebarPath, 'utf-8')

// Remove specific sidebar entries: /api/SIMD-API/, /api/SIMT-API/, /api/Utils-API/, /api/AI-CPU-API/
const keysToRemove = [
  '/api/SIMD-API/',
  '/api/SIMT-API/',
  '/api/Utils-API/',
  '/api/AI-CPU-API/'
]

for (const key of keysToRemove) {
  const searchKey = `  "${key}": [`
  const startIdx = content.indexOf(searchKey)
  if (startIdx === -1) {
    console.log(`Not found: ${key}`)
    continue
  }

  // From startIdx, find matching closing bracket
  let depth = 0
  let idx = startIdx
  let found = false
  for (; idx < content.length; idx++) {
    if (content[idx] === '[') depth++
    else if (content[idx] === ']') {
      depth--
      if (depth === 0) {
        found = true
        break
      }
    }
  }

  if (!found) {
    console.log(`Could not find closing bracket for ${key}`)
    continue
  }

  // Now look for the comma after the closing bracket
  let endIdx = idx + 1
  // Skip whitespace
  while (endIdx < content.length && /\s/.test(content[endIdx])) {
    endIdx++
  }
  if (endIdx < content.length && content[endIdx] === ',') {
    endIdx++
  }

  content = content.substring(0, startIdx) + content.substring(endIdx)
  console.log(`Removed: ${key}`)
}

writeFileSync(sidebarPath, content, 'utf-8')
const lines = content.split('\n').length
console.log(`Done. ${lines} lines remaining.`)
