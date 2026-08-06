/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

const CJK_REGEX = /[\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff]/
const CJK_PHRASE_REGEX = /[\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff]{2,}/g
const RESULTS_PER_VARIANT = 80
const MAX_CANDIDATES = 240
const MAX_IDENTIFIER_MATCHES = 64
const TITLE_INDEX_SOURCE = 'title-route'
const initializedPagefindInstances = new WeakSet()
let identifierIndexPromise = null

function normalizeCase(value) {
  return String(value || '').normalize('NFKC').replace(/\s+/g, ' ').trim()
}

function normalize(value) {
  return normalizeCase(value).toLocaleLowerCase()
}

function compactEnglish(value) {
  return normalize(value).replace(/[^a-z0-9]+/g, '')
}

function normalizeRoute(url) {
  let route = String(url || '').split('#')[0].split('?')[0]
  try {
    route = decodeURIComponent(route)
  }
  catch {}
  route = route.replace(/\.html?$/, '').replace(/\/index$/, '/')
  return route || '/'
}

function routeWithSection(route, section) {
  if (!section) return route
  const path = String(route || '').split('#')[0]
  return `${path}#${encodeURIComponent(section)}`
}

function getLeadingIdentifier(value) {
  return normalize(value).match(/^[a-z_][a-z0-9_]*/)?.[0] || ''
}

function identifierMatch(value, query) {
  const tokens = normalize(value).match(/[a-z_][a-z0-9_]*/g) || []
  if (tokens.includes(query)) return 'exact'
  if (query.length >= 4 && tokens.some(token => token.startsWith(query))) return 'prefix'
  return ''
}

function isDirectoryLandingPage(url, title) {
  const segments = normalizeRoute(url).split('/').filter(Boolean)
  if (segments.length < 2) return false
  return normalize(segments.at(-1)) === normalize(segments.at(-2))
    && normalize(segments.at(-1)) === normalize(title)
}

function isSingleEditMatch(left, right) {
  if (left === right) return true
  if (Math.abs(left.length - right.length) > 1) return false

  if (left.length === right.length) {
    const differences = []
    for (let index = 0; index < left.length; index++) {
      if (left[index] !== right[index]) differences.push(index)
      if (differences.length > 2) return false
    }
    if (differences.length === 1) return true
    return differences.length === 2
      && differences[1] === differences[0] + 1
      && left[differences[0]] === right[differences[1]]
      && left[differences[1]] === right[differences[0]]
  }

  const [shorter, longer] = left.length < right.length ? [left, right] : [right, left]
  let shortIndex = 0
  let longIndex = 0
  let skipped = false
  while (shortIndex < shorter.length && longIndex < longer.length) {
    if (shorter[shortIndex] === longer[longIndex]) {
      shortIndex++
      longIndex++
      continue
    }
    if (skipped) return false
    skipped = true
    longIndex++
  }
  return true
}

function singleEditVariants(value) {
  const variants = new Set()
  for (let index = 0; index < value.length; index++) {
    variants.add(value.slice(0, index) + value.slice(index + 1))
    if (index + 1 < value.length) {
      variants.add(value.slice(0, index) + value[index + 1] + value[index] + value.slice(index + 2))
    }
  }
  variants.delete(value)
  return [...variants].filter(variant => variant.length >= 4)
}

function generateSubPhrases(phrase) {
  const result = []
  for (let length = phrase.length; length >= 2; length--) {
    for (let index = 0; index <= phrase.length - length; index++) {
      result.push(phrase.slice(index, index + length))
    }
  }
  return result
}

function cjkAlias(value) {
  return `cjk${[...value].map(character => character.codePointAt(0).toString(16)).join('')}x`
}

function findLongestMatch(subPhrases, content) {
  let longestLength = 0
  let totalLength = 0
  const matchedLengths = new Set()
  for (const phrase of subPhrases) {
    if (!content.includes(phrase)) continue
    if (!matchedLengths.has(phrase.length)) {
      matchedLengths.add(phrase.length)
      totalLength += phrase.length
    }
    longestLength = Math.max(longestLength, phrase.length)
  }
  return { longestLength, totalLength }
}

