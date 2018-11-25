
// MIT License
//
// Copyright (c) 2018 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <Windows.h>

#include <malloc.h>

#include <list>
#include <set>
#include <string>
#include <vector>


template <typename T, bool S = true>
class SecureLockedAllocator
{
  // http://www.drdobbs.com/the-standard-librarian-what-are-allocato/184403759

public:
  typedef T                 value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef std::size_t       size_type;
  typedef std::ptrdiff_t    difference_type;

  // typedef typename enum mode { normal, secure } SecureLockedAllocator::mode_type;

  template <class U>
  struct rebind { typedef SecureLockedAllocator<U, S> other; };

  SecureLockedAllocator() {}
  SecureLockedAllocator(const SecureLockedAllocator&) {}
  template <class U>
  SecureLockedAllocator(const SecureLockedAllocator<U, S>&) {}
  ~SecureLockedAllocator() {}

  pointer address(reference x) const { return &x; }
  const_pointer address(const_reference x) const {
    return x;
  }

   pointer allocate(size_type n, const_pointer = 0) {

    void* p = LocalAlloc ( LPTR, n * sizeof(T) );

    if (!p)
      throw std::bad_alloc();

    if (!LocalLock(p))
      throw std::bad_alloc();

    return static_cast<pointer>(p);
  }

  void deallocate(pointer p, size_type n) {
    if ( S ) {
        SecureZeroMemory ( ( void* ) p, n * sizeof ( T ) );
    }
    LocalUnlock(p);
    LocalFree(p);
    p = nullptr;
  }

  size_type max_size() const {
    return static_cast<size_type>(-1) / sizeof(T);
  }

  void construct(pointer p, const value_type& x) {
    new(p) value_type(x);
  }
  void destroy(pointer p) { p->~value_type(); p = nullptr; }

  void operator = (const SecureLockedAllocator&) = delete;
};

template<bool S> class SecureLockedAllocator<void, S>
{
  typedef void        value_type;
  typedef void*       pointer;
  typedef const void* const_pointer;

  template <class U>
  struct rebind { typedef SecureLockedAllocator<U, S> other; };
};


template <class T, bool S>
bool operator==(const SecureLockedAllocator<T, S>&,
                       const SecureLockedAllocator<T, S>&) {
  return true;
}

template <class T, bool S>
bool operator!=(const SecureLockedAllocator<T, S>&,
                       const SecureLockedAllocator<T, S>&) {
  return false;
}
