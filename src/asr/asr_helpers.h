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

/// Adds a new setting that the user can modify. This will return either
/// the specified default value or the value that the user has set.
bool user_settings_add_bool_helper(const char* key_ptr, const char* description_ptr, bool default_value);

/// Gets a copy of the setting value from the settings map based on the key.
/// Returns `None` if the key does not exist. Any changes to it are only
/// perceived if it's stored back. You own the setting value and are
/// responsible for freeing it. The pointer needs to point to valid UTF-8
/// encoded text with the given length.
uint64_t settings_map_get_helper(uint64_t map, const char* key_ptr);