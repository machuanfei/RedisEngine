#include "stdafx.h"
#include "ModuleExporter.h"
#include "RedisEngine.h"

void * GetModuleInterface(LPCSTR pszInterfaceName)
{
	return new CRedisEngine();
}