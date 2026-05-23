import DefaultTheme from 'vitepress/theme'
import { h } from 'vue'
// import FilterToggle from './FilterToggle.vue'
import SearchBox from './SearchBox.vue'
import './filter.css'

export default {
  extends: DefaultTheme,
  Layout() {
    return h(DefaultTheme.Layout, null, {
      'nav-bar-content-before': () => h(SearchBox),
      // 'nav-bar-content-after': () => h(FilterToggle),
    })
  },
}
