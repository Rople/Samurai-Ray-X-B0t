#include "main.hpp"


IW5Console::ConsoleUIColor IW5Console::SettingColor;
IW5Console::ConsoleUISetting IW5Console::SettingUI;


char* IW5Console::ConsolePrefix;

int(__cdecl* IW5Console::ProccessRconCommand)(int local, char* cmd);
int(__cdecl* IW5Console::Commands::Reliable::CBuff_addtext)(int local, char* cmd);

char(__cdecl* IW5Console::PrintOnConsole)(int local, int __unk0, char* buff, int time, int bold, char __unk1, int __unk2);


void(__cdecl*IW5Engine::WinApi::EngineEnterCriticalSection)(int x);
void(__cdecl*IW5Engine::WinApi::EngineLeaveCriticalSection)(int x);

void IW5Console::SetConsolePrefix(char* prefix)
{
	DWORD oldprotect = 0x0;


	VirtualProtect((void*)ConsolePrefix, 0x4, PAGE_EXECUTE_READWRITE, &oldprotect); // fk default memory protection, fast solution k...

	if (!prefix)
		*ConsolePrefix = 0x0;
	else
		strcpy_s(ConsolePrefix, 0x4, prefix);

	VirtualProtect((void*)ConsolePrefix, 0x4, oldprotect, NULL);
}
/*
void Color_t::SetRGBA(int R, int G, int B, int A)
{
		DWORD oldprotect = 0x0;

		VirtualProtect(this, 0x10, PAGE_EXECUTE_READWRITE, &oldprotect); // fk default memory protection, fast solution k...

		this->R =  ((float)R / 255.0f); // 1.0f max to 255 -> so le formule math:  f(target) = 1.0f * target / 255, i've done this formule myself, idk if it's fine... i'm so lazy k...
		this->G =  ((float)G / 255.0f);
		this->B =  ((float)B / 255.0f);
		this->A =  ((float)A / 255.0f);

		VirtualProtect(this, 0x10, oldprotect, NULL);
}
*/


Color_t::Color_t(int R, int G, int B, int A)
{
	DWORD oldprotect = 0x0;

	VirtualProtect(this, 0x10, PAGE_EXECUTE_READWRITE, &oldprotect); // fk default memory protection, fast solution k...

	this->R = ((float)R / 255.0f); // 1.0f max to 255 -> so le formule math:  f(target) = 1.0f * target / 255, i've done this pFormula myself, idk if it's fine... i'm so lazy k...
	this->G = ((float)G / 255.0f);
	this->B = ((float)B / 255.0f);
	this->A = ((float)A / 255.0f);

	VirtualProtect(this, 0x10, oldprotect, NULL);
}


Color_t::Color_t()
{

}



void __cdecl IW5Console::ToggleConsole()
{
	*SettingUI.TurnConsoleMax ^= TOGGLE_IW5CONSOLE, *SettingUI.TurnConsoleMin ^= TOGGLE_IW5CONSOLE;
}

void IW5Console::DebugOnConsole(int buffsz, const char* format, ...)
{
	char* buff = new char[buffsz];

	va_list args;
	va_start(args, format);
	vsnprintf_s(buff, buffsz, buffsz, format, args);
	va_end(args);

	IW5Engine::WinApi::EngineEnterCriticalSection(0);
	PrintOnConsole(0, 0, buff, 0, 0, 55, 0);
	IW5Engine::WinApi::EngineLeaveCriticalSection(0);
	delete[] buff;
}


void __cdecl IW5Console::Commands::Reliable::ProcessReliableCommand(char* format)
{

	return;
}

// changed from defaul engine IW5Console to samurai ray x 's console...

int __cdecl IW5Console::OnRconCommand(int local, char* cmd)
{
	

	
	SamurayX::SettingControls::KeyHandler::ProcessRegisterKeyForCommand(cmd);
	IW5Console::Commands::Dvar::ProcessDvar(cmd);	 
	IW5Console::Commands::Default::ProcessCommand(cmd);
	IW5Console::Commands::Reliable::Execute(cmd);

	 //IW5Console::Commands::Reliable::CBuff_addtext(0, cmd);

	// test dvar
	/*
	if (!strcmp(cmd, "logtags"))
	{
		
		static char* PointEntity[20] =
		{
			"j_head", "j_neck", "j_helmet",
			"j_shoulder_le", "j_shoulder_ri", "j_elbow_le", "j_elbow_ri", "j_wrist_le", "j_wrist_ri", "j_gun",
			"j_hip_ri", "j_hip_le", "j_knee_le", "j_knee_ri", "j_ankle_ri", "j_ankle_le",
			"j_mainroot", "j_spineupper", "j_spinelower", "j_spine4"
		};

		for (int i = 0; i < 20; i++)
			DebugOnConsole(256, "^5 %d : %s \n", ((int(__cdecl*)(char*))0x4922E0)(PointEntity[i]), PointEntity[i]);
			

		//DebugOnConsole(256, "^5 %d \n", ((int(__cdecl*)(char*))0x4922E0)("real_deal0044")); // i thought that it returns 0 on no tag found... whatever fuck this...

	}
	*/

	if (!strcmp(cmd, "logtest0"))
	{
		IW5Console::DebugOnConsole(1024, "^5Alive: %d | Origin: %f | %f | %f | State: %d \n", GetEntity(GetCurrentClientNumber()).Alive, GetEntity(GetCurrentClientNumber()).Origin[0], GetEntity(GetCurrentClientNumber()).Origin[1], GetEntity(GetCurrentClientNumber()).Origin[2], GetEntity(GetCurrentClientNumber()).Status);
		IW5Console::DebugOnConsole(1024, "^5NewOrigin:  %f | %f | %f \n^5OldOrigin:  %f | %f | %f \n", GetEntity(GetCurrentClientNumber()).nextState.pos.trBase[0], GetEntity(GetCurrentClientNumber()).nextState.pos.trBase[1], GetEntity(GetCurrentClientNumber()).nextState.pos.trBase[2], GetEntity(GetCurrentClientNumber()).currentState.pos.trBase[0], GetEntity(GetCurrentClientNumber()).currentState.pos.trBase[1], GetEntity(GetCurrentClientNumber()).currentState.pos.trBase[2]);
		IW5Console::DebugOnConsole(1024, "^5OView: %f | %f | %f \n", GetEntity(GetCurrentClientNumber()).currentState.apos.trBase[0], GetEntity(GetCurrentClientNumber()).currentState.apos.trBase[1], GetEntity(GetCurrentClientNumber()).currentState.apos.trBase[2]);
		IW5Console::DebugOnConsole(1024, "^5CView: %f | %f | %f \n", GetEntity(GetCurrentClientNumber()).nextState.apos.trBase[0], GetEntity(GetCurrentClientNumber()).nextState.apos.trBase[1], GetEntity(GetCurrentClientNumber()).nextState.apos.trBase[2]);
		IW5Console::DebugOnConsole(1024, "^5CTrType: %d | OTrType: %d", GetEntity(GetCurrentClientNumber()).currentState.pos.trType, GetEntity(GetCurrentClientNumber()).nextState.pos.trType);

		return 0;
	}
	

	if (!strcmp(cmd, "logtest1"))
	{
		IW5Console::DebugOnConsole(1024, "^5CSnapServerTime: %d | NSnapServerTime: %d \n", CG.snap->ServerTime, CG.nextSnap->ServerTime);
		IW5Console::DebugOnConsole(1024, "^5CPing: %d | NPing: %d \n", CG.snap->ping, CG.nextSnap->ping);
		IW5Console::DebugOnConsole(1024, "^5CEntities: %d | NEntities: %d \n", CG.snap->EntitiesSpawnedNumber, CG.nextSnap->EntitiesSpawnedNumber);
		IW5Console::DebugOnConsole(1024, "^5CPlayers: %d | NPlayers: %d \n", CG.snap->PlayersOnlineNumber, CG.nextSnap->PlayersOnlineNumber);
		IW5Console::DebugOnConsole(1024, "^5CPtr: %X | NPtr: %X \n", CG.snap, CG.nextSnap);
		IW5Console::DebugOnConsole(1024, "^5_CPtr: %X | _NPtr: %X \n", &CG.snap, &CG.nextSnap);
		IW5Console::DebugOnConsole(1024, "^5FrameIntepolation: %f | Ptr: %X \n ", CG.FrameInterpolation, &CG.FrameInterpolation);
		IW5Console::DebugOnConsole(1024, "^5CWeapon: %d | CRealWeapon: %d \n", CG.snap->ps.CurrentWeapon, GetCurrentWeapon());
		IW5Console::DebugOnConsole(1024, "^5CViewOriginAngle: %f | NViewOriginAngle: %f \n", CG.snap->ps.ViewOriginAngle, CG.nextSnap->ps.ViewOriginAngle);
		IW5Console::DebugOnConsole(1024, "^5ActiveSnap1: %X | ActiveSnap2: %X \n", &CG.activeSnapshots[0], &CG.activeSnapshots[1]);
		return 0;
	}

	if (!strcmp(cmd, "logweaponbullettype"))
	{
		IW5Console::DebugOnConsole(1024, "^5 Weapon bullet type:  %d", GetWeaponBulletType(GetCurrentWeapon()));

		return 0;
	}

	if (!strcmp(cmd, "ptr"))
	{
		IW5Console::DebugOnConsole(1024, "^5 %X", &IW5Engine::CEngine::CMath::BG_EvaluateTrajectory);
		return 0;
	}

	if (!strcmp(cmd, "logctrajectory"))
	{

		vec3_t Result = { 0.0f, 0.0f, 0.0f};
		float trajectoryscale = IW5Engine::CEngine::CMath::BG_EvaluateTrajectory(&GetEntity(GetCurrentClientNumber()).currentState.pos, CG.snap->ServerTime, Result, 4.0f);

		IW5Console::DebugOnConsole(1024, "^5Result: %f | TrType: %d", trajectoryscale, GetEntity(GetCurrentClientNumber()).currentState.pos.trType);

		return 0;
	}

	return 0;

	if(!local)
		return IW5Console::ProccessRconCommand(local, cmd);

	return 0;
}




