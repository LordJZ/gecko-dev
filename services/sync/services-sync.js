pref("extensions.weave.serverURL", "@server_url@");
pref("extensions.weave.storageAPI", "1.0");
pref("extensions.weave.userURL", "user/");
pref("extensions.weave.miscURL", "misc/");
pref("extensions.weave.termsURL", "http://mozillalabs.com/weave/tos/");
pref("extensions.weave.privacyURL", "http://mozillalabs.com/weave/weave-privacy-policy/");

pref("extensions.weave.lastversion", "firstrun");
pref("extensions.weave.autoconnect", true);

pref("extensions.weave.engine.bookmarks", true);
pref("extensions.weave.engine.history", true);
pref("extensions.weave.engine.passwords", true);
pref("extensions.weave.engine.prefs", true);
pref("extensions.weave.engine.tabs", true);
pref("extensions.weave.engine.tabs.filteredUrls", "^(about:.*|chrome://weave/.*|wyciwyg:.*|file:.*)$");

pref("extensions.weave.log.appender.console", "Warn");
pref("extensions.weave.log.appender.dump", "Error");
pref("extensions.weave.log.appender.debugLog", "Trace");
pref("extensions.weave.log.rootLogger", "Debug");
pref("extensions.weave.log.logger.service.main", "Debug");
pref("extensions.weave.log.logger.authenticator", "Debug");
pref("extensions.weave.log.logger.network.resources", "Debug");
pref("extensions.weave.log.logger.engine.bookmarks", "Debug");
pref("extensions.weave.log.logger.engine.clients", "Debug");
pref("extensions.weave.log.logger.engine.forms", "Debug");
pref("extensions.weave.log.logger.engine.history", "Debug");
pref("extensions.weave.log.logger.engine.passwords", "Debug");
pref("extensions.weave.log.logger.engine.prefs", "Debug");
pref("extensions.weave.log.logger.engine.tabs", "Debug");
pref("extensions.weave.log.cryptoDebug", false);

