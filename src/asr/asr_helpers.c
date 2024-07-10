#include "asr_helpers.h"
#include "mini_libc.h"

void timer_set_variable_cstr(char const *key, char const *value) {
    timer_set_variable(key, strlen(key), value, strlen(value));
}

void timer_set_game_time_seconds_f32(float seconds) {
    int64_t truncated = seconds;
    timer_set_game_time(truncated, (seconds - truncated) * 1000000000.0f);
}

ProcessId process_attach_cstr(char const *name) {
    return process_attach(name, strlen(name));
}

Address process_get_module_address_cstr(ProcessId process, char const *name) {
    return process_get_module_address(process, name, strlen(name));
}

uint64_t process_get_module_size_cstr(ProcessId process, char const *name) {
    return process_get_module_size(process, name, strlen(name));
}

void runtime_print_message_cstr(char const *text) {
    runtime_print_message(text, strlen(text));
}

bool user_settings_add_bool_helper(const char* key_ptr, const char* description_ptr, bool default_value) {
    return user_settings_add_bool(key_ptr, strlen(key_ptr), description_ptr, strlen(description_ptr), default_value);
}

uint64_t settings_map_get_helper(uint64_t map, const char* key_ptr) {
    return settings_map_get(map, key_ptr, strlen(key_ptr));
}