void IW5Engine::InitEngine()
{
	

	while (1)
	{
		SamurayX::SettingControls::KeyHandler::KeyEventReader();
		SamurayX::WorldRead::InitWorldRead();
		// beta test
		//if (GetAsyncKeyState(VK_F1) & 0x1)
		//	SamurayX::MenuControl::OpenMenu = !SamurayX::MenuControl::OpenMenu;

		//if (GetAsyncKeyState(VK_F2) & 0x1)
		//	SamurayX::ConsoleControl::OpenConsole = !SamurayX::ConsoleControl::OpenConsole;

		Sleep(1);
	}


	return;
}


void __cdecl IW5Console::Commands::Default::OnCommandEvent(const char* command, void* value)
{
	if (XC_Commands_Type::GetCommandMode(command) == XC_MODE_FUNCTION_T)
		return;

	if (XC_Commands_Type::GetCommandDataType(command) == XC_Commands_Type::XC_INT_T)
		IW5Console::DebugOnConsole((strlen(command) + 70), "^5[^;SamurayX^5] ^6%s^5 changed to ^6%d^5 ^2successfully^5!\n", command, value);
	else if (XC_Commands_Type::GetCommandDataType(command) == XC_Commands_Type::XC_FLOAT_T)
		IW5Console::DebugOnConsole((strlen(command) + 70), "^5[^;SamurayX^5] ^6%s^5 changed to ^6%f^5 ^2successfully^5!\n", command, *(float*)&value);
	else if (XC_Commands_Type::GetCommandDataType(command) == XC_Commands_Type::XC_CHARPTR_T)
		IW5Console::DebugOnConsole((strlen(command) + strlen((char*)value) + 70), "^5[^;SamurayX^5] ^6%s^5 changed to ^6%s^5 ^2successfully^5!\n", command, value);
}


/*
void Tmp(char* p)
{
	//static int t = 0;
	//char buff[256];
	//sprintf_s(buff, sizeof(buff), "%s: %d!\nTestB\nTestC\nTestD\n", p, t);
	SamurayX::Console.AddTextToBuff("Test!");
	//t++;
}
*/

