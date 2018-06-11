#ifndef _DEF_H_ 
#define _DEF_H_

#include <iostream>
#include "opencv2/opencv.hpp"

#define HCOM_OK 1
#define HCOM_ERR 0

#define TRUE 1
#define FALSE 0

#ifndef _WINDOWS_

typedef void    VOID;

typedef unsigned int  UINT;
typedef unsigned short USHORT;
typedef unsigned int  ULONG;    // 安卓代码中打印函数unsigned long会产生告警
typedef unsigned char UCHAR;
typedef unsigned char BYTE;

typedef bool    BOOL;
typedef char    CHAR;
typedef int     INT;
typedef long    LONG;

typedef float   FLOAT;

#endif
typedef double  DOUBLE;

#endif