function routeMatchBonus(url, cjkPhrases) {
  const segments = normalizeRoute(url).split('/')
  const bigrams = cjkPhrases.flatMap((phrase) => {
    const values = []
    for (let index = 0; index < phrase.length - 1; index++) values.push(phrase.slice(index, index + 2))
    return values
  })
  if (bigrams.length === 0) return 0
  const bestMatch = segments.reduce((best, segment) => {
    return Math.max(best, bigrams.filter(bigram => segment.includes(bigram)).length)
  }, 0)
  const coverage = bestMatch / bigrams.length
  return bestMatch >= 2 && coverage >= 0.6 ? Math.round(80000 * coverage) : 0
}

function compactCjkRouteBonus(url, cjkPhrases) {
  if (cjkPhrases.length === 0) return 0
  const segments = normalizeRoute(url).split('/').map(segment => segment.replace(/[^\u3400-\u9fff\uf900-\ufaff]+/g, ''))
  return cjkPhrases.some(phrase => segments.includes(phrase)) ? 60000 : 0
}

function createQueryPlan(query) {
  const caseQuery = normalizeCase(query)
  const normalizedQuery = normalize(query)
  const compactQuery = compactEnglish(query)
  const cjkPhrases = caseQuery.match(CJK_PHRASE_REGEX) || []
  const allEnglishParts = normalizedQuery
    .replace(/[\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff]+/g, ' ')
    .match(/[a-z0-9]+/g) || []
  const englishParts = allEnglishParts.filter(part => part.length >= 2)
  const identifierIntent = caseQuery.includes('_') && /^[a-z_][a-z0-9_]*$/i.test(caseQuery)
  const separatedIdentifierIntent = cjkPhrases.length === 0
    && allEnglishParts.length >= 2
    && /^[a-z0-9_\-\s]+$/i.test(caseQuery)
  const identifierQuery = identifierIntent
    ? normalizedQuery
    : separatedIdentifierIntent ? allEnglishParts.join('_') : ''
  return {
    query: String(query),
    caseQuery,
    normalizedQuery,
    compactQuery,
    identifierIntent,
    separatedIdentifierIntent,
    identifierQuery,
    strongIdentifierIntent: identifierIntent,
    cjkPhrases,
    englishParts,
    cjkSubPhrases: cjkPhrases.map(generateSubPhrases),
  }
}

function addVariant(variants, query, weight) {
  const value = normalizeCase(query)
  if (value.length < 2) return
  const existing = variants.get(value)
  if (!existing || weight > existing.weight) variants.set(value, { query: value, weight })
}

function primaryVariants(plan) {
  const variants = new Map()
  addVariant(variants, plan.query, 2)
  addVariant(variants, plan.caseQuery, 2)
  addVariant(variants, `"${plan.caseQuery}"`, 2.4)

  const separated = plan.caseQuery.replace(/[_:<>()[\]{}\-]+/g, ' ').replace(/\s+/g, ' ').trim()
  addVariant(variants, separated, 1.7)
  if (plan.englishParts.length > 0) {
    addVariant(variants, plan.englishParts.join(' '), 1.6)
    addVariant(variants, `"${plan.englishParts.join(' ')}"`, 2.1)
    addVariant(variants, plan.englishParts.join('_'), 1.5)
    if (!plan.identifierIntent) {
      addVariant(variants, plan.compactQuery, 1.4)
      if (!plan.strongIdentifierIntent) {
        for (const part of plan.englishParts) addVariant(variants, part, 0.8)
      }
    }
  }
  for (const phrase of plan.cjkPhrases) {
    addVariant(variants, phrase, 1.4)
    addVariant(variants, `"${phrase}"`, 2)
  }
  return [...variants.values()]
}

function titleVariants(plan, variants) {
  const titleQueries = new Map(variants.map(variant => [variant.query, variant]))
  for (const phrase of plan.cjkPhrases) {
    addVariant(titleQueries, cjkAlias(phrase), 3)
    for (let index = 0; index < phrase.length - 1; index++) {
      addVariant(titleQueries, cjkAlias(phrase.slice(index, index + 2)), 1.5)
    }
  }
  return [...titleQueries.values()]
}

