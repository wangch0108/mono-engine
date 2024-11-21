#pragma once

#include "MonoTypes.h"

// declaration mono Api
#define DO_API(r, n, p) extern r (*n) p;
#include "MonoFunctions.h"