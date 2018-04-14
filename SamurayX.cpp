#include "main.hpp"


/*
		Samurai Ray X Bot coded by 10101001 | TheCyborgX

		Here starts whole:
			IW5Engine will contain all engine functions
			Possesser will contain all hook alterations

		
		note 2k18:
			the code is a terrible mess... :)

		Last changes:
		fixed shaky multibone, disabled the "best power mode", now will hit on bone order without check the high hit chance... (complete)
		added a menu... (incomplete)
		improved prediction interpolation (complete)
		improved time scale for faster shoots (complete)... may be i should fix the camera shaky on adding over 10+ ms on timebase...
		improved some code performance

		ps: Fuck you VSC++ 2013, why? because doesn't support constexpr between other things... it sucks download the new shitty visual studio every year...


*/


std::map<int, std::map<int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*> > SamurayX::SettingControls::KeyHandler::KeyCommandsState;

bool SamurayX::SettingControls::KeyHandler::cmp::operator()(const char* a, const char* b)
{
	return std::strcmp(a, b) < 0;
}

std::map<char*, int, SamurayX::SettingControls::KeyHandler::cmp> SamurayX::SettingControls::KeyHandler::SystemVirtualKeysCodes = 
{
	{ "f1", VK_F1 },
	{ "f2", VK_F2 },
	{ "f3", VK_F3 },
	{ "f4", VK_F4 },
	{ "f5", VK_F5 },
	{ "f6", VK_F6 },
	{ "f7", VK_F7 },
	{ "f8", VK_F8 },
	{ "f9", VK_F9 },
	{ "f10", VK_F10 },
	{ "f11", VK_F11 },
	{ "f12", VK_F12 },
	{ "delete", VK_DELETE },
	{ "end", VK_END }
};


int SamurayX::SettingControls::KeyHandler::GetVirtualKeyCodeByName(char* name)
{

	
	if (strlen(name) == 1)
	{
		if (*name > 0x40 && *name < 0x5B || *name > 0x60 && *name < 0x7B)
			return VkKeyScan(tolower(*name));
		else if (*name > 0x2F && *name < 0x3A)
			return VkKeyScan(*name);
	}
	else
	{
		std::map<char*, int, SamurayX::SettingControls::KeyHandler::cmp>::iterator it;
		it = SystemVirtualKeysCodes.find(name);
		if (it != SystemVirtualKeysCodes.end())
			return SystemVirtualKeysCodes[name];
	}
	
	return 0;
}




int SamurayX::SettingControls::KeyHandler::RegisterNewHotKeyForCommand(char* command)
{

	// some security checks...

	// 
	int space = 0;
	
	for (unsigned int i = 0, b = 0; i < strlen(command); i++)
	{
		if (command[i] == 0x20) // space found
			++space;

		if (!space && i > 14)
			return -1; // command too long


		if (space == 1)
		{
			if (b > 30)
				return -2; // key too long

			b++;
		}
		else if (space == 2)
		{
			break;
		}
	}



	char cmd[16]; // register
	char key[32]; // key

	memset(cmd, 0x0, 16);
	memset(key, 0x0, 32);


	//char newcmd[32];
	char* newcmd = "";


	sscanf_s(command, "%s %s", cmd, 16, key, 32); // fuck off ms compiler
	


	int k = GetVirtualKeyCodeByName(key);

	

	//memset(newcmd, 0x0, 32);

	for (unsigned int i = 0; i < strlen(command); i++)
		if (command[i] == 0x20 && i > strlen(key) && !strncmp(key, &command[i - strlen(key)], strlen(key)))
		{
			char* match = &command[i + 1];
			int len = strlen(match) + 1;
			newcmd = new char[len];
			memset(newcmd, 0x0, len);
			strcpy_s(newcmd, len, match);
			break;
		}
		//strcpy_s(newcmd, &command[i + 1]);

	
	if (!strcmp(cmd, "register") && k)
	{
		if (!strlen(newcmd))
			return 0;

	//	IW5Console::DebugOnConsole(128, "^5Init Register");

		std::map<int, std::map<int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*> >::iterator it;
		
		it = KeyCommandsState.find(k);
		
		std::map< int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*  >::iterator itb;



		if (it == KeyCommandsState.end()) // not found
		{

			// so we add a new cmd
	//		int len = strlen(newcmd) + 1;
	//		char* mem = new char[len];
	//		memset(mem, 0x0, len); // clear trash
	//		strncpy_s(mem, len, newcmd, strlen(newcmd));
			// alloc new memory for struct - set init value on index 0
			KeyCommandsState[k][0] = new SamurayX::SettingControls::KeyHandler::KeyCommand_t;
			memset(KeyCommandsState[k][0], 0x0, sizeof(SamurayX::SettingControls::KeyHandler::KeyCommand_t));
			// so we apply the init values
			KeyCommandsState[k][0]->Command = newcmd;//mem;
			KeyCommandsState[k][0]->LastUsed = 0x0;


			IW5Console::DebugOnConsole((strlen(command) + 80), "^5[^;SamurayX^5] New key ^6%s^5 registered to set ^6%s^5 ^2successfully^5!\n", key, newcmd);

			return 1; // 1 - success new key for command added success...
					
		}
		else
		{

		
				//	return 2; // value for command added success!
				//	return 3; // value for command already exists!
			
			std::map< int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*  >::iterator itb;


			if (!KeyCommandsState[k].size())
			{
				//int len = strlen(newcmd) + 1;
				//char* mem = new char[len];
				//memset(mem, 0x0, len); // clear trash
				//strncpy_s(mem, len, newcmd, strlen(newcmd));
				// alloc new memory for struct - set init value on index 0
				KeyCommandsState[k][0] = new SamurayX::SettingControls::KeyHandler::KeyCommand_t;
				memset(KeyCommandsState[k][0], 0x0, sizeof(SamurayX::SettingControls::KeyHandler::KeyCommand_t));
				// so we apply the init values
				KeyCommandsState[k][0]->Command = newcmd;//mem;
				KeyCommandsState[k][0]->LastUsed = 0x0;

				IW5Console::DebugOnConsole((strlen(command) + 80), "^5[^;SamurayX^5] New value ^6%s^5 for the key ^6%s^5 added ^2successfully^5!\n", newcmd, key);


				return 2;
			}
			else
			{
				// if len > 0
				int i = 0x0;
				std::map< int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*  >::iterator itb;

				int checked = 0x0;
				bool AddNew = false;

				while (1)
				{
					itb = KeyCommandsState[k].find(i);

					if (!AddNew)
					{
						if (itb != KeyCommandsState[k].end())
						{
							if (!strcmp(newcmd, KeyCommandsState[k][i]->Command))
							{
								IW5Console::DebugOnConsole((strlen(command) + 70), "^5[^;SamurayX^5] Value ^6%s^5 for the key ^6%s^5 already exists!\n", newcmd, key);
								delete[] newcmd;
 								return 3;
							}
							else
							{
								++checked;
							}
						}
						else if (checked == KeyCommandsState[k].size())
						{
							i = 0; // reorder stack from index 0 if it's possible...
							AddNew = true;
							continue;
						}
					}
					else if (itb == KeyCommandsState[k].end())
					{

					//	int len = strlen(newcmd) + 1;
					//	char* mem = new char[len];
					//	memset(mem, 0x0, len); // clear trash
					//	strncpy_s(mem, len, newcmd, strlen(newcmd));
						// alloc new memory for struct - set init value on index 0
						KeyCommandsState[k][i] = new SamurayX::SettingControls::KeyHandler::KeyCommand_t;
						memset(KeyCommandsState[k][i], 0x0, sizeof(SamurayX::SettingControls::KeyHandler::KeyCommand_t));
						// so we apply the init values
						KeyCommandsState[k][i]->Command = newcmd;//mem;
						KeyCommandsState[k][i]->LastUsed = 0x0;
						IW5Console::DebugOnConsole((strlen(command) + 80), "^5[^;SamurayX^5] New value ^6%s^5 for the key ^6%s^5 added ^2successfully^5!\n", newcmd, key);
						return 2;
					}
					i++;
				}

			}
		}
	}
	else if (!strcmp("remove", cmd) && k)
	{
		std::map<int, std::map<int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*> >::iterator it;

		it = KeyCommandsState.find(k);
		
		if (it != KeyCommandsState.end() && !strlen(newcmd))
		{
			std::map<int, KeyCommand_t*>::const_iterator st;
			for (st = KeyCommandsState[k].begin(); st != KeyCommandsState[k].end(); st++)
				if (st->second)
				{
					delete[] st->second->Command; // delete buff
					delete[] st->second; // delete struct
				}

			KeyCommandsState.erase(k);
			IW5Console::DebugOnConsole((strlen(command) + 90), "^5[^;SamurayX^5] All registers for the key ^6%s^5 has been deleted ^2successfully^5!\n", key);

			return 4; // key command deleted success!
		}
		else if (it != KeyCommandsState.end())
		{

			std::map< int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*  >::iterator itb;

			int i = 0x0;
			int checked = 0x0;
			while (KeyCommandsState[k].size())
			{

				itb = KeyCommandsState[k].find(i);

				if (itb != KeyCommandsState[k].end())
				{
					if (!strcmp(newcmd, KeyCommandsState[k][i]->Command))
					{
						
						delete[] KeyCommandsState[k][i]->Command; // command buff
						delete[] KeyCommandsState[k][i]; // struct
						KeyCommandsState[k].erase(i);

						IW5Console::DebugOnConsole((strlen(command) + 90), "^5[^;SamurayX^5] Value ^6%s^5 for the key ^6%s^5 has been deleted ^2successfully^5!\n", newcmd, key);

						delete[] newcmd; // current command
						return 5; // value for key command deleted sucess
					}
					else
					{
						++checked;
					}
				}

				if (checked == KeyCommandsState[k].size())
					break;//return 6;

				i++;
			}
			//return 6; // value for key command not found!
		}
	}
	if (strlen(newcmd))
		delete[] newcmd;

	return 0; // wrong command syntax
}

int SamurayX::SettingControls::KeyHandler::CurrentKey = 0;

