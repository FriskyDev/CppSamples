// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// STL - Standard template library
#include <algorithm>
#include <allocators>
#include <array>
#include <atomic>
#include <bitset>
#include <chrono>
#include <codecvt>
#include <complex>
#include <condition_variable>
#include <cvt/wbuffer>
#include <cvt/wstring>
#include <deque>
#include <exception>
#include <filesystem>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <iso646.h>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <regex>
#include <ratio>
#include <scoped_allocator>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <strstream>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <vector>

// C++ headers for C std
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>

// Windows headers

#include "targetver.h"

// remove rarely used headers when using Windows.h to gain compiler performance
// excludes headers: cderr, dde. ddeml, dlgs, lzexpand, mmsystem, nb30, rpc, shellapi, winperf, winsock, winefs, winscard, winspool, ole, ole2, commdlg
#define WIN32_LEAN_AND_MEAN

// VC_EXTRALEAN defines the following in AFXV_W32.h:
//   WIN32_EXTRA_LEAN
//   NOSERVICE
//   NOMCX
//   NOIME
//   NOSOUND
//   NOCOMM
//   NOKANJI
//   NORPC
//   NOPROXYSTUB
//   NOIMAGE
//   NOTAPE
#define VC_EXTRA_LEAN

// keep the Windows.h header from including Winsock.h
#define _WINSOCKAPI_

// include network sockets
#include <Winsock2.h>
#include <WS2tcpip.h>

#include <Windows.h>
#include <tchar.h>
#include <varargs.h>

// TODO: reference additional headers your program requires here
