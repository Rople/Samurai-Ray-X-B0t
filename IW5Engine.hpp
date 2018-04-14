


#pragma region GVALUES
#define TEAM_AXIS 0x1
#define TEAM_ALLIES 0x2
#define TEAM_SPECTATOR 0x3
#define TEAM_FFA 0x0
#define BUTTON_SHOOT 0x1
#define BUTTON_RUN 0x2002
#define BUTTON_AIM 0x10000800
#define BUTTON_RIGHT_MOUSE 0x80000 // akimbo?
#define BUTTON_CROUCHING 0x200
#define BUTTON_RECLINING 0x100
#define BUTTON_JUMP 0x400
#define BUTTON_USE_FIRST_ATTACHMENT 0x8000
#define BUTTON_USE_SECONDARY_ATTACHMENT 0x4000
#define BUTTON_USE_KNIFE 0x4000004
#define BUTTON_USING_CAPTION 0x800000 // "tchoosegame"...
#define FORWARD_MOVEMENT 0x7F
#define BACKWARD_MOVEMENT 0x81
#define LEFT_MOVEMENT 0x81
#define RIGHT_MOVEMENT 0x7F
#define CAPTION_IN_GAME 0x0
#define CAPTION_CONSOLE 0x1
#define CAPTION_CHAT 0x20
#define CAPTION_MENU 0x10


#define TOGGLE_IW5CONSOLE 0x1
#define CURRENT_CMD_NUMBER *(int*)OFFSET_CURRENT_CMD_NUMBER
#define CMD_MASK 0x7F
#define MAX_ENTITIES 2046
#define ENTITY_PLAYER 1
#define ENTITY_PLAYER_DEAD 2
#define ENTITY_WEAPON 3
#define ENTITY_ATTACHMENT_ATTACK 4
#define MAX_PLAYERS 18
#define UKE_WM_KEYDOWN 2
#define UKE_WM_KEYUP 1
#define VK_BUTTON_MOUSE_1 0xC8
#define VK_BUTTON_MOUSE_2 0xC9


#define CG (*(IW5Engine::CEngine::cg_t*)OFFSET_CG_T)
#define OutPackets(x) (((IW5Engine::CEngine::outPacket_t*)OFFSET_OUT_PACKET_T)[(int)x])
#define RefDef (*(IW5Engine::CEngine::RefDef_t*)OFFSET_REFDEF_T)
#define ClientActive (*(IW5Engine::CEngine::ClientActive_t*)OFFSET_CLIENT_ACTIVE_T)
#define RefDefViewAngles (*(IW5Engine::CEngine::RefDefViewAngles_t*)OFFSET_REFDEF_VIEW_ANGLES_T)
#define ScreenSize (*(IW5Engine::CEngine::ScreenSize_t*)OFFSET_SCREEN_SIZE)
#define CursorPos (*(IW5Engine::CEngine::CursorPos_t*)OFFSET_CURSOR_POS)
#define UserCmdButtons (*(int*)OFFSET_USER_CMD_BUTTONS)
#define UserCaption (*(int*)OFFSET_USER_CAPTION)
#pragma endregion


#pragma region FUNCTIONS

/*
#define GetView2DCoordsFrom3D(a, b, c)					\
{														\
	DWORD dwcall = OFFSET_GET_WORLD_2D_COORDS_FROM_3D;	\
	__asm lea edi, c									\
	__asm push edi										\
	__asm lea ebx, a									\
	__asm lea edi, b									\
	__asm call[dwcall]									\
	__asm add esp, 0x4									\
} 

#define GetEntityPos(a, b, c)							\
{														\
	DWORD dwcall = OFFSET_GET_ENTITY_POS;				\
	__asm lea edi, c									\
	__asm push edi										\
	__asm mov edi, b									\
	__asm mov esi, a									\
	__asm call[dwcall]									\
	__asm add esp, 0x4									\
}
*/					



