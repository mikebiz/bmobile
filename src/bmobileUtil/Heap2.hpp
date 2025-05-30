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
