import DefaultTheme from 'vitepress/theme'
import './code-theme.css'
import './search-overrides.css'

if (typeof window !== 'undefined' && !window.localStorage.getItem('vitepress:local-search-detailed-list')) {
  window.localStorage.setItem('vitepress:local-search-detailed-list', 'true')
}

export default {
  extends: DefaultTheme,
}
