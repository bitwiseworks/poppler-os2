//========================================================================
//
// GooMutex.h
//
// Portable mutex macros.
//
// Copyright 2002-2003 Glyph & Cog, LLC
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2009 Kovid Goyal <kovid@kovidgoyal.net>
// Copyright (C) 2013 Thomas Freitag <Thomas.Freitag@alfa.de>
// Copyright (C) 2013 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2013 Adam Reichold <adamreichold@myopera.com>
// Copyright (C) 2014 Bogdan Cristea <cristeab@gmail.com>
// Copyright (C) 2014 Peter Breitenlohner <peb@mppmu.mpg.de>
// Copyright (C) 2017 Adrian Johnson <ajohnson@redneon.com>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#ifndef GMUTEX_H
#define GMUTEX_H

#ifdef MULTITHREADED

// Usage:
//
// GooMutex m;
// gInitMutex(&m);
// ...
// gLockMutex(&m);
//   ... critical section ...
// gUnlockMutex(&m);
// ...
// gDestroyMutex(&m);

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

typedef CRITICAL_SECTION GooMutex;

#define gInitMutex(m) InitializeCriticalSection(m)
#define gDestroyMutex(m) DeleteCriticalSection(m)
#define gLockMutex(m) EnterCriticalSection(m)
#define gUnlockMutex(m) LeaveCriticalSection(m)

#elif defined(__OS2__)

/* OS/2 multi-threaded uses pthreads but differs from Linux support
   in that it uses a single mutex for all protected objects
   rather than a unique mutex for each object
   except when invoked for GlobalParams.cc
*/

#include <pthread.h>

typedef pthread_mutex_t GooMutex2;

#ifdef GLOBAL_PARAMS_CC

typedef pthread_mutex_t GooMutex;

// Keep in sync with Linux style mutexes

inline void gInitMutex(GooMutex *m) {
  pthread_mutexattr_t mutexattr;
  pthread_mutexattr_init(&mutexattr);
  pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(m, &mutexattr);
  pthread_mutexattr_destroy(&mutexattr);
}

#define gDestroyMutex(m) pthread_mutex_destroy(m)

#define gLockMutex(m) pthread_mutex_lock(m)
#define gUnlockMutex(m) pthread_mutex_unlock(m)

#else

/* Use single mutex everywhere other than GlobalParams.cc.
   This avoids OS/2 running out of semaphores when documents
   require a large number of class instances.
   All classes other than those defined in GlobalParams.cc will
   use the same mutex (theOS2Mutex).
   GooMutex is typedef'ed as a pointer so macro users will pass
   a pointer to a pointer.
   The macros dereference this pointer and pass a pointer to apthread_mutex
   to the pthreads functions.
*/

class MutexLocker;
typedef pthread_mutex_t *GooMutex;

inline void gInitMutex(GooMutex *m) {
  extern GooMutex2 theOS2Mutex;
  *(m) = &theOS2Mutex;
}
#define gDestroyMutex(m)                // Do nothing

#define gLockMutex(m) pthread_mutex_lock(*(m))
#define gUnlockMutex(m) pthread_mutex_unlock(*(m))

#endif // __OS2__

#else // assume pthreads

#include <pthread.h>

typedef pthread_mutex_t GooMutex;

inline void gInitMutex(GooMutex *m) {
  pthread_mutexattr_t mutexattr;
  pthread_mutexattr_init(&mutexattr);
  pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(m, &mutexattr);
  pthread_mutexattr_destroy(&mutexattr);
}
#define gDestroyMutex(m) pthread_mutex_destroy(m)
#define gLockMutex(m) pthread_mutex_lock(m)
#define gUnlockMutex(m) pthread_mutex_unlock(m)

#endif

class MutexLocker {
public:
  MutexLocker(GooMutex *mutexA) : mutex(mutexA) { gLockMutex(mutex); }
  ~MutexLocker() { gUnlockMutex(mutex); }

private:
  GooMutex *mutex;
};

#endif // MULTITHREADED

#endif // GMUTEX_H
