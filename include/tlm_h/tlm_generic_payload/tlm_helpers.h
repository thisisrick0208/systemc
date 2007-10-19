#ifndef __TLM_HELPERS_H__
#define __TLM_HELPERS_H__

//#include <sys/param.h>
//#include <cstring>

enum tlm_endianness { TLM_LITTLE_ENDIAN, TLM_BIG_ENDIAN };

inline bool hostHasLittleEndianness()
{
#if defined(__sparc) || defined(macintosh) || defined(__hppa) || defined(__APPLE__)

  return false; // HOST IS BIG ENDIAN

#elif defined(__acorn) || defined(__mvs) || defined(_WIN32) || \
  (defined(__alpha) && defined(__osf__)) || defined(__i386)

  return true; // HOST IS LITTLE ENDIAN

#else

  return (__BYTE_ORDER == __LITTLE_ENDIAN);

#endif
}

inline bool hasHostEndianness(tlm_endianness endianness)
{
  if (hostHasLittleEndianness()) {
    return endianness == TLM_LITTLE_ENDIAN;

  } else {
    return endianness == TLM_BIG_ENDIAN;
  }
} 

inline unsigned char swap_bytes(unsigned char rhs)  { return rhs; }
inline char swap_bytes(char rhs) { return rhs; }
inline signed char swap_bytes(signed char rhs) { return rhs; }

inline unsigned short swap_bytes(unsigned short rhs) { return (rhs >> 8) | (rhs << 8); }
inline signed short swap_bytes(signed short rhs) { return swap_bytes((unsigned short)rhs); }

inline unsigned int swap_bytes(unsigned int rhs) { return (rhs << 24) | (rhs >> 24) | ((rhs << 8) & 0x00FF0000) | ((rhs >> 8) & 0x0000FF00); }
inline signed int swap_bytes(signed int rhs) { return swap_bytes((unsigned int)rhs); }

inline unsigned long swap_bytes(unsigned long rhs) { return (rhs << 24) | (rhs >> 24) | ((rhs << 8) & 0x00FF0000) | ((rhs >> 8) & 0x0000FF00); }
inline signed long swap_bytes(signed long rhs) { return swap_bytes((unsigned long)rhs); }

inline unsigned long long swap_bytes(unsigned long long rhs) { return (static_cast<unsigned long long>(swap_bytes((unsigned long)rhs)) << 32) | swap_bytes((unsigned long)(rhs >> 32)); }
inline signed long long swap_bytes(signed long long rhs) { return swap_bytes((unsigned long long)rhs); }



//
// Copy data from src / dest
// swaps the bytes if needed (simulated endianness != host endianness)
//
inline void copy_data(const unsigned char* src, unsigned char* dest,
                      unsigned int nrOfBytes, tlm_endianness endianness)
{
  if (hasHostEndianness(endianness)) {
    memcpy(dest, src, nrOfBytes);

  } else {
    for (unsigned int i = 0; i < nrOfBytes; ++i) {
      dest[i] = src[nrOfBytes - 1 - i];
    }
  }
}

//
// Swap bytes if needed (simulated endianness != host endianness)
//
template <typename DT>
inline DT swapIfNeeded(DT word, tlm_endianness endianness)
{
  if (hasHostEndianness(endianness)) {
    return word;

  } else {
    return swap_bytes(word);
  }
}

#endif /* __TLM_HELPERS_H__ */
