const { readdirSync, writeFileSync, existsSync, readFileSync } = require('node:fs')
const { join, resolve } = require('node:path')

const apiDir = resolve(__dirname, '..', 'docs', 'api')
const manifestFile = resolve(__dirname, '..', '.stubs-manifest.json')

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

const htmlFiles = findAllHtml(apiDir)
const stubs = []

for (const htmlPath of htmlFiles) {
  const mdPath = htmlPath.replace(/\.html$/, '.md')
  if (!existsSync(mdPath)) {
    writeFileSync(mdPath, '', 'utf-8')
    stubs.push(mdPath)
  }
}

writeFileSync(manifestFile, JSON.stringify(stubs, null, 2), 'utf-8')
console.log(`Generated ${stubs.length} .md stubs (${htmlFiles.length} total .html files)`)
