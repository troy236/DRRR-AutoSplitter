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
uint16_t gameVersion = 0;
ProcessId process = 0;
Address gameModule = 0;
GameAddresses* gameAddresses = 0;
GameState* old = 0;
GameState* current = 0;

uint32_t totalIGT = 0;
bool inCredits = false;

bool settingsInitialized = false;
bool autostartAnyPercentEmerald = true;
bool autostartTutorial = true;
bool splitEndTrack = true;
bool splitEndPrison = true;
bool splitEndEmerald = true;
bool splitCredits = true;
bool splitEndChaosEmeraldRun = true;
bool splitEndSuperEmeraldRun = true;
bool autostartRingCup = true;
bool autostartSneakerCup = false;
bool autostartSpringCup = false;
bool autostartBarrierCup = false;
bool autostartInvincibleCup = false;
bool autostartEmeraldCup = false;
bool autostartExtraCup = false;
bool autostartSPBCup = false;
bool autostartRocketCup = false;
bool autostartAquaCup = false;
bool autostartLightningCup = false;
bool autostartFlameCup = false;
bool autostartSuperCup = false;
bool autostartEggCup = false;
bool autostartGogglesCup = false;
bool autostartTimerCup = false;
bool autostartGrowCup = false;
bool autostartChaoCup = false;
bool autostartWingCup = false;
bool autostartMegaCup = false;
bool autostartPhantomCup = false;
bool autostartFlashCup = false;
bool autostartSwapCup = false;
bool autostartShrinkCup = false;
bool autostartBombCup = false;
bool autostartPowerCup = false;;
bool autostartGenesisCup = false;
bool autostartSkateCup = false;
bool autostartRecycleCupA = false;
bool autostartRecycleCupB = false;

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