void SamurayX::SettingControls::KeyHandler::KeyEventReader()
{
	
	// this must be called constantly... on atleast 1 ms delay between iteration

	
	// so we just start getting the current keys on state...
	//int CurrentKey = 0x0;
	
	CurrentKey = 0;

	for (int i = 0x1; i < 0xFF; i++)
	{
		if (i > 2)
		{
			if (GetAsyncKeyState(i) & 0x1) // LSB
			{
				CurrentKey = i;
				break;
			}
		}
	}
	std::map<int, std::map<int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*> >::iterator it;

	it = KeyCommandsState.find(CurrentKey);

	int LastUsedCount = 0x0;

	if (it != KeyCommandsState.end()) // key found
	{
		std::map<int, SamurayX::SettingControls::KeyHandler::KeyCommand_t*>::iterator itb;

		int size = KeyCommandsState[CurrentKey].size();

		int i = 0;

		int FirstCheckCount = 0x0;

		bool Executed = false;

		while (size)
		{
			itb = KeyCommandsState[CurrentKey].find(i);

			if (itb != KeyCommandsState[CurrentKey].end()) // command found
			{
				++FirstCheckCount;

				int LastUsed = KeyCommandsState[CurrentKey][i]->LastUsed;

				if (!LastUsed)
					IW5Console::OnRconCommand(1, KeyCommandsState[CurrentKey][i]->Command), KeyCommandsState[CurrentKey][i]->LastUsed = 1, Executed = true;


				int initsize = KeyCommandsState[CurrentKey].size();
				int CommandCount = 0;
				
				int index = 0x0;

				int SecondCheckCount = 0x0;
				while (initsize)
				{

					itb = KeyCommandsState[CurrentKey].find(index);

					if (itb != KeyCommandsState[CurrentKey].end())
					{
						++SecondCheckCount;

						if (KeyCommandsState[CurrentKey][index]->LastUsed)
							++CommandCount;

					}

					if (initsize == CommandCount)
					{
						// become all to 0
						int cmd = 0x0;
						// update init size
						initsize = KeyCommandsState[CurrentKey].size();

						int CIndexZero = 0x0;
						int SettedOnZero = 0x0;

						while (initsize)
						{
							itb = KeyCommandsState[CurrentKey].find(CIndexZero);

							if (itb != KeyCommandsState[CurrentKey].end()) // found
							{
								KeyCommandsState[CurrentKey][CIndexZero]->LastUsed = 0;
								++SettedOnZero;
							}


							if (initsize != KeyCommandsState[CurrentKey].size())
								return;
							if (SettedOnZero == initsize)
								return;

							CIndexZero++;
						}


					}


					if (initsize != KeyCommandsState[CurrentKey].size())
						return;

					if (SecondCheckCount == initsize)
						break;

					index++;

				}

				if (Executed)
					return;


			}



			// fix reorder of stack
			if (size != KeyCommandsState[CurrentKey].size())
				return;

			if (FirstCheckCount == size)
				return;

			i++;
		}


		return;
	}

}


int SamurayX::SettingControls::KeyHandler::ProcessRegisterKeyForCommand(char* command)
{
	int result = RegisterNewHotKeyForCommand(command);

	if (result > 0 && result < 6)
		return 1;

	return 0;
}

// algorithm faster and clean by 10101001 aka TheCyborgX...

void SamurayX::WorldRead::OrderDistancesMinToMaxByIndex(float* invec, int* order, int len)
{
	int cindex = 0;

	while (cindex < len)
	{
		for (int i = cindex; i < len; i++)
		{
			if (invec[i] < invec[cindex])
			{
				float bk = invec[cindex];
				invec[cindex] = invec[i];
				invec[i] = bk;

				int bkb = order[cindex];
				order[cindex] = order[i];
				order[i] = bkb;

			}
		}
		cindex++;
	}
}

int SamurayX::WorldRead::WorldList[MAX_ENTITIES] = { 0 };
int SamurayX::WorldRead::EnumWorldList = 0;

void SamurayX::WorldRead::InitWorldRead()
{
	// read world for esp and aimbot...
	int TargetIndex = 0;
	int WorldIndex = 0;
	float TargetsDistances[MAX_PLAYERS] = { 0.0f };
	int Targets[MAX_PLAYERS] = { 0 };
	int World[MAX_ENTITIES] = { 0 };

	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		
		if (i < MAX_PLAYERS)
		{
			
			IW5Engine::CEngine::centity_t cEntity = GetEntity(i);

			IW5Engine::CEngine::PlayerInfo_t cPlayerInfo = GetPlayerInfo(i);

			if (cEntity.Alive & 0x1 && cEntity.Status && (VectorDistance(GetEntity(GetCurrentClientNumber()).Origin, cEntity.Origin) < SamurayX::CBot::TargetMaxDistance) && cEntity.Type == ENTITY_PLAYER && i != GetCurrentClientNumber() && (cPlayerInfo.Team == TEAM_FFA || cPlayerInfo.Team != GetPlayerInfo(GetCurrentClientNumber()).Team))
			{
				TargetsDistances[TargetIndex] = VectorDistance(GetEntity(GetCurrentClientNumber()).Origin, cEntity.Origin);
				Targets[TargetIndex] = i;
				TargetIndex++;
			}
		}
		else
		{
			IW5Engine::CEngine::centity_t cEntity = GetEntity(i);

			if (cEntity.Type == ENTITY_WEAPON || cEntity.Type == ENTITY_ATTACHMENT_ATTACK)
			{
				if (cEntity.Alive & 0x1 && cEntity.Status)
				{
					World[WorldIndex] = i;
					WorldIndex++;
				}
			}
		}



	}

	for (int i = TargetIndex; i < MAX_PLAYERS; i++)
		TargetsDistances[i] = 100000.0f;; // 8192.0f

	OrderDistancesMinToMaxByIndex(TargetsDistances, Targets, TargetIndex);

	memcpy(SamurayX::Bot::TargetList, Targets, sizeof(Targets));
	memcpy(SamurayX::WorldRead::WorldList, World, sizeof(World));

	SamurayX::Bot::EnumTargets = TargetIndex;
	SamurayX::WorldRead::EnumWorldList = WorldIndex;
	
	
	return;
}



#pragma region INIT_SAMURAI_RAY_X_BOT



void __cdecl Settings::CESP::EnemyESP::Toggle(int v)
{
	Enabled = v & 1;
}

void __cdecl Settings::CESP::WeaponESP::Toggle(int v)
{
	Enabled = v & 1;
}

void Settings::LoadDefaultSettings()
{
	strcpy_s(Settings::CBot::CBone, 0x8, "default");
	/*
	IW5Console::SettingColor.BackgroundMax->SetRGBA(0, 0, 0, 100);
	IW5Console::SettingColor.BackgroundMin->SetRGBA(0, 0, 0, 100);
	IW5Console::SettingColor.Prefix->SetRGBA(0, 255, 255, 255);
	IW5Console::SettingColor.ScrollBarOver->SetRGBA(0, 0, 0, 0);
	IW5Console::SettingColor.ScrollBarUnder->SetRGBA(0, 0, 0, 0);
	IW5Console::SettingColor.TextMax->SetRGBA(255, 255, 255, 255);
	IW5Console::SettingColor.TextMin->SetRGBA(255, 255, 255, 255);
	IW5Console::SetConsolePrefix(0);
	*/
	/*
	Settings::CESP::EnemyESP::EnemyESPColor.SetRGBA(0, 255, 255, 255);
	Settings::CESP::WeaponESP::WeaponESPColor.SetRGBA(0, 255, 255, 255);
	*/
	Settings::CESP::EnemyESP::Enabled = false;
	Settings::CESP::WeaponESP::Enabled = false;
	IW5Console::Commands::Dvar::ProcessDvar("com_maxfps 0");


}





int SamurayX::Bot::TargetPoint = 0;

int Settings::UserCommands::TimeScale = 8;

int Settings::AntiAim::AntiAimMode = -1;
int Settings::AntiAim::AntiAimByRiot::AntiAimRiotTarget;

int Settings::CBot::Bot = 1;
int Settings::CBot::MaxBones = 5;
int Settings::CBot::CMultiBoneOrderNumber[19] = { 0 };
float Settings::Prediction::PredictionBySideMove::SideMoveScale = 2.0f;
float Settings::Prediction::PredictionByVelocity::VelocityScale = 0.2f;
float Settings::Prediction::PredictionByTime::TimeScale = 0.0f; 

float Settings::Prediction::LocalPrediction::OriginScaleXYZ = 0.008f;

// time in ms...
int Settings::Prediction::Interpolation::InterpolationTime = 100;
int Settings::Prediction::Interpolation::InterpolationOriginTime = 0;
int Settings::Prediction::Interpolation::InterpolationViewTime = 0;
float Settings::Prediction::Interpolation::InterpolationScale = 0.0f;


int Settings::Prediction::PredictionMode = 2;
char* Settings::CBot::CBone = new char[32];
short Settings::CBot::AimbotMode = 0;
bool Settings::CBot::AutoShoot = false;
Color_t Settings::CESP::EnemyESP::EnemyESPColor(0, 255, 255, 255);
Color_t Settings::CESP::WeaponESP::WeaponESPColor(0, 255, 255, 255);
bool Settings::CESP::WeaponESP::Enabled = false;
bool Settings::CESP::EnemyESP::Enabled = false;


int SamurayX::Bot::TargetList[MAX_PLAYERS] = { 0 };

int SamurayX::Bot::EnumTargets = 0;
float SamurayX::CBot::TargetMaxDistance = 8192.0f;
vec3_t SamurayX::Bot::TargetPos = { 0.0f };
vec3_t SamurayX::Bot::CurrentTargetPos = { 0.0f };


