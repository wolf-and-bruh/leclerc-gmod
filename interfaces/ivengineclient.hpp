#pragma once
#include "mathlib.hpp"

constexpr auto FL_ONGROUND = (1 << 0);	// At rest / on the ground;
constexpr auto FL_DUCKING = (1 << 1);// Player flag -- Player is fully crouched
constexpr auto FL_WATERJUMP = (1 << 2);	// player jumping out of water
constexpr auto FL_ONTRAIN = (1 << 3);// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
constexpr auto FL_INRAIN = (1 << 4);	// Indicates the entity is standing in rain
constexpr auto FL_FROZEN = (1 << 5);// Player is frozen for 3rd person camera
constexpr auto FL_ATCONTROLS = (1 << 6); // Player can't move, but keeps key inputs for controlling another entity
constexpr auto FL_CLIENT = (1 << 7);// Is a player
constexpr auto FL_FAKECLIENT = (1 << 8);// Fake client, simulated server side; don't send network messages to them
constexpr auto MOVETYPE_NONE	=	0;
constexpr auto MOVETYPE_ISOMETRIC=	1;
constexpr auto MOVETYPE_WALK		=	2;
constexpr auto MOVETYPE_STEP		=	3;
constexpr auto MOVETYPE_FLY		=	4;
constexpr auto MOVETYPE_FLYGRAVITY	=	5;
constexpr auto MOVETYPE_VPHYSICS	=	6;
constexpr auto MOVETYPE_PUSH		=	7;
constexpr auto MOVETYPE_NOCLIP		=	8;
constexpr auto MOVETYPE_LADDER		=	9;
constexpr auto MOVETYPE_OBSERVER = 10;
constexpr auto MOVETYPE_CUSTOM = 11;

typedef struct player_info_s
{
	char name[128];
	int userID; // local server user ID, unique while server is running <- THIS IS NOT STEAMID
	char guid[33]; // that is Steamid
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer; // true, if player is a bot controlled by game.dll
	bool ishltv;
	unsigned int customfiles[4]; // custom files CRC for this player
	unsigned char filesdownloaded; // this counter increases each time the server downloaded a new file
} player_info_t;

