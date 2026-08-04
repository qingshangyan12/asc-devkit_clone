import assert from 'node:assert/strict'
import { existsSync, readFileSync } from 'node:fs'
import { resolve } from 'node:path'
import test from 'node:test'
import { fileURLToPath } from 'node:url'

const root = resolve(fileURLToPath(new URL('../..', import.meta.url)))
const docsRoot = resolve(root, 'docs')
const configSource = readFileSync(resolve(docsRoot, '.vitepress', 'config.mjs'), 'utf8')
const homeSource = readFileSync(resolve(docsRoot, 'index.md'), 'utf8')

function resolvePageSource(link) {
  const relativePath = link.replace(/^\//, '')
  return ['.md', '.html'].map(extension => resolve(docsRoot, relativePath + extension))
}

test('Ascend C API navigation uses the existing API landing page', () => {
  const navMatch = configSource.match(
    /\{\s*text:\s*['"]Ascend C API['"],\s*link:\s*['"]([^'"]+)['"]\s*\}/
  )
  assert.ok(navMatch, 'Ascend C API navigation item is missing')

  const [, navLink] = navMatch
  assert.equal(navLink, '/api/README')
  assert.ok(
    resolvePageSource(navLink).some(existsSync),
    `navigation target does not exist: ${navLink}`
  )
  assert.match(homeSource, /^\s*link:\s*\/api\/README\s*$/m)
  assert.doesNotMatch(configSource, /\/api\/Ascend-C-API列表/)
})

test('Nginx deployment separates mutable entrypoints from hashed assets', () => {
  const deploySource = readFileSync(resolve(root, 'deploy.sh'), 'utf8')

  assert.match(deploySource, /location = \/hashmap\.json[\s\S]*?no-cache, no-store, must-revalidate/)
  assert.match(deploySource, /location \^~ \/assets\/[\s\S]*?max-age=31536000, immutable/)
  assert.ok(deploySource.includes('try_files \\$uri @previous_assets;'))
  assert.match(deploySource, /root \$\{NGINX_PREVIOUS\}/)
  assert.match(deploySource, /mv -Tf "\$NEXT_LINK" "\$NGINX_CURRENT"/)
  assert.doesNotMatch(deploySource, /rm -rf "\$NGINX_ROOT"/)
})