#pragma region TEST_0
/*

void CalcViewAnglesDif(float* outvec)
{
	typedef float vec9_t[9];

	typedef struct
	{
		int cg; //				//v22
		int k; // 0x4			//v23
		int fuk; // 0x8			//v24
		float fakoff; // 0xC		//v25
		float w0t;// 0x10			// v26
		float whatever; // 0x14	// v27
		float thisisk; // 0x18	// v28
		float thisisdk; // 0x1C	// v29

	} trash_t;

	vec3_t vAngles = { CG.ViewAngles[0], CG.ViewAngles[1], CG.ViewAngles[2] };

	vec9_t view;

	trash_t dick;

	dick.cg = 0x8FF100; // 0x0
	DWORD chekckdok = *(DWORD*)(0x8FF100 + 0x77070);  // sub_597740 // last impact time
	if (chekckdok)
		dick.k = chekckdok - *(DWORD*)(0x8FF100 + 0xA28); // from snap to cg
	else
		dick.k = 0;
						// time
						// frame init sub_42E2E0
	dick.fuk = *(DWORD*)(0x8FF100 + 0x6B15C) - *(DWORD*)(0x8FF100 + 0xA28); // server time - unk...
	dick.fakoff = *(float *)(0x8FF100 + 0x7708C); // sub_597740 // impact angle x
	dick.w0t = *(float *)(0x8FF100 + 0x77090); // sub_597740 // impact angle y
	dick.whatever = *(float *)(0x8FF100 + 0x77094); // flt_976194 = sub_4D8B80((int)dword_8FF100, dword_96A25C); // writted on 0x5A532A // velocity
	dick.thisisk = (float)((double)*(signed int *)(0x8FF100 + 0x6B158) * 0.001000000047497451); // frame init sub_42E2E0 // frame time  
	dick.thisisdk = *(float *)(0x8FF100 + 0x3314); // sway multiplier




	vec3_t weaponanimations;

	((void(__cdecl*)(trash_t*, vec3_t out))0x523460)(&dick, weaponanimations);

	vAngles[0] += weaponanimations[0];
	vAngles[1] += weaponanimations[1];
	vAngles[2] += weaponanimations[2];

	//AngleVectorsForwardInRadians
	((float*(__cdecl*)(float* in, float* out))0x473DD0)(vAngles, view);

	vec3_t FinalView;

	((void(__cdecl*)(float* in, float *out))0x43D370)(view, FinalView);


	outvec[0] = FinalView[0];
	outvec[1] = FinalView[1];
	outvec[2] = FinalView[2];

}

//vec3_t vecview;
*/
#pragma endregion

//DWORD cping;


vec3_t SamurayX::Bot::CDelta = { 0.0f, 0.0f, 0.0f };




void __cdecl SamurayX::Bot::InitBot()
{
	if (IsInGame())
		IW5Engine::CEngine::UpdateTargetEntityPoints();
	//CalcViewAnglesDif(vecview);


	static IW5Engine::CEngine::centity_t* cEntity;

	static float HigherPower = 0.0f;
	static float CurrentPower = 0.0f;

	HigherPower = 0.0f;

//	VectorCopy(RefDef.Origin, IW5Engine::CEngine::ViewOrigin);
//	if (IsInGamePlaying())
//		IW5Engine::CEngine::CalculateViewOrigin();

	IW5Engine::CEngine::CalculateViewOrigin(Settings::Prediction::LocalPrediction::OriginScaleXYZ);
	// local prediction for jump servers... beta

//	float velocityangle = (CG.VelocityGeneral & 0xFF) / 255.0f * 3.141592741012573f + (CG.VelocityGeneral & 0xFF) / 255.0f * 3.141592741012573f + 6.283185482025146f;
	/*
	float velocityangle = 6.283185482025146f;

	float vel = CG.VelocityLocal[2];
	if (vel < 0)
		vel *= -1;

	float angle = (vel * IW5Engine::CEngine::ViewOriginScaleZ);

	float velocityz = ((float(__cdecl*)(CEngine::cg_t*, float, float))0x489790)(&CG, velocityangle, angle);

	IW5Engine::CEngine::ViewOrigin[2] = CG.OffsetOrigin0[2] + CG.ViewOriginAngleExact;

	if (CG.VelocityLocal[2] > 0)
		IW5Engine::CEngine::ViewOrigin[2] -= velocityz;
	else if(CG.VelocityLocal[2] < 0)
		IW5Engine::CEngine::ViewOrigin[2] += velocityz;
	*/
	/*
	if (GetAsyncKeyState(VK_F2) & 0x1)
		IW5Console::DebugOnConsole(1024, "^5Closest target name: %s", (char*)((0x9FC748 + (0x560 * TargetList[0])) + 0xC));


	*/

	State::TargetFound = false;

	
	for (int i = 0; i < EnumTargets; i++)
	{
		cEntity = &GetEntity(TargetList[i]);

		if (cEntity->Alive & 0x1)
		{
			//IW5Engine::CEngine::GetEntityPosByTagNumber(TargetPos, cEntity, SamurayX::Bot::TargetPoint);
			
			//GetEntityPos(cEntity, SamurayX::Bot::TargetPoint, TargetPos);
			IW5Engine::CEngine::CMath::GetDobjBonePos(cEntity, SamurayX::Bot::TargetPoint, TargetPos);



			if (CurrentPower = IW5Engine::CEngine::CAwall::GetFinalBulletPower(TargetList[i], TargetPos) > HigherPower)
				HigherPower = CurrentPower, VectorCopy(TargetPos, CurrentTargetPos), State::TargetFound = true;

		}
	}


	if (State::TargetFound)
	{

	//	static vec3_t CDelta;
		
		VectorSubtract(CurrentTargetPos, IW5Engine::CEngine::ViewOrigin, CDelta);
		QMath::VectorAngles(CDelta, CDelta);


		Normalize2DAngle360Fast(ClientActive.ViewAngles);
		/*
		ClientActive.ViewAngles[0] += (CDelta[0] - CG.WeaponView[0]);
		ClientActive.ViewAngles[1] += (CDelta[1] - CG.WeaponView[1]);
		*/
		
		ShootWeapon();

		/*
		if (!SamurayX::Bot::State::LastShoot)
			ShootWeapon(), SamurayX::Bot::State::LastShoot = true;
		else
			SamurayX::Bot::State::LastShoot = false;
		*/
	}
	else
	{
		SamurayX::Bot::State::LastShoot = false;
	}

	


}



void __cdecl SamurayX::Bot::InitMultiBoneBot()
{
	if (IsInGame())
		IW5Engine::CEngine::UpdateTargetEntityPoints();
	

	static IW5Engine::CEngine::centity_t* cEntity;
	static vec3_t LastBestTarget = { 0.0f, 0.0f, 0.0f };

	static float HigherPower = 0.0f;
	static float CurrentPower = 0.0f;
	static float LastBestDepth = 0.0f;


	HigherPower = 0.0f;
	LastBestDepth = 0.0f;

//	IW5Engine::CEngine::CMath::CG_GetPlayerViewPoint(0, &CG, IW5Engine::CEngine::ViewOrigin);

	IW5Engine::CEngine::CalculateViewOrigin(Settings::Prediction::LocalPrediction::OriginScaleXYZ);



	State::TargetFound = false;


	for (int i = 0; i < EnumTargets; i++)
	{
		cEntity = &GetEntity(TargetList[i]);

		if (cEntity->Alive & 0x1)
		{
			for (int x = 0; x < Settings::CBot::MaxBones; x++)
			{
				//int c = SamurayX::CBot::CMultiBoneOrderNumber[x];
				//GetEntityPos(cEntity, c, TargetPos);
				IW5Engine::CEngine::CMath::GetDobjBonePos(cEntity, SamurayX::CBot::CMultiBoneOrderNumber[x], TargetPos);
				// fix z for j_helmet
				if (!x)
					TargetPos[2] -= 2;

				CurrentPower = IW5Engine::CEngine::CAwall::GetFinalBulletPower(TargetList[i], TargetPos);
				if (CurrentPower == 4.0f)
				{
					HigherPower = CurrentPower, VectorCopy(TargetPos, LastBestTarget);
					break;
				}
				else if (HigherPower == 0.0f && CurrentPower > 0.0f)
				{
					HigherPower = CurrentPower;
					VectorCopy(TargetPos, LastBestTarget);
				}
			}

			if (HigherPower > LastBestDepth)
				LastBestDepth = HigherPower, VectorCopy(LastBestTarget, CurrentTargetPos), State::TargetFound = true;

		}
	}


	if (State::TargetFound)
	{

		//	static vec3_t CDelta;

		VectorSubtract(CurrentTargetPos, IW5Engine::CEngine::ViewOrigin, CDelta);
		QMath::VectorAngles(CDelta, CDelta);


		Normalize2DAngle360Fast(ClientActive.ViewAngles);
		
		ShootWeapon();

		/*
		if (!SamurayX::Bot::State::LastShoot)
			ShootWeapon(), SamurayX::Bot::State::LastShoot = true;
		else
			SamurayX::Bot::State::LastShoot = false;
		*/
	}
	else
	{
		SamurayX::Bot::State::LastShoot = false;
	}



}


char* Settings::CBot::CMultiBoneOrder[19] = 
{
	"j_helmet",
	"j_wrist_le",
	"j_wrist_ri",
	"j_ankle_ri",
	"j_ankle_le",
	"j_spineupper",
	"j_spine4",
	"j_mainroot",
	"j_spinelower",
	"j_knee_le",
	"j_knee_ri",
	"j_head",
	"j_neck",
	"j_shoulder_le",
	"j_shoulder_ri",
	"j_elbow_le",
	"j_elbow_ri",	
	"j_hip_ri",
	"j_hip_le",
};
// states

bool SamurayX::Bot::State::LastShoot = false;
bool SamurayX::Bot::State::TargetFound = false;
int SamurayX::Bot::State::CurrentTarget = TARGET_NOT_FOUND;
bool SamurayX::ConsoleControl::OpenConsole = false;

#pragma endregion




void SamurayX::Init()
{
	// comment
	Possesser::PossessEngine();
	Settings::LoadDefaultSettings();
	IW5Engine::Init();
	


}


void SamurayX::MenuControl::ToggleMenu()
{
	OpenMenu ^= 1;
}


void SamurayX::ConsoleControl::ToggleConsole()
{
	OpenConsole ^= 1;
}


// this should be enough till i add rest...

void SamurayX::ConsoleControl::OnCommand(char* command)
{
	SamurayX::SettingControls::KeyHandler::ProcessRegisterKeyForCommand(command);
	IW5Console::Commands::Dvar::ProcessDvar(command);
	IW5Console::Commands::Default::ProcessCommand(command);
	IW5Console::Commands::Reliable::Execute(command);


}


void SamurayX::ConsoleControl::ClearConsole()
{
	return Console.ClearOutput();
}

// new console

CConsole SamurayX::ConsoleControl::Console(0.0f, 0.0f, 0.0f, 0.0f, Color_t(0, 0, 0, 100), Color_t(0, 255, 255, 255), Color_t(0, 255, 255, 0), 2046, 2046, &SamurayX::ConsoleControl::OnCommand);


bool SamurayX::MenuControl::OpenMenu = false;

