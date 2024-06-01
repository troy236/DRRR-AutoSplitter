#include "asr/asr_helpers.h"
#include "asr/malloc.h"
#include "asr/mini_libc.h"

typedef struct {
    uint64_t trackTics;
    uint64_t lap;
    uint64_t prisonLap;
    uint64_t totalLaps;
    uint64_t level;
    uint64_t gameState;
    uint64_t tutorialComplete;
    uint64_t inSpecialStage;
} GameAddresses;

typedef struct {
    uint32_t trackTics;
    uint8_t lap;
    uint8_t prisonLap;
    uint8_t totalLaps;
    uint16_t level;
    int32_t gameState;
    bool tutorialComplete;
    bool inSpecialStage;
} GameState;

uint8_t operatingSystem = 0;
uint8_t operatingSystemArch = 0;
ProcessId process = 0;
Address gameModule = 0;
GameAddresses* gameAddresses = 0;
GameState* old = 0;
GameState* current = 0;

uint32_t totalIGT = 0;
bool inCredits = false;

bool set_os() {
    char os[7];
    char arch[7];
    size_t os_length = sizeof(os);
    size_t arch_length = sizeof(arch);
    if (!runtime_get_os(os, &os_length)) {
        runtime_print_message_cstr("Could not get OS");
        return false;
    }
    if (!runtime_get_arch(arch, &arch_length)) {
        runtime_print_message_cstr("Could not get OS Arch");
        return false;
    }
    if (os[0] == 'w' && os[1] == 'i' && os[2] == 'n' && os[3] == 'd' && os[4] == 'o' && os[5] == 'w' && os[6] == 's') {
        runtime_print_message_cstr("Detected Windows");
        operatingSystem = 1;
    }
    else if (os[0] == 'l' && os[1] == 'i' && os[2] == 'n' && os[3] == 'u' && os[4] == 'x') {
        runtime_print_message_cstr("Detected Linux");
        operatingSystem = 2;
    }
    else if (os[0] == 'm' && os[1] == 'a' && os[2] == 'c' && os[3] == 'o' && os[4] == 's') {
        runtime_print_message_cstr("Detected Mac OS");
        operatingSystem = 3;
    }
    else {
        runtime_print_message_cstr("Could not detect OS");
        return false;
    }
    if (arch[0] == 'x' && arch[1] == '8' && arch[2] == '6') operatingSystemArch = 1;
    else operatingSystemArch = 2;
    return true;
}

void setup(uint16_t gameVersion) {
    gameAddresses = malloc(sizeof(GameAddresses));
    old = malloc(sizeof(GameState));
    current = malloc(sizeof(GameState));
    //Windows
    if (gameVersion == 1) { //2.0
        gameAddresses->trackTics = 0x1088434;
        gameAddresses->lap = 0x1088444;
        gameAddresses->prisonLap = 0x1088445;
        gameAddresses->totalLaps = 0x1087795;
        gameAddresses->level = 0xADDFF4;
        gameAddresses->gameState = 0x108F344;
        gameAddresses->tutorialComplete = 0x1103DE0;
        gameAddresses->inSpecialStage = 0x5C69BA0;
    }
    else if (gameVersion == 2) { //2.1
        gameAddresses->trackTics = 0x108A434;
        gameAddresses->lap = 0x108A434;
        gameAddresses->prisonLap = 0x108A445;
        gameAddresses->totalLaps = 0x1089795;
        gameAddresses->level = 0xADEFF4;
        gameAddresses->gameState = 0x1091344;
        gameAddresses->tutorialComplete = 0x1105DE0;
        gameAddresses->inSpecialStage = 0x5C6BBA0;
    }
    else if (gameVersion == 3) { //2.2
        gameAddresses->trackTics = 0x10FA47C;
        gameAddresses->lap = 0x10FA48C;
        gameAddresses->prisonLap = 0x10FA48D;
        gameAddresses->totalLaps = 0x10F97D5;
        gameAddresses->level = 0xB4CFF4;
        gameAddresses->gameState = 0x1101484;
        gameAddresses->tutorialComplete = 0x1176100;
        gameAddresses->inSpecialStage = 0x5CDDEE0;
    }
    else if (gameVersion == 4) { //2.3
        gameAddresses->trackTics = 0x115647C;
        gameAddresses->lap = 0x115648C;
        gameAddresses->prisonLap = 0x115648D;
        gameAddresses->totalLaps = 0x11557D5;
        gameAddresses->level = 0xBA7FF4;
        gameAddresses->gameState = 0x115D4C4;
        gameAddresses->tutorialComplete = 0x11D2140;
        gameAddresses->inSpecialStage = 0x5D39F20;
    }
    //Linux x86
    else if (gameVersion == 101) { //2.3
        gameAddresses->trackTics = 0x99A968;
        gameAddresses->lap = 0x99A978;
        gameAddresses->prisonLap = 0x99A979;
        gameAddresses->totalLaps = 0x99958D;
        gameAddresses->level = 0x71B2EC;
        gameAddresses->gameState = 0x9A2264;
        gameAddresses->tutorialComplete = 0x0;
        gameAddresses->inSpecialStage = 0x0;
    }
    //Mac ARM
    else if (gameVersion == 201) { //2.0
        gameAddresses->trackTics = 0xBC75CC;
        gameAddresses->lap = 0xBC75DC;
        gameAddresses->prisonLap = 0xBC75DD;
        gameAddresses->totalLaps = 0xBC6F38;
        gameAddresses->level = 0x978510;
        gameAddresses->gameState = 0xBC6840;
        gameAddresses->tutorialComplete = 0xBDFCB0;
        gameAddresses->inSpecialStage = 0x650BF80;
    }
    else if (gameVersion == 202) { //2.3
        gameAddresses->trackTics = 0xBD7DD8;
        gameAddresses->lap = 0xBD7DE8;
        gameAddresses->prisonLap = 0xBD7DE9;
        gameAddresses->totalLaps = 0xBD7748;
        gameAddresses->level = 0x988540;
        gameAddresses->gameState = 0xBD7050;
        gameAddresses->tutorialComplete = 0xBF0A40;
        gameAddresses->inSpecialStage = 0x651ED28;
    }
    //Mac x86
    else if (gameVersion == 301) { //2.0
        gameAddresses->trackTics = 0xCC110C;
        gameAddresses->lap = 0xCC111C;
        gameAddresses->prisonLap = 0xCC111D;
        gameAddresses->totalLaps = 0xCC0A7C;
        gameAddresses->level = 0xA71E60;
        gameAddresses->gameState = 0xCC0350;
        gameAddresses->tutorialComplete = 0xCD9858;
        gameAddresses->inSpecialStage = 0x6605C38;
    }
    else if (gameVersion == 302) { //2.3
        gameAddresses->trackTics = 0xCCD93C;
        gameAddresses->lap = 0xCCD94C;
        gameAddresses->prisonLap = 0xCCD94D;
        gameAddresses->totalLaps = 0xCCD2AC;
        gameAddresses->level = 0xA7DEA0;
        gameAddresses->gameState = 0xCCCB80;
        gameAddresses->tutorialComplete = 0xCE6608;
        gameAddresses->inSpecialStage = 0x66149F8;
    }
    old->trackTics = 0;
    old->lap = 0;
    old->prisonLap = 0;
    old->totalLaps = 0;
    old->level = 0;
    old->gameState = 0;
    old->tutorialComplete = false;
    old->inSpecialStage = false;
    current->trackTics = 0;
    current->lap = 0;
    current->prisonLap = 0;
    current->totalLaps = 0;
    current->level = 0;
    current->gameState = 0;
    current->tutorialComplete = false;
    current->inSpecialStage = false;
}