void setup() {
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
    else if (gameVersion == 5) { //2.4
        gameAddresses->trackTics = 0x14608F0;
        gameAddresses->lap = 0x1460900;
        gameAddresses->prisonLap = 0x1460901;
        gameAddresses->totalLaps = 0x145FD15;
        gameAddresses->level = 0xC610B4;
        gameAddresses->gameState = 0x1468004;
        gameAddresses->tutorialComplete = 0x0; //Unused
        gameAddresses->inSpecialStage = 0x1BB99C0;
    }
    //Linux ARM
    /*else if (gameVersion == 101) { //2.3
        //TODO
    }*/
    //Linux x86
    else if (gameVersion == 201) { //2.3
        gameAddresses->trackTics = 0x99A968;
        gameAddresses->lap = 0x99A978;
        gameAddresses->prisonLap = 0x99A979;
        gameAddresses->totalLaps = 0x99958D;
        gameAddresses->level = 0x71B2EC;
        gameAddresses->gameState = 0x9A2264;
        gameAddresses->tutorialComplete = 0xA37EE0;
        gameAddresses->inSpecialStage = 0x0;
    }
    else if (gameVersion == 202) { //2.4
        gameAddresses->trackTics = 0xC85258;
        gameAddresses->lap = 0xC85268;
        gameAddresses->prisonLap = 0xC85269;
        gameAddresses->totalLaps = 0xC83F55;
        gameAddresses->level = 0x789424;
        gameAddresses->gameState = 0xC8D3C4;
        gameAddresses->tutorialComplete = 0x0;
        gameAddresses->inSpecialStage = 0x0;
    }
    //Mac ARM
    else if (gameVersion == 301) { //2.0
        gameAddresses->trackTics = 0xBC75CC;
        gameAddresses->lap = 0xBC75DC;
        gameAddresses->prisonLap = 0xBC75DD;
        gameAddresses->totalLaps = 0xBC6F38;
        gameAddresses->level = 0x978510;
        gameAddresses->gameState = 0xBC6840;
        gameAddresses->tutorialComplete = 0xBDFCB0;
        gameAddresses->inSpecialStage = 0x650BF80;
    }
    else if (gameVersion == 302) { //2.3
        gameAddresses->trackTics = 0xBD7DD8;
        gameAddresses->lap = 0xBD7DE8;
        gameAddresses->prisonLap = 0xBD7DE9;
        gameAddresses->totalLaps = 0xBD7748;
        gameAddresses->level = 0x988540;
        gameAddresses->gameState = 0xBD7050;
        gameAddresses->tutorialComplete = 0xBF0A40;
        gameAddresses->inSpecialStage = 0x651ED28;
    }
    else if (gameVersion == 303) { //2.4
        gameAddresses->trackTics = 0x1549BD8;
        gameAddresses->lap = 0x1549BE8;
        gameAddresses->prisonLap = 0x1549BE9;
        gameAddresses->totalLaps = 0x1549484;
        gameAddresses->level = 0xA4BCB8;
        gameAddresses->gameState = 0xE3E2E8;
        gameAddresses->tutorialComplete = 0x0; //Unused
        gameAddresses->inSpecialStage = 0x17486C0;
    }
    //Mac x86
    else if (gameVersion == 401) { //2.0
        gameAddresses->trackTics = 0xCC110C;
        gameAddresses->lap = 0xCC111C;
        gameAddresses->prisonLap = 0xCC111D;
        gameAddresses->totalLaps = 0xCC0A7C;
        gameAddresses->level = 0xA71E60;
        gameAddresses->gameState = 0xCC0350;
        gameAddresses->tutorialComplete = 0xCD9858;
        gameAddresses->inSpecialStage = 0x6605C38;
    }
    else if (gameVersion == 402) { //2.3
        gameAddresses->trackTics = 0xCCD93C;
        gameAddresses->lap = 0xCCD94C;
        gameAddresses->prisonLap = 0xCCD94D;
        gameAddresses->totalLaps = 0xCCD2AC;
        gameAddresses->level = 0xA7DEA0;
        gameAddresses->gameState = 0xCCCB80;
        gameAddresses->tutorialComplete = 0xCE6608;
        gameAddresses->inSpecialStage = 0x66149F8;
    }
    else if (gameVersion == 403) { //2.4
        gameAddresses->trackTics = 0x161E638;
        gameAddresses->lap = 0x161E648;
        gameAddresses->prisonLap = 0x161E649;
        gameAddresses->totalLaps = 0x161E01C;
        gameAddresses->level = 0xB38130;
        gameAddresses->gameState = 0xF284A0;
        gameAddresses->tutorialComplete = 0x0; //Unused
        gameAddresses->inSpecialStage = 0x1812BC0;
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
    gameVersion = 0;
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
        if (moduleMemorySize == 162025472) gameVersion = 1; //2.0
        else if (moduleMemorySize == 162033664) gameVersion = 2; //2.1
        else if (moduleMemorySize == 162512896) gameVersion = 3; //2.2
        else if (moduleMemorySize == 162881536) gameVersion = 4; //2.3
        else if (moduleMemorySize == 31457280) gameVersion = 5; //2.4
        else {
            cleanup();
            return false;
        }
    }
    /*else if (operatingSystem == 2 && operatingSystemArch == 2) {
		//What program to use to scan process memory on Linux ARM?
        if (moduleMemorySize == 8982528) setup(101); //2.3
        else {
            cleanup();
            return false;
        }
    }*/
    else if (operatingSystem == 2 && operatingSystemArch == 1) {
        if (moduleMemorySize == 9293824) gameVersion = 201; //2.3
        else if (moduleMemorySize == 12009472) gameVersion = 202; //2.4
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
        if (moduleMemorySize == 178798592) gameVersion = 301; //2.0
        else if (moduleMemorySize == 178864128) gameVersion = 302; //2.3
        else if (moduleMemorySize == 1) gameVersion = 303; //2.4 TODO
        else {
            cleanup();
            return false;
        }
    }
    else if (operatingSystem == 3 && operatingSystemArch == 1) {
        if (moduleMemorySize == 179740672) gameVersion = 401; //2.0
        else if (moduleMemorySize == 179814400) gameVersion = 402; //2.3
        else if (moduleMemorySize == 31444992) gameVersion = 403; //2.4
        else {
            cleanup();
            return false;
        }
    }
    setup();
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
        if (gameAddresses->tutorialComplete != 0x0) {
            Address tutorialComplete = 0;
            if (!process_read(process, gameModule + gameAddresses->tutorialComplete, &tutorialComplete, sizeof(tutorialComplete))) return;
            if (!process_read(process, tutorialComplete + 0x70C4, &current->tutorialComplete, sizeof(current->tutorialComplete))) return;
        }
        else current->tutorialComplete = 0;
        if (gameAddresses->inSpecialStage != 0x0) {
            if (!process_read(process, gameModule + gameAddresses->inSpecialStage, &current->inSpecialStage, sizeof(current->inSpecialStage))) return;
        }
        else current->inSpecialStage = 0;
    }
    else if (operatingSystem == 2 || operatingSystem == 3) {
        if (gameAddresses->tutorialComplete != 0x0) {
            Address tutorialComplete = 0;
            if (!process_read(process, gameModule + gameAddresses->tutorialComplete, &tutorialComplete, sizeof(tutorialComplete))) return;
            if (!process_read(process, tutorialComplete + 0x70F8, &current->tutorialComplete, sizeof(current->tutorialComplete))) return;
        }
        else current->tutorialComplete = 0;
        if (gameAddresses->inSpecialStage != 0x0) {
            if (!process_read(process, gameModule + gameAddresses->inSpecialStage, &current->inSpecialStage, sizeof(current->inSpecialStage))) return;
        }
        else current->inSpecialStage = 0;
    }
}

