#include "main.hpp"




void Possesser::MemoryPossesser::MakeDirectJUMP(void* from, void* to)
{
	DWORD Difference = ((DWORD)to - (DWORD)from - 0x5);
	
	DWORD DefaultProtection = 0x0;

	VirtualProtect(from, 0x5, PAGE_EXECUTE_READWRITE, &DefaultProtection);
	
	// near jump
	*(BYTE*)from = 0xE9; 

	*(DWORD*)&(((BYTE*)from)[1]) = Difference;

	VirtualProtect(from, 0x5, DefaultProtection, NULL);
}


void* Possesser::MemoryPossesser::MakeDirectJUMP(void* from, void* to, size_t bytestosave)
{
	size_t len = bytestosave + 0x5 + 0x5; // bytes apart of new jump + new jump

	void* memory = malloc(len);

	memset(memory, 0x0, len);

	DWORD memusedsize = bytestosave + 0x5;

	DWORD newaddr = ((DWORD)from + memusedsize);

	memcpy(memory, from, memusedsize);

	((BYTE*)memory)[memusedsize] = 0xE9;

	DWORD Difference = (newaddr - (DWORD)(&((BYTE*)memory)[memusedsize]) - 0x5);

	*(DWORD*)&(((BYTE*)memory)[memusedsize + 1]) = Difference;

	Difference = ((DWORD)to - (DWORD)from - 0x5);

	DWORD DefaultProtection = 0x0;

	VirtualProtect(from, 0x5, PAGE_EXECUTE_READWRITE, &DefaultProtection);

	// near jump
	*(BYTE*)from = 0xE9;

	*(DWORD*)&(((BYTE*)from)[1]) = Difference;

	VirtualProtect(from, 0x5, DefaultProtection, NULL);

	// set same protection
	VirtualProtect(memory, len, DefaultProtection, NULL);

	return memory;
}


void Possesser::MemoryPossesser::PatchNOP(DWORD addr, size_t len)
{
	memset((void*)addr, 0x90, len);
}


void Possesser::PossessEngine()
{

	
	HANDLE th0 = OpenThread(THREAD_ALL_ACCESS, FALSE, *(DWORD*)OFFSET_DIRECTX_THREAD_ID);
	HANDLE th1 = OpenThread(THREAD_ALL_ACCESS, FALSE, *(DWORD*)OFFSET_RENDER_FRAME_THREAD_ID);

	if (th0 && th1)
		SuspendThread(th0), SuspendThread(th1);
	

	// take engine - setting engine offsets
	IW5Console::SettingColor.BackgroundMin = (Color_t*)(*(DWORD*)0xB1CA74 + 0xC);
	IW5Console::SettingColor.BackgroundMax = (Color_t*)(*(DWORD*)0xB1D254 + 0xC);
	IW5Console::SettingColor.Prefix = (Color_t*)0x825508;
	IW5Console::SettingColor.TextMin = (Color_t*)0x82E788;
	IW5Console::SettingColor.TextMax = (Color_t*)0x825270;
	IW5Console::SettingColor.ScrollBarOver = (Color_t*)(*(DWORD*)0xB1D034 + 0xC);
	IW5Console::SettingColor.ScrollBarUnder = (Color_t*)(*(DWORD*)0xB39D34 + 0xC);
	IW5Console::ConsolePrefix = (char*)0x7EAB54;
	IW5Console::SettingUI.TurnConsoleMin = (int*)0x106020C;
	IW5Console::SettingUI.TurnConsoleMax = (int*)0xB35498;



	IW5Engine::CEngine::CG_Draw2D = (void(__cdecl*)(int))Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CG_DRAW_2D, (void*)&IW5Engine::CEngine::OnCG_Draw2D, 0x0);
	IW5Engine::CEngine::CInterpolateEntityPosition = Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CG_INTERPOLATE_ENTITY, (void*)&IW5Engine::CEngine::OnInterpolateEntityPosition, 0x1);
	IW5Engine::CEngine::CWritePacket = Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CL_WRITE_PACKET, (void*)&IW5Engine::CEngine::CL_WritePacket, 0x2);
	IW5Engine::CEngine::CL_CreateNewCmds = Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CL_CREATE_NEW_CMD, (void*)IW5Engine::CEngine::OnCL_CreateNewCmd, 0x0);
	IW5Engine::CEngine::CG_InitDrawConsole = (char(__cdecl*)(int))Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CG_INIT_DRAW_CONSOLE, &IW5Engine::CEngine::OnCG_InitDrawConsole, 0x0);

	//Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CL_END_WRITE_PACKET, (void*)&IW5Engine::CEngine::CL_EndWritePacket);
	
	//Possesser::MemoryPossesser::MakeDirectJUMP((void*)OFFSET_CL_CREATE_NEW_COMMANDS, (void*)&IW5Engine::CEngine::OnCreateNewCommands);
	IW5Console::ProccessRconCommand = (int(__cdecl*)(int, char*))Possesser::MemoryPossesser::MakeDirectJUMP((void*)0x4B3170, (void*)&IW5Console::OnRconCommand, 0x5);
	IW5Console::PrintOnConsole = (char(__cdecl*)(int, int, char*, int, int, char, int))0x5AE210;
	IW5Console::Commands::Reliable::CBuff_addtext = (int(__cdecl*)(int, char*))0x4C1030;
	IW5Engine::WinApi::EngineEnterCriticalSection = (void(__cdecl*)(int))0x4D6310;
	IW5Engine::WinApi::EngineLeaveCriticalSection = (void(__cdecl*)(int))0x4DCF40;
	


	*(int*)OFFSET_GET_WEAPON_RECOIL = 0xC3; // :)))))))))))))))))))))))))))))))))))))))))))))


	if (th0 && th1)
		ResumeThread(th0), ResumeThread(th1);


	CloseHandle(th0);
	CloseHandle(th1);


}