void cleanup() {
    if (process != 0) process_detach(process);
    process = 0;
    gameModule = 0;
    if (gameAddresses != 0) free(gameAddresses);
    if (old != 0) free(old);
    if (current != 0) free(current);
    gameAddresses = 0;
    old = 0;
    current = 0;
    totalIGT = 0;
    inCredits = false;
}

bool set_process() {
    uint64_t moduleMemorySize = 0;
    if (operatingSystem == 1) {
        process = process_attach_cstr("ringracers.exe");
        if (!process) return false;
        gameModule = process_get_module_address_cstr(process, "ringracers.exe");
        if (!gameModule) {
            cleanup();
            return false;
        }
        moduleMemorySize = process_get_module_size_cstr(process, "ringracers.exe");
    }
    else if (operatingSystem == 2) {
        operatingSystem = 2;
        process = process_attach_cstr("Main"); //Yes this is really the correct process name. No I don't understand why
        if (!process) return false;
        gameModule = process_get_module_address_cstr(process, "ringracers");
        if (!gameModule) {
            cleanup();
            return false;
        }
        moduleMemorySize = process_get_module_size_cstr(process, "ringracers");
    }
    else if (operatingSystem == 3) {
        process = process_attach_cstr("ringracers");
        if (!process) return false;
        gameModule = process_get_module_address_cstr(process, "ringracers");
        if (!gameModule) {
            cleanup();
            return false;
        }
        moduleMemorySize = process_get_module_size_cstr(process, "ringracers");
    }
    if (operatingSystem == 1) {
        if (moduleMemorySize == 162025472) setup(1); //2.0
        else if (moduleMemorySize == 162033664) setup(2); //2.1
        else if (moduleMemorySize == 162512896) setup(3); //2.2
        else if (moduleMemorySize == 162881536) setup(4); //2.3
        else {
            cleanup();
            return false;
        }
    }
    else if (operatingSystem == 2) {
        if (moduleMemorySize == 9293824) setup(101); //2.3
        else {
            cleanup();
            return false;
        }
    }
    else if (operatingSystem == 3 && operatingSystemArch == 2) {
        //Is there some way of getting moduleMemorySize without running the application? In any context not just the autosplitter so i can paste the value here
        //Or something else I can use that I can grab without running the application
        //Like the full file size etc
        //There needs to be some way we can version check something without requiring a M1 Mac to run the program to get a module size
        if (moduleMemorySize == 178798592) setup(201); //2.0
        else if (moduleMemorySize == 178864128) setup(202); //2.3
        else {
            cleanup();
            return false;
        }
    }
    else if (operatingSystem == 3 && operatingSystemArch == 1) {
        if (moduleMemorySize == 179740672) setup(301); //2.0
        else if (moduleMemorySize == 179814400) setup(302); //2.3
        else {
            cleanup();
            return false;
        }
    }
    runtime_set_tick_rate(35);
    totalIGT = 0;
    inCredits = false;
    timer_pause_game_time();
    return true;
}

