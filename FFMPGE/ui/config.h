#ifndef __ZUI_CONFIG_H__
#define __ZUI_CONFIG_H__


//图形引擎配置
#define PLATFORM_GRAPH_SKIA 0
#define PLATFORM_GRAPH_GDI 1
#define PLATFORM_GRAPH_CAIRO 0
#define PLATFORM_GRAPH_AGG 0

//运行平台配置
#ifdef WIN32
	#define PLATFORM_OS_WIN
#elif defined LINUX
	#define PLATFORM_OS_LINUX
#elif defined MACX
	#define PLATFORM_OS_MACX
#endif


#endif