#define GetCurrentClientNumber() (*(int*)OFFSET_LOCAL_CLIENT_NUMBER)
#define IsInGame() (*(DWORD*)OFFSET_IS_IN_GAME & 0x2000)
#define IsInGamePlaying() (*(DWORD*)OFFSET_IS_IN_GAME & 0x1000)
#define GetUserCmd(x) (((IW5Engine::CEngine::usercmd_t*)OFFSET_USER_CMDS)[(int)x])
#define ShootWeapon() (*(byte*)OFFSET_BUTTON_SHOOT = 1)
#define GetCurrentWeapon() (*(int*)OFFSET_CURRENT_WEAPON)
#define GetCurrentWeaponId() (*(byte*)OFFSET_CURRENT_WEAPON)
#define GetEntity(x) (((IW5Engine::CEngine::centity_t*)OFFSET_G_ENTITY)[(int)x])
#define GetPlayerInfo(x) (((IW5Engine::CEngine::PlayerInfo_t*)OFFSET_PLAYER_INFO)[(int)x])
#define GetWeaponBulletType(x) ((*(byte*)(((DWORD*)OFFSET_WEAPON_INFO)[(byte)x] + 0x771) != 0) + 1)
#define GetSpreadMultiplier() (*(float*)OFFSET_SPREAD_MULTIPLIER)
#define GetConsoleFont() (*(DWORD*)OFFSET_CONSOLE_FONT)
#define GetHudSmallFont() (*(DWORD*)OFFSET_HUD_SMALL_FONT)
#define GetObjectiveFont() (*(DWORD*)OFFSET_OBJECTIVE_FONT)
#define GetWhiteShader() (*(DWORD*)OFFSET_WHITE_SHADER)
#define GetWeaponShaderByWeaponId(x) (((int*)OFFSET_WEAPON_SHADER)[(byte)x])
#define GetCurrentUserKeysNumber() ((((byte)(*(DWORD*)OFFSET_NEXT_USER_KEYS_NUMBER)) - 1))
#define GetCurrentUserKeys() (((IW5Engine::CEngine::KeyInfo_t*)OFFSET_USER_KEYS)[GetCurrentUserKeysNumber()])
#define UserKeys (GetCurrentUserKeys())
#pragma endregion



struct Color_t
{
	float R;
	float G;
	float B;
	float A;
	Color_t();
	Color_t(int R, int G, int B, int A);

};




class IW5Console
{
public:
	static int __cdecl OnRconCommand(int local, char* cmd);

	static char* ConsolePrefix;

private:	
	static struct ConsoleUIColor
	{
		Color_t* BackgroundMin;
		Color_t* TextMin;
		Color_t* BackgroundMax;
		Color_t* TextMax;
		Color_t* Prefix;
		Color_t* ScrollBarOver;
		Color_t* ScrollBarUnder;
	};

	static struct ConsoleUISetting
	{
		int* TurnConsoleMax;
		int* TurnConsoleMin;
		float* ConsoleSize;
	};
public:
	static struct Commands
	{
		static struct Default
		{
			static void InitRegisterDefaultCommands();
			static int ProcessCommand(char* command);
			static void __cdecl OnCommandEvent(const char* command, void* value);
		};

		static struct Dvar
		{
			static int ProcessDvar(char* dvar);
			static int FindDvar(char* dvar);
		private:

			static int ProcessDvarValues(char* params, byte DataTypeMode, void* StartAdressValues, int unk0);
			static int FindDvarPointer(char* dvar);

			typedef struct
			{
				char __0x0[0x8];
				byte DataTypeMode; // 0x8
				char __0x9[0x3];
				void* StartPointerValues; // 0xC
				char __0x10[0x2C];	// 0x10
				int Unk0; // 0x3C
			} Dvar_t;
		
		};

		static struct Reliable
		{
			static int(__cdecl*CBuff_addtext)(int local, char* buff);
			static void __cdecl ProcessReliableCommand(char* buff);
			static void Execute(char* command);
		
		};

	};

	static ConsoleUIColor SettingColor;
	static ConsoleUISetting SettingUI;
	

	static void SetConsolePrefix(char* prefix);
	static void __cdecl ToggleConsole();
	static void DebugOnConsole(int buffsz, const char* format, ...);


	static int(__cdecl*ProccessRconCommand)(int local, char* buff);
	static char(__cdecl*PrintOnConsole)(int local, int __unk0, char* buff, int time, int bold, char __unk1, int __unk2);

};






class IW5Engine
{
public:



	static void Init();

	struct CEngine
	{
		static vec3_t ViewOrigin;
		static vec3_t ViewAngles;
	
