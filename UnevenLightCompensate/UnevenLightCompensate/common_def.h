#ifndef _DEF_H_ 
#define _DEF_H_

#include <iostream>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;

#define HCOM_OK 1
#define HCOM_ERR 0

#define TRUE 1
#define FALSE 0

#ifndef _WINDOWS_

typedef void    VOID;

typedef unsigned int  UINT;
typedef unsigned short USHORT;
typedef unsigned int  ULONG;
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