void IW5Console::Commands::Default::InitRegisterDefaultCommands()
{
	// init register defualt commands
	XC_Commands_Handler::XC_Commands_Event_Handler::OnCommandEvent = &IW5Console::Commands::Default::OnCommandEvent;
	//XC_Commands_t<int>::RegisterNewCommand("toggleconsole", 0, &IW5Console::ToggleConsole, XC_MODE_FUNCTION_T, false);
	//SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f1 toggleconsole");
	XC_Commands_t<int>::RegisterNewCommand("toggleconsole", 0, &SamurayX::ConsoleControl::ToggleConsole, XC_MODE_FUNCTION_T, false);
	XC_Commands_t<int>::RegisterNewCommand("togglemenu", 0, &SamurayX::MenuControl::ToggleMenu, XC_MODE_FUNCTION_T, false);
	XC_Commands_t<int>::RegisterNewCommand("clear", 0, &SamurayX::ConsoleControl::ClearConsole, XC_MODE_FUNCTION_T, false);

	
	//XC_Commands_t<char*>::RegisterNewCommand("sendtext", "0", &Tmp, XC_MODE_FUNCTION_PARAM_T, false);

	XC_Commands_t<char*>::RegisterNewCommand("name", "`  ", (void*)OFFSET_NAME, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("name", XC_RESTRICTION_SIZE, 3, 15);
	XC_Commands_t<char*>::RegisterNewCommand("clantag", "^6H?", (void*)OFFSET_CLANTAG, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("clantag", XC_RESTRICTION_SIZE, 0, 4);
	XC_Commands_t<char*>::RegisterNewCommand("emblemtitle", "^510101001", (void*)OFFSET_EMBLEM_TITLE, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("emblemtitle", XC_RESTRICTION_SIZE, 0, 20);
	XC_Commands_t<int>::RegisterNewCommand("autoshoot", 0, &Settings::CBot::AutoShoot, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("autoshoot", XC_RESTRICTION_SIZE, 0, 2);
	XC_Commands_t<float>::RegisterNewCommand("vieworiginscalexy", 0.0f, &IW5Engine::CEngine::ViewOriginScaleXY, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("vieworiginscalez", 0.0f, &IW5Engine::CEngine::ViewOriginScaleZ, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("targetmaxdistance", 0.0f, &Settings::CBot::TargetMaxDistance, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::RegisterNewCommand("bot", 0, &Settings::CBot::Bot, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("bot", XC_RESTRICTION_SIZE, 0, 3);
	XC_Commands_t<int>::RegisterNewCommand("maxbones", 0, &Settings::CBot::MaxBones, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("maxbones", XC_RESTRICTION_SIZE, 0, 19);
	// int bot config commands
	XC_Commands_t<int>::RegisterNewCommand("freeze", 0, (void*)OFFSET_FREEZE, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("freeze", XC_RESTRICTION_SIZE, 0, 1);
	XC_Commands_t<int>::RegisterNewCommand("unlockclasses", 0, (void*)OFFSET_UNLOCK_CLASSES, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("unlockclasses", XC_RESTRICTION_SIZE, 0, 1);

	XC_Commands_t<int>::RegisterNewCommand("predictmode", 0, &Settings::Prediction::PredictionMode, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::SetCommandRestriction("predictmode", XC_RESTRICTION_SIZE, 0, 2);
	XC_Commands_t<float>::RegisterNewCommand("ptimescale", 0.0f, &Settings::Prediction::PredictionByTime::TimeScale, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("psidemovescale", 0.0f, &Settings::Prediction::PredictionBySideMove::SideMoveScale, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("pvelocityscale", 0.0f, &Settings::Prediction::PredictionByVelocity::VelocityScale, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::RegisterNewCommand("antiaimmode", 0, &Settings::AntiAim::AntiAimMode, XC_MODE_VALUE_T, false);
	XC_Commands_t<int>::RegisterNewCommand("espweapon", 1, &Settings::CESP::WeaponESP::Toggle, XC_MODE_FUNCTION_PARAM_T, false);
	XC_Commands_t<int>::RegisterNewCommand("espenemy", 1, &Settings::CESP::EnemyESP::Toggle, XC_MODE_FUNCTION_PARAM_T, false);
	XC_Commands_t<int>::RegisterNewCommand("itime", 0, &Settings::Prediction::Interpolation::InterpolationTime, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("iscale", 0, &Settings::Prediction::Interpolation::InterpolationScale, XC_MODE_VALUE_T, false);
	XC_Commands_t<float>::RegisterNewCommand("lscale", 0.0f, &Settings::Prediction::LocalPrediction::OriginScaleXYZ, XC_MODE_VALUE_T);
	
	// keys
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f1 toggleconsole 0");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f1 toggleconsole 1");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f2 togglemenu 0");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f2 togglemenu 1");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f3 unlockclasses");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f4 freeze 1");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f4 freeze 0");
	SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f5 $disconnect");

	//SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand("register f2 sendtext Test!");
	//IW5Console::Commands::Dvar::ProcessDvar("cl_noprint 1");


	return;
}

int IW5Console::Commands::Default::ProcessCommand(char* command)
{
	// init
	IntellCommandAnalyzer::SafeExecuteCommand(command);
	


	return 0;
}
 
int IW5Console::Commands::Dvar::ProcessDvarValues(char* params, byte DataTypeMode, void* StartAdressValues, int unk0)
{
	DWORD dwcall = OFFSET_PROCESS_DVAR;
	int result = 0x0;
	__asm
	{
		mov ecx, params;
		mov al, DataTypeMode;
		mov ebx, StartAdressValues;
		push unk0;
		call dwcall;
		mov result, eax;
		add esp, 0x4;
	}

	return result;
}

int IW5Console::Commands::Dvar::FindDvarPointer(char* dvar)
{
	int dwcall = OFFSET_FIND_DVAR;
	int result = 0x0;

	char* params = 0x0;
	__asm
	{
		mov edi, dvar;
		call[dwcall];
		mov result, eax;
	}
	return result;
}

int IW5Console::Commands::Dvar::ProcessDvar(char* dvar)
{
	int len = strlen(dvar) + 1;
	char*  cdvar = new char[len];
	memset(cdvar, 0x0, len);
	strcpy_s(cdvar, len, dvar);


	char* dvarparams = 0x0;

	for (unsigned int i = 0; i < strlen(dvar); i++)
	{
		if (cdvar[i] == 0x20)
		{
			cdvar[i] = 0x0;
			
			dvarparams = &cdvar[i + 1];
			if (!strlen(dvarparams))
				return 0;
			break;
		}
	}
	if (!dvarparams)
		return 0;

	int dvarptr = FindDvarPointer(cdvar);

	if (dvarptr)
	{
		Dvar_t* Dvar = (Dvar_t*)dvarptr;
		ProcessDvarValues(dvarparams, Dvar->DataTypeMode, &Dvar->StartPointerValues, Dvar->Unk0);
		int bufflen = (70 + strlen(dvar)); // default
		DebugOnConsole(bufflen, "^5[^;SamurayX^5] ^6%s^5 setting up to ^6%s^5 ^2successfully^5!\n", cdvar, dvarparams);	
	}

	return 0;
}


float IW5Engine::CEngine::ViewOriginScaleXY = 1.0f;
float IW5Engine::CEngine::ViewOriginScaleZ = 1.0f;

int(__cdecl* IW5Engine::CEngine::GetPointNumberByName)(char* name) = (int(__cdecl*)(char*))OFFSET_GET_POINT_NUMBER_BY_NAME;


void IW5Engine::CEngine::UpdateTargetEntityPoints()
{
	for (int i = 0; i < 20; i++)
		PlayerBonesNumber[i] = GetPointNumberByName(PlayerBonesName[i]);

	
	if (!strcmp(Settings::CBot::CBone, "default"))
		SamurayX::Bot::TargetPoint = GetPointNumberByName(PlayerBonesName[0]);
	else
		SamurayX::Bot::TargetPoint = GetPointNumberByName(Settings::CBot::CBone);

	
	for (int i = 0; i < 19; i++)
		Settings::CBot::CMultiBoneOrderNumber[i] = GetPointNumberByName(Settings::CBot::CMultiBoneOrder[i]);

}

char* IW5Engine::CEngine::PlayerBonesName[20] =
{
	"j_head",
	"j_neck",
	"j_helmet",
	"j_shoulder_le",
	"j_shoulder_ri",
	"j_elbow_le",
	"j_elbow_ri",
	"j_wrist_le",
	"j_wrist_ri",
	"j_gun",
	"j_hip_ri",
	"j_hip_le",
	"j_knee_le",
	"j_knee_ri",
	"j_ankle_ri",
	"j_ankle_le",
	"j_mainroot",
	"j_spineupper",
	"j_spinelower",
	"j_spine4",
};

int IW5Engine::CEngine::PlayerBonesNumber[20] = { 0 };
/*
void __forceinline IW5Engine::CEngine::CalculateViewOrigin()
{

	// (CG.nextSnap->ps.OffsetOrigin0[0] - CG.snap->ps.OffsetOrigin0[0]
	ViewOrigin[0] = CG.snap->ps.OffsetOrigin0[0] + ((CG.snap->ps.Velocity[0]) * ViewOriginScaleXY * CG.FrameInterpolation);
	ViewOrigin[1] = CG.snap->ps.OffsetOrigin0[1] + ((CG.snap->ps.Velocity[1]) * ViewOriginScaleXY * CG.FrameInterpolation);
	ViewOrigin[2] = CG.snap->ps.OffsetOrigin0[2] + ((CG.snap->ps.Velocity[2]) * ViewOriginScaleZ * CG.FrameInterpolation);

	ViewOrigin[2] += CG.snap->ps.ViewOriginAngle;

}
*/



vec3_t IW5Engine::CEngine::ViewAngles = { 0.0f, 0.0f, 0.0f };

void __forceinline IW5Engine::CEngine::CalculateViewAngles()
{
	static ViewInfo_t vi;

	vi.cg = &CG;
	
	if (CG.LastImpactTime)
		vi.LastImpactTime = (CG.LastImpactTime - CG.unk0);
	else
		vi.LastImpactTime = 0;


	vi.ServerTime = (CG.Cl_ServerTime - CG.unk0);
	vi.ImpactAngles[0] = CG.ImpactAngles[0];
	vi.ImpactAngles[1] = CG.ImpactAngles[1];
	vi.SideVelocity = CG.VelocitySide;
	vi.FrameTime = (float)((double)CG.FrameTime * 0.001000000047497451);
	vi.SwayMultiplier = CG.SwayMultiplier;

	CMath::CG_GetViewAngleAnimations(&vi, ViewAngles);

	VectorAdd(CG.ViewAngles, ViewAngles, ViewAngles);

}

void IW5Engine::Init()
{
	// init

	//IW5Console::ToggleConsole();
	IW5Console::Commands::Default::InitRegisterDefaultCommands();
	SamurayX::MenuControl::LoadCMenu();
	SamurayX::ConsoleControl::ToggleConsole();


	// this must be called at end for start whole like key events, world entity read etc...

	IW5Engine::InitEngine();
}





vec3_t IW5Engine::CEngine::ViewOrigin = { 0.0f, 0.0f, 0.0f };

float(__cdecl* IW5Engine::CEngine::CMath::VectorNormalize)(float*) = (float(__cdecl*)(float*))OFFSET_VECTOR_NORMALIZE;







void(__cdecl*IW5Engine::CEngine::CMath::CG_GetPlayerViewPoint)(int zero, IW5Engine::CEngine::cg_t*, float* outvec) = (void(__cdecl*)(int, IW5Engine::CEngine::cg_t*, float*))OFFSET_CG_GET_PLAYER_VIEW_POINT;

void(__cdecl*IW5Engine::CEngine::CMath::CG_GetViewAngleAnimations)(IW5Engine::CEngine::ViewInfo_t*, vec3_t Out) = (void(__cdecl*)(IW5Engine::CEngine::ViewInfo_t*, vec3_t))OFFSET_CG_GET_VIEW_ANGLE_ANIMATIONS;

float(__cdecl*IW5Engine::CEngine::CMath::__VectorDistance)(float* a, float* b) = (float(__cdecl*)(float*, float*))OFFSET_VECTOR_DISTANCE;

int(__cdecl*IW5Engine::CEngine::CMath::GetTextWidth)(char* txt, int len, int font) = (int(__cdecl*)(char*, int, int))OFFSET_GET_TEXT_WIDTH;

int(__cdecl*IW5Engine::CEngine::CMath::GetFontHeightByScaleOne)(int font) = (int(__cdecl*)(int))OFFSET_GET_FONT_HEIGHT_BY_SCALE_ONE;


void(__cdecl*IW5Engine::CEngine::CG_ServerCommand)(int local, char* cmd) = (void(__cdecl*)(int, char*))OFFSET_CG_SERVER_COMMAND;

void IW5Console::Commands::Reliable::Execute(char* command)
{
	if (*command == '$')
		IW5Engine::CEngine::CG_ServerCommand(0, &command[1]);
}


// copiler doesn't save and restore ebx, edi automatically on release mode... inline or macro? macro won...
/*

*/

void* IW5Engine::CEngine::CInterpolateEntityPosition = 0x0;

void __cdecl IW5Engine::CEngine::CG_InterpolateEntityPosition(IW5Engine::CEngine::centity_t* entity)
{
	// SideTashing?

	switch (Settings::Prediction::PredictionMode)
	{


		case 0:
		{

			if (entity->Type == ENTITY_PLAYER)
			{
				int CTime = CG.snap->ServerTime;
				int NTime = CG.nextSnap->ServerTime;

				if (CTime != NTime)
				{

					float Interpolation = (((float)Settings::Prediction::Interpolation::InterpolationTime) / ((float)(NTime - CTime)));

					if (Interpolation > 0.0f)
					{

						vec3_t Next, Current;

						float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
						CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);


						entity->Origin[0] += ((Next[0] - Current[0]) * Interpolation);
						entity->Origin[1] += ((Next[1] - Current[1]) * Interpolation);
						entity->Origin[2] += ((Next[2] - Current[2]) * Interpolation);

						FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.apos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
						CMath::BG_EvaluateTrajectory(&entity->nextState.apos, CG.nextSnap->ServerTime, Next, FIResult);


						vec3_t Diff0 = { ((Next[0] - Current[0]) * 0.002777777845039964f), ((Next[1] - Current[1]) * 0.002777777845039964f), ((Next[2] - Current[2]) * 0.002777777845039964f) };
						vec3_t Diff1 = { floor((Diff0[0] + 0.5f)), floor((Diff0[1] + 0.5f)), floor((Diff0[2] + 0.5f)) };

						Diff0[0] = (Diff0[0] - Diff1[0]) * 360.0f;
						Diff0[1] = (Diff0[1] - Diff1[1]) * 360.0f;
						Diff0[2] = (Diff0[2] - Diff1[2]) * 360.0f;

						entity->ViewAngles[0] += (Diff0[0] * Interpolation);
						entity->ViewAngles[1] += (Diff0[1] * Interpolation);
						entity->ViewAngles[2] += (Diff0[2] * Interpolation);
					}
				}

			}


			break;
		}

		case 1:
		{
			// interpolation from View angles to Origin by scale... 
			if (entity->Type == ENTITY_PLAYER)
			{

				vec3_t Next, Current;

				float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);

				// linear ms prediction

				entity->Origin[0] += ((Next[0] - Current[0]) * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));
				entity->Origin[1] += ((Next[1] - Current[1]) * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));
				entity->Origin[2] += ((Next[2] - Current[2]) * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));


				FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.apos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.apos, CG.nextSnap->ServerTime, Next, FIResult);


				vec3_t Diff0 = { ((Next[0] - Current[0]) * 0.002777777845039964f), ((Next[1] - Current[1]) * 0.002777777845039964f), ((Next[2] - Current[2]) * 0.002777777845039964f) };
				vec3_t Diff1 = { floor((Diff0[0] + 0.5f)), floor((Diff0[1] + 0.5f)), floor((Diff0[2] + 0.5f)) };

				Diff0[0] = (Diff0[0] - Diff1[0]) * 360.0f;
				Diff0[1] = (Diff0[1] - Diff1[1]) * 360.0f;
				Diff0[2] = (Diff0[2] - Diff1[2]) * 360.0f;

				entity->ViewAngles[0] += (Diff0[0] * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));
				entity->ViewAngles[1] += (Diff0[1] * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));
				entity->ViewAngles[2] += (Diff0[2] * (Settings::Prediction::Interpolation::InterpolationTime * 0.001f));

			}


			break;
		}
		


		case 2:
		{

			if (entity->Type == ENTITY_PLAYER)
			{
				
				vec3_t Current = { 0.0f, 0.0f, 0.0f };
				vec3_t Next = { 0.0f, 0.0f, 0.0f };


			

				float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);

				vec3_t Velocity = { 0.0f, 0.0f, 0.0f };

				Velocity[0] = (Next[0] - Current[0]);
				Velocity[1] = (Next[1] - Current[1]);
				Velocity[2] = (Next[2] - Current[2]);


				entity->Origin[0] += (GetSign(Velocity[0]) * Settings::Prediction::Interpolation::InterpolationScale);
				entity->Origin[1] += (GetSign(Velocity[1]) * Settings::Prediction::Interpolation::InterpolationScale);
				entity->Origin[2] += (GetSign(Velocity[2]) * Settings::Prediction::Interpolation::InterpolationScale);
				


				FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.apos, CG.snap->ServerTime, Current, CG.FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.apos, CG.nextSnap->ServerTime, Next, FIResult);


				vec3_t Diff0 = { ((Next[0] - Current[0]) * 0.002777777845039964f), ((Next[1] - Current[1]) * 0.002777777845039964f), ((Next[2] - Current[2]) * 0.002777777845039964f) };
				vec3_t Diff1 = { floor((Diff0[0] + 0.5f)), floor((Diff0[1] + 0.5f)), floor((Diff0[2] + 0.5f)) };

				Diff0[0] = (Diff0[0] - Diff1[0]) * 360.0f;
				Diff0[1] = (Diff0[1] - Diff1[1]) * 360.0f;
				Diff0[2] = (Diff0[2] - Diff1[2]) * 360.0f;

				entity->ViewAngles[0] += (GetSign(Diff0[0]) * Settings::Prediction::Interpolation::InterpolationScale);
				entity->ViewAngles[1] += (GetSign(Diff0[1]) * Settings::Prediction::Interpolation::InterpolationScale);
				entity->ViewAngles[2] += (GetSign(Diff0[2]) * Settings::Prediction::Interpolation::InterpolationScale);

				

			}


			break;
		}
		case 3:
		{

			if (entity->Type == ENTITY_PLAYER)
			{
				vec3_t Current = { 0.0f, 0.0f, 0.0f };
				vec3_t Next = { 0.0f, 0.0f, 0.0f };


				float FrameInterpolation = CG.FrameInterpolation;

				float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);

				entity->Origin[0] += ((Next[0] - Current[0]) * Settings::Prediction::PredictionByVelocity::VelocityScale);
				entity->Origin[1] += ((Next[1] - Current[1]) * Settings::Prediction::PredictionByVelocity::VelocityScale);
				entity->Origin[2] += ((Next[2] - Current[2]) * Settings::Prediction::PredictionByVelocity::VelocityScale);
			}
			break;
		}
		case 4:
		{
			// using trajectory linear based on quake's linear prediction
			if (entity->Type == ENTITY_PLAYER)
			{
				float currentdeltatime = CG.Cl_ServerTime * 0.001f;
				float olddeltatime = CG.OldTime * 0.001f;
				// current
				vec3_t trDelta;
				VectorSubtract(entity->nextState.pos.trBase, entity->currentState.pos.trBase, trDelta);
				vec3_t Current;
				vec3_t Old;

				VectorMA(entity->currentState.pos.trBase, currentdeltatime, trDelta, Current);
				VectorMA(entity->currentState.pos.trBase, olddeltatime, trDelta, Old);

				vec3_t Dif;

				VectorSubtract(Current, Old, Dif);

				// ping diff...
				Dif[0] += (trDelta[0] * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				Dif[1] += (trDelta[1] * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				Dif[2] += (trDelta[2] * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));

				entity->Origin[0] += Dif[0];
				entity->Origin[1] += Dif[1];
				entity->Origin[2] += Dif[2];


				/*
					// shorter way
					velocity * timedifference * 0.001; 

				*/

			}

			break;
		}
		case 5:
		{
			if (entity->Type == ENTITY_PLAYER)
			{

				vec3_t Current = { 0.0f, 0.0f, 0.0f };
				vec3_t Next = { 0.0f, 0.0f, 0.0f };


				float FrameInterpolation = CG.FrameInterpolation;

				float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);

				vec3_t Velocity = { 0.0f, 0.0f, 0.0f };

				Velocity[0] = (Next[0] - Current[0]);
				Velocity[1] = (Next[1] - Current[1]);
				Velocity[2] = (Next[2] - Current[2]);


				entity->Origin[0] += (GetSign(Velocity[0]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				entity->Origin[1] += (GetSign(Velocity[1]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				entity->Origin[2] += (GetSign(Velocity[2]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));


			}

			break;
		}
		case 6:
		{
			if (entity->Type == ENTITY_PLAYER)
			{
				vec3_t Current = { 0.0f, 0.0f, 0.0f };
				vec3_t Next = { 0.0f, 0.0f, 0.0f };


				float FrameInterpolation = CG.FrameInterpolation;

				float FIResult = CMath::BG_EvaluateTrajectory(&entity->currentState.pos, CG.snap->ServerTime, Current, FrameInterpolation); // current state
				CMath::BG_EvaluateTrajectory(&entity->nextState.pos, CG.nextSnap->ServerTime, Next, FIResult);

				vec3_t Velocity = { 0.0f, 0.0f, 0.0f };

				Velocity[0] = (Next[0] - Current[0]);
				Velocity[1] = (Next[1] - Current[1]);
				Velocity[2] = (Next[2] - Current[2]);

				// 
				entity->Origin[0] += (GetSign(Velocity[0]) * (CG.FrameTime * 0.001f));
				entity->Origin[1] += (GetSign(Velocity[1]) * (CG.FrameTime * 0.001f));
				entity->Origin[2] += (GetSign(Velocity[2]) * (CG.FrameTime * 0.001f));


				entity->Origin[0] += (GetSign(Velocity[0]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				entity->Origin[1] += (GetSign(Velocity[1]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));
				entity->Origin[2] += (GetSign(Velocity[2]) * (Settings::Prediction::PredictionByTime::TimeScale * 0.001f));

			}

			break;
		}


	}
	return;
}


// event
__declspec(naked) void IW5Engine::CEngine::OnInterpolateEntityPosition()
{
	__asm call[CInterpolateEntityPosition];
	__asm push eax;
	__asm push esi;
	__asm call[CG_InterpolateEntityPosition];
	__asm add esp, 0x4;
	__asm pop eax;
	__asm retn;
}




// default


float(__cdecl*IW5Engine::CEngine::CAwall::GetWallDepth)(int weapon, byte zero, int WallType) = (float(__cdecl*)(int, byte, int))OFFSET_GET_WALL_DEPTH;


bool(__cdecl* IW5Engine::CEngine::CAwall::AdvanceTrace)(BulletTrace_t *stbt, WorldInfo_t* stwi, float unk0) = (bool(__cdecl*)(IW5Engine::CEngine::BulletTrace_t* stbt, IW5Engine::CEngine::WorldInfo_t*, float))OFFSET_ADVANCE_TRACE;


int __forceinline IW5Engine::CEngine::CAwall::BulletTrace(centity_t * MainEntity, int WallType, BulletTrace_t* stbt, WorldInfo_t * stwi)
{
	static const DWORD dwx = OFFSET_BULLET_TRACE;

	int Result = 0;

	__asm
	{
		push WallType;
		push MainEntity;
		mov eax, stbt;
		mov esi, stwi;
		call[dwx];
		add esp, 0x8;
		mov[Result], eax;
	}

	return Result;
}


float IW5Engine::CEngine::CAwall::GetFinalBulletPower(int CEntityIndex, vec3_t Pos)
{
	int Weapon = GetCurrentWeaponId();

	BulletTrace_t WallTrace;

	WallTrace.WorldEntityNumber = 2046; // default random entity number that never will exists...
	WallTrace.Power = 1.0f; // default aka depth
	WallTrace.BulletType = GetWeaponBulletType(GetCurrentWeapon());
	WallTrace.EntityMatched = GetCurrentClientNumber();

	// this is that game engine in server it does, the difference it's that uses your vieworigin like: ForwardView + EntityOrigin... 
	// and it's called on every time that you shots...
	VectorCopy(ViewOrigin, WallTrace.ViewOrg);
	VectorCopy(ViewOrigin, WallTrace.Start);
	VectorCopy(Pos, WallTrace.End); //  so we just put the pos and will be fine too...

	vec3_t Delta = { 0.0f, 0.0f, 0.0f };

	VectorSubtract(Pos, ViewOrigin, Delta);


	CMath::VectorNormalize(Delta);

	VectorCopy(Delta, WallTrace.ViewDirection);



	centity_t* MainEntity = &GetEntity(GetCurrentClientNumber());


	WorldInfo_t WorldInfo;


	int btresult0 = BulletTrace(MainEntity, 0, &WallTrace, &WorldInfo);//(&WallTrace, &WorldInfo, MainEntity, 0);


	if (WorldInfo.EntityHittedIndex == CEntityIndex)
		return 4.0f; // there isn't any object or pWall between main entity and target entity... 
	// maybe some formula: normalvisiblepower = (Perk_BulletPenetrationMultiplier * 1.0f) + 0.1; 



	if (!btresult0)
		return WallTrace.Power;

	
	
	// 2nd results
	WorldInfo_t WorldInfoB;
	BulletTrace_t WallTraceB;
	int walls = 0;
	int hit0 = 0;
	int hit1 = 0;
	bool Resultx = false;

	float InitPower = 0.0f;

	vec3_t FirstHit = { 0.0f, 0.0f, 0.0f };

	while (1)
	{

		// init

		InitPower = GetWallDepth(Weapon, 0x0, WorldInfo.WallType);

		// PerkBulletPenetrationMultiplier
		InitPower *= 2.0f;


		
		if (InitPower <= 0.0f)
			return 0.0f;

		VectorCopy(WorldInfo.ViewDirection, FirstHit);

		if (!AdvanceTrace(&WallTrace, &WorldInfo, 0.13500000536441803f))
			return 0.0f;

		hit0 = BulletTrace(MainEntity, WorldInfo.WallType, &WallTrace, &WorldInfo);


		memcpy(&WallTraceB, &WallTrace, sizeof(BulletTrace_t));


		VectorScale(WallTrace.ViewDirection, -1.0f, WallTraceB.ViewDirection);
		VectorCopy(WallTrace.End, WallTraceB.Start);
		VectorMA(FirstHit, 0.009999999776482582f, WallTraceB.ViewDirection, WallTraceB.End);

		memcpy(&WorldInfoB, &WorldInfo, sizeof(WorldInfo_t));


		VectorScale(WorldInfoB.Direction, -1.0f, WorldInfoB.Direction);

		if (hit0)
			AdvanceTrace(&WallTraceB, &WorldInfoB, 0.009999999776482582f);


		hit1 = BulletTrace(MainEntity, WorldInfoB.WallType, &WallTraceB, &WorldInfoB);


		Resultx = (hit1 != 0 && WorldInfoB.check0 || WorldInfo.check1 && WorldInfoB.check1);

		if (hit1 || Resultx)
			break;

		if (!hit0)
			return WallTrace.Power;


	nextwall:

		if (hit0)
		{
			walls++;
			if (walls < 5)
				continue;
		}

		return 0.0f;
	}

	float Depth = 0.0f;


	if (Resultx)
		Depth = VectorDistance(WallTraceB.Start, WallTraceB.End);
	else
		Depth = VectorDistance(WorldInfoB.ViewDirection, FirstHit);

	if (Depth < 0.0f)
		Depth = 1.0f;

	if (hit1)
	{
		float power2 = GetWallDepth(Weapon, 0, WorldInfoB.WallType);

		power2 *= 2.0f;

		InitPower = min(InitPower, power2);

		if (InitPower <= 0.0f)
			return 0.0f;


	}




	WallTrace.Power -= Depth / InitPower;

	if (WallTrace.Power <= 0.0f)
		return 0.0f;
	else
		goto nextwall;



}



float* (__cdecl*IW5Engine::CEngine::CSpread::GetWeaponSpreadAngle)(cg_t* cg, byte Weapon, float* MultiplierA, float* MultiplierB) = (float*(__cdecl*)(cg_t*, byte, float*, float*))OFFSET_GET_SPREAD_ANGLE;




// void __usercall GetSpreadVector(int StartVec3@<ebx>, int EndVec3@<edi>, unsigned int *seed@<esi>, float SpreadAngle, int vDirectionVec3, int ForwardVec3, int RightVec3, int spread, float Distance)


void __forceinline IW5Engine::CEngine::CSpread::GetSpreadVector(float spreadangle, float* outvdir, float* forward, float* right, float* up, float distance, float* startvec, float* endvec, int* seedptr)
{
	static const DWORD dwcall = OFFSET_GET_SPREAD_VECTOR;
	
	__asm
	{
		mov ebx, startvec;
		mov edi, endvec;
		mov esi, seedptr;
		push distance;
		push up;
		push right;
		push forward;
		push outvdir;
		push spreadangle;
		call dwcall;
		add esp, 0x18;
	}
}


int IW5Engine::CEngine::CSpread::LastAnglesAdded[3] = { 0 };

void __cdecl IW5Engine::CEngine::CSpread::ReverseSpread(usercmd_t* cmd)
{
	vec3_t WeaponView = { ViewAngles[0], ViewAngles[1], 0.0f }; // { CG.WeaponView[0], CG.WeaponView[1], 0.0f }; //{ CG.ViewAngles[0], CG.ViewAngles[1],  0.0f }; 
	vec3_t SpreadVector = { 0.0f, 0.0f, 0.0f };
	vec3_t Forward, Right, Up;

	QMath::AngleVectors(WeaponView, Forward, Right, Up);

	int seed = 214013 * (214013 * (214013 * (214013 * cmd->ServerTime + 2531011) + 2531011) + 2531011) + 2531011;

	vec3_t ViewVector = { 0.0f, 0.0f, 0.0f };
	vec3_t EndVec = { 0.0f, 0.0f, 0.0f };
	vec3_t StartVec = { 0.0f, 0.0f, 0.0f };

	float SpreadMultiplierA = 0.0f;
	float SpreadMultiplierB = 0.0f;

	GetWeaponSpreadAngle(&CG, GetCurrentWeaponId(), &SpreadMultiplierA, &SpreadMultiplierB);

	float SpreadAngle = (SpreadMultiplierB - SpreadMultiplierA) * (GetSpreadMultiplier() / 255.0f) + SpreadMultiplierA;

	GetSpreadVector(SpreadAngle, SpreadVector, Forward, Right, Up, 8192.0f, StartVec, EndVec, &seed);
	
	QMath::VectorAngles(SpreadVector, SpreadVector); // now we have the view angles of spread

	VectorSubtract(WeaponView, SpreadVector, SpreadVector);

	if (CG.AimProgress != 1.0f)
		cmd->ViewAngles[0] += ANGLE2SHORT(SpreadVector[0]),
		cmd->ViewAngles[1] += ANGLE2SHORT(SpreadVector[1]),
		cmd->ViewAngles[2] = 0;

}


#pragma region test_x

static IW5Engine::CEngine::usercmd_t* cmd0;
static IW5Engine::CEngine::usercmd_t* cmd1;


int orgcmdnumber = 0x0;

#pragma endregion
IW5Engine::CEngine::usercmd_t* IW5Engine::CEngine::cmd = (IW5Engine::CEngine::usercmd_t*)OFFSET_USER_CMDS;

void IW5Engine::CEngine::OnStartWritePacket()
{





	if (IsInGamePlaying())
	{

		
		// testing new cmd backup
		cmd = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		
		// beta
		(CURRENT_CMD_NUMBER)++;
		cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		*cmd0 = *cmd;
		cmd0->Buttons &= ~BUTTON_SHOOT;
		//cmd0->ServerTime += 2;
		cmd0->ServerTime += Settings::UserCommands::TimeScale;
		
		// faster shoot method beta... packets fucked up literally... test later...
		

		// 1nd propouse
		/*
		cmd = &GetUserCmd((CURRENT_CMD_NUMBER - 1) & CMD_MASK);
		cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		*cmd = *cmd0;
		cmd->ServerTime += 1 + Settings::UserCommands::TimeScale;
		cmd0->ServerTime += 2 + Settings::UserCommands::TimeScale;
		cmd0->Buttons &= ~BUTTON_SHOOT;
		*/

		// test 

		/*
		cmd = &GetUserCmd((CURRENT_CMD_NUMBER - 1) & CMD_MASK);
		cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		*cmd = *cmd0;
		cmd0->ServerTime += Settings::UserCommands::TimeScale;
		cmd0->Buttons &= ~BUTTON_SHOOT;
		*/

		// 2nd propouse...
		
		// * faster shoots, but inaccurate spread, view angles gets fucked up probably due increased server time..., it can be fixed decreasing seed time on view angles calculation...
		/*
		cmd = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		(CURRENT_CMD_NUMBER)++;
		cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
		*cmd0 = *cmd;
		cmd->ServerTime += 1 + Settings::UserCommands::TimeScale;
		cmd0->ServerTime += 2 + Settings::UserCommands::TimeScale;
		cmd0->Buttons &= ~BUTTON_SHOOT;
		*/



		if (SamurayX::Bot::State::TargetFound)
		{

			IW5Engine::CEngine::CalculateViewAngles();


			static vec3_t Delta;

			Delta[0] = (SamurayX::Bot::CDelta[0] - IW5Engine::CEngine::ViewAngles[0]);
			Delta[1] = (SamurayX::Bot::CDelta[1] - IW5Engine::CEngine::ViewAngles[1]);

			
			ClientActive.ViewAngles[0] += Delta[0];
			ClientActive.ViewAngles[1] += Delta[1];
			
	
			cmd->ViewAngles[0] += ANGLE2SHORT(Delta[0]);
			cmd->ViewAngles[1] += ANGLE2SHORT(Delta[1]);


			CSpread::ReverseSpread(cmd);
			
		}
		
		// stuffs...
		// orgcmdnumber = (CURRENT_CMD_NUMBER & CMD_MASK);


		

		
		switch (Settings::AntiAim::AntiAimMode)
		{

			case Settings::AntiAim::ANTI_AIM_MOVEMENT_TYPE_A:
			{

				/*
				static usercmd_t* cmd0;

				(CURRENT_CMD_NUMBER)++;

				cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
				*cmd0 = *cmd;
				cmd0->ServerTime += 1;
				*/
				static byte Mode = 1;
				

				switch (Mode) // jmp ds:off_0x0[ins*x]
				{
					case 1:
						cmd0->ViewAngles[0] = ANGLE2SHORT(-85), cmd0->ViewAngles[1] = ANGLE2SHORT(0), Mode = 2;
						break;
					case 2:
						cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(90), Mode = 3;
						break;
					case 3:
						cmd0->ViewAngles[0] = ANGLE2SHORT(-85), cmd0->ViewAngles[1] = ANGLE2SHORT(180), Mode = 4;
						break;
					case 4:
						cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(270), Mode = 1;
						break;
					default:
						break;
				}


				break;
			}
			case Settings::AntiAim::ANTI_AIM_MOVEMENT_TYPE_B:
			{
				/*
				(CURRENT_CMD_NUMBER)++;
				cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);
				*cmd0 = *cmd;
				cmd0->ServerTime += 1;
				*/

				// fake state... let me run bitch...
				/*
				cmd0->ForwardBackwardMovement = 0;
				cmd0->LeftRightMovement = 0;
				cmd1->ForwardBackwardMovement = 0;
				cmd1->LeftRightMovement = 0;
				*/


				

				static byte Mode = 1;


				switch (Mode) // jmp ds:off_0x0[ins*x]
				{
				case 1:
					cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(0), Mode = 2;
					break;
				case 2:
					cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(90), Mode = 3;
					break;
				case 3:
					cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(180), Mode = 4;
					break;
				case 4:
					cmd0->ViewAngles[0] = ANGLE2SHORT(85), cmd0->ViewAngles[1] = ANGLE2SHORT(270), Mode = 1;
					break;
				default:
					break;
				}


				break;
			}
			case Settings::AntiAim::ANTI_AIM_MOVEMENT_TYPE_C:
			{
				static usercmd_t* cmd0;

				(CURRENT_CMD_NUMBER)++;

				cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);



				break;
			}
			case Settings::AntiAim::ANTI_AIM_RIOT_TYPE_A:
			{
				static usercmd_t* cmd0;

				(CURRENT_CMD_NUMBER)++;

				cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);


				break;
			}
			case Settings::AntiAim::ANTI_AIM_RIOT_TYPE_B:
			{
				static usercmd_t* cmd0;

				(CURRENT_CMD_NUMBER)++;

				cmd0 = &GetUserCmd(CURRENT_CMD_NUMBER & CMD_MASK);

				break;
			}

			default:
				break;
		}


		//if (SamurayX::Bot::State::LastShoot)
		//	CSpread::ReverseSpread(cmd);
		

	}
	


	/*
	int PacketNumber = *(int*)0xB81580 - 1;

	int k = 0;

	while (*(int*)0x10603B8 < ((int*)0x106D6C0)[3 * (*(int*)0x106D6B0 & PacketNumber)])
	{
		++k;
		--PacketNumber;
		if (k >= *(int*)0x106D6AC)
		{
			cping = (((int*)0x106D6C4)[3 * (*(int*)0x106D6B0 & PacketNumber)] - *(int*)0xBA2B10) * -1;
			return;
		}
	}

	cping = *(int*)0xBA2B10 - ((int*)0x106D6C4)[3 * (*(int*)0x106D6B0 & PacketNumber)];
	*/

	

}

void IW5Engine::CEngine::OnEndWritePacket()
{
	
	if (SamurayX::Bot::State::LastShoot)
	{
		cmd->ViewAngles[0] -= CSpread::LastAnglesAdded[0];
		cmd->ViewAngles[1] -= CSpread::LastAnglesAdded[1];
	}
	

	/*
	if (IsInGamePlaying())
	{
		if (Settings::AntiAim::AntiAimMode != -1)
		{
			*cmd0 = GetUserCmd(orgcmdnumber);
			*cmd1 = *cmd0;

		}
	}
	*/

	/*
	int* packetbackup = (int*)0x106D6B0;
	int* cpacket = (int*)0xB81580;
	int* realtime = (int*)0xBA2B10;
	int* commandtime = (int*)0x10603B8;
	if (IsInGamePlaying())
	{
		for (int i = 0; i < *packetbackup; i++)
		{
			int packetnum = (*cpacket - 1 - i);
			int cpacketn = (packetnum & *packetbackup);
			if (CG.snap->ps.CommandTime >= OutPackets(cpacketn).p_serverTime)
				cping = *realtime - OutPackets(cpacketn).p_realtime;
		}
	}
	*/
}


__declspec(naked) void IW5Engine::CEngine::CL_EndWritePacket()
{
	__asm
	{
		pushad;
		pushfd;
	}
	
	OnEndWritePacket();

	__asm
	{
		popfd;
		popad;
		add esp, 0x1058;
		retn;
	}

}


void* IW5Engine::CEngine::CWritePacket = 0x0;



__declspec(naked) void IW5Engine::CEngine::CL_WritePacket()
{
	
	__asm
	{
		push edi;
		call[OnStartWritePacket];
		pop edi;
		jmp[CWritePacket];
	}


}


void IW5Engine::CEngine::OnCreateNewCmds()
{
	switch (Settings::CBot::Bot)
	{

	case 1:
		SamurayX::Bot::InitBot();
		break;

	case 2:
		SamurayX::Bot::InitMultiBoneBot();
		break;

	default:
		break;
	}

	
}



void* IW5Engine::CEngine::CL_CreateNewCmds = 0x0;


__declspec(naked) void IW5Engine::CEngine::OnCL_CreateNewCmd()
{


	__asm
	{
		//push eax;
		pushad;
	}
	/*
	switch (Settings::CBot::Bot)
	{
		case 1:
		{
			SamurayX::Bot::InitBot();
			break;
		}
		case 2:
		{
			SamurayX::Bot::InitMultiBoneBot();
			break;
		}

		default:
			break;
	}
	*/
	SamurayX::Bot::InitBot();

	__asm
	{
		popad;
		//pop eax;
		jmp[CL_CreateNewCmds];
	}
}

/*
__declspec(naked) void IW5Engine::CEngine::CL_CreateNewCommands()
{
	static const DWORD CreateNewCommands = OFFSET_CL_CREATE_NEW_COMMANDS + 0xA; 

	__asm
	{
		sub esp, 0x58;
		push eax;
		mov eax, 0x1060214;
		cmp[eax], 0xA; // set flags
 		pop eax;
		jmp[CreateNewCommands];
	}
}
*/

int (__cdecl*IW5Engine::CEngine::LoadShader)(char* name) = (int(__cdecl*)(char*))OFFSET_LOAD_SHADER;

char*(__cdecl*IW5Engine::CEngine::CDraw::CDrawText)(char* text, size_t len, int Font, float x, float y, float sizex, float sizey, float rotate, Color_t* color, int zero) = (char*(__cdecl*)(char*, size_t, int, float, float, float, float, float, Color_t*, int))OFFSET_DRAW_TEXT;
char(__cdecl*IW5Engine::CEngine::CDraw::CDrawShader)(float x, float y, float sizex, float sizey, float unk0, float unk1, float unk2, float unk3, Color_t* color, int Shader) = (char(__cdecl*)(float, float, float, float, float, float, float, float, Color_t*, int))OFFSET_DRAW_SHADER;

Color_t IW5Engine::CEngine::CDraw::TestColor(0, 255, 255, 255);

void(__cdecl*IW5Engine::CEngine::CG_Draw2D)(int local);

void __cdecl IW5Engine::CEngine::OnCG_Draw2D(int local)
{

	for (int i = 0; i < SamurayX::Bot::EnumTargets; i++)
	{
		static centity_t* cent;
		static vec3_t pos;
		static float x, y;

		static int ctag = 0;
	
		cent = &GetEntity(SamurayX::Bot::TargetList[i]);

		if (cent->Alive & 0x1)
		{
			for (int i = 0; i < 20; i++)
			{

				IW5Engine::CEngine::CMath::GetDobjBonePos(cent, PlayerBonesNumber[i], pos);


				if (IW5Engine::CEngine::CMath::GetWorld2DCoordsFrom3D(pos, &x, &y))
					IW5Engine::CEngine::CDraw::CDrawText(".", 1, GetConsoleFont(), x, y, 1.0f, 1.0f, 0.0, &Settings::CESP::EnemyESP::EnemyESPColor, 0);
			}
		}

	}

	if (Settings::CESP::WeaponESP::Enabled)
	{
		for (int i = 0; i < SamurayX::WorldRead::EnumWorldList; i++)
		{
			static int shader = 0;
			
			static IW5Engine::CEngine::centity_t* cEntity;
	//		static vec3_t pos;

			cEntity = &GetEntity(SamurayX::WorldRead::WorldList[i]);

			shader = GetWeaponShaderByWeaponId(cEntity->Weapon);

			if (shader)
			{
				static float x, y;
			
				if (IW5Engine::CEngine::CMath::GetWorld2DCoordsFrom3D(cEntity->Origin, &x, &y))
					CEngine::CDraw::CDrawShader(x, y, 40.0f, 40.0f, 0.0f, 0.0f, 1.0f, 1.0f, &Settings::CESP::WeaponESP::WeaponESPColor, shader);
			}
		}
	}
	


	/*
		char buff[1024];

		sprintf_s(buff, sizeof(buff), "^5 VXA: %f | CXB: %f | VYA: %f | VYB: %f", vecview[0], RefDefViewAngles.ViewAngles[0], vecview[1], RefDefViewAngles.ViewAngles[1]);
		CDraw::CDrawText(buff, strlen(buff), GetConsoleFont(), 200, 50, 1.0f, 1.0f, 0.0f, &Settings::CESP::EnemyEspColor, 0);
	*/

//	CEngine::CDraw::CDrawShader(200.0f, 50.0f, 40.0f, 40.0f, 0.0f, 0.0f, 1.0f, 1.0f, &Settings::CESP::EnemyEspColor, GetWeaponShaderByWeaponId(GetCurrentWeaponId()));


	/*

	*/

	// working method
	/*
		for (int i = 0; i < MAX_ENTITIES; i++)
		{
			
			static centity_t* ent;
			
			ent = &GetEntity(i);

			if (ent->Type == ENTITY_WEAPON || ent->Type == ENTITY_ATTACHMENT_ATTACK)
			{
				if (ent->Status && ent->Alive)
				{
					static int shader;

					shader = GetWeaponShaderByWeaponId(ent->Weapon);

					if (shader)
					{
						static float x, y;
						IW5Engine::CEngine::CMath::GetWorld2DCoordsFrom3D(ent->Origin, &x, &y);
						CEngine::CDraw::CDrawShader(x, y, 40.0f, 40.0f, 0.0f, 0.0f, 1.0f, 1.0f, &Settings::CESP::EnemyEspColor, shader);
					}
				}
			}
		}
	*/





	/*
	char buff[1024];

	sprintf_s(buff, sizeof(buff), "^5View X: %f | Y: %f", ViewAngles[0], ViewAngles[1]);

	IW5Engine::CEngine::CDraw::CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 50.0f, 1.0f, 1.0f, 0.0f, &Settings::CESP::EnemyEspColor, 0);
	*/

	// fak off... if you see this, i've done this because default game engine on pings > ~200 always in snap.ping it's 999...
	// i would call CG_ServerCommand(0, "s") and use the score ping, but that "faster" way makes me cry...

	/*
	// test 1

	static char buff[256];

	int* lastpackettime = (int*)0xB3D36C;
	int* lastpacketsenttime = (int*)0xB3D368;
	int* realtime = (int*)0xBA2B10;
	static int clastpackettime = 0x0;
	static int clastpacketsenttime = 0x0;
	static int updateinterval = 0x0;
	static int timex;

	if (clastpackettime != *lastpackettime)
	{
		timex = *lastpackettime - clastpacketsenttime;
		clastpackettime = *lastpackettime;
		clastpacketsenttime = *lastpacketsenttime;
	
	}

	if (GetTickCount() - updateinterval > 500)
	{
		sprintf_s(buff, sizeof(buff), "^5 Interval: %d", timex);
		updateinterval = GetTickCount();
	}

	IW5Engine::CEngine::CDraw::CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 50.0f, 1.0f, 1.0f, 0.0f, &Settings::CESP::EnemyEspColor, 0);
	*/

	/*
	char buff[256];

	sprintf_s(buff, sizeof(buff), "^5Ping: %d", cping);

	IW5Engine::CEngine::CDraw::CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 50.0f, 1.0f, 1.0f, 0.0f, &Settings::CESP::EnemyEspColor, 0);
	*/

	/*

	// test 0

	int* packetbackup = (int*)0x106D6B0;
	int* cpacket = (int*)0xB81580;
	int* realtime = (int*)0xBA2B10;
	int* commandtime = (int*)0x10603B8;
	if (IsInGamePlaying())
	{
		int m = 1;
		for (int i = 0; i < (*packetbackup + 1); i++)
		{
	
			int packetnum = (*cpacket - 1 - i);
			int cpacketn = (packetnum & *packetbackup);
			if (CG.snap->ps.CommandTime >= OutPackets(cpacketn).p_serverTime)
				cping = *realtime - OutPackets(cpacketn).p_realtime;
	
			char buff[256];

			sprintf_s(buff, sizeof(buff), "^5T: %d Ping: %d", m, (*realtime - OutPackets(i).p_realtime));
			IW5Engine::CEngine::CDraw::CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, (10.0f * (float)m), 1.0f, 1.0f, 0.0f, &Settings::CESP::EnemyEspColor, 0);
			++m;

		}
	}
	*/



	// init



	
	
	return CG_Draw2D(local);
}


char(__cdecl*IW5Engine::CEngine::CG_InitDrawConsole)(int);

char IW5Engine::CEngine::OnCG_InitDrawConsole(int local)
{

	if (SamurayX::MenuControl::OpenMenu)
		SamurayX::MenuControl::Menu.Init();
	else if (SamurayX::ConsoleControl::OpenConsole)
		SamurayX::ConsoleControl::Console.Run();
	
	return CG_InitDrawConsole(local);
}