void check_start() {
    totalIGT = 0;
    inCredits = false;
    //Start for NG runs
    if (gameVersion == 5 || gameVersion == 202 || gameVersion == 303 || gameVersion == 403) { //2.4 NG run autostart support
        if (autostartAnyPercentEmerald && old->gameState == 10 && old->level == 232 && current->level != 232) timer_start();
    }
    else {
        if (autostartAnyPercentEmerald && old->gameState == 6 && !old->tutorialComplete && current->tutorialComplete) timer_start();
    }
    //Start for Beat The Tutorial
    if (autostartTutorial && old->level != 230 && current->gameState == 1 && current->level == 230) timer_start();
    //Start for All Cups/Ring Cup
    else if (autostartRingCup && old->level != 5 && current->gameState == 1 && current->level == 5) timer_start();
    //Start for Sneaker Cup
    else if (autostartSneakerCup && old->level != 10 && current->gameState == 1 && current->level == 10) timer_start();
    //Start for Spring Cup
    else if (autostartSpringCup && old->level != 15 && current->gameState == 1 && current->level == 15) timer_start();
    //Start for Barrier Cup
    else if (autostartBarrierCup && old->level != 20 && current->gameState == 1 && current->level == 20) timer_start();
    //Start for Invincible Cup
    else if (autostartInvincibleCup && old->level != 25 && current->gameState == 1 && current->level == 25) timer_start();
    //Start for Emerald Cup
    else if (autostartEmeraldCup && old->level != 30 && current->gameState == 1 && current->level == 30) timer_start();
    //Start for Extra Cup
    else if (autostartExtraCup && old->level != 36 && current->gameState == 1 && current->level == 36) timer_start();
    //Start for S.P.B Cup
    else if (autostartSPBCup && old->level != 41 && current->gameState == 1 && current->level == 41) timer_start();
    //Start for Rocket Cup
    else if (autostartRocketCup && old->level != 46 && current->gameState == 1 && current->level == 46) timer_start();
    //Start for Aqua Cup
    else if (autostartAquaCup && old->level != 51 && current->gameState == 1 && current->level == 51) timer_start();
    //Start for Lightning Cup
    else if (autostartLightningCup && old->level != 56 && current->gameState == 1 && current->level == 56) timer_start();
    //Start for Flame Cup
    else if (autostartFlameCup && old->level != 61 && current->gameState == 1 && current->level == 61) timer_start();
    //Start for Super Cup
    else if (autostartSuperCup && old->level != 66 && current->gameState == 1 && current->level == 66) timer_start();
    //Start for Egg Cup
    else if (autostartEggCup && old->level != 71 && current->gameState == 1 && current->level == 71) timer_start();
    //Start for Goggles Cup
    else if (autostartGogglesCup && old->level != 75 && current->gameState == 1 && current->level == 75) timer_start();
    //Start for Timer Cup
    else if (autostartTimerCup && old->level != 80 && current->gameState == 1 && current->level == 80) timer_start();
    //Start for Grow Cup
    else if (autostartGrowCup && old->level != 85 && current->gameState == 1 && current->level == 85) timer_start();
    //Start for Chao Cup
    else if (autostartChaoCup && old->level != 90 && current->gameState == 1 && current->level == 90) timer_start();
    //Start for Wing Cup
    else if (autostartWingCup && old->level != 95 && current->gameState == 1 && current->level == 95) timer_start();
    //Start for Mega Cup
    else if (autostartMegaCup && old->level != 100 && current->gameState == 1 && current->level == 100) timer_start();
    //Start for Phantom Cup
    else if (autostartPhantomCup && old->level != 107 && current->gameState == 1 && current->level == 107) timer_start();
    //Start for Flash Cup
    else if (autostartFlashCup && old->level != 110 && current->gameState == 1 && current->level == 110) timer_start();
    //Start for Swap Cup
    else if (autostartSwapCup && old->level != 115 && current->gameState == 1 && current->level == 115) timer_start();
    //Start for Shrink Cup
    else if (autostartShrinkCup && old->level != 120 && current->gameState == 1 && current->level == 120) timer_start();
    //Start for Bomb Cup
    else if (autostartBombCup && old->level != 125 && current->gameState == 1 && current->level == 125) timer_start();
    //Start for Power Cup
    else if (autostartPowerCup && old->level != 130 && current->gameState == 1 && current->level == 130) timer_start();
    //Start for Genesis Cup
    else if (autostartGenesisCup && old->level != 135 && current->gameState == 1 && current->level == 135) timer_start();
    //Start for Skate Cup
    else if (autostartSkateCup && old->level != 140 && current->gameState == 1 && current->level == 140) timer_start();
    //Start for Recycle Cup A
    else if (autostartRecycleCupA && old->level != 145 && current->gameState == 1 && current->level == 145) timer_start();
    //Start for Recycle Cup B
    else if (autostartRecycleCupB && old->level != 150 && current->gameState == 1 && current->level == 150) timer_start();
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
    if (old->lap > old->totalLaps && current->level != old->level) {
        if (old->level == 221) return splitEndChaosEmeraldRun || splitEndEmerald;
        if (old->level == 228) return splitEndSuperEmeraldRun || splitEndEmerald;
        if (old->level >= 215 && old->level <= 228) return splitEndEmerald;
        return splitEndTrack;
    }
    //Split on Prison stage complete
    if (old->prisonLap == 1 && old->lap == 0 && current->level != old->level) return splitEndPrison;
    //Split on Tutorial stage complete
    if (old->level >= 230 && old->level <= 234 && current->gameState == 1 && current->level != old->level) return true;
    //Split on Springs Tutorial stage to menu
    //This would also split quitting to menu from Springs
    if (old->level == 234 && old->gameState == 1 && current->gameState == 6) return true;
    if (!inCredits && old->gameState != 7 && current->gameState == 7) {
        //Avoids multiple splits at Credits due to demo footage changing gameState
        inCredits = true;
        return splitCredits;
    }
    return false;
}

