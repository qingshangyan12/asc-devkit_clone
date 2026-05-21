import { readFileSync, writeFileSync, globSync, cpSync, rmSync } from 'node:fs'
import { resolve, relative } from 'node:path'

const docsDir = resolve(import.meta.dirname, '..')
const distDir = resolve(docsDir, '.vitepress', 'dist')
const staticDir = resolve(docsDir, '.vitepress', 'dist-static')

function processHtml(dir, { decodeUrls = true, fixAbsoluteToRelative = false, stripSpa = false } = {}) {
  let pathFixed = 0, decoded = 0, stripped = 0
  const htmlFiles = globSync('**/*.html', { cwd: dir }).map(f => resolve(dir, f))

  for (const filePath of htmlFiles) {
    const relPath = relative(dir, filePath)
    const depth = relPath.split(/[\\/]/).length - 1
    let content = readFileSync(filePath, 'utf-8')

    if (fixAbsoluteToRelative) {
      const prefix = '../'.repeat(depth)
      content = content.replace(
        /(href|src|content)="(\/(?!\/)[^"]*)"/g,
        (match, attr, url) => {
          if (url === '/') return `${attr}="${prefix}./"`
          return `${attr}="${prefix}${url.slice(1)}"`
        }
      )
      pathFixed++
    }

    if (decodeUrls) {
      const urlAttr = /(href|src)\s*=\s*"([^"]*%[0-9A-Fa-f]{2}[^"]*)"/gi
      let changed = false
      content = content.replace(urlAttr, (match, attr, url) => {
        if (!/%[0-9A-Fa-f]{2}/.test(url)) return match
        try {
          const d = decodeURI(url)
          if (d !== url) { changed = true; return `${attr}="${d}"` }
        } catch {}
        return match
      })
      if (changed) decoded++
    }

    if (stripSpa) {
      content = content.replace(/[\t ]*<script\b[^>]*?\btype="module"[^>]*?>\s*<\/script>\s*/g, '')
      content = content.replace(/[\t ]*<script\b[^>]*?\btype="module"[^>]*?src="[^"]*"[^>]*?>\s*<\/script>\s*/g, '')
      content = content.replace(/[\t ]*<link\b[^>]*?\brel="modulepreload"[^>]*?>\s*/g, '')
      stripped++
    }

    writeFileSync(filePath, content, 'utf-8')
  }

  return { pathFixed, decoded, stripped }
}

console.log('Processing SPA dist...')
const spaResult = processHtml(distDir, { decodeUrls: true })
console.log(`  URL decoded: ${spaResult.decoded}`)

console.log('\nCreating static dist...')
rmSync(staticDir, { recursive: true, force: true })
cpSync(distDir, staticDir, { recursive: true })
const staticResult = processHtml(staticDir, { fixAbsoluteToRelative: true, stripSpa: true })
console.log(`  Paths fixed: ${staticResult.pathFixed}`)
console.log(`  SPA stripped: ${staticResult.stripped}`)

console.log(`\ndist/       -> SPA (full interactivity, use vitepress preview)`)
console.log(`dist-static/ -> Static (file:// compatible, zip & send)`)
