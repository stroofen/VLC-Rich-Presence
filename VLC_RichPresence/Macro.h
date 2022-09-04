#ifndef MACRO_H
#define MACRO_H

#define DECL_GLOBAL(type, name, value) inline type g_##name = value
#define DECL_GLOBAL_CUSTOM(type, name) inline type g_##name
#define DECL_CONST_GLOBAL(type, name, value) static constexpr type g_##name = value
#define DECL_CONST_GLOBAL_CUSTOM(type, name) static constexpr type g_##name

#define RGBA(r, g, b, a) static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f, static_cast<float>(a) / 255.f

#endif