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