async function loadIdentifierIndex() {
  if (!identifierIndexPromise) {
    identifierIndexPromise = fetch('/search-identifiers.json?v=5', { cache: 'no-store' })
      .then((response) => {
        if (!response.ok) throw new Error(`Identifier index request failed with ${response.status}`)
        return response.json()
      })
      .then(index => Array.isArray(index?.p) && Array.isArray(index?.i) ? index : { p: [], i: [], s: [], h: [] })
      .catch(() => ({ p: [], i: [], s: [], h: [] }))
  }
  return identifierIndexPromise
}

function escapeHtml(value) {
  return String(value || '')
    .replaceAll('&', '&amp;')
    .replaceAll('<', '&lt;')
    .replaceAll('>', '&gt;')
    .replaceAll('"', '&quot;')
    .replaceAll("'", '&#39;')
}

function highlightIdentifierExcerpt(excerpt, query) {
  const text = String(excerpt || '')
  const position = text.toLocaleLowerCase().indexOf(query)
  if (position < 0) return escapeHtml(text)
  const end = position + query.length
  return `${escapeHtml(text.slice(0, position))}<mark>${escapeHtml(text.slice(position, end))}</mark>${escapeHtml(text.slice(end))}`
}

function lowerBoundIdentifier(entries, query) {
  let left = 0
  let right = entries.length
  while (left < right) {
    const middle = Math.floor((left + right) / 2)
    if (String(entries[middle]?.[0] || '') < query) left = middle + 1
    else right = middle
  }
  return left
}

async function searchIdentifierIndex(plan) {
  const query = plan.identifierQuery
  if (!query || compactEnglish(query).length < 3) {
    return { exact: false, exclusive: false, strong: false, results: [] }
  }
  const index = await loadIdentifierIndex()
  const entries = index.i
  const pages = index.p
  const snippets = Array.isArray(index.s) ? index.s : []
  const sections = Array.isArray(index.h) ? index.h : []
  const start = lowerBoundIdentifier(entries, query)
  const matches = []
  for (let offset = start; offset < entries.length && matches.length <= MAX_IDENTIFIER_MATCHES; offset++) {
    const entry = entries[offset]
    if (!String(entry?.[0] || '').startsWith(query)) break
    matches.push(entry)
  }
  if (matches.length === 0) return { exact: false, exclusive: false, strong: false, results: [] }
  const truncated = matches.length > MAX_IDENTIFIER_MATCHES
  const selectedMatches = matches.slice(0, MAX_IDENTIFIER_MATCHES)

  const resultsByRoute = new Map()
  let exactIdentifier = false
  for (const [identifier, pageIds, contextIds = [], sectionIds = []] of selectedMatches) {
    const exact = identifier === query
    exactIdentifier ||= exact
    const identifierScore = (exact ? 3300000 : 3100000)
      - Math.max(0, identifier.length - query.length) * 1000
    for (let rank = 0; rank < pageIds.length; rank++) {
      const page = pages[pageIds[rank]]
      if (!page) continue
      const [pageRoute, title, pageDescription = ''] = page
      const contextId = contextIds[rank]
      const sourceDescription = Number.isInteger(contextId) && contextId >= 0
        ? snippets[contextId] || pageDescription
        : pageDescription
      const description = highlightIdentifierExcerpt(sourceDescription, query)
      const sectionId = sectionIds[rank]
      const route = routeWithSection(
        pageRoute,
        Number.isInteger(sectionId) && sectionId >= 0 ? sections[sectionId] : ''
      )
      const result = {
        route,
        title: title || route,
        description,
        meta: { title: title || route, description },
        score: identifierScore - rank * 20000,
      }
      const normalizedRoute = normalizeRoute(route)
      resultsByRoute.set(normalizedRoute, mergeRouteResult(resultsByRoute.get(normalizedRoute), result))
    }
  }
  const results = [...resultsByRoute.values()]
    .sort((left, right) => right.score - left.score || left.route.localeCompare(right.route))
    .slice(0, 20)
  const uniquePrefix = !exactIdentifier && !truncated && matches.length === 1
  const ambiguousSeparatedPrefix = plan.separatedIdentifierIntent
    && !plan.identifierIntent
    && !exactIdentifier
    && !uniquePrefix
  if (ambiguousSeparatedPrefix) {
    return { exact: false, exclusive: false, strong: false, results: [] }
  }
  return {
    exact: exactIdentifier,
    exclusive: uniquePrefix && results.length === 1,
    strong: plan.identifierIntent || !exactIdentifier,
    results,
  }
}

