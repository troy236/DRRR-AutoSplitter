state("ringracers", "2.0")
{
	uint trackTics : 0x1088434;
	byte lap : 0x1088444;
	byte prisonLap : 0x1088445;
	byte totalLaps : 0x1087795;
	ushort level : 0xADDFF4;
	int gameState : 0x108F344;
	bool tutorialComplete : 0x1103DE0, 0x70C4;
	bool inSpecialStage : 0x5C69BA0
}
state("ringracers", "2.1")
{
	uint trackTics : 0x108A434;
	byte lap : 0x108A444;
	byte prisonLap : 0x108A445;
	byte totalLaps : 0x1089795;
	ushort level : 0xADEFF4;
	int gameState : 0x1091344;
	bool tutorialComplete : 0x1105DE0, 0x70C4;
	bool inSpecialStage : 0x5C6BBA0
}
state("ringracers", "2.2")
{
	uint trackTics : 0x10FA47C;
	byte lap : 0x10FA48C;
	byte prisonLap : 0x10FA48D;
	byte totalLaps : 0x10F97D5;
	ushort level : 0xB4CFF4;
	int gameState : 0x1101484;
	bool tutorialComplete : 0x1176100, 0x70C4;
	bool inSpecialStage : 0x5CDDEE0
}
state("ringracers", "2.3")
{
	uint trackTics : 0x115647C;
	byte lap : 0x115648C;
	byte prisonLap : 0x115648D;
	byte totalLaps : 0x11557D5;
	ushort level : 0xBA7FF4;
	int gameState : 0x115D4C4;
	bool tutorialComplete : 0x11D2140, 0x70C4;
	bool inSpecialStage : 0x5D39F20
}

init
{
	var moduleMemorySize = modules.First().ModuleMemorySize;
	if (moduleMemorySize == 162025472) version = "2.0";
	if (moduleMemorySize == 162033664) version = "2.1";
	if (moduleMemorySize == 162512896) version = "2.2";
	if (moduleMemorySize == 162881536) version = "2.3";

	refreshRate = 35;
}

startup
{
	vars.totalIGT = 0u;
	vars.inCredits = false;
}

start
{
	vars.totalIGT = 0u;
	vars.inCredits = false;
	//Start for NG runs
	if (old.gameState == 6 && !old.tutorialComplete && current.tutorialComplete) return true;
	//Start for Beat The Tutorial
	if (old.level != 230 && current.gameState == 1 && current.level == 230) return true;
	//Start for All Cups
	if (old.level != 5 && current.gameState == 1 && current.level == 5) return true;
}

update
{
	if (vars.inCredits && current.gameState == 6)
	{
		//We are now back at the menu
		vars.inCredits = false;
		return;
	}
	if (vars.inCredits) return;
	if (current.gameState == 9 || current.gameState == 0 || current.gameState == 7) return;
	if (current.trackTics > old.trackTics)
	{
		vars.totalIGT += current.trackTics - old.trackTics;
	}
}

split
{
	//Special stages needs better handling. 
	//Outrunning the Emerald and finishing causes a split
	//Occasionally completing a Special Stage successfully does not split. Invisible 'lap' checkpoints can be missed
	//Cannot find a decent Emerald collected boolean
	
	//Split on Race/Special stage complete
	if (old.lap > old.totalLaps && current.level != old.level) return true;
	//Split on Prison stage complete
	if (old.prisonLap == 1 && old.lap == 0 && current.level != old.level) return true;
	//Split on Tutorial stage complete
	if (old.level >= 230 && old.level <= 234 && current.gameState == 1 && current.level != old.level) return true;
	//Split on Springs Tutorial stage to menu
	//This would also split quitting to menu from Springs
	if (old.level == 234 && old.gameState == 1 && current.gameState == 6) return true;
	if (!vars.inCredits && old.gameState != 7 && current.gameState == 7)
	{
		//Avoids multiple splits at Credits due to demo footage changing gameState
		vars.inCredits = true;
		return true;
	}
}

gameTime
{
	return TimeSpan.FromMilliseconds(vars.totalIGT*28.5714285714);
}

isLoading
{
	return true;
}
