#pragma once

#define SVCMAN_C_FUNCTION extern "C" __declspec(dllexport)

//#include "ServiceController.h"
//#include "ServiceEnumerator.h"

SVCMAN_C_FUNCTION
int GetServiceList();

