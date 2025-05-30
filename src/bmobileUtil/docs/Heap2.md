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