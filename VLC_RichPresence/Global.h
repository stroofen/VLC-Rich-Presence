#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _WIN32
#pragma once
#endif

#include "Types.h"
#include "Macro.h"
#include <numeric>

// I want to brutalize whoever made these macros and included them in EVERY STANDARD HEADER.
#undef min
#undef max

#if defined(_WIN32) || (defined(_WIN64))
#define SYSTEM_WINDOWS
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux)
/*
I'd just like to interject for a moment. What you’re referring to as Linux,
is in fact, GNU/Linux, or as I’ve recently taken to calling it, GNU plus
Linux. Linux is not an operating system unto itself, but rather another free
component of a fully functioning GNU system made useful by the GNU corelibs,
shell utilities and vital system components comprising a full OS as defined
by POSIX. Many computer users run a modified version of the GNU system every
day, without realizing it. Through a peculiar turn of events, the version of
GNU which is widely used today is often called “Linux”, and many of its users
are not aware that it is basically the GNU system, developed by the GNU Project.
There really is a Linux, and these people are using it, but it is just a part
of the system they use. Linux is the kernel: the program in the system that
allocates the machine’s resources to the other programs that you run. The
kernel is an essential part of an operating system, but useless by itself; it
can only function in the context of a complete operating system. Linux is
normally used in combination with the GNU operating system: the whole system is
basically GNU with Linux added, or GNU/Linux. All the so-called “Linux”
distributions are really distributions of GNU/Linux.
*/
#define SYSTEM_GAY
#endif

#if defined (SYSTEM_WINDOWS)
#include <windows.h>
#elif defined (SYSTEM_GAY)

#endif

DECL_CONST_GLOBAL(const wchar_t*, VersionW, L"VLC Discord Manager [ALPHA 1.0]");
DECL_CONST_GLOBAL(const char*, VersionA, "VLC Discord Manager [ALPHA 1.0]");

DECL_CONST_GLOBAL(const char*, DiscordAppID, "410664151334256663");
DECL_CONST_GLOBAL(u32, DiscordIconCount, 5u);
DECL_CONST_GLOBAL_CUSTOM(const char*, DiscordIcons)[g_DiscordIconCount] = { "vlc", "vlcflat", "vlcblue", "vlcneon", "vlcxmas" };
DECL_GLOBAL(i32, DiscordIcon, 0);
DECL_GLOBAL(const char*, DiscordIconString, g_DiscordIcons[0u]);

DECL_CONST_GLOBAL(u32, MaxErrorTitleLength, 64u);
DECL_CONST_GLOBAL(u32, MaxErrorTextLength, 256u);

DECL_CONST_GLOBAL(u32, GUIWidth, 640u);
DECL_CONST_GLOBAL(u32, GUIHeight, 480u);

DECL_CONST_GLOBAL(u32, HookNameLength, 32u);

// Process scan interval (in seconds)
DECL_GLOBAL(f32, ScanInterval, 3.0f);
// Title update interval (in seconds)
DECL_GLOBAL(f32, UpdateInterval, 0.5f);
DECL_CONST_GLOBAL(f32, DiscordUpdateInterval, 0.25f);

#if defined(SYSTEM_WINDOWS)
DECL_CONST_GLOBAL(const wchar_t*, VLCDefaultPath, L"C:\\Program Files\\VideoLAN\\VLC\\vlc.exe");
#elif defined(SYSTEM_GAY)

#endif

// Title of window
DECL_GLOBAL_CUSTOM(char, VLCTitle)[256u] = "";
DECL_CONST_GLOBAL(const char*, DefaultVLCTitle, "VLC media player");

DECL_GLOBAL(bool, ForceUpdate, false);

#endif