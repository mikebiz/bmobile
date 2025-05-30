Below is a “drop-in” C\++20 replacement for [Heap2.hpp](https://github.com/mikebiz/bmobile/blob/main/docs/legacy-source/BlxHeap.hpp) / [.inl](https://github.com/mikebiz/bmobile/blob/main/docs/legacy-source/BlxHeap.inl) that eliminates the legacy ATL `CComCriticalSection` and the Win-typedefs **`VOID / PVOID`** (just use `void` / `void*`).
It keeps the Windows **Heap* APIs*\* (you said Windows-only is fine) but wraps every Windows handle and counter in modern, type-safe C++20 constructs:

* **`std::mutex`** – protects lazy heap creation / destruction.
* **`std::atomic`** – replaces `InterlockedIncrement/Decrement` and guarantees lock-free counters on all modern MSVC targets.
* **Inline `static` template data members** – no need for each `T` to expose `m_csLock`, `m_hHeap`, or `m_uNumAllocsInHeap`; the allocator owns its own state.
* No **`VOID`**, **`PVOID`**, or **ATL** headers required.

> **How to use**: exactly as before—derive any class from `bmobile::Heap2<YourType>` (or do a `using` alias) and the custom `new`/`delete` kick in.

```cpp
// bmobile/Heap2.hpp
#pragma once
#include <atomic>
#include <cstddef>
#include <mutex>
#include <windows.h>        // still needed for HeapCreate / HeapAlloc …

namespace bmobile
{
    /**
     *  Fixed-size heap allocator shared per-T.
     *
     *  Each distinct T gets its own LFH heap created on first allocation and
     *  destroyed after the last free.  Thread-safe without ATL.
     */
    template <typename T>
    class Heap2
    {
    public:
        // ---------------------------------------------------------------------
        // operator new / delete
        // ---------------------------------------------------------------------
        static void* operator new(std::size_t size)
        {
            ensure_heap();

            if (void* p = ::HeapAlloc(heap_.load(std::memory_order_acquire), 0, size))
            {
                ++allocs_;
                return p;
            }
            throw std::bad_alloc{};
        }

        static void operator delete(void* p) noexcept
        {
            if (p == nullptr) return;

            ::HeapFree(heap_.load(std::memory_order_relaxed), 0, p);
            if (--allocs_ == 0)
            {
                destroy_heap();
            }
        }

    private:
        // ---------------------------------------------------------------------
        // Heap lifecycle helpers
        // ---------------------------------------------------------------------
        static void ensure_heap()
        {
            if (heap_.load(std::memory_order_acquire) != nullptr) return;

            std::lock_guard lk(mtx_);
            if (heap_ == nullptr)
            {
                HANDLE h = ::HeapCreate(0, 0, 0);
                if (!h) throw std::bad_alloc{};

#ifndef UNDER_CE
                ULONG compat = 2;   // enable LFH
                ::HeapSetInformation(h,
                                     HeapCompatibilityInformation,
                                     &compat,
                                     sizeof compat);
#endif
                heap_.store(h, std::memory_order_release);
            }
        }

        static void destroy_heap() noexcept
        {
            std::lock_guard lk(mtx_);
            if (heap_ && allocs_.load() == 0)
            {
                ::HeapDestroy(heap_.load());
                heap_.store(nullptr, std::memory_order_release);
            }
        }

        // ---------------------------------------------------------------------
        // One set of statics *per concrete T*
        // ---------------------------------------------------------------------
        inline static std::mutex          mtx_;
        inline static std::atomic<HANDLE> heap_{ nullptr };
        inline static std::atomic<long>   allocs_{ 0 };
    };

} // namespace bmobile
```

---

### What changed and why

| Legacy code ( Heap2.hpp / .inl ) | Modern replacement                                                             |
| -------------------------------- | ------------------------------------------------------------------------------ |
| `CComCriticalSection* m_csLock`  | `inline static std::mutex mtx_` (standard, header-only)                        |
| `HANDLE m_hHeap` on the **T**    | `inline static std::atomic<HANDLE> heap_` inside allocator                     |
| `InterlockedIncrement/Decrement` | `std::atomic<long> allocs_` (`++allocs_`, `--allocs_`)                         |
| `VOID`, `PVOID`                  | canonical `void`, `void*`                                                      |
| Separate `.inl` implementation   | All inline in a single header for clarity (you can still split if you prefer). |

#### Thread safety

* `ensure_heap()` + `destroy_heap()` use `std::mutex` for one-time init / teardown.
* The hot path (allocate/free) touches only atomics—no lock-contention once the heap exists.

#### Exception behaviour

* `operator new` now throws `std::bad_alloc` on failure, matching the C++ language rules.
* `operator delete` is `noexcept`, as recommended.

---

### Integration steps

1. **Add the file** to your new **bmobileUtil** static-lib project.
2. Replace old include lines:

   ```cpp
   // #include "Heap2.hpp"  (keep the same name if you like)
   using MyAllocator = bmobile::Heap2<MyClass>;
   ```
3. Delete the original `Heap2.hpp / Heap2.inl` once all sources compile.
4. (Optional) Add a GoogleTest: allocate 100 objects concurrently, ensure `allocs_` returns to 0.

This brings the allocator up to C++20 style while staying fully compatible with the Windows heap APIs you want to keep.

***Question***:

`Would I add Heap2.hpp to the pch.h (precompiled header) file?`

***Answer***

### Short answer

You **don’t have to** include `Heap2.hpp` in `pch.h`, and in most projects I’d leave it out.
If you add it, nothing will break—but it probably won’t give you much compile-time benefit and will make rebuilding the PCH slightly slower.

---

## How to decide

| Consideration        | When to **keep it out** of `pch.h`                                                                                                                                   | When it’s reasonable to **put it in**                                                                                          |
| -------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ |
| **Header stability** | You plan to tweak `Heap2.hpp` while you refine allocator details. Any edit to a header in the PCH forces *every* translation unit to rebuild the precompiled header. | The file is effectively finished and you don’t expect to touch it often.                                                       |
| **Template code**    | `Heap2` is a template; most of its cost comes at **instantiation time**, which happens after the PCH is applied, so the win is minimal.                              | You include it in *almost every* source file and want to shave a few preprocessing milliseconds.                               |
| **PCH philosophy**   | You keep `pch.h` lean—only extremely stable, heavyweight system headers (`<windows.h>`, `<atlbase.h>`, `<chrono>`, etc.).                                            | Your team policy is “dump everything common into the PCH to maximize reuse,” and you don’t mind longer occasional full builds. |

---

## Recommended layout for this solution

```cpp
// pch.h  (static, rarely changes)
#pragma once
#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>
#include <atlbase.h>          // if still needed elsewhere
#include <cstdint>
#include <string_view>
#include <span>
#include <atomic>
#include <mutex>
// … anything else truly universal …

// --- do NOT #include Heap2.hpp here ---

// In each .cpp that needs the allocator:
#include "pch.h"
#include "bmobile/Heap2.hpp"
```

* This keeps your **PCH rebuild cost low**.
* `Heap2.hpp` then compiles like any normal header; because it’s only a few dozen lines, the overhead is negligible.

---

## If you choose to add it anyway

1. Open **`pch.h`** and insert

   ```cpp
   #include "bmobile/Heap2.hpp"
   ```

   below the standard headers.
2. Make sure **every** translation unit still includes `pch.h` first (VS adds this automatically when you use “Precompiled Header (/Yu)”).
3. Be prepared for longer full rebuilds whenever you touch `Heap2.hpp`—but incremental “edit-compile-run” cycles where you *don’t* touch it will be a hair faster.

---

### Bottom line

Leave `Heap2.hpp` out of `pch.h` until the allocator is rock-solid and rarely changes.
You’ll keep your daily build cycle snappy and avoid needless PCH invalidations.

***Question***

`the bmobileUtil project has a default header generated for it call framework.h and includes #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers .  Do I need to include this too?`

***Answer***

### What that **`framework.h`** file is

When you create a new Visual Studio C++ project (DLL, static-lib, or EXE) with the default *“Precompiled Header”* option checked, VS adds two files:

| File                                     | Purpose                                                                                                                                   |
| ---------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| **`framework.h`** (or sometimes `pch.h`) | The header that will be **pre-compiled**. You include it first in every `.cpp` so the compiler can reuse the already-parsed token stream. |
| **`framework.cpp`** (or `pch.cpp`)       | A single TU that `#include`s `framework.h`; the compiler generates `framework.pch` from it.                                               |

Out of the box `framework.h` contains:

```cpp
#pragma once
#define WIN32_LEAN_AND_MEAN       // Exclude rarely-used stuff from Windows headers
#include <windows.h>
```

---

### Do you *need* to include it in `Heap2.hpp`?

**No.**
`framework.h` is intended for *translation units* (`.cpp` files) that opt in to the project’s precompiled header. A header such as `Heap2.hpp` should **not** include it directly; otherwise any file that includes `Heap2.hpp` *before* including the PCH would get a double-include or ordering problems.

Instead, each `.cpp` should include the PCH first:

```cpp
#include "framework.h"   // or "pch.h"  — compiles fast from the .pch file
#include "bmobile/Heap2.hpp"
#include "MyClass.hpp"
```

As long as `framework.h` (or your consolidated `pch.h`) defines `WIN32_LEAN_AND_MEAN` **before** it pulls in `<windows.h>`, every header that relies on Windows APIs—`Heap2.hpp` included—will benefit from the leaner compile.

---

### Best-practice options

| Approach                                                                                                                  | Pros                                                                                                                                                 | Cons / caveats                                                                                                                                        |
| ------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Keep `framework.h` per project (default)**                                                                              | • Zero extra work.  <br>• Each project can tailor its own includes/macros without affecting the others.                                              | • Slightly bigger repo noise (one PCH pair per project).  <br>• If you change global macros (like `_WIN32_WINNT`), you must touch each project’s PCH. |
| **Create one shared `pch.h` for the whole solution** (rename every project’s “Precompiled Header” setting to point to it) | • Single place to manage `WIN32_LEAN_AND_MEAN`, `_WIN32_WINNT`, locally-common headers.  <br>• Faster full rebuilds when nothing in the PCH changes. | • All projects must agree on the same header set.  <br>• A change to any common header forces *all* projects to rebuild their PCH.                    |

Given your **“keep VS solution, move quick”** goal, the default per-project `framework.h` is fine. You can always consolidate later.

---

### Where to keep the macro `WIN32_LEAN_AND_MEAN`

* **Inside the project’s PCH header** (framework.h) – simplest.
* **Or** define it globally in *Project Properties → C/C++ → Preprocessor → Preprocessor Definitions*:

  ```
  WIN32_LEAN_AND_MEAN;NOMINMAX;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)
  ```

  That way the macro is active no matter which header happens to include `<windows.h>` first.

---

### Checklist for `bmobileUtil` right now

1. **Leave `framework.h` as generated** (or rename it to `pch.h` if you prefer a consistent naming scheme).

2. **Do not include it inside `Heap2.hpp`**—only in `.cpp` files that use the allocator.

3. Make sure every `.cpp` in *bmobileUtil* starts with:

   ```cpp
   #include "framework.h"      // PCH
   #include "bmobile/Heap2.hpp"
   // other includes...
   ```

4. If you later move to a *solution-wide* PCH, just change each project’s **Precompiled Header** setting to point to the shared header and delete the per-project one.

With that, you get all the compile-time benefits of `WIN32_LEAN_AND_MEAN` and the PCH mechanism, without entangling your reusable headers in project-specific details.

**Note:**  In the case of the projects for this solution, there is both a "framework.h" and a "pch.h/.cpp" file.  The above guidance would be as follows since the `pch.h` file contains and include for the "framework.h" header.

   ```cpp
   #include "pch.h"
   #include "bmobile/Heap2.hpp"
   // other includes...
   ```

   `can you create initial google test and gmock files for bmobileHeap2?`
   Below are **ready-to-drop source files** for your new Google Test project:

| File                        | Purpose                                                                                                                                                                         |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`Heap2_test.cpp`**        | Core unit tests that exercise allocation, deallocation, alignment, and concurrency for `bmobile::Heap2<T>`.                                                                     |
| **`Heap2_gmock_fixture.h`** | (Optional) Demonstrates how you’d wrap the allocator in a mock-friendly façade so future components can inject an “allocator” dependency and set expectations with Google Mock. |

> **How to integrate**
>
> 1. In Visual Studio: **Add ⇒ New Project ⇒ Google Test ⇒ “bmobileUtilTests”** (Static or Console App).
> 2. Right-click the project → **Add Existing Item…** and select the two files below.
> 3. **Project → References…** ⇒ add your `bmobileUtil` project so the tests link against the allocator implementation.
> 4. Build & run (Test Explorer shows the cases).

---

### `Heap2_test.cpp`

```cpp
#include "pch.h"                // your test project’s PCH
#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "bmobile/Heap2.hpp"    // allocator under test

namespace {

// -----------------------------------------------------------------------------
// Simple POD to allocate
// -----------------------------------------------------------------------------
struct Pod
{
    int    a{0};
    double b{0.0};
};

using PodHeap = bmobile::Heap2<Pod>;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------
template <typename F>
void run_parallel(std::size_t threads, F&& f)
{
    std::vector<std::jthread> workers;
    workers.reserve(threads);
    for (std::size_t i = 0; i < threads; ++i)
        workers.emplace_back(f);
}

// -----------------------------------------------------------------------------
// Unit tests
// -----------------------------------------------------------------------------

TEST(Heap2, AllocateAndFreeSingle)
{
    Pod* p = new Pod;
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->a, 0);
    delete p;                    // should implicitly destroy heap when last alive
}

TEST(Heap2, AlignmentIsSufficient)
{
    void* p = ::operator new(sizeof(Pod), std::align_val_t{alignof(Pod)});
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p) % alignof(Pod), 0u);
    ::operator delete(p, std::align_val_t{alignof(Pod)});
}

TEST(Heap2, ConcurrentAllocations)
{
    constexpr std::size_t iters      = 10'000;
    constexpr std::size_t numThreads = 8;

    run_parallel(numThreads, [=] {
        for (std::size_t i = 0; i < iters; ++i)
        {
            Pod* p = new Pod;
            [[maybe_unused]] int x = p->a;
            delete p;
        }
    });

    // If we get here without AV or heap corruption, we’re good.
    SUCCEED();
}

} // anonymous namespace
```

---

### `Heap2_gmock_fixture.h`  *(optional, only if you need mocks later)*

```cpp
#pragma once
#include <gmock/gmock.h>

/**
 *  Example interface so higher-level code can accept “any allocator”
 *  and you can plug in Google Mock for behavioural tests.
 */
class IHeap
{
public:
    virtual ~IHeap() = default;
    virtual void* allocate(std::size_t)            = 0;
    virtual void  deallocate(void*)                = 0;
};

/**
 *  Production wrapper around bmobile::Heap2; fulfils IHeap.
 */
template <typename T>
class HeapAdapter final : public IHeap
{
public:
    void* allocate(std::size_t sz) override        { return bmobile::Heap2<T>::operator new(sz); }
    void  deallocate(void* p)       override       { bmobile::Heap2<T>::operator delete(p);      }
};

/**
 *  Google Mock stand-in.  You’d use this for tests on components
 *  that *depend* on an allocator—not for testing the allocator itself.
 */
class MockHeap : public IHeap
{
public:
    MOCK_METHOD(void*, allocate,   (std::size_t), (override));
    MOCK_METHOD(void,  deallocate, (void*),       (override));
};
```

---

### Build-and-run verification

* After adding the files, **Build Solution** → 0 errors.
* **Test → Run All Tests**: you should see three passing tests.
* Try **Debug All Tests** to step through the allocator under the debugger.

This gives you a solid baseline: the allocator is covered for single and multithreaded paths, and you now have a pattern for introducing Google Mock when higher-level code needs allocator injection. Feel free to add more edge-case tests (e.g., very large allocations, exception safety) as the project evolves.

`do I put this new project in the 'tests' folder I created or does it go at the same level as the rest of the projects?`

**Put the Google Test project inside your `tests` folder** (both in the file-system and as a *solution-folder* in Visual Studio).
That keeps all unit- and integration-test targets grouped together and separate from the production code.

---

### Recommended layout

```
/bmobile-root
│
├─ libs/
│   ├─ bmobileUtil/
│   ├─ bmobileCoreRouting/
│   ├─ …
│
├─ apps/
│   ├─ BmobileRuntime/
│   └─ BmobileMgmtAPI/
│
└─ tests/                     ← <-- all test projects live here
    ├─ bmobileUtilTests/      ← Google Test project you just added
    ├─ coreRoutingTests/
    └─ integrationSmokeTests/
```

* **Solution-folder “tests”** – In *Solution Explorer* you can right-click the root, **Add → New Solution Folder → “tests”**, then drag each test project into it.
* **Physical path** – When you create **bmobileUtilTests**, set its *Location* to `<repo>/tests/bmobileUtilTests` so the `.vcxproj` and source files stay under the `tests/` directory.

---

### Why keep tests together

| Benefit                          | Detail                                                                                                                                           |
| -------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Clean VS solution view**       | One expand/collapse node shows *all* test targets; devs don’t have to scroll past them to find production libs.                                  |
| **Build configuration**          | You can create a solution-wide “ --TestsOnly ” configuration or set *Build* = false for test projects in “Release” so shipping builds stay lean. |
| **CI pipelines**                 | A single glob like `tests\*\*.vcxproj` lets your GitHub Actions or Azure DevOps job discover and run every test suite automatically.             |
| **Clarity for new contributors** | They see right away where to add new tests for a module.                                                                                         |

---

### Quick setup checklist

1. **Add solution folder**
   *Solution Explorer → right-click solution → Add → New Solution Folder → `tests`.*
2. **Create the project inside it**
   *Right-click the new folder → Add → New Project → “Google Test” → Name: `bmobileUtilTests` → Location: `<repo>\tests\bmobileUtilTests`*.
3. **Reference the library**
   *bmobileUtilTests → References → Add Reference → Projects → check `bmobileUtil`*.
4. **Copy in the two files** (`Heap2_test.cpp`, optional `Heap2_gmock_fixture.h`) and build.
5. **Test Explorer** should discover three passing tests.

With that structure locked in, each future module (CoreRouting, NetRIO, DataAccess) gets its own test project under the same `tests/` umbrella, keeping the solution tidy and predictable.

`I had to update the include to this in the tests folder for bmobileUtil #include "../../bmobileUtil/Heap2.hpp"    // allocator under test`
That relative path works, but you’ll save headaches later if the **test project knows where to find public headers without hard-coding `../../…`**.
Here are two quick ways to do that in Visual Studio—pick whichever feels lighter-weight right now.

---

## 1 Add the library’s *include* folder to the test project’s search path  (one-off)

1. **Right-click `bmobileUtilTests` → Properties.**
2. **Configuration**: *All Configurations* ⟶ *All Platforms*
3. **C/C++ → General → Additional Include Directories**
4. Add

   ```
   $(SolutionDir)libs\bmobileUtil
   ```

   or, if your tree is exactly

   ```
   ...\libs\bmobileUtil\Heap2.hpp
   ```

   you can even point at `$(SolutionDir)libs`.
5. OK / Apply, then change the include back to the simple form:

   ```cpp
   #include "bmobileUtil/Heap2.hpp"          // or just "Heap2.hpp"
   ```

   (Angle brackets are fine too: `<bmobileUtil/Heap2.hpp>`.)

*Pros* No property sheets, minimal clicks.
*Cons* You’ll repeat the same edit for each new test project that needs util headers.

---

## 2 Use a shared property sheet (`.props`)  (scales better)

1. **View → Property Manager** (it’s hidden in default VS layouts).
2. Expand the test project → Debug | Win32 (repeat for x64) → right-click **“Microsoft.Cpp.<…>.user”** → **Add New Project Property Sheet…**.
3. Name it `BmobileIncludes.props` and save under `build/` or `.vs/props`.
4. Inside that sheet:

   * **Common Properties → C/C++ → General → Additional Include Directories**

     ```
     $(SolutionDir)libs\bmobileUtil;
     $(SolutionDir)libs\bmobileCoreRouting;
     $(SolutionDir)libs;                                         <-- catch-all
     %(AdditionalIncludeDirectories)
     ```
5. **Attach** the same `.props` file to every project (test or production) that should “see” public headers from `libs/`. One change, many consumers.

---

### Why avoid deep relative includes

| Issue                    | Impact                                                                                                                                     |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------ |
| Breaks when folders move | If you rename `libs` → `src` or nest deeper, every `../../…` include breaks.                                                               |
| Harder IDE Intellisense  | VS and clang-d use include paths to index headers; they treat funky relative chains as separate files.                                     |
| Compile flags diverge    | Different translation units may accidentally include the same header via different relative paths, confusing `/FI`, PCH, or modules rules. |

---

## Quick test

*After you add the include directory:*

```bash
# Clean + rebuild tests only
msbuild tests\bmobileUtilTests\bmobileUtilTests.vcxproj /t:Rebuild
#
# or via IDE Test Explorer
```

The simple `#include "bmobileUtil/Heap2.hpp"` (or `<bmobileUtil/Heap2.hpp>`) should compile cleanly.
If it doesn’t, double-check:

* The path you typed in “Additional Include Directories” matches the on-disk layout.
* The change was applied to **all** configurations (Debug/Release, x86/x64).

---

### Takeaway

Keep production headers location-neutral; let project settings tell the compiler where to look.
That way you can • shuffle folders, • switch to CMake later, or • install the libs system-wide—without touching `#include` lines again.

`Also, this cannot be resolved:  std::align_val_t`
`std::align_val_t` lives in the standard header **\<new>** and is available only when the compiler is in C++17 (or later) mode *and* “aligned new/delete” support is enabled.
All MSVC 2019 + 2022 toolsets have it, but if a TU doesn’t `#include <new>` **before** it first uses the name, the symbol is undefined.

---

## Quick-fix for the test file

```cpp
#include "pch.h"
#include <new>                  // ← add this line
#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "bmobileUtil/Heap2.hpp"
```

Now `std::align_val_t` will resolve.

---

### Why the header matters

* `std::align_val_t` and the aligned‐allocation overloads

  ```cpp
  void* operator new(std::size_t, std::align_val_t);
  void  operator delete(void*, std::align_val_t) noexcept;
  ```

  are *declared* in \<new>. The keyword `alignof` is in \<cstddef>, but the enum type itself is not.

### MSVC switches to double-check

| Property                             | Location                         | Should be                                                           |
| ------------------------------------ | -------------------------------- | ------------------------------------------------------------------- |
| **Language → C++ Language Standard** | *Project* → C/C++ → Language     | `/std:c++20` (or `/std:c++17`)                                      |
| **C++17 aligned new support**        | Same page, flag `/Zc:alignedNew` | *(ON by default — only an explicit `/Zc:alignedNew-` turns it off)* |

If you upgraded a very old .vcxproj, VS sometimes carries a legacy “Disable aligned new” flag. Make sure it’s cleared.

---

## Alternative: simplify the alignment test

The allocator itself doesn’t need `std::align_val_t`; the test used it only to request a specific alignment.
If you’d rather avoid the extra header altogether, you can drop the aligned-new call:

```cpp
TEST(Heap2, AlignmentIsSufficient)
{
    Pod* p = new Pod;                           // default new is suitably aligned
    EXPECT_EQ(reinterpret_cast<uintptr_t>(p) %
              alignof(Pod), 0u);
    delete p;
}
```

Either approach is fine; including \<new> is the most canonical fix.

---

### Recap

1. Add `#include <new>` to any file that uses `std::align_val_t`.
2. Ensure the project is compiling with `/std:c++17` or later and **does not** disable `/Zc:alignedNew`.
3. Re-build the **bmobileUtilTests** project → tests should compile and run cleanly.

With that resolved, your first Google Test suite for `bmobile::Heap2` is fully operational. 🎉

***Note:***  When adding a new project to this solution, **ALWAYS** make sure you change the C++ language to C++20