async function collectCandidates(pagefind, variants, candidates = new Map(), source = 'content') {
  const responses = await Promise.all(variants.map(async (variant) => {
    try {
      return { variant, response: await pagefind.search(variant.query) }
    }
    catch {
      return { variant, response: null }
    }
  }))

  for (const { variant, response } of responses) {
    const results = response?.results?.slice(0, RESULTS_PER_VARIANT) || []
    for (let rank = 0; rank < results.length; rank++) {
      const result = results[rank]
      const key = `${source}:${result.id || `${variant.query}:${rank}`}`
      const sourceWeight = source === 'title' ? 1.3 : 1
      const reciprocalRank = sourceWeight * variant.weight / (60 + rank + 1)
      const existing = candidates.get(key)
      if (existing) {
        existing.retrievalScore += reciprocalRank
        existing.bestEngineScore = Math.max(existing.bestEngineScore, Number(result.score) || 0)
        existing.bestRank = Math.min(existing.bestRank, rank)
      }
      else {
        candidates.set(key, {
          result,
          retrievalScore: reciprocalRank,
          bestEngineScore: Number(result.score) || 0,
          bestRank: rank,
          data: null,
        })
      }
    }
  }
  return candidates
}

async function materializeCandidates(candidates) {
  const selected = [...candidates.values()]
    .sort((left, right) => right.retrievalScore - left.retrievalScore
      || right.bestEngineScore - left.bestEngineScore)
    .slice(0, MAX_CANDIDATES)
  await Promise.all(selected.map(async (candidate) => {
    if (!candidate.data) {
      try {
        candidate.data = await candidate.result.data()
      }
      catch {
        candidate.data = {}
      }
    }
  }))
  return selected
}

function bestPagefindSectionRoute(data) {
  const pageRoute = data.url || data.raw_url || ''
  const subResults = Array.isArray(data.sub_results) ? data.sub_results : []
  if (subResults.length === 0) return pageRoute
  const weightedLocations = Array.isArray(data.weighted_locations)
    ? [...data.weighted_locations].sort((left, right) => right.weight - left.weight || left.location - right.location)
    : []
  for (const { location } of weightedLocations) {
    const matching = subResults.filter((subResult) => {
      const locations = subResult.locations || []
      const first = locations[0]
      if (!Number.isFinite(first)) return false
      const last = locations.length === 1 ? Number.POSITIVE_INFINITY : locations.at(-1)
      return first <= location && location <= last
    })
    const best = matching.reduce((selected, candidate) => {
      if (!selected || candidate.locations.length > selected.locations.length) return candidate
      return selected
    }, null)
    if (best?.url) return best.url
  }
  return subResults[0]?.url || pageRoute
}

function candidateFields(candidate) {
  const data = candidate.data || {}
  const title = String(data.meta?.title || '').replace(/___PAGEFIND_ANCHOR___\S*/g, '').trim()
  const route = bestPagefindSectionRoute(data)
  const headings = (data.anchors || []).map(anchor => String(anchor.text || '').trim()).filter(Boolean)
  const text = [title, data.content, data.raw_content, data.excerpt, ...headings, normalizeRoute(route)]
    .filter(Boolean)
    .join(' ')
  return { data, title, route, headings, text }
}

