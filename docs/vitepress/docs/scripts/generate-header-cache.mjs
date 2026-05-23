import { readFileSync, writeFileSync, readdirSync, statSync, existsSync } from 'node:fs'
import { resolve, dirname } from 'node:path'
import { fileURLToPath } from 'node:url'

const __dirname = dirname(fileURLToPath(import.meta.url))
const docsRoot = resolve(__dirname, '..')

function slugify(text) {
  return text
    .toLowerCase()
    .replace(/[^\w\u4e00-\u9fff\s-]/g, '')
    .replace(/\s+/g, '-')
    .replace(/-+/g, '-')
    .replace(/(^-|-$)/g, '')
}

function extractHeaders(src) {
  const headers = []
  const lines = src.split('\n')
  for (const line of lines) {
    const match = line.match(/^(#{1,6})\s+(.+)/)
    if (match) {
      const level = match[1].length
      const title = match[2].replace(/ \{#[\w-]+\}$/, '').replace(/<[^>]*>/g, '').trim()
      const slug = slugify(title)
      headers.push({ level, title, slug })
    }
  }
  return headers
}

let total = 0

function walk(dir) {
  const entries = readdirSync(dir)
  for (const entry of entries) {
    const full = resolve(dir, entry)
    if (statSync(full).isDirectory()) {
      if (entry === '.vitepress' || entry === 'node_modules' || entry === 'figures' || entry === 'scripts') continue
      walk(full)
    } else if (entry.endsWith('.md')) {
      const src = readFileSync(full, 'utf-8')
      const headers = extractHeaders(src)
      const cachePath = full + '.header'
      writeFileSync(cachePath, JSON.stringify(headers))
      total++
    }
  }
}

console.log('Generating header cache files...')
walk(docsRoot)
console.log(`Done. Cached headers for ${total} .md files.`)