		static void __forceinline CalculateViewOrigin(float ScaleXYZ)
		{
			IW5Engine::CEngine::CMath::CG_GetPlayerViewPoint(0, &CG, IW5Engine::CEngine::ViewOrigin);

			IW5Engine::CEngine::ViewOrigin[0] += (ScaleXYZ * CG.VelocityLocal[0]);
			IW5Engine::CEngine::ViewOrigin[1] += (ScaleXYZ * CG.VelocityLocal[1]);
			IW5Engine::CEngine::ViewOrigin[2] += (ScaleXYZ * CG.VelocityLocal[2]);
		}
	
		static void __forceinline CalculateViewAngles();
		


		static float ViewOriginScaleXY;
		static float ViewOriginScaleZ;
		static int PlayerBonesNumber[20];
		static char* PlayerBonesName[20];
		

		static void UpdateTargetEntityPoints();
		static int(__cdecl*GetPointNumberByName)(char* name);
		static int(__cdecl*LoadShader)(char* name);

		typedef struct
		{
			int trType; // 0x0 //  i'm not setting a enum like the original source bcs i'm not sure if iw5ngine it's 100% acc to original src... anyway seems accesed thru' 0x4DEEE0 aka ~BG_EvaluateTrajectory();
			char __0x4[0x8]; // 0x4 // trTime, trDuration probably...
			vec3_t trBase; // 0xC
			char __0x18[0xC]; // 0x18 // trDelta, velocity probably, entity player must be calculated manually...
		} trajectory_t; // 0x24

		typedef struct
		{
			int CommandTime; // 0x0 // ccmd->ServerTime;
			char __0x4[0x18];
			vec3_t OffsetOrigin0; // 0x1C
			vec3_t Velocity; // 0x28
			char __0x34[0x134];
			float ViewOriginAngle; // 0x168
			char __0x16C[0x108];
			int CurrentSecondaryWeapon; // 0x274
			int CurrentPrimaryAttachment; // 0x278
			int CurrentPrimaryWeapon; // 0x27C
			int CurrentSecondayAttachment; // 0x280		
			char __0x284[0xEC];
			int CurrentWeapon; // 0x370
			char __0x374[0x160];
			int AimingTime; // 0x4D4
			char __0x4D8[0x14];
			int BulletPenetrationType; // 0x4EC
			char __0x4F0[0x2E20];
		} playerState_t; // 0x3310

		typedef struct
		{
			int eIndex; // 0x0
			int eType; // 0x4
			char __0x0[0x4]; // 0x8
			trajectory_t pos;// 0xC
			trajectory_t apos; // 0x30
			char __0x54[0xAC]; // 0x54
		} centityState_t; // 0x100

		/*
			0x90246C = 0x90578C - 0x3320
			0x936360 = 0x939680 - 0x3320
		*/

		typedef struct
		{
			playerState_t ps; // 0x0
			int ping; // 0x3310
			int ServerTime; // 0x3314
			int EntitiesSpawnedNumber; // 0x3318 // on server in realtime // dobj?
			int PlayersOnlineNumber; // 0x331C // on server in realtime too...
			centityState_t entities[0x12]; // 0x3320 // i'm not 100% sure about size... probably something similar to MAX_ENTITIES_IN_SNAPSHOT, i'm so lazy right now to reverse useless shit...
			char __0x4520[0x2F9D4];
		} snapshot_t; // 0x33EF4

