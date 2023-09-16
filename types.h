#ifndef _types_h_included
#define _types_h_included

typedef signed   char	s8;
typedef unsigned char	u8;

typedef signed   short	s16;
typedef unsigned short	u16;

typedef signed   int	s32;
typedef unsigned int	u32;

#ifndef __cplusplus
typedef u8 bool;
#endif
#define false	0
#define true	1

#define FALSE	false
#define TRUE	true

#define NULL_FUNC_COMP	(bool(*)(void *))0

#define NULL_FUNC	(void(*)())0
#ifndef NULL
#define NULL		((void*)0)
#endif


#endif