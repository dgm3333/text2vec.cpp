#ifndef __PMURHASH_H__
#define __PMURHASH_H__

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

using MH_UINT32 = uint32_t;

MH_UINT32 PMurHash32(MH_UINT32 seed, const void *key, int len);

#ifdef __cplusplus
}
#endif

#endif /* __PMURHASH_H__ */