void SamurayX::MenuControl::LoadCMenu()
{
	Menu.AddColumn("Aimbot");
	Menu.AddColumn("ESP");
	Menu.AddColumn("Accuracy");
	Menu.AddColumn("Misc");



	Menu.CControls.AddCheckBox("Test", 0, 0, 0, 30.0f, 30.0f, 0, &Settings::CBot::Bot, CMenu::Values_t::BOOL_T);
	Menu.CControls.AddProgressSlider("Interpolation \\ms", 0, 0, 1, 200.0f, 20.0f, 0.0f, 4000.0f, 0.0f, &Settings::Prediction::Interpolation::InterpolationTime, CMenu::Values_t::INT_T);
	Menu.CControls.AddCheckBox("ESP Weapons", 1, 0, 0, 30.0f, 30.0f, 0, &Settings::CESP::WeaponESP::Enabled, CMenu::Values_t::BOOL_T);
	Menu.CControls.AddProgressSlider("Interpolation scale", 2, 0, 0, 200.0f, 20.0f, 0.0f, 100.0f, 0.0f, &Settings::Prediction::Interpolation::InterpolationScale, CMenu::Values_t::FLOAT_T); 
	Menu.CControls.AddProgressSlider("Time scale", 0, 0, 2, 200.0f, 20.0f, 0.0f, 20.0f, 0.0f, &Settings::UserCommands::TimeScale, CMenu::Values_t::INT_T);
	Menu.CControls.AddItemSelector("Test", 0, 0, 3, 200.0f, 20.0f, &Settings::CBot::CBone, IW5Engine::CEngine::PlayerBonesName, 20, CMenu::Values_t::PCHAR_T);



}

CMenu SamurayX::MenuControl::Menu(0, 4, Color_t(0, 0, 0, 255), Color_t(0, 0, 0, 0), Color_t(0, 255, 255, 255), Color_t(255, 255, 255, 255));





/*
	Aimbot
	Accuaracy
	Esp
	Misc
*/





CConsole::CConsole(float x, float y, float szx, float szy, Color_t BackgroundColor, Color_t UpDownBorderColor, Color_t LeftRightBorderColor, size_t InputMaxSz, size_t OutputMaxSz, void(__cdecl* EventHandler)(char*))
{
	
	// Init
	this->SizeX = szx;
	this->SizeY = szy;
	this->X = x;
	this->Y = y;
	this->BackgroundColor = BackgroundColor;
	this->UpDownBorderColor = UpDownBorderColor;
	this->LeftRightBorderColor = LeftRightBorderColor;
	this->InBuffSz = InputMaxSz;
	this->OutBuffSz = OutputMaxSz;
	this->InputBuff = new char[InputMaxSz];
	this->OutputBuff = new char[OutputMaxSz];
	this->First = true;
	this->TextLines = 0;
	this->OnCommand = EventHandler;
	this->InBuffCount = 0;

	memset(this->InputBuff, 0x0, InputMaxSz);
	memset(this->OutputBuff, 0x0, OutputMaxSz);
}

/*

int CConsole::GetCurrentKey()
{
	int key = 0;

	if (msg.message == WM_KEYDOWN)
		key = msg.wParam;

	return key;
}
*/

int CConsole::GetCurrentVirtualKeyCode()
{
	//WORD key = 0;
	//int Result = 0;

	/*
	if (msg.message == WM_KEYDOWN)
	{
		byte States[256] = { 0 };
		GetKeyboardState(States);
		Result = ToAscii(msg.wParam, msg.lParam, States, &key, 0);
	}
	

	if (Result & 1)
		return key;
	*/
	// changed to engine default mode, due render gets fucked on calls to GetMessage();..
	static int lastime = 0;
	

	if (lastime != UserKeys.LastTimeKeyPressed) // fix delay, one time...
	{
		if (UserKeys.MessageEvent & UKE_WM_KEYDOWN)
		{
			lastime = UserKeys.LastTimeKeyPressed;

			return UserKeys.VirtualKeyCode;
		}
	}

	return 0;
}




void CConsole::Run()
{

	float MaxSizeX = this->SizeX;
	float MaxSizeY = this->SizeY;

	float SpaceX = (ScreenSize.x * 10.0f) / 100.0f;
	float SpaceY = (ScreenSize.y * 10.0f) / 100.0f;


	if (MaxSizeX > 0.0f)
	{
		if (MaxSizeX >= (ScreenSize.x - SpaceX))
			MaxSizeX = (ScreenSize.x - SpaceX);
	}
	else
	{
		MaxSizeX = (ScreenSize.x - SpaceX); // fix center
	}

	if (MaxSizeY > 0.0f)
	{
		if (MaxSizeY >= (ScreenSize.y - SpaceY))
			MaxSizeY = (ScreenSize.y - SpaceY);
	}
	else
	{
		MaxSizeY = (ScreenSize.y - SpaceY); // fix center
	}


	float StartX = 0.0f;
	float StartY = 0.0f;

	if (this->X > 0.0f)
		StartX = this->X;
	else
		StartX = ((ScreenSize.x - MaxSizeX) / 2.0f);


	if (this->Y > 0.0f)
		StartY = this->Y;
	else
		StartY = ((ScreenSize.y - MaxSizeY) / 2.0f);




	Color_t Background = this->BackgroundColor;


	// draw background
	CDrawShader(StartX, StartY, MaxSizeX, MaxSizeY, 0.0f, 0.0f, 0.0f, 0.0f, &Background, GetWhiteShader());

	

	// init draw borders

	Color_t BorderUpDown = this->UpDownBorderColor;


	// up
	CDrawShader(StartX, StartY, MaxSizeX, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, &BorderUpDown, GetWhiteShader());

	// down
	CDrawShader(StartX, (StartY + MaxSizeY), MaxSizeX, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, &BorderUpDown, GetWhiteShader());



	Color_t BorderLeftRight = this->LeftRightBorderColor;

	// left
	CDrawShader(StartX, (StartY + 2.0f), 2.0f, (MaxSizeY - 2.0f), 0.0f, 0.0f, 0.0f, 0.0f, &BorderLeftRight, GetWhiteShader());

	// right
	CDrawShader(((StartX + MaxSizeX) - 2.0f), (StartY + 2.0f), 2.0f, (MaxSizeY - 2.0f), 0.0f, 0.0f, 0.0f, 0.0f, &BorderLeftRight, GetWhiteShader());


	// init draw buffers

	// calculate for center buffers and don't exceed design limits...

	float SpaceBX = (MaxSizeX * 10.0f) / 100.0f;
	float SpaceBY = (MaxSizeY * 20.0f) / 100.0f;

	// subtract space
	float MaxSizeBX = MaxSizeX - SpaceBX;
	float MaxSizeBY = MaxSizeY - SpaceBY;

	this->bMaxSizeX = MaxSizeBX; // for add text to buff

	float InputBuffSizeY = (GetFontHeightByScaleOne(GetConsoleFont()) + ((MaxSizeBY * 10.0f) / 100.0f));


	// start point of draw
	float StartBX = StartX + (SpaceBX / 2.0f);
	float StartBY = StartY + (SpaceBY / 2.0f);

	// test color
	Color_t white(255, 255, 255, 255);

	// draw prefix


	// added round for fix font color render acc...

	char prefix[] = "> ";
	const char* cursor = "_";

	CDrawText(prefix, 3, GetConsoleFont(), round(StartBX), round(StartBY), 1.0f, 1.0f, 0.0f, &white, 0);


	int SizeUsed = GetTextWidth("> _", 4, GetConsoleFont()) + GetTextWidth(this->InputBuff, 0, GetConsoleFont());

	// init add key to buff



	//GetMessage(&this->msg, GetActiveWindow(), NULL, NULL); //


	int CurrentKey = GetCurrentVirtualKeyCode();





	
	if (SizeUsed < MaxSizeBX)
	{
		int Key = CurrentKey;


		if (Key > 0x1F && Key < 0x7F)
		{
			InputBuff[InBuffCount] = (char)Key; // shut up
			InBuffCount++;
		}

	}
	
	/*
	if (SizeUsed < MaxSizeBX)
	{
		int Key = CurrentKey;


		if (Key > 0x1F && Key < 0x7F)
		{
			InputBuff[InBuffCount] = (char)Key; // shut up
			InBuffCount++;
			InputBuff[InBuffCount] = CPrefix;
		}

	}
	*/

	// delete function...
	
	if (CurrentKey == VK_BACK)
	{
		if (InBuffCount > 0) // k anti-violation...
		{
			if (InputBuff[(InBuffCount - 1)])
			{
				InputBuff[(InBuffCount - 1)] = 0x0;
				InBuffCount--;
			}
		}
	}
	
	/*
	if (CurrentKey == VK_BACK)
	{
		if (InBuffCount > 0) // k anti-violation...
		{
			if (InputBuff[(InBuffCount - 1)])
			{
				InputBuff[(InBuffCount - 1)] = CPrefix;
				InputBuff[(InBuffCount)] = 0x0;
				InBuffCount--;
			}
		}
	}
	*/

	if (CurrentKey == VK_RETURN)
	{
		if (InBuffCount > 0)
		{
			AddTextToBuff(InputBuff);
			
			if (OnCommand)
				OnCommand(InputBuff);

			ClearInput();
		}
	}

	// i will add left right cursor move if i'm not so lazy later...

	// fix... continue to game WindProc messages...	

	//TranslateMessage(&this->msg);
	//DispatchMessage(&this->msg);

	// init draw input buffer



	CDrawText(this->InputBuff, strlen(this->InputBuff), GetConsoleFont(), round(StartBX + GetTextWidth(prefix, 4, GetConsoleFont())), round(StartBY), 1.0f, 1.0f, 0.0f, &white, 0);



	/*
	char debug[1024];
	sprintf_s(debug, sizeof(debug), "VK: %d | Ev: %d | Time: %d", UserKeys.VirtualKeyCode, UserKeys.MessageEvent, UserKeys.LastTimeKeyPressed);
	CDrawText(debug, strlen(debug), GetConsoleFont(), 200.0f, 80.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);
	*/

	int OutputBuffMaxLines = (int)((MaxSizeBY - InputBuffSizeY) / GetFontHeightByScaleOne(GetConsoleFont()));

	this->bMaxLines = OutputBuffMaxLines; // add text to buff

	// test space
	//CDrawShader(StartBX, StartBY, MaxSizeBX, MaxSizeBY, 0.0f, 0.0f, 0.0f, 0.0f, &TestColor, GetWhiteShader());

	// init draw output buffer


	CDrawText(this->OutputBuff, strlen(this->OutputBuff), GetConsoleFont(), round(StartBX), round((StartBY + InputBuffSizeY)), 1.0f, 1.0f, 0.0f, &white, 0);
	
	// tmp clear
	//if (GetAsyncKeyState(VK_F4) & 0x1)
	//	ClearOutput();


	UserCmdButtons = BUTTON_USING_CAPTION;

	if (UserCaption & CAPTION_CHAT)
		UserCaption &= ~CAPTION_CHAT;


	this->Init = true;
}