// Preferences to be synced by default
pref("extensions.weave.prefs.sync.accessibility.blockautorefresh", true);
pref("extensions.weave.prefs.sync.accessibility.browsewithcaret", true);
pref("extensions.weave.prefs.sync.accessibility.typeaheadfind", true);
pref("extensions.weave.prefs.sync.accessibility.typeaheadfind.linksonly", true);
pref("extensions.weave.prefs.sync.app.update.mode", true);
pref("extensions.weave.prefs.sync.browser.download.manager.closeWhenDone", true);
pref("extensions.weave.prefs.sync.browser.download.manager.retention", true);
pref("extensions.weave.prefs.sync.browser.download.manager.scanWhenDone", true);
pref("extensions.weave.prefs.sync.browser.download.manager.showWhenStarting", true);
pref("extensions.weave.prefs.sync.browser.formfill.enable", true);
pref("extensions.weave.prefs.sync.browser.history_expire_days", true);
pref("extensions.weave.prefs.sync.browser.history_expire_days_min", true);
pref("extensions.weave.prefs.sync.browser.link.open_newwindow", true);
pref("extensions.weave.prefs.sync.browser.offline-apps.notify", true);
pref("extensions.weave.prefs.sync.browser.safebrowsing.enabled", true);
pref("extensions.weave.prefs.sync.browser.safebrowsing.malware.enabled", true);
pref("extensions.weave.prefs.sync.browser.search.selectedEngine", true);
pref("extensions.weave.prefs.sync.browser.search.update", true);
pref("extensions.weave.prefs.sync.browser.startup.homepage", true);
pref("extensions.weave.prefs.sync.browser.startup.page", true);
pref("extensions.weave.prefs.sync.browser.tabs.autoHide", true);
pref("extensions.weave.prefs.sync.browser.tabs.closeButtons", true);
pref("extensions.weave.prefs.sync.browser.tabs.loadInBackground", true);
pref("extensions.weave.prefs.sync.browser.tabs.tabMaxWidth", true);
pref("extensions.weave.prefs.sync.browser.tabs.tabMinWidth", true);
pref("extensions.weave.prefs.sync.browser.tabs.warnOnClose", true);
pref("extensions.weave.prefs.sync.browser.tabs.warnOnOpen", true);
pref("extensions.weave.prefs.sync.browser.urlbar.autocomplete.enabled", true);
pref("extensions.weave.prefs.sync.browser.urlbar.autoFill", true);
pref("extensions.weave.prefs.sync.browser.urlbar.default.behavior", true);
pref("extensions.weave.prefs.sync.browser.urlbar.maxRichResults", true);
pref("extensions.weave.prefs.sync.dom.disable_open_during_load", true);
pref("extensions.weave.prefs.sync.dom.disable_window_flip", true);
pref("extensions.weave.prefs.sync.dom.disable_window_move_resize", true);
pref("extensions.weave.prefs.sync.dom.disable_window_open_feature.status", true);
pref("extensions.weave.prefs.sync.dom.disable_window_status_change", true);
pref("extensions.weave.prefs.sync.dom.event.contextmenu.enabled", true);
pref("extensions.weave.prefs.sync.extensions.personas.current", true);
pref("extensions.weave.prefs.sync.extensions.update.enabled", true);
pref("extensions.weave.prefs.sync.general.autoScroll", true);
pref("extensions.weave.prefs.sync.general.smoothScroll", true);
pref("extensions.weave.prefs.sync.intl.accept_languages", true);
pref("extensions.weave.prefs.sync.javascript.enabled", true);
pref("extensions.weave.prefs.sync.layout.spellcheckDefault", true);
pref("extensions.weave.prefs.sync.lightweightThemes.isThemeSelected", true);
pref("extensions.weave.prefs.sync.lightweightThemes.usedThemes", true);
pref("extensions.weave.prefs.sync.network.cookie.cookieBehavior", true);
pref("extensions.weave.prefs.sync.network.cookie.lifetimePolicy", true);
pref("extensions.weave.prefs.sync.permissions.default.image", true);
pref("extensions.weave.prefs.sync.pref.advanced.images.disable_button.view_image", true);
pref("extensions.weave.prefs.sync.pref.advanced.javascript.disable_button.advanced", true);
pref("extensions.weave.prefs.sync.pref.downloads.disable_button.edit_actions", true);
pref("extensions.weave.prefs.sync.pref.privacy.disable_button.cookie_exceptions", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.cache", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.cookies", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.downloads", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.formdata", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.history", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.offlineApps", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.passwords", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.sessions", true);
pref("extensions.weave.prefs.sync.privacy.clearOnShutdown.siteSettings", true);
pref("extensions.weave.prefs.sync.privacy.sanitize.sanitizeOnShutdown", true);
pref("extensions.weave.prefs.sync.security.OCSP.disable_button.managecrl", true);
pref("extensions.weave.prefs.sync.security.OCSP.enabled", true);
pref("extensions.weave.prefs.sync.security.OCSP.require", true);
pref("extensions.weave.prefs.sync.security.default_personal_cert", true);
pref("extensions.weave.prefs.sync.security.enable_java", true);
pref("extensions.weave.prefs.sync.security.enable_ssl3", true);
pref("extensions.weave.prefs.sync.security.enable_tls", true);
pref("extensions.weave.prefs.sync.security.warn_entering_secure", true);
pref("extensions.weave.prefs.sync.security.warn_entering_weak", true);
pref("extensions.weave.prefs.sync.security.warn_leaving_secure", true);
pref("extensions.weave.prefs.sync.security.warn_submit_insecure", true);
pref("extensions.weave.prefs.sync.security.warn_viewing_mixed", true);
pref("extensions.weave.prefs.sync.signon.rememberSignons", true);
pref("extensions.weave.prefs.sync.spellchecker.dictionary", true);
pref("extensions.weave.prefs.sync.xpinstall.whitelist.required", true);

