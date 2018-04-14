#include <map>
#include <vector>

#define TARGET_NOT_FOUND 0x20
#define CONSOLE_MAX_INPUT_LEN 256
#define CONSOLE_MAX_OUTPUT_LEN 1024

class Settings
{
public:
	static vec3_t test;
	static void LoadDefaultSettings();


	static struct Prediction
	{
		static int PredictionMode;
		
		// by frame
		static struct PredictionByTime
		{
			static float TimeScale;
			

		};

		static struct PredictionByVelocity
		{
			static float VelocityScale;

		};

		static struct PredictionBySideMove
		{
			static float SideMoveScale;


		};

		static struct Interpolation
		{
			static int InterpolationTime;
			static int InterpolationViewTime;
			static int InterpolationOriginTime;
			static float InterpolationScale;
		};

		static struct LocalPrediction
		{
			static float OriginScaleXYZ;
		};
	};

	static struct AntiAim
	{
		static int AntiAimMode;

		static struct AntiAimByMovement
		{
			

		};

		static struct AntiAimByRiot
		{
			static int AntiAimRiotTarget;
		};

		enum
		{
			ANTI_AIM_MOVEMENT_TYPE_A,
			ANTI_AIM_MOVEMENT_TYPE_B,
			ANTI_AIM_MOVEMENT_TYPE_C,
			ANTI_AIM_RIOT_TYPE_A,
			ANTI_AIM_RIOT_TYPE_B
		};

	};

	static struct CBot
	{
		static bool AutoShoot;
		static short AimbotMode;
		static float TargetMaxDistance;
		static char* CBone;
		static char* CMultiBoneOrder[19];
		static int CMultiBoneOrderNumber[19];
		static int Bot;
		static int MaxBones;
	};

	static struct UserCommands
	{
		static int TimeScale;
	};

	static struct CESP
	{
		static struct EnemyESP
		{
			static bool Enabled;
			static void __cdecl Toggle(int v);
			static Color_t EnemyESPColor;
		};
		
		static struct WeaponESP
		{
			static bool Enabled;
			static void __cdecl Toggle(int v);
			static Color_t WeaponESPColor;
		};
	};

};



class CConsole : private IW5Engine::CEngine::CDraw, private IW5Engine::CEngine::CMath
{
private:
	size_t InBuffSz;
	size_t OutBuffSz;

	char* InputBuff;
	char* OutputBuff;
	float bMaxSizeX;
	int bMaxLines;
	int InBuffCount;
//	MSG msg;
//	int GetCurrentKey();
	int GetCurrentVirtualKeyCode();
	bool Init; // if is initialized, console has been initialized and there is a bMaxLines aviable between other things... if true, AddTextToBuff can be used otherwise no...
	void(__cdecl* OnCommand)(char* command);

public:
	CConsole(float x, float y, float szx, float szy, Color_t BackgroundColor, Color_t UpDownBorderColor, Color_t LeftRightBorderColor, size_t InputMaxSz, size_t OutputMaxSz, void(__cdecl* EventHandler)(char*));
	
	
	void Run(); // init console
	void AddTextToBuff(char* text);
	void ClearOutput();
	void ClearInput();

	~CConsole();
	bool First; // if first line
	int TextLines; // current lines used
	float SizeX;
	float SizeY;
	float X;
	float Y;
	Color_t BackgroundColor;
	Color_t UpDownBorderColor;
	Color_t LeftRightBorderColor;

	void AutoSize();
};


class CMenu : IW5Engine::CEngine::CDraw, IW5Engine::CEngine::CMath
{
public:

	typedef const enum
	{
		BOOL_T,
		FLOAT_T,
		INT_T,
		PCHAR_T
	} Values_t;


	unsigned int ColumnsLen;
	unsigned int BiggestTextWidth;
	Color_t BackgroundColor;
	Color_t BorderUpDownColor;
	Color_t BorderLeftRightColor;
	Color_t FrontColor;
	unsigned int CListIndex;
	void(__cdecl* pHand)(float DrawAxes[4], unsigned int CurrentListIndex);
	std::map<int, char* > CList;
private:
	typedef void* Control_t;

	
	template<class _Tx>
	class CValue
	{
	public:
		static _Tx ReadValue(Control_t p);
		static _Tx SecureReadValue(Control_t p);
		static void WriteValue(Control_t p, _Tx v = 0);
		static void SecureWriteValue(Control_t p, _Tx v);

	};
	

	struct DesignLimits_t
	{
		float MinX, MinY, MaxX, MaxY;
	};

	std::map <int, DesignLimits_t> CListDesignLimits;
	void EventState();
	int GetLastClick();
	bool IsMouseOnCoords(DesignLimits_t vec);

	struct ControlDesignLimits_t
	{
		float MaxSizeX; // we cannot define a min size due smaller monitors but yes a max size or we will get a giant control...
		float MaxSizeY;
		DesignLimits_t c; // this will set a draw event
		DesignLimits_t s;
	};


	class Controls
	{
		
	public:
		void DrawControl(Control_t p);
		void OrderControls();
		class Types
		{
		public:
			const enum Controls_Type
			{
				CHECK_BOX,
				PROGRESS_SLIDER_BAR,
				ITEM_SELECTOR,
				BUTTON,
				TEXT
			};