void CConsole::AddTextToBuff(char* text)
{
	if (!this->Init)
		return;

	char** Lines = 0;
	Lines = new char*[this->bMaxLines];
	memset(&Lines[0], 0x0, (sizeof(char*) * this->bMaxLines));
	int tmplen = strlen(text) + 1 + this->bMaxLines; // future end of lines
	char* tmp = new char[tmplen];
	memset(tmp, 0x0, tmplen);
	char* result = tmp;
	int CBuffLines = 0;	
	int it = 0;
	int cit = 0;

	while (1)
	{

		if (text[it] != 0xA && text[it])
		{
			tmp[cit] = text[it];

			if (GetTextWidth(tmp, strlen(tmp), GetConsoleFont()) < this->bMaxSizeX)
			{
				cit++;
			}
			else
			{
				tmp[cit] = 0xA;
				tmp[cit + 1] = text[it];
				tmp = &tmp[cit + 2];
				cit = 0;
				CBuffLines++;
			}
		}
		else if (text[it] == 0xA)
		{
			tmp[cit] = text[it];
			tmp = &tmp[cit + 1];
			cit = 0;
			CBuffLines++;
		}
		else
		{
			// ...
			tmp[cit] = 0xA; // default standard end of line...
			tmp[cit + 1] = 0x0;
			CBuffLines++;
			break;
		}

		it++;
	}

	tmp = result;

	
	
	//memcpy(tmp, text, strlen(text));



	// updated to new version k

	int baseMaxLines = (this->bMaxLines - 1);

	// CBuff line parser...
	int i = strlen(this->OutputBuff) - 1;
	char* pBuff = this->OutputBuff;
	int L = 0;
	char* pEnd = &pBuff[0];
	int LineCount = 0;

	while (1)
	{

		if (pBuff[i] == 0xA)
		{
			int b = (i - 1);
			int Len = 1; // less current end of line
			char* p = 0;
			while (1)
			{

				if (&pBuff[b] >= pEnd && pBuff[b] != 0xA)
					Len++, p = &pBuff[b];
				else
					break;

				b--;
			}

			if (p)
			{
				char* d = new char[Len + 1];
				memset(d, 0x0, (Len + 1));
				Lines[L] = d;
				memcpy(Lines[L], p, Len);
				L++;
			}
			else if (Len) // fix not secure for lines without text
			{
				char* d = new char[2]; // [ 0xA, 0x0 ]
				d[0] = 0xA;
				d[1] = 0x0;
				Lines[L] = d;
				L++;
			}
			if (L > baseMaxLines)
				break;
		}

		if (&pBuff[i] == pEnd)
			break;

		i--;
	}


	LineCount = L;
	

	char** clines = 0;
	clines = new char*[this->bMaxLines];
	memset(clines, 0x0, (sizeof(char*) * this->bMaxLines));

	for (int i = (LineCount - 1), c = 0; i > -1; i--, c++)
		clines[c] = Lines[i];





	
	// first line
	//static int TextLines = 0; // current text lines used

	memset(this->OutputBuff, 0x0, this->OutBuffSz);

	if (First)
	{
		// add default lines
		// fix : bMaxLines - CBuffLines
		int rLines = baseMaxLines - CBuffLines;
		memset(this->OutputBuff, 0xA, rLines);
		memcpy(&this->OutputBuff[rLines], result, strlen(result));
	
		// increase CBuff lines

	//	TextLines += CBuffLines;
		First = false;
	}
	else
	{
		char* LastStart = this->OutputBuff;

		for (int i = CBuffLines; i < LineCount; i++)
		{
			int CLen = strlen(clines[i]);
			memcpy(LastStart, clines[i], CLen);
			LastStart += CLen;
		}

		// copy current line
		memcpy(LastStart, result, strlen(result));

	}





	// clear useless memory buffers
	
	delete[] tmp;


	for (int i = 0; i < (this->bMaxLines); i++)
		delete[] Lines[i];

	
	delete[] Lines;
	delete[] clines;
	
}


// clear complete console, the console it's limited to this->bMaxLines

void CConsole::ClearOutput()
{
	memset(this->OutputBuff, 0x0, this->OutBuffSz);
	this->First = true;
	this->TextLines = 0;
}

void CConsole::ClearInput()
{
	memset(this->InputBuff, 0x0, this->InBuffSz);
	this->InBuffCount = 0;
}


CConsole::~CConsole()
{
	delete[] this->OutputBuff;
	delete[] this->InputBuff;
}





// 


//
/*
void CMenu::Controls::OrderControls()
{
	
	float StartControlsX = this->in->ControlsSpace.MinX;
	float StartControlsY = this->in->ControlsSpace.MinY;
	float ControlsMaxSizeX = this->in->ControlsSpace.MaxX;
	float ControlsMaxSizeY = this->in->ControlsSpace.MaxY;
	
	//SX_DebugMsgBox("StartX: %f | StartY: %f", StartControlsX, StartControlsY);

	float HeightOfLineY = ControlsMaxSizeY / this->in->ControlMaxLines;

	std::vector< std::vector<void*> > _CList(this->in->ColumnsLen);

	for (unsigned int i = 0; i < this->in->ControlListCount; i++)
	{
		byte ControlColumn = ((byte*)(this->in->ControlList[i]))[1];
		_CList[ControlColumn].push_back(this->in->ControlList[i]);
	}

	for (unsigned int i = 0; i < this->in->ColumnsLen; i++)
	{
		std::vector< std::vector<void*> > CurrentControlLine(this->in->ControlMaxLines);

		
		
		
		//column1:	___________________________
		//	Line0	|__c1__|___c2____|__c3____|				
		//	Line1	|__c1__|___c2____|__c3____|
		//
		//column2:	___________________________
		//	Line0	|__c1__|___c2____|__c3____|				
		//	Line1	|__c1__|___c2____|__c3____|
		
		


		for (unsigned int x = 0; x < _CList[i].size(); x++)
		{
			void* control = _CList[i][x];
			if (control)
			{
				int Line = ((byte*)control)[2];
				CurrentControlLine[Line].push_back(control);
			}
		}

		
		for (unsigned int y = 0; y < this->in->ControlMaxLines; y++)
		{
			size_t ControlsNumberOnCurrentLine = CurrentControlLine[y].size();
			float ControlSizeX = 0.0f;

			if (ControlsNumberOnCurrentLine > 0)
				ControlSizeX = ControlsMaxSizeX / ControlsNumberOnCurrentLine;

			float HeightOfControlY = ControlsMaxSizeY / this->in->ControlMaxLines;
			
			float LastLineXSz = 0.0f;

			for (unsigned int z = 0; z < ControlsNumberOnCurrentLine; z++)
			{
				void* pControl = CurrentControlLine[y][z];
				
				if (pControl)
				{
					unsigned char ControlType = *(unsigned char*)pControl & 0xFF;

					switch (ControlType)
					{
						case Types::CHECK_BOX:
						{
							Types::CheckBox_t* _pControl = (Types::CheckBox_t*)pControl;
							float DefaultMaxXY = _pControl->DesignLimits.MaxSizeXY;
							float MaxX = 0.0f; // sz
							float MaxY = 0.0f; // sz

							if (ControlSizeX > DefaultMaxXY && HeightOfControlY > DefaultMaxXY)
								MaxX = DefaultMaxXY, MaxY = DefaultMaxXY;
							else
								MaxX = std::fmin(ControlSizeX, HeightOfControlY), MaxY = MaxX;

							float StartX = (ControlSizeX * z);
							float StartY = (HeightOfControlY * y);

							_pControl->DesignLimits.c.MinX = (StartControlsX + StartX);
							_pControl->DesignLimits.c.MinY = (StartControlsY + StartY);
							_pControl->DesignLimits.c.MaxX = MaxX;
							_pControl->DesignLimits.c.MaxY = MaxY;
							//SX_DebugMsgBox("Check box found at: X: %d Y: %d", z, y);
							break;
						}
						case Types::PROGRESS_SLIDER_BAR:
						{

							break;
						}
						case Types::ITEM_SELECTOR:
						{

							break;
						}


						default:
							break;
					}


				}

			}


		}




		

	}

}
*/

void CMenu::Controls::OrderControls()
{

	float StartControlsX = this->in->ControlsSpace.MinX;
	float StartControlsY = this->in->ControlsSpace.MinY;
	float ControlsMaxSizeX = this->in->ControlsSpace.MaxX;
	float ControlsMaxSizeY = this->in->ControlsSpace.MaxY;


	// improve performance sorting out only current item controls

	std::vector<void*> CItemControlList;

	// add current controls
	for (unsigned int i = 0; i < this->in->ControlListCount; i++)
		if (this->in->ControlList[i] && ((byte*)this->in->ControlList[i])[1] == this->in->CListIndex)
			CItemControlList.push_back(this->in->ControlList[i]);


	std::map<int, std::map<int, void*> > SortedControlList;


	for (unsigned int i = 0; i < CItemControlList.size(); i++)
	{
		unsigned char Column = ((byte*)CItemControlList[i])[2];
		unsigned char Row = ((byte*)CItemControlList[i])[3];
		SortedControlList[Row][Column] = CItemControlList[i];
	}

	size_t LenOfRows = SortedControlList.size();

	float UniqueControlSizeY = 0.0f;
	if (LenOfRows > 0)
		UniqueControlSizeY = ControlsMaxSizeY / LenOfRows;
	

	for (auto ity = SortedControlList.begin(); ity != SortedControlList.end(); ity++)
	{
		for (auto itx = SortedControlList[ity->first].begin(); itx != SortedControlList[ity->first].end(); itx++)
		{
			unsigned int CurrentLenOfColumns = SortedControlList[ity->first].size();
			unsigned int Column = itx->first;
			unsigned int Row = ity->first;
			float UniqueControlSizeX = ControlsMaxSizeX;
			if (CurrentLenOfColumns > 0)
				UniqueControlSizeX /= CurrentLenOfColumns;
			
			ControlDesignLimits_t* DesignLimits = (ControlDesignLimits_t*)&(((int*)itx->second)[1]);
			
			float RequestedMaxSizeX = DesignLimits->MaxSizeX;
			float RequestedMaxSizeY = DesignLimits->MaxSizeY;

			if (RequestedMaxSizeX > UniqueControlSizeX)
				DesignLimits->c.MaxX = UniqueControlSizeX;
			else
				DesignLimits->c.MaxX = RequestedMaxSizeX;

			if (RequestedMaxSizeY > UniqueControlSizeY)
				DesignLimits->c.MaxY = UniqueControlSizeY;
			else
				DesignLimits->c.MaxY = RequestedMaxSizeY;


			/*
			float RequestedMaxSizeXY = DesignLimits->MaxSizeXY;
			if (RequestedMaxSizeXY > UniqueControlSizeX || RequestedMaxSizeXY > UniqueControlSizeY)
				DesignLimits->c.MaxX = UniqueControlSizeX, DesignLimits->c.MaxY = UniqueControlSizeY;
			else
				DesignLimits->c.MaxX = RequestedMaxSizeXY, DesignLimits->c.MaxY = RequestedMaxSizeXY;
			*/

			DesignLimits->c.MinX = StartControlsX + (UniqueControlSizeX * Column);
			DesignLimits->c.MinY = StartControlsY + (UniqueControlSizeY * Row);

		}
	}

}



