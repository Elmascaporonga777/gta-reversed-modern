#pragma once

#define APP_CLASS "Grand theft auto San Andreas"

//! Win32 fail check (returns from function automagically)
#define WIN_FCHECK(x) do { \
        if (HRESULT hr = (x); FAILED(hr)) { \
	        DEV_LOG(TEXT("FAILED(hr=0x{:x}) in ") TEXT(#x) TEXT("\n"), (size_t)(hr)); \
            return; \
        } \
    } while (0) \

// Custom enum, generated by ChatGPT ~~thank you~~ - ChatGPT was wrong.
//
enum class WinVer {
    WIN_95,             //< Windows 95
    WIN_98,             //< Windows 98
    WIN_NT,             //< Windows NT
    WIN_2000_XP_2003,   //< Windows 2000/XP/Server 2003
    WIN_VISTA_OR_LATER, //< Windows Vista/7/8/10

    UNKNOWN
};
static auto& s_WinVer = StaticRef<WinVer, 0xC8CF68>();

struct OSStatus {
    WinVer OSVer;
    DWORD  DxVer; // Always 0x900
    struct { // TODO: Just use MEMORYSTATUS stuct here
        SIZE_T TotalPhys;
        SIZE_T AvailPhys;
        SIZE_T TotalVirtual;
        SIZE_T AvailVirtual;
    } RAM;
    struct {
        SIZE_T Total;
        SIZE_T Avail;
    } VRAM;
};
static inline auto& s_OSStatus = StaticRef<OSStatus, 0xC8CF68>();

static bool& anisotropySupportedByGFX = *(bool*)0xC87FFC;
static bool& isForeground = *(bool*)0xC920EC;
static bool& Windowed = *(bool*)0xC920CC;

void Win32InjectHooks();

#define IDD_DIALOG1                     104
#define IDC_DEVICESEL                   1000
#define IDC_VIDMODE                     1001
#define IDEXIT                          1002
#define IDC_SELECTDEVICE                1005

#define IDI_MAIN_ICON                   1042
