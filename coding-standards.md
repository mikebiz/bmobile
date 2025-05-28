### Modern C++ Coding Standards

*A concise, opinionated guide for a Principal SDE drawing on practices at Microsoft, Amazon, Facebook/Meta, Oracle-Sun, Google and the wider C++ community.*

---

## 1 · Naming Conventions

| Element                       | Recommended form                                                                       | Rationale / Notes                           |
| ----------------------------- | -------------------------------------------------------------------------------------- | ------------------------------------------- |
| **Namespaces**                | `lower_snake_case`                                                                     | Clear hierarchy; matches Google & Facebook. |
| **Files**                     | `lower_snake_case.{h,cpp,cppm}`                                                        | Easy grepping; mirrors LLVM/G-style.        |
| **Classes / Structs / Enums** | `PascalCase`                                                                           | Dominant across MS, Amazon, Meta.           |
| **Functions (public API)**    | `PascalCase`                                                                           | Signals “callable entry-point” (§CORE F.1). |
| **Functions (internal / λ)**  | `lowerCamelCase`                                                                       | Differentiates implementation details.      |
| **Variables**                 | `lower_snake_case`                                                                     | Readable in long expressions.               |
| **Members**                   | `name_` suffix (FB/Amazon) **or** `m_name` prefix (MS legacy) – pick one and automate. |                                             |
| **Constants**                 | `kPascalCase` (e.g., `kMaxRetries`)                                                    | Google-style; instantly recognisable.       |
| **Template params**           | `typename T`, `class Rep`                                                              | Avoid cryptic names; follow Core GSL.       |
| **Macros**                    | `PROJECT_ALL_CAPS`                                                                     | Rare, shouty, and collision-proof.          |

*Guiding rule:* choose a single scheme per repository, encode it in `.clang-format` and enforce in CI.  ([isocpp.github.io][1], [Google GitHub][2], [GitHub][3])

---

## 2 · Style Guidelines

1. **Baseline** – adopt the C++ Core Guidelines and enable `clang-tidy` / MSVC CppCoreCheck profiles `cppcoreguidelines-*`, `modernize-*`.  ([isocpp.github.io][1], [Microsoft Learn][4])
2. **Formatting** – use `clang-format` (Google or LLVM preset, 100-col limit, spaces-only).
3. **Brace & indent** – Allman or Stroustrup, but *one* per repo.
4. **Initialization** – uniform `{}`; never leave a variable un-initialised.
5. **`auto`** – only when the type is obvious from the RHS (`auto it = v.begin();`).
6. **RAII / ownership** – no raw `new`/`delete`; favour `std::unique_ptr`, `std::shared_ptr`, `std::span`, `gsl::not_null`.
7. **Error handling** – throw by value, catch by `const&`; in low-latency paths prefer `std::error_code` or Facebook’s `folly::Expected`.
8. **Includes** – `<standard>`, then `<third-party>`, then `"project"`; **never** `using namespace` in headers.
9. **Concurrency** – default to `std::jthread`; guard shared data with `std::mutex` or `folly::Synchronized`.
10. **Modern features** – Concepts, ranges, `std::format`, modules (`.ixx/.cppm`) for new libraries.

---

## 3 · Unit-Testing Guidance

| Aspect                | Standard                                                                             |
| --------------------- | ------------------------------------------------------------------------------------ |
| **Framework**         | GoogleTest + GoogleMock (or Catch2 when header-only is vital).                       |
| **Naming**            | `TEST(OrderService, CreateOrderReturnsId)` – *Given\_When\_Then* phrasing preferred. |
| **Structure**         | Arrange–Act–Assert; one logical check per test.                                      |
| **Speed constraints** | < 200 ms each; suites run < 5 min total.                                             |
| **Coverage gates**    | ≥ 80 % line, ≥ 70 % branch.                                                          |
| **Continuous checks** | PR: build + unit; Nightly: ASan, UBSan, TSAN, fuzzing, perf micro-benchmarks.        |
| **Test doubles**      | Mock only external systems; favour real data structures.                             |
| **Determinism**       | Seed all RNGs; freeze time via dependency injection.                                 |

