#pragma once

#ifdef __clang__
#pragma clang diagnostic ignored "-Wfixed-enum-extension"
#define enum_t(T) enum : T
#else
#define enum_t(T) enum
#endif