		typedef struct
		{
			int ServerTime; // 0x0
			char __0x4[0x14];
			int VelocityGeneral; // 0x18
			vec3_t OffsetOrigin0; // 0x1C
			vec3_t VelocityLocal; // 0x28
			char __0x34[0x11C];
			int CurrentClientNumber; // 0x150
			char __0x154[0x4];
			vec3_t ViewAngles; // 0x158
			int ViewOriginAngle; // 0x164
			float ViewOriginAngleExact; // 0x168
			char __0x16C[0x108];
			int CurrentSecondaryWeapon; // 0x274
			int CurrentPrimaryAttachment; // 0x278
			int CurrentPrimaryWeapon; // 0x27C
			int CurrentSecondayAttachment; // 0x280
			char ___0x284[0xE0];
			int LastAttachmentUsed; // 0x364
			char __0x368[0x8];
			int CurrentWeapon; // 0x370
			char __0x374[0x4];
			float AimProgress; // 0x378
			float SpreadMultiplier; // 0x37C
			char __0x380[0x14];
			int CurrentSecondaryWeapon_; // 0x394
			int BulletsRemainingToReloadSecondaryWeapon; // 0x398
			char __0x39C[0x8];
			int CurrentPrimaryWeapon_; // 0x3A4
			int BulletsRemainingToReloadPrimaryWeapon; // 0x3A8
			char __0x3AC[0x140];
			int BulletPenetrationType; // 0x4EC
			char __0x4F0[0x538];
			int unk0; // 0xA28
			char __0xA2C[0x28E8];
			float SwayMultiplier;// 0x3314
			char __0x3318[0x4C];
			snapshot_t* snap; // 0x3364
			snapshot_t* nextSnap; // 0x3368
			snapshot_t activeSnapshots[2];	// 0x336C & 0x37260
			float FrameInterpolation; // 0x6B154
			int FrameTime; //  0x6B158 // Cl_ServerTime - OldTime
			int Cl_ServerTime; // 0x6B15C // current time
			int OldTime;// 0x6B160 // time of back frame
			char __0x6B164[0xBF0C];
			int LastImpactTime;  // 0x77070
			char __0x77074[0x18];
			vec2_t ImpactAngles; //0x7708C
			float VelocitySide; //0x77094
			char __0x77098[0x24];
			vec3_t WeaponView; //0x770BC // viewangles + sway angles + impact angles
		} cg_t;


		// same shit...
		typedef struct
		{
			int		p_cmdNumber;		// cl.cmdNumber when packet was sent
			int		p_serverTime; //0x4		// usercmd->serverTime when packet was sent
			int		p_realtime;	// 0x8		// cls.realtime when packet was sent
		} outPacket_t;


		struct usercmd_t
		{
			int ServerTime; // 0x0
			int Buttons; // 0x4
			int ViewAngles[3]; // 0x8
			int CurrentWeapon; // 0x14
			int LastAttachmentUsed; // 0x18
			byte ForwardBackwardMovement; // 0x1C
			byte LeftRightMovement; // 0x1D
			byte __0x1E[0xE];	// 0x1E
		} static *cmd; // 0x2C

		

		typedef struct
		{
			int eFlags; // 0x0 // i'm not sure... i need trace it, i'm so lazy right now, may be tomorrow...
			trajectory_t pos; // 0x4 //  origin
			trajectory_t apos; // 0x28 // viewangles
			char __0x4C[0x10]; // 0x4C
			int Weapon; // 0x5C 
		} entityState_t; // 0x60 // probably incorrect but the rest it's like unique value without repeat...

		typedef struct
		{
			char __0x0[0x2];
			byte Status; // 2 // | 1 = spawned | 5 = in game spectator
			byte __0x3; // 3
			char __0x4[0xC]; // 4
			int __0x10;// 16
			vec3_t Origin; // 20
			vec3_t ViewAngles; // 32
			char __0x2C[0xC];
			int __0x38; // 56
			int __0x3C; // 60
			char __0x40[0x4];
			byte __0x44; // 68
			byte __0x45; // 69
			char __0x46[0x22];
			entityState_t currentState; // 0x68 = nextState of back frame
			char __0xC8[0x8];
			int LocalNumber; // 208  // 0xD0
			int Type; // 212
			entityState_t nextState; // 0xD8
			char __0x138[0x28];
			int PlayerInfoLocalNumber; // 0x160 // 352
			char __0x164[0x8];
			int __0x16C;	// 364
			char __0x170[0x28];
			int Weapon;	// 408
			int __0x19C; // 412
			char __1A0[0x10];
			short __0x1B0;	// 432
			char __0x1B2;
			char __0x1B3; // 435
			int __0x1B4;// 436
			int __0x1B8; // 440
			int __0x1BC; // 444 
			int __0x1C0; // 448
			int __0x1C4; // 452
			int __0x1C8; // 456
			char __0x1CC[0x4];
			byte Alive; // 464
			char __0x1D1[0x1B];
			int __0x1EC; // 492
			char __0x1F0[0x4];
			int __0x1F4;	// 500
		} centity_t;

