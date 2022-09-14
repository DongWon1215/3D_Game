#pragma once




#include <d3d9.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <io.h>

#include <process.h>

#ifdef _DEBUG

#pragma warning (disable : 4005)
#pragma warning (disable : 4251)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 

#endif

#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

#include "Engine_TypeDef.h"
#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Functor.h"
#include "Engine_Struct.h"

USING(Engine)
USING(std)