void set_settings() {
    if (!settingsInitialized) {
        autostartAnyPercentEmerald = user_settings_add_bool_helper("autostart_any_percent_emerald", "Auto start: Any%/All Emeralds", true);
        autostartTutorial = user_settings_add_bool_helper("autostart_tutorial", "Auto start: Beat the Tutorial", false);
        splitEndTrack = user_settings_add_bool_helper("split_end_track", "Split: On completed tracks", true);
        splitEndPrison = user_settings_add_bool_helper("split_end_prison", "Split: On completed Prisons", false);
        splitEndEmerald = user_settings_add_bool_helper("split_end_emerald", "Split: On completed Sealed Stars", true);
        splitCredits = user_settings_add_bool_helper("split_credits", "Split: Credits/End of Any%", true);
        splitEndChaosEmeraldRun = user_settings_add_bool_helper("split_end_chaos_emerald_run", "Split: End of All Chaos Emeralds", true);
        splitEndSuperEmeraldRun = user_settings_add_bool_helper("split_end_super_emerald_run", "Split: End of All Super Emeralds", true);
        autostartRingCup = user_settings_add_bool_helper("autostart_ring_cup", "Auto start: Ring Cup", true);
        autostartSneakerCup = user_settings_add_bool_helper("autostart_sneaker_cup", "Auto start: Sneaker Cup", false);
        autostartSpringCup = user_settings_add_bool_helper("autostart_spring_cup", "Auto start: Spring Cup", false);
        autostartBarrierCup = user_settings_add_bool_helper("autostart_barrier_cup", "Auto start: Barrier Cup", false);
        autostartInvincibleCup = user_settings_add_bool_helper("autostart_invincible_cup", "Auto start: Invincible Cup", false);
        autostartEmeraldCup = user_settings_add_bool_helper("autostart_emerald_cup", "Auto start: Emerald Cup", false);
        autostartExtraCup = user_settings_add_bool_helper("autostart_extra_cup", "Auto start: Extra Cup", false);
        autostartSPBCup = user_settings_add_bool_helper("autostart_spb_cup", "Auto start: S.P.B Cup", false);
        autostartRocketCup = user_settings_add_bool_helper("autostart_rocket_cup", "Auto start: Rocket Cup", false);
        autostartAquaCup = user_settings_add_bool_helper("autostart_aqua_cup", "Auto start: Aqua Cup", false);
        autostartLightningCup = user_settings_add_bool_helper("autostart_lightning_cup", "Auto start: Lightning Cup", false);
        autostartFlameCup = user_settings_add_bool_helper("autostart_flame_cup", "Auto start: Flame Cup", false);
        autostartSuperCup = user_settings_add_bool_helper("autostart_super_cup", "Auto start: Super Cup", false);
        autostartEggCup = user_settings_add_bool_helper("autostart_egg_cup", "Auto start: Egg Cup", false);
        autostartGogglesCup = user_settings_add_bool_helper("autostart_goggles_cup", "Auto start: Goggles Cup", false);
        autostartTimerCup = user_settings_add_bool_helper("autostart_timer_cup", "Auto start: Timer Cup", false);
        autostartGrowCup = user_settings_add_bool_helper("autostart_grow_cup", "Auto start: Grow Cup", false);
        autostartChaoCup = user_settings_add_bool_helper("autostart_chao_cup", "Auto start: Chao Cup", false);
        autostartWingCup = user_settings_add_bool_helper("autostart_wing_cup", "Auto start: Wing Cup", false);
        autostartMegaCup = user_settings_add_bool_helper("autostart_mega_cup", "Auto start: Mega Cup", false);
        autostartPhantomCup = user_settings_add_bool_helper("autostart_phantom_cup", "Auto start: Phantom Cup", false);
        autostartFlashCup = user_settings_add_bool_helper("autostart_flash_cup", "Auto start: Flash Cup", false);
        autostartSwapCup = user_settings_add_bool_helper("autostart_swap_cup", "Auto start: Swap Cup", false);
        autostartShrinkCup = user_settings_add_bool_helper("autostart_shrink_cup", "Auto start: Shrink Cup", false);
        autostartBombCup = user_settings_add_bool_helper("autostart_bomb_cup", "Auto start: Bomb Cup", false);
        autostartPowerCup = user_settings_add_bool_helper("autostart_power_cup", "Auto start: Power Cup", false);
        autostartGenesisCup = user_settings_add_bool_helper("autostart_genesis_cup", "Auto start: Genesis Cup", false);
        autostartSkateCup = user_settings_add_bool_helper("autostart_skate_cup", "Auto start: Skate Cup", false);
        autostartRecycleCupA = user_settings_add_bool_helper("autostart_recycle_cup_a", "Auto start: Recycle Cup A", false);
        autostartRecycleCupB = user_settings_add_bool_helper("autostart_recycle_cup_b", "Auto start: Recycle Cup B", false);
        settingsInitialized = true;
        return;
    }
    uint64_t settingsMap = settings_map_load();
    uint64_t mapLength = settings_map_len(settingsMap);
    if (mapLength == 0) return;
    uint64_t autostartAnyPercentEmeraldSettingValue = settings_map_get_helper(settingsMap, "autostart_any_percent_emerald");
    uint64_t autostartTutorialSettingValue = settings_map_get_helper(settingsMap, "autostart_tutorial");
    uint64_t splitEndTrackSettingValue = settings_map_get_helper(settingsMap, "split_end_track");
    uint64_t splitEndPrisonSettingValue = settings_map_get_helper(settingsMap, "split_end_prison");
    uint64_t splitEndEmeraldSettingValue = settings_map_get_helper(settingsMap, "split_end_emerald");
    uint64_t creditsSettingValue = settings_map_get_helper(settingsMap, "split_credits");
    uint64_t splitEndChaosEmeraldRunSettingValue = settings_map_get_helper(settingsMap, "split_end_chaos_emerald_run");
    uint64_t splitEndSuperEmeraldRunSettingValue = settings_map_get_helper(settingsMap, "split_end_super_emerald_run");
    uint64_t ringCupSettingValue = settings_map_get_helper(settingsMap, "autostart_ring_cup");
    uint64_t sneakerCupSettingValue = settings_map_get_helper(settingsMap, "autostart_sneaker_cup");
    uint64_t springCupSettingValue = settings_map_get_helper(settingsMap, "autostart_spring_cup");
    uint64_t barrierCupSettingValue = settings_map_get_helper(settingsMap, "autostart_barrier_cup");
    uint64_t invincibleCupSettingValue = settings_map_get_helper(settingsMap, "autostart_invincible_cup");
    uint64_t emeraldCupSettingValue = settings_map_get_helper(settingsMap, "autostart_emerald_cup");
    uint64_t extraCupSettingValue = settings_map_get_helper(settingsMap, "autostart_extra_cup");
    uint64_t spbCupSettingValue = settings_map_get_helper(settingsMap, "autostart_spb_cup");
    uint64_t rocketCupSettingValue = settings_map_get_helper(settingsMap, "autostart_rocket_cup");
    uint64_t aquaCupSettingValue = settings_map_get_helper(settingsMap, "autostart_aqua_cup");
    uint64_t lightningCupSettingValue = settings_map_get_helper(settingsMap, "autostart_lightning_cup");
    uint64_t flameCupSettingValue = settings_map_get_helper(settingsMap, "autostart_flame_cup");
    uint64_t superCupSettingValue = settings_map_get_helper(settingsMap, "autostart_super_cup");
    uint64_t eggCupSettingValue = settings_map_get_helper(settingsMap, "autostart_egg_cup");
    uint64_t gogglesCupSettingValue = settings_map_get_helper(settingsMap, "autostart_goggles_cup");
    uint64_t timerCupSettingValue = settings_map_get_helper(settingsMap, "autostart_timer_cup");
    uint64_t growCupSettingValue = settings_map_get_helper(settingsMap, "autostart_grow_cup");
    uint64_t chaoCupSettingValue = settings_map_get_helper(settingsMap, "autostart_chao_cup");
    uint64_t wingCupSettingValue = settings_map_get_helper(settingsMap, "autostart_wing_cup");
    uint64_t megaCupSettingValue = settings_map_get_helper(settingsMap, "autostart_mega_cup");
    uint64_t phantomCupSettingValue = settings_map_get_helper(settingsMap, "autostart_phantom_cup");
    uint64_t flashCupSettingValue = settings_map_get_helper(settingsMap, "autostart_flash_cup");
    uint64_t swapCupSettingValue = settings_map_get_helper(settingsMap, "autostart_swap_cup");
    uint64_t shrinkCupSettingValue = settings_map_get_helper(settingsMap, "autostart_shrink_cup");
    uint64_t bombCupSettingValue = settings_map_get_helper(settingsMap, "autostart_bomb_cup");
    uint64_t powerCupSettingValue = settings_map_get_helper(settingsMap, "autostart_power_cup");
    uint64_t genesisCupSettingValue = settings_map_get_helper(settingsMap, "autostart_genesis_cup");
    uint64_t skateCupSettingValue = settings_map_get_helper(settingsMap, "autostart_skate_cup");
    uint64_t recycleCupASettingValue = settings_map_get_helper(settingsMap, "autostart_recycle_cup_a");
    uint64_t recycleCupBSettingValue = settings_map_get_helper(settingsMap, "autostart_recycle_cup_b");
    if (autostartAnyPercentEmeraldSettingValue != 0) {
        setting_value_get_bool(autostartAnyPercentEmeraldSettingValue, &autostartAnyPercentEmerald);
        setting_value_free(autostartAnyPercentEmeraldSettingValue);
    }
    if (autostartTutorialSettingValue != 0) {
        setting_value_get_bool(autostartTutorialSettingValue, &autostartTutorial);
        setting_value_free(autostartTutorialSettingValue);
    }
    if (splitEndTrackSettingValue != 0) {
        setting_value_get_bool(splitEndTrackSettingValue, &splitEndTrack);
        setting_value_free(splitEndTrackSettingValue);
    }
    if (splitEndPrisonSettingValue != 0) {
        setting_value_get_bool(splitEndPrisonSettingValue, &splitEndPrison);
        setting_value_free(splitEndPrisonSettingValue);
    }
    if (splitEndEmeraldSettingValue != 0) {
        setting_value_get_bool(splitEndEmeraldSettingValue, &splitEndEmerald);
        setting_value_free(splitEndEmeraldSettingValue);
    }
    if (creditsSettingValue != 0) {
        setting_value_get_bool(creditsSettingValue, &splitCredits);
        setting_value_free(creditsSettingValue);
    }
    if (splitEndChaosEmeraldRunSettingValue != 0) {
        setting_value_get_bool(splitEndChaosEmeraldRunSettingValue, &splitEndChaosEmeraldRun);
        setting_value_free(splitEndChaosEmeraldRunSettingValue);
    }
    if (splitEndSuperEmeraldRunSettingValue != 0) {
        setting_value_get_bool(splitEndSuperEmeraldRunSettingValue, &splitEndSuperEmeraldRun);
        setting_value_free(splitEndSuperEmeraldRunSettingValue);
    }
    if (ringCupSettingValue != 0) {
        setting_value_get_bool(ringCupSettingValue, &autostartRingCup);
        setting_value_free(ringCupSettingValue);
    }
    if (sneakerCupSettingValue != 0) {
        setting_value_get_bool(sneakerCupSettingValue, &autostartSneakerCup);
        setting_value_free(sneakerCupSettingValue);
    }
    if (springCupSettingValue != 0) {
        setting_value_get_bool(springCupSettingValue, &autostartSpringCup);
        setting_value_free(springCupSettingValue);
    }
    if (barrierCupSettingValue != 0) {
        setting_value_get_bool(barrierCupSettingValue, &autostartBarrierCup);
        setting_value_free(barrierCupSettingValue);
    }
    if (invincibleCupSettingValue != 0) {
        setting_value_get_bool(invincibleCupSettingValue, &autostartInvincibleCup);
        setting_value_free(invincibleCupSettingValue);
    }
    if (emeraldCupSettingValue != 0) {
        setting_value_get_bool(emeraldCupSettingValue, &autostartEmeraldCup);
        setting_value_free(emeraldCupSettingValue);
    }
    if (extraCupSettingValue != 0) {
        setting_value_get_bool(extraCupSettingValue, &autostartExtraCup);
        setting_value_free(extraCupSettingValue);
    }
    if (spbCupSettingValue != 0) {
        setting_value_get_bool(spbCupSettingValue, &autostartSPBCup);
        setting_value_free(spbCupSettingValue);
    }
    if (rocketCupSettingValue != 0) {
        setting_value_get_bool(rocketCupSettingValue, &autostartRocketCup);
        setting_value_free(rocketCupSettingValue);
    }
    if (aquaCupSettingValue != 0) {
        setting_value_get_bool(aquaCupSettingValue, &autostartAquaCup);
        setting_value_free(aquaCupSettingValue);
    }
    if (lightningCupSettingValue != 0) {
        setting_value_get_bool(lightningCupSettingValue, &autostartLightningCup);
        setting_value_free(lightningCupSettingValue);
    }
    if (flameCupSettingValue != 0) {
        setting_value_get_bool(flameCupSettingValue, &autostartFlameCup);
        setting_value_free(flameCupSettingValue);
    }
    if (superCupSettingValue != 0) {
        setting_value_get_bool(superCupSettingValue, &autostartSuperCup);
        setting_value_free(superCupSettingValue);
    }
    if (eggCupSettingValue != 0) {
        setting_value_get_bool(eggCupSettingValue, &autostartEggCup);
        setting_value_free(eggCupSettingValue);
    }
    if (gogglesCupSettingValue != 0) {
        setting_value_get_bool(gogglesCupSettingValue, &autostartGogglesCup);
        setting_value_free(gogglesCupSettingValue);
    }
    if (timerCupSettingValue != 0) {
        setting_value_get_bool(timerCupSettingValue, &autostartTimerCup);
        setting_value_free(timerCupSettingValue);
    }
    if (growCupSettingValue != 0) {
        setting_value_get_bool(growCupSettingValue, &autostartGrowCup);
        setting_value_free(growCupSettingValue);
    }
    if (chaoCupSettingValue != 0) {
        setting_value_get_bool(chaoCupSettingValue, &autostartChaoCup);
        setting_value_free(chaoCupSettingValue);
    }
    if (wingCupSettingValue != 0) {
        setting_value_get_bool(wingCupSettingValue, &autostartWingCup);
        setting_value_free(wingCupSettingValue);
    }
    if (megaCupSettingValue != 0) {
        setting_value_get_bool(megaCupSettingValue, &autostartMegaCup);
        setting_value_free(megaCupSettingValue);
    }
    if (phantomCupSettingValue != 0) {
        setting_value_get_bool(phantomCupSettingValue, &autostartPhantomCup);
        setting_value_free(phantomCupSettingValue);
    }
    if (flashCupSettingValue != 0) {
        setting_value_get_bool(flashCupSettingValue, &autostartFlashCup);
        setting_value_free(flashCupSettingValue);
    }
    if (swapCupSettingValue != 0) {
        setting_value_get_bool(swapCupSettingValue, &autostartSwapCup);
        setting_value_free(swapCupSettingValue);
    }
    if (shrinkCupSettingValue != 0) {
        setting_value_get_bool(shrinkCupSettingValue, &autostartShrinkCup);
        setting_value_free(shrinkCupSettingValue);
    }
    if (bombCupSettingValue != 0) {
        setting_value_get_bool(bombCupSettingValue, &autostartBombCup);
        setting_value_free(bombCupSettingValue);
    }
    if (powerCupSettingValue != 0) {
        setting_value_get_bool(powerCupSettingValue, &autostartPowerCup);
        setting_value_free(powerCupSettingValue);
    }
    if (genesisCupSettingValue != 0) {
        setting_value_get_bool(genesisCupSettingValue, &autostartGenesisCup);
        setting_value_free(genesisCupSettingValue);
    }
    if (skateCupSettingValue != 0) {
        setting_value_get_bool(skateCupSettingValue, &autostartSkateCup);
        setting_value_free(skateCupSettingValue);
    }
    if (recycleCupASettingValue != 0) {
        setting_value_get_bool(recycleCupASettingValue, &autostartRecycleCupA);
        setting_value_free(recycleCupASettingValue);
    }
    if (recycleCupBSettingValue != 0) {
        setting_value_get_bool(recycleCupBSettingValue, &autostartRecycleCupB);
        setting_value_free(recycleCupBSettingValue);
    }
    settings_map_free(settingsMap);
}

__attribute__((export_name("update"))) void update(void) {
    set_settings();
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
            timer_set_game_time(totalIGT / 35, ((uint32_t)((totalIGT % 35) * (1000.00f / 35))) * 1000000);
            if (check_split()) timer_split();
            break;
        }
        default: {}
    }
}