		typedef struct
		{
			BYTE __0x0[0xC];
			char Name[0x10]; // 0xC // 16 bytes
			int Team; // 0x1C
			int TeamB; // 0x20
			char __0x24[0x10];
			int BulletPenetrationType;	// 0x34
			BYTE __0x38[0x18]; // 0x38
			char BodyType[0x40]; // 64 bytes // 0x50
			char TeamType[0x40]; // 64 bytes // 0x90
			char RiotType[0x40]; // 64 bytes // 0xD0 // riot jugger or normal jugger
			BYTE __0x110[0x140]; // 320 bytes // 0x110
			char RiotStatus[0x40]; // 64 bytes // 0x250
			BYTE __0x290[0x200]; // 0x290
			int PlayerStatus; // 0x490 // prone, couched, stay...
			BYTE __0x494[0x78]; // 0x494
			int Weapon; // 0x50C
			char __0x510[0x2A];
			char Title[0x18]; // 0x53A
			BYTE __0x552[0x6]; // 0x552
			char ClanTag[0x5]; // 0x558
			BYTE __0x55D[0x3]; // 0x55D
		} PlayerInfo_t;
		
		typedef struct
		{
			int Width; // cscreen
			int Height; // cscreen
			float fov_x; // in radians
			float fov_y; // in radians
			vec3_t Origin;
		} RefDef_t;

		typedef struct
		{
			char mapname[0x40]; // 0x0 // path + cmap name
			byte __0x40[0xE0]; // 0x40 stuffs from cg like viewangles, origin, velocity... whatever doesn't make sense put him here...
			vec3_t ViewAngles; // 0x120 // set current viewangles... usefull k...
		} ClientActive_t;

		typedef struct
		{
			float x;
			float y;

		} ScreenSize_t;


		typedef struct
		{
			vec3_t ViewAngles; // 0x0
		} RefDefViewAngles_t;
	


		typedef struct
		{
			int WorldEntityNumber; // 
			int	EntityMatched; // // 4
			float Power; // // 8
			int	BulletType; // // 12
			char __0x10[0x4]; // // 16
			vec3_t ViewOrg; //  // 20 // 24 // 28
			vec3_t Start; //  // // 32 // 36 // 40
			vec3_t End; // // 44 // 48 // 52
			vec3_t ViewDirection; // 56 // 60 // 64
		} BulletTrace_t;


		typedef struct
		{
			float SkyDirectionDistance; // 0x0
			vec3_t Direction; // 0x4
			int WorldObjectType; //0x10 // i'm not sure what it's this, but on ENTITY_PLAYER it sets on 0x700000
			char __0x14[0xC]; // 0x14
			int EntityHittedIndex; // 0x20 // Current entity hit if there is a entity, otherwise it sets on WorldEntityNumber = entity that doesn't exists = 2046
			char __0x24[0x4]; // 0x24
			byte check0; // 0x28
			byte check1; // 0x29
			char __0x2A[0x6]; // 0x2A
			vec3_t ViewDirection; // 0x30
			char __0x3C[0x4]; // 0x3C
			int WallType; // 0x40
			char __0x44[0x4]; // 0x44
		} WorldInfo_t;

		typedef struct
		{
			cg_t* cg; // 0x0
			int LastImpactTime; // 0x4
			int ServerTime; // 0x8
			vec2_t ImpactAngles; // 0xC
			float SideVelocity; // 0x14
			float FrameTime; // 0x18
			float SwayMultiplier; // 0x1C
		} ViewInfo_t;

		typedef struct
		{
			char __0x0[4]; // 0x0
			int LastTimeKeyPressed; // 0x4
			int MessageEvent;// 0x8  WM_KEYDOWN = 2 | WM_KEYUP = 1
			int VirtualKeyCode; // 0xC
			int vkkeyScancode; // 0x10 //
			char __0x14[0x8]; // unk3
		} KeyInfo_t;

		typedef struct
		{
			float X;
			float Y;
		} CursorPos_t;

		struct CMath
		{
			static void __forceinline IW5Engine::CEngine::CMath::GetDobjBonePos(centity_t* p, int nBone, float* pos)
			{
				static const DWORD dwcall = OFFSET_GET_ENTITY_POS;

				__asm
				{
					push pos;
					mov edi, nBone;
					mov esi, p;
					call[dwcall];
					add esp, 0x4;
				}
			}