void CMenu::Controls::DrawControl(Control_t p)
{
	unsigned char ControlType = *(unsigned char*)p & 0xFF;


	switch (ControlType)
	{
		case Types::CHECK_BOX:
		{
			Types::CheckBox_t* pControl = (Types::CheckBox_t*)p;

			float StartControlX = pControl->DesignLimits.c.MinX;
			float StartControlY = pControl->DesignLimits.c.MinY;
			float MaxControlX = pControl->DesignLimits.c.MaxX;
			float MaxControlY = pControl->DesignLimits.c.MaxY;

			float SpaceX = (MaxControlX * 5.0f) / 100.0f;
			float SpaceY = (MaxControlX * 10.0f) / 100.0f;

			MaxControlX -= SpaceX;
			MaxControlY -= SpaceY;

			StartControlX += (SpaceX / 2.0f);
			StartControlY += (SpaceY / 2.0f);


			float BorderSize = 1.0f;
			float Width = MaxControlX;
			float Height = MaxControlY / 2.0f;



			// |

			static Color_t White(255, 255, 255, 255);

			CDrawShader(StartControlX, StartControlY, BorderSize, Height, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());

			//  __
			// |
			CDrawShader(StartControlX, StartControlY, Width, BorderSize, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());

			// __
			//|  |

			CDrawShader((StartControlX + Width), StartControlY, BorderSize, (Height + BorderSize), 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());

			// __
			//|__|
		
			CDrawShader(StartControlX, (StartControlY + Height), Width, BorderSize, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());
			

			float FillStateSizeX = Width / 2.0f;
			float FillStateSizeY = (Height - BorderSize);

			static Color_t Cyan(0, 200, 255, 255);

			if (pControl->value)
				CDrawShader((StartControlX + BorderSize), (StartControlY + BorderSize), FillStateSizeX, FillStateSizeY, 0.0f, 0.0f, 0.0f, 0.0f, &Cyan, GetWhiteShader());
			else
				CDrawShader((((StartControlX + BorderSize) + FillStateSizeX) - BorderSize), (StartControlY + BorderSize), FillStateSizeX, FillStateSizeY, 0.0f, 0.0f, 0.0f, 0.0f, &Cyan, GetWhiteShader());

			// read design limits of control on mouse event
			pControl->DesignLimits.s.MinX = StartControlX;
			pControl->DesignLimits.s.MinY = StartControlY;
			pControl->DesignLimits.s.MaxX = (StartControlX + Width);
			pControl->DesignLimits.s.MaxY = (StartControlY + Height);


			// tmp test

			//CDrawText("Test!", 5, GetConsoleFont(), 200.0f, 100.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);
			//CDrawShader(200.0f, 100.0f, 8.0f, 8.0f, 0.0f, 0.0f, 1.0f, 1.0f, &TestColor, *(int*)0x58DD378);
			//CDrawShader(200.0f, 140.0f, 8.0f, 8.0f, 0.0f, 0.0f, 1.0f, 1.0f, &TestColor, *(int*)0x58DD37C);
			/*
			static int pShader = 0;

			if (!pShader)
				pShader = ((int(__cdecl*)(char*))0x41B6B0)("demo_play");

			static float a = 0.0f, b = 0.0f;

			if (pShader)
				CDrawShader(200.0f, 140.0f, 15.0f, 15.0f, a, b, 1.0f, 1.0f, &TestColor, pShader);

			char buff[256];
			sprintf_s(buff, sizeof(buff), "A: %f | B: %f", a, b);
			if (GetAsyncKeyState(VK_NUMPAD0) & 1)
				a += 1.0f;
			if (GetAsyncKeyState(VK_NUMPAD1) & 1)
				a -= 1.0f;

			if (GetAsyncKeyState(VK_NUMPAD2) & 1)
				b += 1.0f;

			if (GetAsyncKeyState(VK_NUMPAD3) & 1)
				b -= 1.0f;

			CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 100.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);

			*/

			break;
		}
		case Types::PROGRESS_SLIDER_BAR:
		{
			// ...

			Types::ProgressSlider_t* pControl = (Types::ProgressSlider_t*)p;


			float StartControlX = pControl->DesignLimits.c.MinX;
			float StartControlY = pControl->DesignLimits.c.MinY;


			//CDrawShader(StartControlX, StartControlY, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, &TestColor, GetWhiteShader());

			float MaxControlSizeX = pControl->DesignLimits.c.MaxX;
			float MaxControlSizeY = pControl->DesignLimits.c.MaxY;

			// just in case if add a text display, otherwise the control will on center... 
			// this just will avoid to control stay over min border design but no on center...
			float SpaceControlX = (MaxControlSizeX * 10.0f) / 100.0f;
			float SpaceControlY = (MaxControlSizeY * 5.0f) / 100.0f;


			if (!pControl->pText)
			{
				MaxControlSizeX -= SpaceControlX;
				MaxControlSizeY -= SpaceControlY;

				StartControlX += SpaceControlX / 2.0f;
				StartControlY += SpaceControlY / 2.0f;
			}
			else
			{
				MaxControlSizeX -= SpaceControlX;
				MaxControlSizeY -= SpaceControlY;

				StartControlX += SpaceControlX / 2.0f;
				StartControlY += SpaceControlY;
			}


			float Width = MaxControlSizeX;
			float Height = MaxControlSizeY / 2.0f;
			float BorderSize = 1.0f;

			// ____________________________________________
			// 

			static Color_t White(255, 255, 255, 255);

			CDrawShader(StartControlX, StartControlY, Width, BorderSize, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());

			//  ____________________________________________
			// |

			CDrawShader(StartControlX, StartControlY, BorderSize, Height, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());

			//  ____________________________________________
			// |											|

			CDrawShader((StartControlX + Width), StartControlY, BorderSize, (Height + BorderSize), 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());


			//  ____________________________________________
			// |____________________________________________|

			CDrawShader(StartControlX, (StartControlY + Height), Width, BorderSize, 0.0f, 0.0f, 0.0f, 0.0f, &White, GetWhiteShader());


			// draw fill

			float Fill = 0.0f;

			if (pControl->Value > 0.0f)
				Fill = pControl->Value - (StartControlX + BorderSize);

			static Color_t Cyan(0, 200, 255, 255);


			CDrawShader((StartControlX + BorderSize), (StartControlY + BorderSize), Fill, (Height - BorderSize), 0.0f, 0.0f, 0.0f, 0.0f, &Cyan, GetWhiteShader());


			// read design limits of control on mouse event
			pControl->DesignLimits.s.MinX = round(StartControlX); // fix negative value
			pControl->DesignLimits.s.MinY = StartControlY;
			pControl->DesignLimits.s.MaxX = (StartControlX + Width);
			pControl->DesignLimits.s.MaxY = (StartControlY + Height);

			/*
			char buff[256];
			sprintf_s(buff, sizeof(buff), "Porcentage: %f", (((pControl->Value - StartControlX + BorderSize) * 100.0f) / ((StartControlX + BorderSize + Width) - (StartControlX + BorderSize))) );

			CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 100.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);
			*/
			/*
			char buff[256];


			int* pFont = (int*)0x58DD3B0;
			static int Index = 0;

			if (GetAsyncKeyState(VK_NUMPAD0) & 0x1)
				Index++;
			if (GetAsyncKeyState(VK_NUMPAD1) & 0x1)
				Index--;


			sprintf_s(buff, sizeof(buff), "Percentage value: %f | Off: %X", CValueTest, &pFont[Index]);



			CDrawText(buff, strlen(buff), pFont[Index], 200.0f, 200.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);
			*/


			Controls::Types::Value_t * t = (Controls::Types::Value_t*)((int)pControl + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t));

			char buff[256];
			
			if (t->T == Values_t::BOOL_T || t->T == Values_t::INT_T)
				sprintf_s(buff, sizeof(buff), " %d", CValue<int>::SecureReadValue(pControl));
			else if (t->T == Values_t::FLOAT_T)
				sprintf_s(buff, sizeof(buff), " %f", CValue<float>::SecureReadValue(pControl));

			// tmp log
			CDrawText(buff, strlen(buff), GetObjectiveFont(), (StartControlX + Width + BorderSize), (StartControlY + Height + BorderSize), 0.40f, 0.40f, 0.0f, &White, 0);



			//
			if (pControl->pText)
				CDrawText(pControl->pText, strlen(pControl->pText), GetObjectiveFont(), (StartControlX + BorderSize), (StartControlY - SpaceControlY), 0.40f, 0.40f, 0.0f, &White, 0);
			
			break;
		}
		case Types::ITEM_SELECTOR:
		{
			// ...
			Types::ItemSelector_t* pControl = (Types::ItemSelector_t*)p;

			float StartControlX = pControl->DesignLimits.c.MinX;
			float StartControlY = pControl->DesignLimits.c.MinY;
			float MaxControlSizeX = pControl->DesignLimits.c.MaxX;
			float MaxControlSizeY = pControl->DesignLimits.c.MaxY;

			Controls::Types::Value_t * t = (Controls::Types::Value_t*)((int)pControl + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t));

			float MaxTextSizeX = ((MaxControlSizeX * 80.0f) / 100.0f);
			float MaxButtonsSizeX = MaxControlSizeX - MaxTextSizeX;

			float SizeOfUniqueButtonX = MaxButtonsSizeX / 2.0f;

			//float StartTextX = StartControlX + SizeOfUniqueButtonX;


			switch (t->T)
			{
				case Values_t::PCHAR_T:
				{
					unsigned int CTxtIndex = pControl->vIndex;
					if (CTxtIndex >= pControl->VecSz)
						CTxtIndex = 0;

					if (pControl->pVec)
					{
						float CharXY = 0.40f;
						char* pText = ((char**)pControl->pVec)[CTxtIndex];
						int cw = GetTextWidth("A", 1, GetObjectiveFont());
						float cW0 = (((float)cw * 0.50f) / 1.0f);
						//float pTxtW = (float)strlen(pText) * cW0;

						float FinalTextWidth = 0.0f;
						unsigned int MaxLen = 0; 
						if (cW0)
							MaxLen = (int)MaxTextSizeX / (int)cW0;

						if (MaxLen >= strlen(pText))
							MaxLen = strlen(pText);

						float StartTextX = (StartControlX + SizeOfUniqueButtonX) + ((MaxTextSizeX - (cW0 * MaxLen)) / 2.0f);

						// draw current text on a max len
						float StartTextY = StartControlY + ((MaxControlSizeY - cW0) / 2.0f);

						static Color_t White(255, 255, 255, 255);

						

						// draw buttons
						float ButtonSizeXY = 10.0f;

						if (ButtonSizeXY > MaxButtonsSizeX)
							ButtonSizeXY = MaxButtonsSizeX;


						float StartButton0X = StartControlX;
						float StartButton1X = StartControlX + MaxTextSizeX + ButtonSizeXY;

						float StartButtonsY =  StartControlY + ((MaxControlSizeY - ButtonSizeXY) / 2.0f);


						static int ButtonShader = 0;

						if(!ButtonShader)
							ButtonShader = IW5Engine::CEngine::LoadShader("demo_play");

						if (ButtonShader)
						{
							CDrawShader(StartButton0X, StartButtonsY, ButtonSizeXY, ButtonSizeXY, 2.0f, 0.0f, 1.0f, 1.0f, &TestColor, ButtonShader);
							CDrawShader(StartButton1X, StartButtonsY, ButtonSizeXY, ButtonSizeXY, 0.0f, 0.0f, 1.0f, 1.0f, &TestColor, ButtonShader);
							CDrawText(pText, MaxLen, GetObjectiveFont(), StartTextX, (StartButtonsY + ButtonSizeXY), CharXY, CharXY, 0.0f, &White, 0);


							// apply design limits

							pControl->DesignLimits.s.MinX = StartButton0X; // i will divide on 2... otherwise i will need change struct standard...
							pControl->DesignLimits.s.MinY = StartButtonsY;
							pControl->DesignLimits.s.MaxX = (StartButton0X + MaxControlSizeX);
							pControl->DesignLimits.s.MaxY = (StartButtonsY + ButtonSizeXY);


						}




					}


					break;
				}
				case Values_t::FLOAT_T:
				{



					break;
				}



			default:
				break;
			}



			break;
		}


		default:
			break;
	}

}





