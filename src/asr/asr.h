#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum TimerState
{
    /// The timer is not running.
    NOT_RUNNING = 0,
    /// The timer is running.
    RUNNING = 1,
    /// The timer started but got paused. This is separate from the game
    /// time being paused. Game time may even always be paused.
    PAUSED = 2,
    /// The timer has ended, but didn't get reset yet.
    ENDED = 3,
} TimerState;

typedef enum MemoryRangeFlags
{
    /// The memory range is readable.
    MEMORYRANGEFLAGS_READ = 1 << 1,
    /// The memory range is writable.
    MEMORYRANGEFLAGS_WRITE = 1 << 2,
    /// The memory range is executable.
    MEMORYRANGEFLAGS_EXECUTE = 1 << 3,
    /// The memory range has a file path.
    MEMORYRANGEFLAGS_PATH = 1 << 4,
} MemoryRangeFlags;

typedef uint64_t ProcessId;
typedef uint64_t Address;

/// Gets the state that the timer currently is in.
TimerState timer_get_state(void);

/// Starts the timer.
void timer_start(void);
/// Splits the current segment.
void timer_split(void);
/// Resets the timer.
void timer_reset(void);
/// Sets a custom key value pair. This may be arbitrary information that
/// the auto splitter wants to provide for visualization.
void timer_set_variable(char const *key_ptr, size_t key_len, char const *value_ptr, size_t value_len);

/// Sets the game time.
void timer_set_game_time(int64_t secs, int32_t nanos);
/// Pauses the game time. This does not pause the timer, only the
/// automatic flow of time for the game time.
void timer_pause_game_time(void);
/// Resumes the game time. This does not resume the timer, only the
/// automatic flow of time for the game time.
void timer_resume_game_time(void);

/// Attaches to a process based on its name. Returns 0 if the process can't be
/// found.
ProcessId process_attach(char const *name_ptr, size_t name_len);
/// Detaches from a process.
void process_detach(ProcessId process);
/// Checks whether is a process is still open. You should detach from a
/// process and stop using it if this returns `false`.
bool process_is_open(ProcessId process);
/// Reads memory from a process at the address given. This will write
/// the memory to the buffer given. Returns `false` if this fails.
bool process_read(ProcessId process, Address address, void* buf_ptr, size_t buf_len);
/// Gets the address of a module in a process.
Address process_get_module_address(ProcessId process, char const *name_ptr, size_t name_len);
/// Gets the size of a module in a process.
uint64_t process_get_module_size(ProcessId process, const char* name_ptr, size_t name_len);
bool process_get_path(ProcessId process, char* buf_ptr, size_t* buf_len_ptr);

/// Stores the name of the operating system that the runtime is running
/// on in the buffer given. Returns `false` if the buffer is too small.
/// After this call, no matter whether it was successful or not, the
/// `buf_len_ptr` will be set to the required buffer size. The name is
/// guaranteed to be valid UTF-8 and is not nul-terminated.
/// Example values: `windows`, `linux`, `macos`
bool runtime_get_os(char* buf_ptr, size_t* buf_len_ptr);
/// Stores the name of the architecture that the runtime is running on
/// in the buffer given. Returns `false` if the buffer is too small.
/// After this call, no matter whether it was successful or not, the
/// `buf_len_ptr` will be set to the required buffer size. The name is
/// guaranteed to be valid UTF-8 and is not nul-terminated.
/// Example values: `x86`, `x86_64`, `arm`, `aarch64`
bool runtime_get_arch(char* buf_ptr, size_t* buf_len_ptr);
/// Sets the tick rate of the runtime. This influences the amount of
/// times the `update` function is called per second.
void runtime_set_tick_rate(double ticks_per_second);
/// Prints a log message for debugging purposes.
void runtime_print_message(char const *text_ptr, size_t text_len);

/// Adds a new setting that the user can modify. This will return either
/// the specified default value or the value that the user has set.
bool user_settings_add_bool(const char* key_ptr, size_t key_len, const char* description_ptr, size_t description_len, bool default_value);

/// Loads a copy of the currently set global settings map. Any changes to it
/// are only perceived if it's stored back. You own the settings map and are
/// responsible for freeing it.
uint64_t settings_map_load();
/// Gets a copy of the setting value from the settings map based on the key.
/// Returns `None` if the key does not exist. Any changes to it are only
/// perceived if it's stored back. You own the setting value and are
/// responsible for freeing it. The pointer needs to point to valid UTF-8
/// encoded text with the given length.
uint64_t settings_map_get(uint64_t map, const char* key_ptr, size_t key_len);
/// Gets the length of a settings map.
uint64_t settings_map_len(uint64_t map);
/// Frees a settings map.
void settings_map_free(uint64_t map);

/// Gets the length of a settings map.
bool setting_value_get_bool(uint64_t value, bool* value_ptr);
/// Frees a setting value.
void setting_value_free(uint64_t value);