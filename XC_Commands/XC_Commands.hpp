//#pragma warning(disable : 4996)
/*

	author: TheCyborgX
	date: 07/05/2017
	
	license: idk...
*/

#include <type_traits>
#include <map>
#include <cstring>
#include <cstdio>

// commands target mode




/* 

	Init, TODO should have enought information...
*/




#define XC_MODE_FUNCTION_T 0
#define XC_MODE_FUNCTION_PARAM_T 1
#define XC_MODE_VALUE_T 2

#define XC_RESTRICTION_ALLOWONLY 1
#define XC_RESTRICTION_RESTRICT 2
#define XC_RESTRICTION_SIZE 3
#define XC_RESTRICTION_HANDLER 4
#define XC_RESTRICTION_NOLIMITS -1

struct cmp_str
{
   bool operator()(char const *a, char const *b);
};




template<class T>
bool IsInt(T);


template<class T>
bool IsFloat(T);


template<class T>
bool IsCharPtr(T);


template<class _T>
struct XC_Commands
{
public:
	static std::map<const char*, std::map<char, _T>, cmp_str > cmds_t;
	static bool CommandExists(const char* command);
};




template<class _t>
struct XC_Restriction_t
{
	signed short restriction;
	_t min;
	_t max;
	void* restricted_values_or_allowonly_vec;
	int veclen;
	bool(__cdecl*RestrictHandler)(const char* command, _t value);

};

template<class _T>
class XC_Commands_t : XC_Commands<_T>
{
	public:
		static void RegisterNewCommand(const char* command, _T val_t, void* target, short mode_t, bool InitValTarget = false);
		static void SetCommandRestriction(const char* command, signed short restriction_t, _T min = 0, _T max = 0, void* xptrparam = 0x0, size_t vecsz = 0); // default null ptr...

		enum COMMAND_MODE { FUNCTION_T = 0, FUNCTION_PARAM_T = 1, VALUE_T = 2 };
		
	private:	
		static void RegisterCommandOffset(const char* command, void* offset);
		
};



template<class _T>
class XC_Commands_V
{
	public:
	static _T GetCommandValue(const char* command);
	static _T GetValueFromTheCommand(char* buff);
	static bool IsValueOnCommand(char* buff);
};

class XC_Commands_Type
{
	public:
		static int GetCommandDataType(const char* command);
		static int GetCommandMode(const char* command);
		static void* GetCommandOffset(const char* offset);
		static size_t GetCommandMaxLen(const char* command);
		enum XC_Data_T { XC_INT_T = 1, XC_FLOAT_T = 2, XC_CHARPTR_T  = 3 };
		template<class _t>
		static int GetDataTypeCode(_t v);

};



class XC_Commands_Handler : XC_Commands_Type
{
	public:
		template<class _T>
		static void ExecuteCommand(const char* command, _T value);	

		struct XC_Commands_Event_Handler
		{
			static void(__cdecl*OnCommandEvent)(const char* command, void* value);
		};
		
};











class IntellCommandAnalyzer : XC_Commands_Type
{
	public:
	static int SafeExecuteCommand(char* buff);
	static int GetCommandLen(char* buff);
	static void GetCommand(char* incommand, char* outcommand);
	static int GetCurrentCommandDataType(char* buff);
};



#if _ITERATOR_DEBUG_LEVEL == 0
#pragma comment(lib, "../XC_Commands/XC_Commands_r")
#else
#pragma comment(lib, "../XC_Commands/XC_Commands_d")
#endif