void CMenu::Controls::AddCheckBox(char* CaptionText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, int DefaultValue, void* TargetToChange, Values_t _pV_t)
{
	Types::CheckBox_t* p = new Types::CheckBox_t(CaptionText, IndexOfItem, C, R, MaxX, MaxY, DefaultValue, TargetToChange, _pV_t);
	this->in->ControlList[this->in->ControlListCount] = p;
	this->in->ControlListCount++;
}

CMenu::Controls::Types::CheckBox_t::CheckBox_t(char* pText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, int dValue, void* pTargetToWrite, Values_t pV_t) : _T(CHECK_BOX), _I(IndexOfItem), _C(C), _R(R), pV({ pV_t, pTargetToWrite })
{
	const int CLen = strlen(pText) + 1;
	this->Text = new char[CLen];
	strcpy_s(this->Text, CLen, pText);
	this->value = dValue;
	//this->Target = pTargetToWrite;
	this->DesignLimits.MaxSizeX = MaxX;
	this->DesignLimits.MaxSizeY = MaxY;


}



void CMenu::Controls::AddProgressSlider(char* pTxt, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, float MinState, float MaxState, float DefualtValue, void* pTargetToWrite, Values_t _pV_t)
{
	Types::ProgressSlider_t* p = new Types::ProgressSlider_t(pTxt, IndexOfItem, C, R, MaxX, MaxY, MinState, MaxState, DefualtValue, pTargetToWrite, _pV_t);
	this->in->ControlList[this->in->ControlListCount] = p;
	this->in->ControlListCount++;
}


CMenu::Controls::Types::ProgressSlider_t::ProgressSlider_t(char* pTxt, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, float MinState, float MaxState, float DefaultValue, void* pTargetToWrite, Values_t pV_t) : _T(PROGRESS_SLIDER_BAR), _C(C), _R(R), _I(IndexOfItem), pV({ pV_t, pTargetToWrite })
{

	this->Min = MinState;
	this->Max = MaxState;
	this->Value = DefaultValue;
	this->DesignLimits.MaxSizeX = MaxX;
	this->DesignLimits.MaxSizeY = MaxY;
	this->Target = pTargetToWrite;
	if (pTxt)
		this->pText = new char[strlen(pTxt) + 1], strcpy_s(this->pText, (strlen(pTxt) + 1), pTxt);
	else
		this->pText = 0;

}


CMenu::Controls::Types::ItemSelector_t::ItemSelector_t(char* pTxt, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, void* pTargetToWrite, void* v, size_t vsz, Values_t pV_t) : _T(ITEM_SELECTOR), _C(C), _R(R), _I(IndexOfItem), pV({ pV_t, pTargetToWrite })
{
	// 
	this->pVec = v;
	this->VecSz = vsz;
	this->vIndex = 0;
	this->DesignLimits.MaxSizeX = MaxX;
	this->DesignLimits.MaxSizeY = MaxY;
	if (pText)
		this->pText = new char[strlen(pTxt) + 1], strcpy_s(this->pText, (strlen(pTxt) + 1), pTxt);
	else
		this->pText = 0;
}


void CMenu::Controls::AddItemSelector(char* CaptionText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, void* TargetToWrite, void* vec, size_t vsz, Values_t t)
{
	Types::ItemSelector_t* p = new Types::ItemSelector_t(CaptionText, IndexOfItem, C, R, MaxX, MaxY, TargetToWrite, vec, vsz, t);
	this->in->ControlList[this->in->ControlListCount] = p;
	this->in->ControlListCount++;
}




CMenu::CMenu(int c, unsigned int ControlNLines, Color_t bkcolor, Color_t LRc, Color_t UDc, Color_t Frc)
{
	this->CListIndex = c;
	this->ColumnsLen = 0;
	this->ControlMaxLines = ControlNLines;
	this->BackgroundColor = bkcolor;
	this->BorderLeftRightColor = LRc;
	this->BorderUpDownColor = UDc;
	this->FrontColor = Frc;

	this->CControls.in = this;
}


CMenu::~CMenu() 
{
	for (unsigned int i = 0; i < this->ColumnsLen; i++)
		delete[] this->CList[i];

	for (unsigned int i = 0; i < this->ControlListCount; i++)
		delete this->ControlList[i];

}

void CMenu::AddColumn(char* name)
{
	const int CLen = strlen(name) + 1;
	this->CList[this->ColumnsLen] = new char[CLen];
	strcpy_s(this->CList[this->ColumnsLen], CLen, name);

	unsigned int CWidth = IW5Engine::CEngine::CMath::GetTextWidth(name, (CLen - 1), GetHudSmallFont());
	
	if (CWidth > this->BiggestTextWidth)
		this->BiggestTextWidth = CWidth;

	this->ColumnsLen++;
}

int CMenu::GetLastClick()
{
	/*
	static int lastime = 0;
	
	if (lastime != UserKeys.LastTimeKeyPressed)
	{
		lastime = UserKeys.LastTimeKeyPressed;

		return (UserKeys.VirtualKeyCode == VK_BUTTON_MOUSE_1 || UserKeys.VirtualKeyCode == VK_BUTTON_MOUSE_2) ? UserKeys.VirtualKeyCode : 0;
	}
	*/


	return GetAsyncKeyState(VK_LBUTTON);
}


bool CMenu::IsMouseOnCoords(DesignLimits_t vec)
{
	return (CursorPos.X > vec.MinX && CursorPos.X < vec.MaxX && CursorPos.Y > vec.MinY && CursorPos.Y < vec.MaxY);
}


