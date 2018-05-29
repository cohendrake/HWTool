#ifndef COMMON_H_
#define COMMON_H_

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define MIN_DIV0 1e-16f

#define MIN_FLOAT_DIFF 1e-8f

#define FILECLOSE(a) {if (a) {fclose(a); a = NULL;}}

#define INT16_MAX ((1 << (16 - 1)) - 1)

#define INT16_MIN (1 << (16 - 1))

#endif

