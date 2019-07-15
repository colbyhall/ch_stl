#pragma once

#include "core.h"

#if CH_PLATFORM_WINDOWS
#include "win32/os_win32.h"
#else
#error Platform not supported
#endif