void CMenu::EventState()
{
	int LastClick = GetLastClick();

	if (LastClick)
	{

			// checking default menu items
			for (unsigned int i = 0; i < this->ColumnsLen; i++)
			{
				if (IsMouseOnCoords(CListDesignLimits[i]))
				{
					this->CListIndex = i;
					return;
				}
			}

			// checking default controls

			for (unsigned int i = 0; i < this->ControlListCount; i++)
			{

				if (!this->ControlList[i])
					continue;

				unsigned char ControlType = *(unsigned char*)this->ControlList[i] & 0xFF;

				switch (ControlType)
				{
					case Controls::Types::CHECK_BOX:
					{
						if (LastClick & 0x1)
						{
							Controls::Types::CheckBox_t* pControl = (Controls::Types::CheckBox_t*)this->ControlList[i];

							if (this->IsMouseOnCoords(pControl->DesignLimits.s))
							{
								pControl->value = !pControl->value;
								CMenu::CValue<int>::SecureWriteValue(pControl, pControl->value);
								return;
							}
						}
						break;
					}
					case Controls::Types::ITEM_SELECTOR:
					{
						// ...

						if (LastClick & 0x1)
						{
							Controls::Types::ItemSelector_t* pControl = (Controls::Types::ItemSelector_t*)this->ControlList[i];

							if (this->IsMouseOnCoords(pControl->DesignLimits.s))
							{
								// 

								DesignLimits_t Left = { pControl->DesignLimits.s.MinX, pControl->DesignLimits.s.MinY, pControl->DesignLimits.s.MinX + ((pControl->DesignLimits.s.MaxX - pControl->DesignLimits.s.MinX) / 2.0f), pControl->DesignLimits.s.MaxY };
								DesignLimits_t Right = { Left.MaxX, pControl->DesignLimits.s.MinY, pControl->DesignLimits.s.MaxX, pControl->DesignLimits.s.MaxY };
								
								if (this->IsMouseOnCoords(Left))
								{
									if (pControl->vIndex == 0)
										pControl->vIndex = (pControl->VecSz - 1);
									else if (pControl->vIndex > 0)
										pControl->vIndex--;

									CMenu::CValue<int>::SecureWriteValue(pControl, (int)(((char**)pControl->pVec)[pControl->vIndex]));

									return;
								}
								else if (this->IsMouseOnCoords(Right))
								{
									if (pControl->vIndex == (pControl->VecSz - 1))
										pControl->vIndex = 0;
									else if (pControl->vIndex < (pControl->VecSz - 1))
										pControl->vIndex++;
									
									CMenu::CValue<int>::SecureWriteValue(pControl, (int)(((char**)pControl->pVec)[pControl->vIndex]));
									
									return;
								}
							}
						}
						break;
					}
					case Controls::Types::PROGRESS_SLIDER_BAR:
					{
						if (LastClick & 0x8000)
						{
							Controls::Types::ProgressSlider_t* pControl = (Controls::Types::ProgressSlider_t*)this->ControlList[i];

							if (this->IsMouseOnCoords(pControl->DesignLimits.s))
							{
								pControl->Value = round(CursorPos.X);
								float Percentage = (((pControl->Value - pControl->DesignLimits.s.MinX) * 100.0f) / ((pControl->DesignLimits.s.MaxX) - (pControl->DesignLimits.s.MinX)));
								float Value = (((Percentage * (pControl->Max - pControl->Min)) / 100.0f) + pControl->Min);
								//CValueTest = Value;
								CValue<float>::SecureWriteValue(pControl, Value);

								return;
							}
						}
						break;
					}
					default:
						break;
				}
			}
		}
	
}

void CMenu::Init()
{
	this->EventState();
	this->CControls.OrderControls();


	// tmp
	const auto DrawTestLine = [](float x, float y) { return CDrawShader(x, y, 10.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, &TestColor, GetWhiteShader()); };


	// auto sizeable menu


	float MaxSizeX = ScreenSize.x;
	float MaxSizeY = ScreenSize.y;



	float SpaceMenuX = (MaxSizeX * 10.f) / 100.0f;
	float SpaceMenuY = (MaxSizeY * 10.f) / 100.0f;

	float MaxMenuSizeX = MaxSizeX - SpaceMenuX;
	float MaxMenuSizeY = MaxSizeY - SpaceMenuY;

	// init draw design coords
	float StartMenuX = SpaceMenuX / 2.0f;
	float StartMenuY = SpaceMenuY / 2.0f;


	// init draw background

	CDrawShader(StartMenuX, StartMenuY, MaxMenuSizeX, MaxMenuSizeY, 0.0f, 0.0f, 0.0f, 0.0f, &this->BackgroundColor, GetWhiteShader());


	// init draw up down borders
	CDrawShader(StartMenuX, (StartMenuY - 2.0f), MaxMenuSizeX, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, &this->BorderUpDownColor, GetWhiteShader());

	CDrawShader(StartMenuX, (StartMenuY + MaxMenuSizeY), MaxMenuSizeX, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, &this->BorderUpDownColor, GetWhiteShader());


	// init draw title

	char title[] = "Samurai Ray X";

	float TitleSpaceY = (MaxMenuSizeY * 10.0f) / 100.0f;

	float ColumnFontSpace = ((MaxMenuSizeX * 2.0f) / 100.0f);

	float ControlsMaxSizeX = (MaxMenuSizeX - this->BiggestTextWidth) - ColumnFontSpace;
	float ControlsMaxSizeY = MaxMenuSizeY - TitleSpaceY;

	float StartControlsX = StartMenuX + this->BiggestTextWidth + ColumnFontSpace;//+ ControlsMaxSizeX;
	float StartControlsY = StartMenuY + TitleSpaceY;

	float StartFontX = StartMenuX + ColumnFontSpace;
	float StartFontY = StartMenuY + TitleSpaceY;

	float StartColunmX = StartMenuX;
	float StartColunmY = StartMenuY + TitleSpaceY;


	float ColumnMaxSizeX = this->BiggestTextWidth + ColumnFontSpace;
	float ColumnMaxSizeY = MaxMenuSizeY - TitleSpaceY;


	float Height = (float)IW5Engine::CEngine::CMath::GetFontHeightByScaleOne(GetHudSmallFont());

	float ColumnBorderHeight = 0.0f;

	Height *= (float)this->ColumnsLen;

	if (Height > ColumnMaxSizeY)
		Height = ColumnMaxSizeY / this->ColumnsLen;
	else
		Height += ColumnBorderHeight = ((ColumnMaxSizeY - Height) * 20.0f) / 100.f;

	float FontY = Height / this->ColumnsLen;
	float CFontHeight = (Height - ColumnBorderHeight) / this->ColumnsLen;
	// fix center Y
	float StartCFY = (ColumnMaxSizeY - Height) / 2.0f;


	float ControlSpaceX = (ControlsMaxSizeX * 10.0f / 100.0f);
	float ControlSpaceY = TitleSpaceY;
	ControlsMaxSizeX -= ControlSpaceX;
	ControlsMaxSizeY -= ControlSpaceY;
	StartControlsX += ControlSpaceX / 2.0f;


	this->ControlsSpace.MinX = StartControlsX;
	this->ControlsSpace.MinY = StartControlsY;
	this->ControlsSpace.MaxX = ControlsMaxSizeX;
	this->ControlsSpace.MaxY = ControlsMaxSizeY;

	

	for (unsigned int i = 0; i < this->ColumnsLen; i++)
	{

		CDrawText(this->CList[i], strlen(this->CList[i]), GetHudSmallFont(), StartFontX, (StartFontY + StartCFY + (i * FontY)), 1.0f, 1.0f, 0.0f, &this->FrontColor, 0);

		CListDesignLimits[i].MinX = StartFontX;
		CListDesignLimits[i].MaxX = StartFontX + GetTextWidth(this->CList[i], strlen(this->CList[i]), GetHudSmallFont());
		CListDesignLimits[i].MinY = (StartFontY + StartCFY + (i * FontY)) - FontY;
		CListDesignLimits[i].MaxY = (StartFontY + StartCFY + (i * FontY));
		

		if (this->CListIndex == i)
			CDrawShader(StartColunmX, (StartFontY + StartCFY + (FontY * i)), ColumnMaxSizeX, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, &this->BorderUpDownColor, GetWhiteShader());
		else
			CDrawShader(StartColunmX, (StartFontY + StartCFY + (FontY * i)), ColumnMaxSizeX, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, &this->FrontColor, GetWhiteShader());

	}


	for (unsigned int i = 0; i < this->ControlListCount; i++)
		if (this->ControlList[i] && ((byte*)this->ControlList[i])[1] == this->CListIndex)
			this->CControls.DrawControl(this->ControlList[i]);


	/*
	float* someunk0 = (float*)((void*(*)())0x4B6350)();

	float CursorX = someunk0[0] * 32.0f / someunk0[2];
	float CursorY = someunk0[1] * 32.0f / someunk0[3];

	float* CursorPos = (float*)0x58DA490;


	CursorX = CursorPos[0] - CursorX * 0.5f;
	CursorY = CursorPos[1] - CursorY* 0.5f;
	*/

	// tmp cursor
	CDrawShader(CursorPos.X, CursorPos.Y, 5.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, &TestColor, GetWhiteShader());
	/*
	char buff[256];

	int k = GetCurrentVirtualKeyCode();

	sprintf_s(buff, sizeof(buff), "CKey: %d", k);

	CDrawText(buff, strlen(buff), GetConsoleFont(), 200.0f, 80.0f, 1.0f, 1.0f, 0.0f, &TestColor, 0);
	*/
	
	/*
	for (unsigned int i = 0; i < this->ControlListCount; i++)
	{
		void* Control = this->ControlList[i];
		if (Control)
		{
			unsigned char* controlType = (unsigned char*)Control;

			if (*controlType == Controls::Types::CHECK_BOX)
			{
				Controls::Types::CheckBox_t* pControl = (Controls::Types::CheckBox_t*)Control;
			
				CDrawShader(pControl->DesignLimits.c.MinX, pControl->DesignLimits.c.MinY, pControl->DesignLimits.c.MaxX, pControl->DesignLimits.c.MaxY, 0.0f, 0.0f, 0.0f, 0.0f, &TestColor, GetWhiteShader());
			}

		}
	}
	*/

	


}







template<class _T>
_T CMenu::CValue<_T>::ReadValue(Control_t p)
{
	return *(_T*)(((CMenu::Controls::Types::Value_t*)((int)p + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t)))->p);
}

template<class _T>
void CMenu::CValue<_T>::WriteValue(Control_t p, _T v)
{
	*(_T*)(((CMenu::Controls::Types::Value_t*)((int)p + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t)))->p) = v;
}

template<class _T>
void CMenu::CValue<_T>::SecureWriteValue(Control_t p, _T v)
{
	Controls::Types::Value_t * t = (Controls::Types::Value_t*)((int)p + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t));

	switch (t->T)
	{
		case Values_t::BOOL_T:
		{
			*(bool*)t->p = (v ? true : false);
			break;
		}
		case Values_t::FLOAT_T:
		{
			*(float*)t->p = (float)v;
			break;
		}
		case Values_t::INT_T:
		{
			*(int*)t->p = (int)v;
			break;
		}
		case Values_t::PCHAR_T:
		{
			*(int*)t->p = (int)v;
			break;
		}

		default:
			break;
	}
}

template<class _T>
_T CMenu::CValue<_T>::SecureReadValue(Control_t p)
{
	Controls::Types::Value_t* t = (Controls::Types::Value_t*)((int)p + sizeof(int) + sizeof(CMenu::ControlDesignLimits_t));

	switch (t->T)
	{
		case Values_t::BOOL_T:
			return (_T)*(bool*)t->p;
		
		case Values_t::FLOAT_T:
			return (_T)*(float*)t->p;

		case Values_t::INT_T:
			return (_T)*(int*)t->p;

		default:
			break;
	}

	return 0;
}