function scoreCandidate(candidate, plan) {
  const { data, title, route, headings, text } = candidateFields(candidate)
  const normalizedTitle = normalize(title)
  const caseTitle = normalizeCase(title)
  const normalizedText = normalize(text)
  const normalizedHeadings = headings.map(normalize)
  const compactTitle = compactEnglish(title)
  const leadingIdentifier = getLeadingIdentifier(title)
  const identifierTitleMatch = plan.identifierIntent
    ? identifierMatch(title, plan.normalizedQuery)
    : ''
  const identifierRouteMatch = plan.identifierIntent
    ? identifierMatch(normalizeRoute(route), plan.normalizedQuery)
    : ''
  const identifierTextMatch = plan.identifierIntent
    ? identifierMatch(text, plan.normalizedQuery)
    : ''
  if (plan.identifierIntent && !identifierTextMatch) return null
  const pureEnglishTitle = !CJK_REGEX.test(title)
  const routeDepth = normalizeRoute(route).split('/').filter(Boolean).length
  const retrievalTieScore = candidate.retrievalScore * 5000
    + Math.log1p(candidate.bestEngineScore) * 1000
    + Math.max(0, 80 - candidate.bestRank) * 30
  let mixedEnglishTitleParts = 0
  if (plan.cjkPhrases.length > 0 && plan.englishParts.length > 0 && pureEnglishTitle) {
    let joinedParts = ''
    for (const part of plan.englishParts) {
      joinedParts += part
      if (compactTitle === joinedParts) mixedEnglishTitleParts++
    }
  }
  const mixedPrimaryTitle = mixedEnglishTitleParts > 0

  let pageScore = candidate.retrievalScore * 100000
    + Math.log1p(candidate.bestEngineScore) * 1000
  if (identifierTextMatch === 'exact') {
    const canonicalIdentifier = identifierTitleMatch === 'exact' || identifierRouteMatch === 'exact'
    pageScore = Math.max(pageScore, canonicalIdentifier ? 2600000 : 2300000)
  }
  else if (identifierTextMatch === 'prefix') {
    const canonicalIdentifier = identifierTitleMatch === 'prefix' || identifierRouteMatch === 'prefix'
    pageScore = Math.max(pageScore, canonicalIdentifier ? 2300000 : 2150000)
  }
  let coverage = plan.cjkPhrases.length > 0 ? 1 : 0
  let cjkScore = 0

  for (let index = 0; index < plan.cjkPhrases.length; index++) {
    const phrase = plan.cjkPhrases[index]
    const matches = findLongestMatch(plan.cjkSubPhrases[index], normalizedText)
    if (matches.longestLength === 0) {
      if (!mixedPrimaryTitle) return null
      coverage = Math.min(coverage, 0.75)
      continue
    }
    const phraseCoverage = matches.longestLength / phrase.length
    coverage = Math.min(coverage, phraseCoverage)
    cjkScore = Math.max(cjkScore, matches.longestLength * 10000 + matches.totalLength * 100)
    if (normalizedTitle === normalize(phrase)) cjkScore = Math.max(cjkScore, 1000000 + phrase.length * 100)
    else if (normalizedTitle.includes(normalize(phrase))) cjkScore = Math.max(cjkScore, 900000 + phrase.length * 100)
  }
  if (plan.cjkPhrases.length > 0 && coverage < 0.5) return null
  pageScore = Math.max(pageScore, cjkScore)

  const typoTitleMatch = plan.cjkPhrases.length === 0
    && plan.englishParts.length === 1
    && plan.compactQuery.length >= 5
    && (isSingleEditMatch(plan.compactQuery, compactTitle)
      || (leadingIdentifier.length >= 5 && isSingleEditMatch(plan.compactQuery, leadingIdentifier)))
  let englishMatched = true
  for (const part of plan.englishParts) {
    if (!normalizedText.includes(part)) {
      englishMatched = false
      break
    }
  }
  if (plan.englishParts.length > 0 && !englishMatched && !typoTitleMatch) return null
  if (plan.cjkPhrases.length === 0 && plan.englishParts.length > 0) {
    coverage = englishMatched ? 1 : (plan.compactQuery.length - 1) / plan.compactQuery.length
  }

  if (plan.cjkPhrases.length === 0 && plan.englishParts.length > 0) {
    let englishScore = Math.max(...plan.englishParts.map(part => part.length * 10000), 0)
      + plan.englishParts.reduce((sum, part) => sum + part.length, 0) * 100
    const titleExact = plan.compactQuery.length >= 2 && compactTitle === plan.compactQuery
    const titlePrefix = plan.compactQuery.length >= 2 && compactTitle.startsWith(plan.compactQuery)
    const titleContains = plan.compactQuery.length >= 2 && compactTitle.includes(plan.compactQuery)
    const identifierExact = leadingIdentifier && leadingIdentifier === plan.compactQuery
    if (typoTitleMatch) englishScore = Math.max(englishScore, 700000 + plan.compactQuery.length * 100)
    if (titleContains && plan.englishParts.length === 1) {
      englishScore = Math.max(englishScore, 800000 + plan.compactQuery.length * 100)
    }
    if (titlePrefix) englishScore = Math.max(englishScore, 900000 + plan.compactQuery.length * 100)
    if (identifierExact) englishScore = Math.max(englishScore, 1100000 + plan.compactQuery.length * 100)
    if (titleExact) englishScore = Math.max(englishScore, 1200000 + plan.compactQuery.length * 100)
    if (titleExact && pureEnglishTitle) {
      englishScore = Math.max(englishScore, 1400000 + plan.compactQuery.length * 100)
    }
    if (titlePrefix && pureEnglishTitle) {
      englishScore = Math.max(englishScore, 1050000 + plan.compactQuery.length * 100)
    }
    if (typoTitleMatch && pureEnglishTitle) {
      englishScore = Math.max(englishScore, 850000 + plan.compactQuery.length * 100)
    }
    pageScore = Math.max(pageScore, englishScore)
  }

  const normalizedExactTitle = normalizedTitle === plan.normalizedQuery
  const caseExactTitle = caseTitle === plan.caseQuery
  const titlePartCount = plan.cjkPhrases.length + plan.englishParts.length
  const titlePartMatches = plan.cjkPhrases.filter(phrase => normalizedTitle.includes(normalize(phrase))).length
    + plan.englishParts.filter(part => normalizedTitle.includes(part)).length
  const titlePartCoverage = titlePartCount > 0 ? titlePartMatches / titlePartCount : 0
  const allEnglishPartsInTitle = plan.englishParts.length >= 2
    && plan.englishParts.every(part => normalizedTitle.includes(part))
  const mixedEnglishExact = plan.cjkPhrases.length > 0
    && plan.englishParts.length >= 2
    && compactTitle === plan.compactQuery
    && !CJK_REGEX.test(title)

  if (plan.cjkPhrases.length > 0 && plan.englishParts.length > 0) {
    if (titlePartCoverage === 1) pageScore += 150000
    else if (plan.cjkPhrases.length >= 2) pageScore += Math.round(150000 * titlePartCoverage)
    if (mixedPrimaryTitle) {
      pageScore = Math.max(pageScore, 1700000 + mixedEnglishTitleParts * 10000)
    }
    if (allEnglishPartsInTitle) pageScore += 200000
  }
  if (plan.cjkPhrases.length === 0 && plan.englishParts.length >= 2
    && normalizedHeadings.some(heading => heading === plan.normalizedQuery)) {
    pageScore = Math.max(pageScore, 1300000 + plan.normalizedQuery.length * 100)
  }
  if (normalizedExactTitle || mixedEnglishExact) {
    pageScore = Math.max(pageScore, 2000000 + plan.normalizedQuery.length * 100)
  }
  if (caseExactTitle) pageScore = Math.max(pageScore, 2100000 + plan.caseQuery.length * 100)

  const directoryBonus = isDirectoryLandingPage(route, title)
    && (normalizedExactTitle || typoTitleMatch || mixedPrimaryTitle || compactTitle.startsWith(plan.compactQuery))
    ? 20000
    : 0
  const canonicalRouteBonus = (normalizedExactTitle || mixedPrimaryTitle || compactTitle.startsWith(plan.compactQuery))
    ? Math.max(0, 12000 - normalizeRoute(route).length * 30 - routeDepth * 100)
    : 0
  const wrappedEnglishPhrasePenalty = plan.cjkPhrases.length === 0
    && plan.englishParts.length >= 2
    && compactTitle.includes(plan.compactQuery)
    && compactTitle !== plan.compactQuery
    ? 5000
    : 0
  const score = pageScore + retrievalTieScore + directoryBonus + canonicalRouteBonus
    + routeMatchBonus(route, plan.cjkPhrases)
    + compactCjkRouteBonus(route, plan.cjkPhrases)
    + Math.round(coverage * 20000)
    - wrappedEnglishPhrasePenalty
  const { ascSearchSource, ...displayMeta } = data.meta || {}
  const description = ascSearchSource === TITLE_INDEX_SOURCE ? '' : data.excerpt || ''
  return {
    route,
    title: title || route,
    description,
    meta: {
      ...displayMeta,
      title: title || route,
      description,
    },
    score,
  }
}

