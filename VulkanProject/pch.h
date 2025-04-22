// プリコンパイル済みヘッダー
#pragma once

// ワイド文字列用
#ifndef UNICODE
#define UNICODE
#endif

// 不要なWindowsAPI省略用
#define WIN32_LEAN_AND_MEAN
// Standard
#include <windows.h>
#include <comdef.h>
#include <assert.h>
#include <memory>
#include <wrl/client.h>
#include <windows.applicationmodel.core.h>
#include <Shobjidl.h>

#include <algorithm>
#include <memory>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <mutex>
#include <chrono>
#include <optional>
#include <unordered_set>

#include <math.h>
#include <fstream>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>
#include <utility>
#include <codecvt>
#include <locale>
#include <sys/types.h>

// Utilities

// Assimp

// ImGUI

// Vulkan Helpers

// Vulkan
using namespace Microsoft::WRL;


// Libs
#pragma comment(lib, "vulkan-1.lib")
#pragma comment(lib, "glfw3.lib")
