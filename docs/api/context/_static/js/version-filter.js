(function () {
  var currentVersion = null;

  function openDropdown(el) {
    el.classList.add('show');
    var menu = el.querySelector('.version-filter__menu');
    if (menu) menu.classList.add('show');
  }

  function closeDropdown(el) {
    el.classList.remove('show');
    var menu = el.querySelector('.version-filter__menu');
    if (menu) menu.classList.remove('show');
  }

  function closeAllDropdowns() {
    document.querySelectorAll('.version-filter__container.show').forEach(function (c) {
      closeDropdown(c);
    });
  }

  function syncDropdownButtons() {
    var containers = document.querySelectorAll('.version-filter__container');
    containers.forEach(function (c) {
      var btn = c.querySelector('.version-filter__menu-item[data-version="' + currentVersion + '"]');
      var name = btn ? btn.getAttribute('data-version-name') : currentVersion;
      var current = c.querySelector('.version-filter__current');
      if (current) current.textContent = name;

      c.querySelectorAll('.version-filter__menu-item').forEach(function (item) {
        if (item.getAttribute('data-version') === currentVersion) {
          item.classList.add('active');
        } else {
          item.classList.remove('active');
        }
      });
    });
  }

  function getSidebar() {
    return document.getElementById('pst-primary-sidebar');
  }

  function showAllSidebarItems() {
    var sidebar = getSidebar();
    if (!sidebar) return;
    sidebar.querySelectorAll('li[class^="toctree-l"]').forEach(function (li) {
      li.style.display = '';
    });
    sidebar.querySelectorAll('details').forEach(function (d) {
      d.style.display = '';
      d.setAttribute('open', 'open');
    });
  }

  function getLinkHref(li) {
    var link = li.querySelector('a.reference.internal, a.reference.external');
    if (!link) return null;
    var href = link.getAttribute('href');
    if (!href) return null;
    return href.split('#')[0];
  }

  function filterSidebarItems() {
    var hidden = window.VERSION_950_HIDDEN;
    if (!hidden || !hidden.length) return;
    var sidebar = getSidebar();
    if (!sidebar) return;

    var hiddenSet = {};
    for (var i = 0; i < hidden.length; i++) {
      hiddenSet[hidden[i]] = true;
    }

    var allItems = sidebar.querySelectorAll('li[class^="toctree-l"]');

    // Reset all items first
    allItems.forEach(function (li) { li.style.display = ''; });

    // Walk DOM bottom-up: hide li only if it's in hidden set AND has no visible children
    // Use a reverse-order array to process children before parents
    var itemsArray = [];
    allItems.forEach(function (li) { itemsArray.push(li); });
    itemsArray.reverse();

    itemsArray.forEach(function (li) {
      var href = getLinkHref(li);
      if (!href || !hiddenSet[href]) return;

      var children = li.querySelectorAll('li[class^="toctree-l"]');
      if (children.length === 0) {
        // Leaf item: hide directly
        li.style.display = 'none';
        return;
      }

      // Check if all children are hidden
      var allChildrenHidden = true;
      children.forEach(function (child) {
        if (child.style.display !== 'none') {
          allChildrenHidden = false;
        }
      });

      if (allChildrenHidden) {
        li.style.display = 'none';
      }
      // else: keep visible because children need to be accessible
    });

    // Handle details: open those with visible items, hide empty ones
    sidebar.querySelectorAll('details').forEach(function (d) {
      var lis = d.querySelectorAll('li[class^="toctree-l"]');
      var hasVisible = false;
      for (var j = 0; j < lis.length; j++) {
        if (lis[j].style.display !== 'none') {
          hasVisible = true;
          break;
        }
      }
      if (hasVisible) {
        d.setAttribute('open', 'open');
        d.style.display = '';
      } else {
        d.removeAttribute('open');
        d.style.display = 'none';
      }
    });
  }

  function syncDownloadButton() {
    var btn = document.getElementById('download-btn');
    if (!btn) return;
    if (currentVersion === '950') {
      btn.href = 'download_950.zip';
    } else {
      btn.href = 'download_all.zip';
    }
  }

  function findProductSupportTables() {
    var tables = [];
    document.querySelectorAll('h2').forEach(function (h2) {
      if (h2.textContent.indexOf('产品支持情况') === -1) return;
      var section = h2.closest('section');
      var table = section ? section.querySelector(':scope > table') : null;
      if (!table) {
        var el = h2.nextElementSibling;
        while (el) {
          if (el.tagName === 'TABLE') { table = el; break; }
          var nested = el.querySelector('table');
          if (nested) { table = nested; break; }
          el = el.nextElementSibling;
        }
      }
      if (table) tables.push(table);
    });
    return tables;
  }

  function is950Row(row) {
    var firstTd = row.querySelector('td:first-child');
    if (!firstTd) return false;
    var text = firstTd.textContent;
    return text.indexOf('Ascend 950PR') !== -1 || text.indexOf('Ascend 950DT') !== -1;
  }

  function filterProductSupportTables() {
    var tables = findProductSupportTables();
    tables.forEach(function (table) {
      var tbody = table.querySelector('tbody');
      if (!tbody) return;
      tbody.querySelectorAll('tr').forEach(function (row) {
        if (is950Row(row)) {
          row.style.display = '';
        } else {
          row.style.display = 'none';
        }
      });
    });
  }

  function showAllProductSupportRows() {
    var tables = findProductSupportTables();
    tables.forEach(function (table) {
      var tbody = table.querySelector('tbody');
      if (!tbody) return;
      tbody.querySelectorAll('tr').forEach(function (row) {
        row.style.display = '';
      });
    });
  }

  function applyFilter(version) {
    currentVersion = version;
    syncDropdownButtons();
    syncDownloadButton();

    try { localStorage.setItem('asc-version', version); } catch (e) {}

    if (version === '全量') {
      showAllSidebarItems();
      showAllProductSupportRows();
    } else {
      filterSidebarItems();
      filterProductSupportTables();
    }
  }

  document.addEventListener('click', function (e) {
    var menuItem = e.target.closest('.version-filter__menu-item');
    if (menuItem) {
      e.stopPropagation();
      var version = menuItem.getAttribute('data-version');
      if (!version) return;
      var container = menuItem.closest('.version-filter__container');
      if (container) closeDropdown(container);
      applyFilter(version);
      return;
    }

    var toggleBtn = e.target.closest('.version-filter__button');
    if (toggleBtn) {
      e.stopPropagation();
      var container = toggleBtn.closest('.version-filter__container');
      if (container.classList.contains('show')) {
        closeDropdown(container);
      } else {
        closeAllDropdowns();
        openDropdown(container);
      }
      return;
    }

    closeAllDropdowns();
  });

  document.addEventListener('DOMContentLoaded', function () {
    var savedVersion;
    try { savedVersion = localStorage.getItem('asc-version'); } catch (e) {}

    if (savedVersion && savedVersion !== '全量') {
      applyFilter(savedVersion);
    } else {
      currentVersion = '全量';
    }
  });
})();