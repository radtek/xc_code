#pragma once
#include "Xczb_Struct.h"

// api interface
typedef int(*p_Connect)(char* IP, char* Port, char* strJson);
typedef int(*p_Subscribe)(char* strJson);
typedef void(*p_Release)();
typedef int(*p_RegisterSpi)(char* SpiPath);

// spi interface
typedef void(*pOnData)(char* strData);
typedef void(*pOnClose)();

