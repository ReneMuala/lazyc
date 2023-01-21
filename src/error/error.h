#pragma once

/// @brief  logs an error
/// @param message error message
/// @param function function that caused the error
/// @param line line that caused the error
extern void lazy_error(const char * message, const char * function , long long line);