#pragma once

#include <vector>
#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/enable_shared_from_this.hpp>

template<typename T, size_t CacheWidth = 1>
class ContiguousCache : public boost::enable_shared_from_this<ContiguousCache<T, CacheWidth> >, private boost::noncopyable {
public:
	BOOST_STATIC_ASSERT(CacheWidth > 0);
	typedef ContiguousCache<T, CacheWidth> ContiguousCacheT;
	typedef boost::shared_ptr<ContiguousCacheT> ContiguousCachePtr;

	//ensure that ContiguousCaches are created on the heap
	static ContiguousCachePtr getContiguousCache(const size_t reserveCacheHeight) {
		ContiguousCachePtr cc(new ContiguousCacheT(reserveCacheHeight));
		return cc;
	}

	//only call getPtr() if CacheWidth == 1
	boost::shared_ptr<T> getPtr() {
		BOOST_STATIC_ASSERT(CacheWidth == 1);
		return get<boost::shared_ptr<T> >();
	}
	//use getArray() if you are treating the cache as a cache of arrays
	boost::shared_array<T> getArray() {
		return get<boost::shared_array<T> >();
	}
	~ContiguousCache() { }

	size_t getAvailableSize() const { return availableTs.size(); }
	size_t getCacheWidth() const { return CacheWidth; }
	size_t getReservedCacheHeight() const { return reservedCacheHeight; }
	size_t getContiguousCacheSize() const { return contiguousCacheSize; }

private:
	ContiguousCache(const size_t reserveCacheHeight) 
		: reservedCacheHeight(reserveCacheHeight), 
		contiguousCacheSize(reserveCacheHeight * CacheWidth),
		contiguousCache(new T[reserveCacheHeight * CacheWidth])
	{
		for(size_t i=0; i<reservedCacheHeight; ++i) {
			availableTs.push_back(contiguousCache.get() + (i*CacheWidth));
		}
	}

	//SmartPtrType should be boost::shared_ptr<T> or boost::shared_array<T>
	template<typename SmartPtrType>
	SmartPtrType get() {
		boost::recursive_mutex::scoped_lock lock(mutex);
		if(availableTs.size() > 0) {
			T* t = availableTs.back();
			availableTs.pop_back();

			//return a tPtr with a custom deallocator which points to the cache t came from
			//when this tPtr is destroyed the ContiguousCacheDeallocator will return the 
			//memory to the cache
			SmartPtrType tPtr(t, ContiguousCacheDeallocator(shared_from_this()));
			return tPtr;
		}
		//no T's left in the cache, return a newly allocated one
		//if CacheWidth == 1 then it is fine if SmartPtrType is shared_ptr or shared_array
		SmartPtrType tPtr(new T[CacheWidth]);
		return tPtr;
	}

	void returnToAvailable(T* t) {
		boost::recursive_mutex::scoped_lock lock(mutex);
		availableTs.push_back(t);
	}

	//Functor to faciliate the "deallocation" of cached Ts
	//In this case, "deallocation" means return to available cache
	class ContiguousCacheDeallocator {
	public:
		ContiguousCacheDeallocator(ContiguousCachePtr ccPtr) : cc(ccPtr) {}

		//default copy constructor and assignment operator are fine

		//t must have been allocated by the ContiguousCache pointed to by ccPtr
		void operator()(T* t) {
			cc->returnToAvailable(t);
		}
	private:
		ContiguousCachePtr cc;
	};
	friend ContiguousCacheDeallocator; //make the deallocator a friend of this so it can call returnToAvailable(T* t)

	boost::recursive_mutex mutex;
	const size_t reservedCacheHeight;
	const size_t contiguousCacheSize;
	boost::shared_array<T> contiguousCache;
	std::vector<T*> availableTs;
};