# Copilot instructions for this repository

Purpose: give Copilot sessions quick, precise guidance for builds, tests, and repo conventions used by Botfather (C++/Qt + CEF + Flatpak).

---
Build, test, and lint commands

- Local Linux build (recommended):
  - Prepare CEF and deps (see README): set CEF_ROOT to extracted CEF archive.
  - mkdir build && cd build
  - cmake -DCMAKE_BUILD_TYPE=Release ..
  - cmake --build . --config Release
  - To build a single target: cmake --build . --target <target>

- Run tests (if tests were configured):
  - From build dir: ctest -R <regex> -j$(nproc) -V
  - Or run a google-test binary directly with gtest filter: ./path/to/test_binary --gtest_filter=Suite.TestName
  - If no tests exist, inspect 3rdparty/ for test frameworks (Google Test used in vendor files).

- Flatpak/nightly build:
  - flatpak-builder --force-clean build-dir flatpak/io.botfather.Nightly.yaml
  - Install & run: flatpak --user install ./flatpak/repo io.botfather.Nightly && flatpak run io.botfather.Nightly

- Formatting / linting:
  - make format  # runs clang-format over *.cpp/ *.hpp in botfather and webhelper directories (see Makefile)
  - Prefer clang-format and project-specific style; no global ESLint/Python linters detected.

- CI:
  - GitHub Actions workflow: .github/workflows/build.yml — builds via cmake on ubuntu-24.04 and includes a separate flatpak job.

---
High-level architecture (big picture)

- Language & UI: C++ (C++11) with Qt5 for UI and QtScript. The project builds cross-platform (Linux, Windows, macOS) and embeds Chromium via CEF.
- Rendering/Browser: Chromium Embedded Framework (CEF) is a prebuilt binary distribution; CEF_ROOT must point to the extracted CEF tree.
- Native helpers: the repo contains botfather and a companion webhelper binary; both are built via CMake into the build/ tree.
- Media & CV: OpenCV is used for image matching features in the JS runtime APIs.
- Packaging: Flatpak manifest under flatpak/ for nightly builds; Makefile provides a flatpak target and a format target.
- Third-party: 3rdparty/ contains vendored libraries (e.g., QHotkey). The project uses git submodules — CI checks out submodules.

---
Key repository conventions and notes

- CEF handling: The build requires a CEF binary archive (CEF_ARCHIVE). CI caches and downloads it; locally download and set CEF_ROOT before configuring CMake.
- Build dir policy: out-of-tree build under ./build. Use cmake -B build -S . or mkdir build && cd build.
- Formatting: make format invokes clang-format on botfather and webhelper sources. Run this prior to commits.
- Submodules: repository contains submodules; ensure `git submodule update --init --recursive` when cloning.
- Flatpak: flatpak build expects matching runtime/sdk versions; update flatpak/io.botfather.Nightly.yaml runtime-version when upgrading.
- VSCode hints: README contains suggested include paths for VSCode C/C++ IntelliSense for Linux/Windows.

---
Files to consult when assisting developers

- README.md — platform-specific build and runtime notes.
- Makefile — quick targets (format, flatpak).
- CMakeLists.txt (top-level) — build targets and test discovery.
- flatpak/io.botfather.Nightly.yaml — packaging/runtime configuration.
- .github/workflows/build.yml — CI steps (dependencies, caching CEF, build commands).

---
If you update this file

- Keep the Build and High-level Architecture sections aligned with README.md and CMakeLists.txt.
- If adding new CI jobs or test frameworks, add the exact commands here for single-test execution.

---
End of file.
