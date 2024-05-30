#pragma once

#include "asr.h"

/// Sets a custom key value pair. This may be arbitrary information that
/// the auto splitter wants to provide for visualization.
void timer_set_variable_cstr(char const *key, char const *value);

/// Sets the game time.
void timer_set_game_time_seconds_f32(float seconds);

/// Attaches to a process based on its name.
ProcessId process_attach_cstr(char const *name);
/// Gets the address of a module in a process.
Address process_get_module_address_cstr(ProcessId process, char const *name);
/// Gets the size of a module in a process.
uint64_t process_get_module_size_cstr(ProcessId process, const char* name);

/// Prints a log message for debugging purposes.
void runtime_print_message_cstr(char const *text);
