#ifndef __ZUI_PLATFORM_H__
#define __ZUI_PLATFORM_H__
/*
* 平台移植层头文件
*/
#include "../config.h"
#include "../zui.h"


#if (defined PLATFORM_GRAPH_SKIA) && (PLATFORM_GRAPH_SKIA == 1)
	#include "../include/Skia.h"
	#include "graph\skia.h"
#elif (defined PLATFORM_GRAPH_GDI) && (PLATFORM_GRAPH_GDI == 1) && (defined PLATFORM_OS_WIN)
	//只适应于WIN平台
	#pragma comment(lib, "Msimg32.lib")  
	#pragma comment(lib, "Gdiplus.lib")
	#include "graph\gdi.h"
#elif (defined PLATFORM_GRAPH_CAIRO) && (PLATFORM_GRAPH_CAIRO == 1)
	#include "graph\cairo.h"
#elif (defined PLATFORM_GRAPH_AGG) && (PLATFORM_GRAPH_AGG == 1)
	#include "graph\agg.h"
#endif //PLATFORM_GRAPH

#if (defined PLATFORM_OS_WIN)
	#pragma comment(lib, "Imm32.lib")
	#include "os\win.h"
#elif (defined PLATFORM_OS_LINUX)
	#include "os\linux.h"
#elif (defined PLATFORM_OS_MACX)

#endif


/*移植层接口定义头文件*/
#include "graph\graph.h"
#include "os\os.h"


#endif //__ZUI_PLATFORM_H__