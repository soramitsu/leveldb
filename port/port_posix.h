// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// See port_example.h for documentation for the following types/functions.

#ifndef STORAGE_LEVELDB_PORT_PORT_POSIX_H_
#define STORAGE_LEVELDB_PORT_PORT_POSIX_H_

#include "port/port_config.h"

#include <pthread.h>
#if HAVE_SNAPPY
#include <snappy.h>
#endif  // HAVE_SNAPPY
#include <stdint.h>
#include <string>
#include "port/atomic_pointer.h"

#if !HAVE_FDATASYNC
// Use fsync() on platforms without fdatasync()
#define fdatasync fsync
#endif  // !HAVE_FDATASYNC

#if !HAVE_FREAD_UNLOCKED
// Use fread() on platforms without fread_unlocked()
#define fread_unlocked fread
#endif  // !HAVE_FREAD_UNLOCKED

#if !HAVE_FWRITE_UNLOCKED
// Use fwrite() on platforms without fwrite_unlocked()
#define fwrite_unlocked fwrite
#endif  // !HAVE_FWRITE_UNLOCKED

#if !HAVE_FFLUSH_UNLOCKED
// Use fflush() on platforms without fflush_unlocked()
#define fflush_unlocked fflush
#endif  // !HAVE_FFLUSH_UNLOCKED

namespace leveldb {
namespace port {

static const bool kLittleEndian = !LEVELDB_IS_BIG_ENDIAN;
#undef PLATFORM_IS_LITTLE_ENDIAN

class CondVar;

class Mutex {
 public:
  Mutex();
  ~Mutex();

  void Lock();
  void Unlock();
  void AssertHeld() { }

 private:
  friend class CondVar;
  pthread_mutex_t mu_;

  // No copying
  Mutex(const Mutex&);
  void operator=(const Mutex&);
};

class CondVar {
 public:
  explicit CondVar(Mutex* mu);
  ~CondVar();
  void Wait();
  void Signal();
  void SignalAll();
 private:
  pthread_cond_t cv_;
  Mutex* mu_;
};

typedef pthread_once_t OnceType;
#define LEVELDB_ONCE_INIT PTHREAD_ONCE_INIT
extern void InitOnce(OnceType* once, void (*initializer)());

inline bool Snappy_Compress(const char* input, size_t length,
                            ::std::string* output) {
#if HAVE_SNAPPY
  output->resize(snappy::MaxCompressedLength(length));
  size_t outlen;
  snappy::RawCompress(input, length, &(*output)[0], &outlen);
  output->resize(outlen);
  return true;
#endif  // HAVE_SNAPPY

  return false;
}

inline bool Snappy_GetUncompressedLength(const char* input, size_t length,
                                         size_t* result) {
#if HAVE_SNAPPY
  return snappy::GetUncompressedLength(input, length, result);
#else
  return false;
#endif  // HAVE_SNAPPY
}

inline bool Snappy_Uncompress(const char* input, size_t length,
                              char* output) {
#ifdef SNAPPY
  return snappy::RawUncompress(input, length, output);
#else
  return false;
#endif
}

inline bool GetHeapProfile(void (*func)(void*, const char*, int), void* arg) {
  return false;
}

uint32_t AcceleratedCRC32C(uint32_t crc, const char* buf, size_t size);

} // namespace port
} // namespace leveldb

#endif  // STORAGE_LEVELDB_PORT_PORT_POSIX_H_