class CEngineClient
{
public:
	/*0*/	virtual void* GetIntersectingSurfaces(void const*, Vector const&, float, bool, void*, int) = 0;
	/*1*/	virtual void* GetLightForPoint(Vector const&, bool) = 0;
	/*2*/	virtual void* TraceLineMaterialAndLighting(Vector const&, Vector const&, Vector&, Vector&) = 0;
	/*3*/	virtual void* ParseFile(char const*, char*, int) = 0;
	/*4*/	virtual void* CopyLocalFile(char const*, char const*) = 0;
	/*5*/	virtual void* GetScreenSize(int&, int&) = 0;
	/*6*/	virtual void ServerCmd(char const*, bool) = 0;
	/*7*/	virtual void ClientCmd(char const*) = 0;
	/*8*/	virtual bool GetPlayerInfo(int, player_info_t*) = 0;
	/*9*/	virtual int GetPlayerForUserID(int) = 0;
	/*10*/	virtual void* TextMessageGet(char const*) = 0;
	/*11*/	virtual bool Con_IsVisible(void) = 0;
	/*12*/	virtual int GetLocalPlayer(void) = 0;
	/*13*/	virtual void* LoadModel(char const*, bool) = 0;
	/*14*/	virtual float Time(void) = 0;
	/*15*/	virtual float GetLastTimeStamp(void) = 0;
	/*16*/	virtual void* GetSentence(void*) = 0;
	/*17*/	virtual float GetSentenceLength(void*) = 0;
	/*18*/	virtual bool IsStreaming(void*)const = 0;
	/*19*/	virtual void GetViewAngles(Angle&) = 0;
	/*20*/	virtual void SetViewAngles(Angle&) = 0;
	/*21*/	virtual int GetMaxClients(void) = 0;
	/*22*/	virtual void* Key_LookupBinding(char const*) = 0;
	/*23*/	virtual void* Key_BindingForKey(int) = 0;
	/*24*/	virtual void* StartKeyTrapMode(void) = 0;
	/*25*/	virtual void* CheckDoneKeyTrapping(int&) = 0;
	/*26*/	virtual bool IsInGame(void) = 0;
	/*27*/	virtual bool IsConnected(void) = 0;
	/*28*/	virtual bool IsDrawingLoadingImage(void) = 0;
	/*29*/	virtual void* Con_NPrintf(int, char const*, ...) = 0;
	/*30*/	virtual void* Con_NXPrintf(void const*, char const*, ...) = 0;
	/*31*/	virtual bool IsBoxVisible(Vector const&, Vector const&) = 0;
	/*32*/	virtual bool IsBoxInViewCluster(Vector const&, Vector const&) = 0;
	/*33*/	virtual void* CullBox(Vector const&, Vector const&) = 0;
	/*34*/	virtual void* Sound_ExtraUpdate(void) = 0;
	/*35*/	virtual const char* GetGameDirectory(void) = 0;
	/*36*/	virtual const int& WorldToScreenMatrix() = 0;
	/*37*/	virtual const int& WorldToViewMatrix() = 0;
	/*38*/	virtual void* GameLumpVersion(int)const = 0;
	/*39*/	virtual void* GameLumpSize(int)const = 0;
	/*40*/	virtual void* LoadGameLump(int, void*, int) = 0;
	/*41*/	virtual void* LevelLeafCount(void)const = 0;
	/*42*/	virtual void* GetBSPTreeQuery(void) = 0;
	/*43*/	virtual void* LinearToGamma(float*, float*) = 0;
	/*44*/	virtual void* LightStyleValue(int) = 0;
	/*45*/	virtual void* ComputeDynamicLighting(Vector const&, Vector const*, Vector&) = 0;
	/*46*/	virtual void* GetAmbientLightColor(Vector&) = 0;
	/*47*/	virtual void* GetDXSupportLevel(void) = 0;
	/*48*/	virtual void* SupportsHDR(void) = 0;
	/*49*/	virtual void* Mat_Stub(void*) = 0;
	/*50*/	virtual void* GetChapterName(char*, int) = 0;
	/*51*/	virtual void* GetLevelName(void) = 0;
	/*52*/	virtual void* GetLevelVersion(void) = 0;
	/*53*/	virtual void* GetVoiceTweakAPI(void) = 0;
	/*54*/	virtual void* EngineStats_BeginFrame(void) = 0;
	/*55*/	virtual void* EngineStats_EndFrame(void) = 0;
	/*56*/	virtual void* FireEvents(void) = 0;
	/*57*/	virtual void* GetLeavesArea(int*, int) = 0;
	/*58*/	virtual void* DoesBoxTouchAreaFrustum(Vector const&, Vector const&, int) = 0;
	/*59*/	virtual void* SetAudioState(void const*) = 0;
	/*60*/	virtual void* SentenceGroupPick(int, char*, int) = 0;
	/*61*/	virtual void* SentenceGroupPickSequential(int, char*, int, int, int) = 0;
	/*62*/	virtual void* SentenceIndexFromName(char const*) = 0;
	/*63*/	virtual void* SentenceNameFromIndex(int) = 0;
	/*64*/	virtual void* SentenceGroupIndexFromName(char const*) = 0;
	/*65*/	virtual void* SentenceGroupNameFromIndex(int) = 0;
	/*66*/	virtual void* SentenceLength(int) = 0;
	/*67*/	virtual void* ComputeLighting(Vector const&, Vector const*, bool, Vector&, Vector*) = 0;
	/*68*/	virtual void* ActivateOccluder(int, bool) = 0;
	/*69*/	virtual bool IsOccluded(Vector const&, Vector const&) = 0;
	/*70*/	virtual void* SaveAllocMemory(unsigned long, unsigned long) = 0;
	/*71*/	virtual void* SaveFreeMemory(void*) = 0;
	/*72*/	virtual void* GetNetChannelInfo(void) = 0;
	/*73*/	virtual void* DebugDrawPhysCollide(void const*, void*, int&, int const&) = 0;
	/*74*/	virtual void* CheckPoint(char const*) = 0;
	/*75*/	virtual void* DrawPortals(void) = 0;
	/*76*/	virtual bool IsPlayingDemo(void) = 0;
	/*77*/	virtual bool IsRecordingDemo(void) = 0;
	/*78*/	virtual bool IsPlayingTimeDemo(void) = 0;
	/*79*/	virtual void* GetDemoRecordingTick(void) = 0;
	/*80*/	virtual void* GetDemoPlaybackTick(void) = 0;
	/*81*/	virtual void* GetDemoPlaybackStartTick(void) = 0;
	/*82*/	virtual void* GetDemoPlaybackTimeScale(void) = 0;
	/*83*/	virtual void* GetDemoPlaybackTotalTicks(void) = 0;
	/*84*/	virtual bool IsPaused(void) = 0;
	/*85*/	virtual bool IsTakingScreenshot(void) = 0;
	/*86*/	virtual bool IsHLTV(void) = 0;
	/*87*/	virtual bool IsLevelMainMenuBackground(void) = 0;
	/*88*/	virtual void* GetMainMenuBackgroundName(char*, int) = 0;
	/*89*/	virtual void* GetVideoModes(int&, void*&) = 0;
	/*90*/	virtual void* SetOcclusionParameters(void const*) = 0;
	/*91*/	virtual void* GetUILanguage(char*, int) = 0;
	/*92*/	virtual bool IsSkyboxVisibleFromPoint(Vector const&) = 0;
	/*93*/	virtual const char* GetMapEntitiesString(void) = 0;
	/*94*/	virtual bool IsInEditMode(void) = 0;
	/*95*/	virtual void* GetScreenAspectRatio(void) = 0;
	/*96*/	virtual void* REMOVED_SteamRefreshLogin(char const*, bool) = 0;
	/*97*/	virtual void* REMOVED_SteamProcessCall(bool&) = 0;
	/*98*/	virtual void* GetEngineBuildNumber(void) = 0;
	/*99*/	virtual void* GetProductVersionString(void) = 0;
	/*100*/	virtual void* GrabPreColorCorrectedFrame(int, int, int, int) = 0;
	/*101*/	virtual bool IsHammerRunning(void)const = 0;
	/*102*/	virtual void* ExecuteClientCmd(char const*) = 0;
	/*103*/	virtual void* MapHasHDRLighting(void) = 0;
	/*104*/	virtual void* GetAppID(void) = 0;
	/*105*/	virtual void* GetLightForPointFast(Vector const&, bool) = 0;
	/*106*/	virtual void* ClientCmd_Unrestricted(char const*) = 0;
	/*107*/	virtual void* SetRestrictServerCommands(bool) = 0;
	/*108*/	virtual void* SetRestrictClientCommands(bool) = 0;
	/*109*/	virtual void* SetOverlayBindProxy(int, void*) = 0;
	/*110*/	virtual void* CopyFrameBufferToMaterial(char const*) = 0;
	/*111*/	virtual void* ChangeTeam(char const*) = 0;
	/*112*/	virtual void* ReadConfiguration(bool) = 0;
	/*113*/	virtual void* SetAchievementMgr(void*) = 0;
	/*114*/	virtual void* GetAchievementMgr(void) = 0;
	/*115*/	virtual void* MapLoadFailed(void) = 0;
	/*116*/	virtual void* SetMapLoadFailed(bool) = 0;
	/*117*/	virtual bool IsLowViolence(void) = 0;
	/*118*/	virtual void* GetMostRecentSaveGame(void) = 0;
	/*119*/	virtual void* SetMostRecentSaveGame(char const*) = 0;
	/*120*/	virtual void* StartXboxExitingProcess(void) = 0;
	/*121*/	virtual bool IsSaveInProgress(void) = 0;
	/*122*/	virtual void* OnStorageDeviceAttached(void) = 0;
	/*123*/	virtual void* OnStorageDeviceDetached(void) = 0;
	/*124*/	virtual void* ResetDemoInterpolation(void) = 0;
	/*125*/	virtual void* SetGamestatsData(void*) = 0;
	/*126*/	virtual void* GetGamestatsData(void) = 0;
	/*127*/	virtual void* GetMouseDelta(int&, int&, bool) = 0;
	/*128*/	virtual void* ServerCmdKeyValues(void*) = 0;
	/*129*/	virtual bool IsSkippingPlayback(void) = 0;
	/*130*/	virtual bool IsLoadingDemo(void) = 0;
	/*131*/	virtual bool IsPlayingDemoALocallyRecordedDemo(void) = 0;
	/*132*/	virtual void* Key_LookupBindingExact(char const*) = 0;
	/*133*/	virtual void* GMOD_SetTimeManipulator(float) = 0;
	/*134*/	virtual void* GMOD_SendToServer(void*, unsigned int, bool) = 0;
	/*135*/	virtual void* GMOD_PlaceDecalMaterial(void*, bool, int, void*, Vector const&, Vector const&, int const&, float, float) = 0;
	/*136*/	virtual void* GMOD_GetSpew(char*, unsigned long) = 0;
	/*137*/	virtual void* GMOD_SetViewEntity(unsigned int) = 0;
	/*138*/	virtual void* GMOD_BrushMaterialOverride(void*) = 0;
	/*139*/	virtual void* GMOD_R_RedownloadAllLightmaps(bool) = 0;
	/*140*/	virtual void* GMOD_RawClientCmd_Unrestricted(char const*) = 0;
	/*141*/	virtual void* GMOD_CreateDataTable(void (*)(void*, int, void const*)) = 0;
	/*142*/	virtual void* GMOD_DestroyDataTable(void*) = 0;
	/*143*/	virtual void* GMOD_LoadModel(char const*) = 0;
	/*144*/	virtual void* GMOD_DecalRemoveEntity(int) = 0;
	/*145*/	virtual void* GMOD_TranslateAlias(char const*) = 0;
	/*146*/	virtual void* GMOD_R_StudioInitLightingCache(void) = 0;
	/*147*/	virtual void* PrecacheSentenceFile(char const*) = 0;
	/*148*/	virtual void* GetProtocolVersion(void) = 0;
	/*149*/	virtual bool IsWindowedMode(void) = 0;
	/*150*/	virtual void* FlashWindow(void) = 0;
	/*151*/	virtual void* GetClientVersion(void)const = 0;
	/*152*/	virtual bool IsActiveApp(void) = 0;
	/*153*/	virtual void* DisconnectInternal(void) = 0;
	/*154*/	virtual bool IsInCommentaryMode(void) = 0;

	Angle getViewAngles()
	{
		Angle out;
		this->GetViewAngles(out);
		return out;
	}
};