			static byte __forceinline IW5Engine::CEngine::CMath::GetWorld2DCoordsFrom3D(float* Pos, float* x, float* y)
			{
				static const DWORD dwcall = OFFSET_GET_WORLD_2D_COORDS_FROM_3D;

				byte Result = 0;

				__asm
				{
					push Pos;
					mov ebx, x;
					mov edi, y;
					call[dwcall];
					add esp, 0x4;
					mov Result, al;
				}

				return Result;
			}

			static float __forceinline IW5Engine::CEngine::CMath::BG_EvaluateTrajectory(trajectory_t* tr, int atTime, float* result, float InterpolationScale)
			{
				static const DWORD dwx = OFFSET_BG_EVALUATE_TRAJECTORY;

				float flresult = 0.0f;

				__asm
				{
					fld[InterpolationScale];
					push result;
					push atTime;
					push tr;
					call[dwx];
					add esp, 0xC;
					fstp[flresult];
				}
				return flresult;
			}

			static float (__cdecl*VectorNormalize)(float* vec);
			static void(__cdecl*CG_GetPlayerViewPoint)(int zero, IW5Engine::CEngine::cg_t*, float* outvec);
			static void(__cdecl*CG_GetViewAngleAnimations)(ViewInfo_t* wi, vec3_t Out);
			static float(__cdecl*__VectorDistance)(float* a, float* b);
			static int(__cdecl*GetTextWidth)(char* text, int len, int font);
			static int(__cdecl*GetFontHeightByScaleOne)(int font);
		};



		static void __cdecl CG_InterpolateEntityPosition(centity_t* entity);
		static void OnInterpolateEntityPosition(); // event hook
		static void* CInterpolateEntityPosition;


		static void OnCreateNewCmds();
		//static void CL_CreateNewCommands();
		static void OnCL_CreateNewCmd();
		static void* CL_CreateNewCmds;


		static void CL_WritePacket();
		static void OnStartWritePacket();
		static void OnEndWritePacket();
		static void CL_EndWritePacket();
		static void* CWritePacket;


		static void __cdecl OnCG_Draw2D(int local);
		static void (__cdecl*CG_Draw2D)(int local);

	
		static char __cdecl OnCG_InitDrawConsole(int local);
		static char(__cdecl*CG_InitDrawConsole)(int local);


		static struct CAwall
		{
		private:
			static float(__cdecl*GetWallDepth)(int weapon, byte zero, int WallType);
			static bool(__cdecl*AdvanceTrace)(BulletTrace_t *stbt, WorldInfo_t* stwi, float unk0);
			static int __forceinline BulletTrace(centity_t * MainEntity, int WallType, BulletTrace_t* stbt, WorldInfo_t * stwi);

		public:
			static float GetFinalBulletPower(int CEntityIndex, vec3_t Pos);

		};

		struct CSpread
		{
		private:
			static float* (__cdecl* GetWeaponSpreadAngle)(cg_t* cg, byte Weapon, float* MultiplierA, float* MultiplierB);
			static void __forceinline GetSpreadVector(float spreadangle, float* outvdir, float* forward, float* right, float* up, float distance, float* startvec, float* endvec, int* seedptr);

		public:
			static int LastAnglesAdded[3];
			static void __cdecl ReverseSpread(usercmd_t* cmd);

		};

		struct CDraw
		{
			static Color_t TestColor;

			static char*(__cdecl*CDrawText)(char* text, size_t len, int Font, float x, float y, float sizex, float sizey, float rotate, Color_t* color, int zero);
			// char __cdecl sub_4EE060(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, int color, int shader)
			static char(__cdecl*CDrawShader)(float x, float y, float sizex, float sizey, float unk0, float unk1, float unk2, float unk3, Color_t* color, int Shader);
		};

		static void(__cdecl* CG_ServerCommand)(int local, char* cmd);

	};

	struct WinApi
	{
		static void(__cdecl*EngineEnterCriticalSection)(int x);
		static void(__cdecl*EngineLeaveCriticalSection)(int x);
	};



private:
	static void InitEngine();

};







