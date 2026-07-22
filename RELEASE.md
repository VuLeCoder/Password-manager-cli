# 🎉 LPTV Password Manager

First public stable release.

## ✨ Highlights

* AES-256-GCM encryption
* Argon2id key derivation
* SecureString integration
* Binary encrypted storage
* Cross-platform support (Windows & Linux)

## Features

### Vault

* Initialize vault
* Unlock vault
* Lock vault

### Password Management

* Add account
* Get password
* Update account
* Delete account
* Search accounts
* List accounts

### Security

* AES-256-GCM encryption
* Argon2id key derivation
* SecureString for sensitive data
* Encrypted binary storage

## ✅ Platform Support

* **Windows (x64):** Fully supported and tested.
* **Ubuntu (x64):** Fully supported and tested.

## 📦 Downloads

Download the package for your operating system from the **Assets** section below.

## ❤️ Thanks

Thank you for trying **LPTV Password Manager**! Your feedback and suggestions are always welcome.

---

## 🔧 Recent Updates & Bug Fixes

### 📋 List Command Fixes
* **Resolved Indexing & Filtering Bug**: Fixed a bug in `PasswordManager::list` where incorrect indexing caused accounts to not display, and filtering by category (`lptv list <category>`) failed to return existing entries due to a loop breaking logic mismatch.
* **Improved Dispatcher Logic**: Updated `CommandDispatcher::handleList` to correctly route category filters when running `lptv list <category>`.

### 📋 Linux Clipboard & Auto-clear Support
* **Multi-utility Detection**: Added dynamic support for `xclip`, `xsel`, `wl-copy`, and `pbcopy` (macOS) clipboard tools.
* **Asynchronous Auto-clear**: Clipboard cleanup runs in the background using non-blocking shell tasks, preventing the CLI application from locking up during the clearing delay.

### ⌨️ Terminal Input & Password Hiding Fixes
* **Improved Raw Mode Handshake**: Restructured terminal canonical/raw state management to prevent OS-level echoing. Passwords entered via `readSecureHiddenInput` are now completely hidden on Linux.
* **Linux Keystroke Handling**: Added native support for Linux backspace (`127`) and newline (`\n`) characters in raw mode.