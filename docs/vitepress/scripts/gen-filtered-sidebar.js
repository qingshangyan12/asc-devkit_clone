const { existsSync, writeFileSync, readFileSync } = require('node:fs')
const { resolve } = require('node:path')
const { pathToFileURL } = require('node:url')

const docsRoot = resolve(__dirname, '..', 'docs')
const outFile = resolve(__dirname, '..', 'docs', '.vitepress', 'filtered-sidebar.json')

function filterEmptyPages(items, docsRoot) {
  return items.reduce((acc, item) => {
    const entry = { ...item }
    if (item.items) {
      entry.items = filterEmptyPages(item.items, docsRoot)
    }
    if (item.items && entry.items.length > 0) {
      acc.push(entry)
      return acc
    }
    if (item.link) {
      const relPath = item.link.startsWith('/') ? item.link.slice(1) : item.link
      const headerPath = resolve(docsRoot, relPath + '.md.header')
      if (!existsSync(headerPath)) {
        acc.push(entry)
        return acc
      }
      try {
        const headers = JSON.parse(readFileSync(headerPath, 'utf-8'))
        if (headers.length === 0) return acc
      } catch {
        return acc
      }
    }
    acc.push(entry)
    return acc
  }, [])
}

async function main() {
  const sidebarPath = resolve(__dirname, '..', 'docs', '.vitepress', 'sidebar.mjs')
  const sidebarsModule = await import(pathToFileURL(sidebarPath).href)
  const raw = sidebarsModule.default || sidebarsModule

  const filtered = {}
  for (const [key, items] of Object.entries(raw)) {
    filtered[key] = filterEmptyPages(items, docsRoot)
  }

  writeFileSync(outFile, JSON.stringify(filtered), 'utf-8')
  console.log(`Generated filtered sidebar: ${outFile}`)
}

main().catch(err => {
  console.error(err)
  process.exit(1)
})
