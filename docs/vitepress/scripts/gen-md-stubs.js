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
