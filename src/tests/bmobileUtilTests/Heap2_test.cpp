#include "pch.h"                // your test project’s PCH
#include <new>
#include <gtest/gtest.h>
#include <thread>
#include <vector>

#include "../../bmobileUtil/Heap2.hpp"    // allocator under test

namespace {

    // -----------------------------------------------------------------------------
    // Simple POD to allocate
    // -----------------------------------------------------------------------------
    struct Pod
    {
        int    a{ 0 };
        double b{ 0.0 };
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
        void* p = ::operator new(sizeof(Pod), std::align_val_t{ alignof(Pod) });
        EXPECT_EQ(reinterpret_cast<uintptr_t>(p) % alignof(Pod), 0u);
        ::operator delete(p, std::align_val_t{ alignof(Pod) });
    }

    TEST(Heap2, ConcurrentAllocations)
    {
        constexpr std::size_t iters = 10'000;
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
