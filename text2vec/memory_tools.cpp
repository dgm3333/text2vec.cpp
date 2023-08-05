

#include <stdlib.h>

#ifdef __GLIBC__
#include <malloc.h>
#endif

#ifdef USE_BRK
#include <unistd.h>
#include <stdint.h>
#endif

bool trim_heap() {
#ifdef __GLIBC__
	// workaround for issue when glibc on ubuntu doesn't release memory
	// https://bugs.r-project.org/bugzilla3/show_bug.cgi?id=14611
	return malloc_trim(0) == 0;  // Return true if successful, else false
#else
	return false;  // Always return false if not using glibc
#endif
}
