import DefaultTheme from 'vitepress/theme'
import { h } from 'vue'
import FilterToggle from './FilterToggle.vue'
import './filter.css'

export default {
  extends: DefaultTheme,
  Layout() {
    return h(DefaultTheme.Layout, null, {
      'nav-bar-content-after': () => h(FilterToggle),
    })
  },
}