void update_gamestate() {
    old->trackTics = current->trackTics;
    old->lap = current->lap;
    old->prisonLap = current->prisonLap;
    old->totalLaps = current->totalLaps;
    old->level = current->level;
    old->gameState = current->gameState;
    old->tutorialComplete = current->tutorialComplete;
    old->inSpecialStage = current->inSpecialStage;
    if (!process_read(process, gameModule + gameAddresses->trackTics, &current->trackTics, sizeof(current->trackTics))) return;
    if (!process_read(process, gameModule + gameAddresses->lap, &current->lap, sizeof(current->lap))) return;
    if (!process_read(process, gameModule + gameAddresses->prisonLap, &current->prisonLap, sizeof(current->prisonLap))) return;
    if (!process_read(process, gameModule + gameAddresses->totalLaps, &current->totalLaps, sizeof(current->totalLaps))) return;
    if (!process_read(process, gameModule + gameAddresses->level, &current->level, sizeof(current->level))) return;
    if (!process_read(process, gameModule + gameAddresses->gameState, &current->gameState, sizeof(current->gameState))) return;
    if (operatingSystem == 1) {
        Address tutorialComplete = 0;
        if (!process_read(process, gameModule + gameAddresses->tutorialComplete, &tutorialComplete, sizeof(tutorialComplete))) return;
        if (!process_read(process, tutorialComplete + 0x70C4, &current->tutorialComplete, sizeof(current->tutorialComplete))) return;
        if (!process_read(process, gameModule + gameAddresses->inSpecialStage, &current->inSpecialStage, sizeof(current->inSpecialStage))) return;
    }
    else if (operatingSystem == 3) {
        Address tutorialComplete = 0;
        if (!process_read(process, gameModule + gameAddresses->tutorialComplete, &tutorialComplete, sizeof(tutorialComplete))) return;
        if (!process_read(process, tutorialComplete + 0x70F8, &current->tutorialComplete, sizeof(current->tutorialComplete))) return;
        if (!process_read(process, gameModule + gameAddresses->inSpecialStage, &current->inSpecialStage, sizeof(current->inSpecialStage))) return;
    }
}

void check_start() {
    totalIGT = 0;
    inCredits = false;
    //Start for NG runs
    if (old->gameState == 6 && !old->tutorialComplete && current->tutorialComplete) timer_start();
    //Start for Beat The Tutorial
    else if (old->level != 230 && current->gameState == 1 && current->level == 230) timer_start();
    //Start for All Cups
    else if (old->level != 5 && current->gameState == 1 && current->level == 5) timer_start();
}

void check_update() {
    if (inCredits && current->gameState == 6) {
        //We are now back at the menu
        inCredits = false;
        return;
    }
    if (inCredits) return;
    if (current->gameState == 9 || current->gameState == 0 || current->gameState == 7) return;
    if (current->trackTics > old->trackTics) {
        totalIGT += current->trackTics - old->trackTics;
    }
}

bool check_split() {
    //Special stages needs better handling. 
    //Outrunning the Emerald and finishing causes a split
    //Occasionally completing a Special Stage successfully does not split. Invisible 'lap' checkpoints can be missed
    //Cannot find a decent Emerald collected boolean
	
    //Split on Race/Special stage complete
    if (old->lap > old->totalLaps && current->level != old->level) return true;
    //Split on Prison stage complete
    if (old->prisonLap == 1 && old->lap == 0 && current->level != old->level) return true;
    //Split on Tutorial stage complete
    if (old->level >= 230 && old->level <= 234 && current->gameState == 1 && current->level != old->level) return true;
    //Split on Springs Tutorial stage to menu
    //This would also split quitting to menu from Springs
    if (old->level == 234 && old->gameState == 1 && current->gameState == 6) return true;
    if (!inCredits && old->gameState != 7 && current->gameState == 7) {
        //Avoids multiple splits at Credits due to demo footage changing gameState
        inCredits = true;
        return true;
    }
    return false;
}

__attribute__((export_name("update"))) void update(void) {
    if (operatingSystem == 0) {
        if (!set_os()) return;
    }
    if (!process) {
        if (!set_process()) return;
    }
    if (!process_is_open(process)) {
        cleanup();
        return;
    }
    update_gamestate();
    switch (timer_get_state()) {
        case NOT_RUNNING: {
            check_start();
            break;
        }
        case RUNNING: {
            timer_pause_game_time();
            check_update();
            timer_set_game_time_seconds_f32((totalIGT * 28.5714285714) / 1000);
            if (check_split()) timer_split();
            break;
        }
        default: {}
    }
}