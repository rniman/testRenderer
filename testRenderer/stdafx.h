// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

//stl
#include <chrono>
#include <array>
#include <vector>
#include <memory>

constexpr UINT FRAMEBUFFER_WIDTH{ 1080 };
constexpr UINT FRAMEBUFFER_HEIGHT{ 720 };

constexpr UCHAR VK_W{ 0x57 };
constexpr UCHAR VK_S{ 0x53 };
constexpr UCHAR VK_D{ 0x44 };
constexpr UCHAR VK_A{ 0x41 };
constexpr UCHAR VK_Q{ 0x51 };
constexpr UCHAR VK_E{ 0x45 };

constexpr DWORD DIR_FORWARD{ 0x01 };
constexpr DWORD DIR_BACKWARD{ 0x02 };
constexpr DWORD DIR_RIGHT{ 0x04 };
constexpr DWORD DIR_LEFT{ 0x08 };
constexpr DWORD DIR_UP{ 0x10 };
constexpr DWORD DIR_DOWN{ 0x20 };
