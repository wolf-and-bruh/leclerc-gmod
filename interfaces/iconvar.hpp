/*class ConCommandBase {
public:
    virtual void DESTROY1() = 0;
    virtual void unk() = 0;
    virtual bool IsCommand(void) = 0;
    virtual bool IsBoundedVar(void) = 0;
    virtual bool IsFlagSet(long long) = 0;
    virtual void AddFlags(long long) = 0;
    virtual void RemoveFlags(long long) = 0;
    virtual long long GetFlags(void) = 0;
    virtual const char* GetName(void) = 0;
    virtual const char* GetHelpText(void) = 0;
    virtual bool IsRegistered(void) = 0;
    virtual void* GetDLLIdentifier(void) = 0;
    virtual void Create(char const*, char const*, long long) = 0;
    virtual void Init(void) = 0;
    virtual const char* GetBaseName(void) = 0;
    virtual int GetSplitScreenPlayerSlot(void) = 0;
    virtual void SetValue(char const*) = 0; // 16
    virtual void SetValue(float) = 0; // 17
    virtual void SetValue(int) = 0; // 18
    virtual void SetValue(void*) = 0;
    virtual float GetFloat(void) = 0;
    virtual int GetInt(void) = 0;
    virtual bool GetBool(void) = 0;
    virtual void InternalSetValue(char const*) = 0;
    virtual void InternalSetFloatValue(float) = 0;
    virtual void InternalSetIntValue(int) = 0;
    virtual void InternalSetColorValue(void*) = 0;
    virtual void ClampValue(float&) = 0;
    virtual void ChangeStringValue(char const*, float) = 0;
    virtual void Create() = 0;
};*/

#define FCVAR_ARCHIVE 128
#define FCVAR_ARCHIVE_XBOX 16777216
#define FCVAR_CHEAT 16384
#define FCVAR_CLIENTCMD_CAN_EXECUTE 1073741824
#define FCVAR_CLIENTDLL 8
#define FCVAR_DEMO 65536
#define FCVAR_DONTRECORD 131072
#define FCVAR_GAMEDLL 4
#define FCVAR_LUA_CLIENT 262144
#define FCVAR_LUA_SERVER 524288
#define FCVAR_NEVER_AS_STRING 4096
#define FCVAR_NONE 0
#define FCVAR_NOTIFY 256
#define FCVAR_NOT_CONNECTED 4194304
#define FCVAR_PRINTABLEONLY 1024
#define FCVAR_PROTECTED 32
#define FCVAR_REPLICATED 8192
#define FCVAR_SERVER_CANNOT_QUERY 536870912
#define FCVAR_SERVER_CAN_EXECUTE 268435456
#define FCVAR_SPONLY 64
#define FCVAR_UNLOGGED 2048
#define FCVAR_UNREGISTERED 1
#define FCVAR_USERINFO 512

class ConVar {
public:
    ConVar* pNextConvar; //0x0008
    uint32_t bRegistered; //0x0010
    char pad_0014[4]; //0x0014
    char* pszName; //0x0018
    char* pszHelpString; //0x0020
    uint32_t nflags; //0x0028
    char pad_002C[4]; //0x002C
    void* s_pConCommandBases; //0x0030
    ConVar* pParent; //0x0038
    char* pszDefaultValue; //0x0040
    char* pszValueStr; //0x0048
    uint32_t strLength; //0x0050
    float fVal; //0x0054
    int32_t intValue; //0x0058
    uint32_t bHasMin; //0x005C
    float fMinVal; //0x0060
    uint32_t bHasMax; //0x0064
    float fMaxVal; //0x0068
    char pad_006C[4]; //0x006C
    PVOID CALLBACKPTR; //0x0070

    void AddFlags(int value) {
        mem::Call<void>(this, 3, value);
    }

    void RemoveFlag(int value) {
        mem::Call<void>(this, 4, value);
    }

    void InternalSetValue(const char* value) {
        mem::Call<void>(this, 14, value);
    }

    void InternalSetValue(float value) {
        mem::Call<void>(this, 15, value);
    }

    void InternalSetValue(int value) {
        mem::Call<void>(this, 16, value);
    }
};

class ICVar {
public:
    ConVar* FindCommandBase(char const* convar) {
        return mem::Call<ConVar*>(this, 15, convar);
    }
};