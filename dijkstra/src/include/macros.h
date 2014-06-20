/**
 * @file macros.h
 * @brief Macros.
 */

#ifndef DEF_MACROS
#define DEF_MACROS

/**
 * Warning logger.
 */
#ifdef DEBUG

#define LOGWARNING(msg) { \
	fprintf(stderr, "[WARNING] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

#else

#define LOGWARNING(msg) 

#endif

/**
 * Error logger.
 */
#ifdef DEBUG

#define LOGERR(msg) { \
	fprintf(stderr, "[ERROR] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

#else

#define LOGERR(msg) 

#endif

#define LOGINFO(msg) { \
	fprintf(stderr, "[INFO] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

#define LOGINFO1I(msg, i) { \
	fprintf(stderr, "[INFO] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg, i); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

#define LOGINFO2I(msg, i, j) { \
	fprintf(stderr, "[INFO] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg, i, j); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

#define LOGINFO3I(msg, i, j, k) { \
	fprintf(stderr, "[INFO] %s:%d: ", __FILE__, __LINE__); \
	fprintf(stderr, msg, i, j, k); \
	fprintf(stderr, "\n"); \
	fflush(stderr); }

/**
 * Pointer cast.
 */
#define PTRCAST(ptr, type) ((type*) ptr)

/**
 * Pointer cast and dereferencing.
 */
#define PTRVAL(ptr, type) *((type*) ptr)

#endif