function mergeRouteResult(existing, result) {
  if (!existing) return result
  const preferred = result.score > existing.score ? result : existing
  const fallback = preferred === result ? existing : result
  const description = preferred.description || fallback.description || ''
  const route = preferred.route.includes('#') || !fallback.route.includes('#')
    ? preferred.route
    : fallback.route
  if (description === preferred.description && route === preferred.route) return preferred
  return {
    ...preferred,
    route,
    description,
    meta: { ...preferred.meta, description },
  }
}

function rankCandidates(candidates, plan) {
  const resultsByRoute = new Map()
  for (const candidate of candidates) {
    const result = scoreCandidate(candidate, plan)
    if (!result) continue
    const route = normalizeRoute(result.route)
    if (!route) continue
    resultsByRoute.set(route, mergeRouteResult(resultsByRoute.get(route), result))
  }
  const ranked = [...resultsByRoute.values()]
  ranked.sort((left, right) => right.score - left.score)
  return ranked.slice(0, 20)
}

function mergeRankedResults(primary, additional) {
  const resultsByRoute = new Map()
  for (const result of [...primary, ...additional]) {
    const route = normalizeRoute(result.route)
    resultsByRoute.set(route, mergeRouteResult(resultsByRoute.get(route), result))
  }
  return [...resultsByRoute.values()]
    .sort((left, right) => right.score - left.score || left.route.localeCompare(right.route))
    .slice(0, 20)
}