			typedef struct
			{
				const Values_t T;
				void* p;
			} Value_t;

			class CheckBox_t
			{
				private:
					const unsigned char _T; // defines statically T
					const unsigned char _I; // index
					const unsigned char _C; // Column
					const unsigned char _R; // Row
				public:
					ControlDesignLimits_t DesignLimits;
					Value_t pV;
					int value;
					char* Text;
					unsigned int ColumnIndex;
					CheckBox_t(char* pText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, int dValue, void* pTargetToWrite, Values_t v);
			};

			class ProgressSlider_t
			{
				private:
					const unsigned char _T;
					const unsigned char _I;
					const unsigned char _C; // Column
					const unsigned char _R; // Row			
				public:
					ControlDesignLimits_t DesignLimits;
					Value_t pV;
					void* Target;
					float Min;
					float Max;
					float Value;
					char* pText;
					
					ProgressSlider_t(char* pText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, float MinState, float MaxState, float dValue, void* pTargetToWrite, Values_t pV_t);
			};

			class ItemSelector_t
			{
			private:
				const unsigned char _T;
				const unsigned char _I;
				const unsigned char _C; // Column
				const unsigned char _R; // Row			
			public:
				ControlDesignLimits_t DesignLimits;
				Value_t pV;
				char* pText;
				void* pVec; // vector of ints, floats, pchars...
				size_t VecSz; // vec size if exists...
				unsigned int vIndex;
				ItemSelector_t(char* pText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, void* pTargetToWrite, void* v, size_t vsz, Values_t pV_t);
			};



		};




		CMenu* in;

		void AddProgressSlider(char* CaptionText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, float MinState, float MaxState, float DefualtValue, void* pTargetToWrite, Values_t);
		void AddCheckBox(char* CaptionText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, int DefaultValue, void* TargetToChange, Values_t);
		void AddItemSelector(char* CaptionText, const unsigned char IndexOfItem, const unsigned char C, const unsigned char R, float MaxX, float MaxY, void* TargetToWrite, void* vec, size_t vsz, Values_t t);



	};



	DesignLimits_t ControlsSpace;
	std::map<int, void*> ControlList; 
	unsigned int ControlListCount;
	unsigned int ControlMaxLines;


									  /*
											Standard:
												first byte defines the type
									  */


public:


	Controls CControls;
	
	CMenu(int c, unsigned int ControlNLines, Color_t bkcolor, Color_t LRc, Color_t UDc, Color_t Frc);
	~CMenu();
	void AddColumn(char* name);


	void Init();
	/*
	class Controls
	{
	public:


	private:
		class CControls : IW5Engine::CEngine::CDraw
		{
		public:
			static void DrawCheckBox(float x, float y, float szxy, Color_t* Bkcolor, Color_t* BColor);
			static void DrawButton(float x, float y, float szxy, Color_t* BkColor, Color_t* BColor);
			static void DrawProgressSlider(float x, float y, float szxy, Color_t BkColor, Color_t* BColor);
			// ...

		};

	};
	*/
};
class SamurayX : IW5Engine, Settings
{
public:
	// Init engine
	static void Init();
	
	// console beta 0.1 
	static struct ConsoleControl
	{
		
		static CConsole Console;
		static bool OpenConsole;
		static void ToggleConsole();
		static void ClearConsole();
		// event
		static void __cdecl OnCommand(char* command);
	};

	static struct MenuControl
	{
		static CMenu Menu;
		static bool OpenMenu;
		static void ToggleMenu();
		static void LoadCMenu();
	};


	static struct SettingControls
	{
		static struct KeyHandler
		{
		public:
			static int ProcessRegisterKeyForCommand(char* command);
			static int RegisterNewHotKeyForCommand(char* command);
			static int CurrentKey;
			static void KeyEventReader();

		private:

			static struct KeyCommand_t
			{
				int LastUsed;
				char* Command;
			};

			struct cmp
			{
				bool operator()(const char* a, const char* b);
			};

			static int GetVirtualKeyCodeByName(char* name);
			
			static std::map<int, std::map <int, KeyCommand_t*> > KeyCommandsState; // key => { index => command }

			static std::map<char*, int, cmp> SystemVirtualKeysCodes;
		};

		static struct CommandHandler
		{
		public:
			void InitRegisterCommands();
		};

		
	};

	struct WorldRead
	{
	public:

		static int WorldList[MAX_ENTITIES];
		static int EnumWorldList;
		static void InitWorldRead();
		static void OrderDistancesMinToMaxByIndex(float* invec, int* order, int len);


	};
	
	struct Bot
	{
	private:
		static vec3_t TargetPos; // target to check

	public:
		static int TargetList[MAX_PLAYERS];
		static int EnumTargets;
		static vec3_t CurrentTargetPos;
		static void __cdecl InitBot(); // rage bot
		static void __cdecl InitMultiBoneBot(); // hvh or whatever...
		static int TargetPoint;
		static vec3_t CDelta;

		struct State
		{
			static bool LastShoot; // if main player has been shot in back frame...
			static bool TargetFound; // if main player has been found a target
			static int CurrentTarget; // current entity target number on world 

		};
	};

};