---

## 4 · Code Commenting Practices

*Principle: “**Explain why, not what.**”*

| Location          | Style                               | Content                                                                            |
| ----------------- | ----------------------------------- | ---------------------------------------------------------------------------------- |
| **Public API**    | Doxygen/Javadoc `/** … */` or `///` | One-line summary, detailed description, `@param`, `@return`, `@throws`, *example*. |
| **Complex logic** | `//` inline                         | Invariants, algorithm choice, complexity hints.                                    |
| **TODO/FIXME**    | `// TODO(username-or-JIRA-123): …`  | Must include owner & link; tracked in backlog.                                     |
| **Dead code**     | Delete, don’t comment-out.          |                                                                                    |

---

## 5 · Documentation Standards

1. **Repository roots**

   * `README.md` – purpose, build & quick-start.
   * `BUILD.md` – tool-chains, Conan/Vcpkg, CMake presets.
   * `STYLE_GUIDE.md` – this document, version-controlled.

2. **Design docs**

   * ADRs (Architecture Decision Records) in `/docs/adr/YYYY-MM-DD-title.md`.
   * High-level diagrams (PlantUML, Mermaid) stored alongside code.

3. **Generated API docs**

   * Doxygen → Breathe → Sphinx HTML; auto-publish on each main-branch commit.

4. **Changelog**

   * Keep-a-Changelog format, semantic-version tags.

5. **Review process**

   * 2 reviewers minimum; checklist: builds, tests, docs, security scan pass.
   * CI gates: clang-format, clang-tidy, cpplint, CodeQL/Semgrep security rules.

---

## 6 · Enforcement & Tooling

| Goal                   | Tool                                             | Automation hook         |
| ---------------------- | ------------------------------------------------ | ----------------------- |
| **Format**             | `clang-format`                                   | Pre-commit & CI.        |
| **Static analysis**    | `clang-tidy`, `cppcorecheck`, `folly/FBInfer`    | CI & nightly full scan. |
| **Security**           | CodeQL, Semgrep, Oracle Secure Coding checklists | Nightly.                |
| **Dependency hygiene** | Conan + SPDX SBOM                                | Release gate.           |
| **Test & coverage**    | CTest, GTest, gcov/llvm-cov                      | Pull-request.           |
| **Perf / ABI drift**   | Google Benchmark, ABI Laboratory                 | Weekly.                 |

---

### Quick On-Boarding Checklist

1. Clone, run `./scripts/setup-hooks.sh` → installs git hooks.
2. Build with `cmake -S . -B build -DENABLE_SANITIZERS=ON`.
3. Run `ninja test && ninja coverage`.
4. Generate docs: `ninja docs && open build/docs/index.html`.
5. Submit PR with CHANGELOG entry and ADR (if architecture-impacting).

---

### Key References

* C++ Core Guidelines, Stroustrup & Sutter ([isocpp.github.io][1])
* Google C++ Style Guide ([Google GitHub][2])
* Facebook/HHVM C++ Conventions ([GitHub][3])
* Microsoft CppCoreCheck / Code-Analysis ([Microsoft Learn][4])
* Oracle Secure Coding Guidelines ([Oracle Docs][5])

---

**Use this as the living source of truth for every new C++ repository.**  Automate everything possible, review for the rest, and you’ll keep code quality and delivery speed high without over-burdening engineers.

[1]: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines?utm_source=chatgpt.com "C++ Core Guidelines - GitHub Pages"
[2]: https://google.github.io/styleguide/cppguide.html?utm_source=chatgpt.com "Google C++ Style Guide"
[3]: https://github.com/facebook/hhvm/blob/master/hphp/doc/coding-conventions.md?utm_source=chatgpt.com "hhvm/hphp/doc/coding-conventions.md at master - GitHub"
[4]: https://learn.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-170&utm_source=chatgpt.com "Using the C++ Core Guidelines checkers | Microsoft Learn"
[5]: https://docs.oracle.com/cd/E26502_01/html/E29016/scode-1.html?utm_source=chatgpt.com "Secure Coding Guidelines for Developers"