export async function searchPagefind(pagefind, query) {
  const plan = createQueryPlan(query)
  const identifierSearch = await searchIdentifierIndex(plan)
  if (identifierSearch.exclusive) return identifierSearch.results
  plan.strongIdentifierIntent ||= identifierSearch.strong
  const variants = primaryVariants(plan)
  const candidateMap = await collectCandidates(pagefind, variants)
  await collectCandidates(pagefind, titleVariants(plan, []), candidateMap, 'title-alias')
  let candidates = await materializeCandidates(candidateMap)
  let ranked = rankCandidates(candidates, plan)

  const canTryTypoVariants = plan.cjkPhrases.length === 0
    && plan.englishParts.length === 1
    && plan.compactQuery.length >= 5
    && plan.compactQuery.length <= 24
  const hasNearTitle = ranked.some(result => isSingleEditMatch(plan.compactQuery, compactEnglish(result.title)))
  if (canTryTypoVariants && !hasNearTitle) {
    const variants = singleEditVariants(plan.compactQuery).map(queryVariant => ({
      query: queryVariant,
      weight: 0.7,
    }))
    await collectCandidates(pagefind, variants, candidateMap)
    candidates = await materializeCandidates(candidateMap)
    ranked = rankCandidates(candidates, plan)
  }
  const hasExactLexicalTitle = plan.separatedIdentifierIntent
    && identifierSearch.exact
    && ranked.some(result => compactEnglish(result.title) === plan.compactQuery)
  return mergeRankedResults(hasExactLexicalTitle ? [] : identifierSearch.results, ranked)
}

export async function initializePagefindSearch(pagefind) {
  if (initializedPagefindInstances.has(pagefind)) return pagefind
  await pagefind.mergeIndex('/pagefind-titles/', { indexWeight: 2 })
  initializedPagefindInstances.add(pagefind)
  return pagefind
}
