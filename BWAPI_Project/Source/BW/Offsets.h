#pragma once
#pragma pack(1)

#include <windows.h>
#include <Util/Types.h>
#include "../Storm/storm.h"

#include "Position.h"
#include "TilePosition.h"
#include "Dialog.h"
#include "Pathing.h"

#include "Unit.h"
#include "Sprite.h"
#include "TriggerEngine.h"

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */

#define PLAYER_COUNT             12
#define PLAYABLE_PLAYER_COUNT     8
#define RACE_COUNT                3
#define UNIT_TYPE_COUNT         228
#define TECH_TYPE_COUNT          44
#define UPGRADE_TYPE_COUNT       61
#define WEAPON_TYPE_COUNT       130
#define DAMAGE_TYPE_COUNT         5
#define EXPLOSION_TYPE_COUNT     25
#define FLINGY_TYPE_COUNT       209
#define UNIT_ARRAY_MAX_LENGTH  1700
#define BULLET_ARRAY_MAX_LENGTH 100
#define MAX_SEARCH             3400

namespace BW
{
  class  Unit;
  class  Sprite;
  class  Image;
  struct Bullet;
  class  TileType;
  class  dialog;
  struct bitmap;
  struct dlgEvent;
  struct fntHead;

  struct DatLoad
  {
    u32   address;
    u32   length;
    u32   entries;
  };

  //----------------------------------------------- DATA TABLES ----------------------------------------------
#ifdef _MAC
  static DatLoad *upgradesDat = (DatLoad*)0x0015AFCC;  /** 1.16.1 */
  static DatLoad *techdataDat = (DatLoad*)0x0015A6F4;
  static DatLoad *weaponsDat  = (DatLoad*)0x0015C19C;
  static DatLoad *unitsDat    = (DatLoad*)0x0015AD38;
  static DatLoad *flingyDat   = (DatLoad*)0x0014BC08;
#else
  static DatLoad *upgradesDat = (DatLoad*)0x005136E0;  /** 1.15.3, 1.16, 1.16.1 */
  static DatLoad *techdataDat = (DatLoad*)0x005137D8;
  static DatLoad *weaponsDat  = (DatLoad*)0x00513868;
  static DatLoad *unitsDat    = (DatLoad*)0x00513C30;
  static DatLoad *flingyDat   = (DatLoad*)0x00515A38;
#endif
  //----------------------------------------------- PLAYER DATA ----------------------------------------------
  /** Player resource counts */
  struct PlayerResources
  {
    s32 minerals[PLAYER_COUNT];
    s32 gas[PLAYER_COUNT];
    s32 cumulativeGas[PLAYER_COUNT];
    s32 cumulativeMinerals[PLAYER_COUNT];
  };
  static PlayerResources *BWDATA_PlayerResources = (PlayerResources*) 0x0057F0F0;

  static u8        *BWDATA_PlayerVictory  = (u8*)       0x00650974;
  static Position  *BWDATA_startPositions = (Position*) 0x0058D720;

  /** Force Names */
  struct ForceName
  {
    char  name[30];
  };
  static ForceName *BWDATA_ForceNames = (ForceName*) 0x0058D5BC;

  /** Player Information */
  struct PlayerInfo
  {
    int  dwPlayerID;
    int  dwStormId;
    u8   nType;
    u8   nRace;
    u8   nTeam;
    char szName[25];
  };
  static PlayerInfo *BWDATA_Players       = (PlayerInfo*) 0x0057EEE0;

  static u32 *BWDATA_PlayerDownloadStatus = (u32*) 0x0068F4FC;

  /** Player Alliances */
  struct PlayerAlliance
  {
    u8 player[PLAYER_COUNT];
  };
  static PlayerAlliance *BWDATA_Alliance     = (PlayerAlliance*) 0x0058D634;
  static u32            *BWDATA_PlayerVision = (u32*)            0x0057F1EC;

  // Replay Vision
  static u32            *BWDATA_ReplayVision = (u32*)            0x006D0F18;
  static u32            *BWDATA_ReplayRevealAll = (u32*)         0x006D0F1C;

  /** Code Patches */
  static u32 BWDATA_ServerMenuIn      = 0x004DD5A2;
  static u32 BWDATA_ServerMenuOut     = 0x004DD5C9;
  static u32 BWDATA_OpponentStartHack = 0x004B995D;
  static u32 BWDATA_SingleSpeedHack   = 0x004D99FB;

  struct swishTimer
  {
    WORD wIndex;
    WORD wType;
  };
  static swishTimer *BWDATA_commonSwishControllers  = (swishTimer*)0x005129EC;   // count: 43
  static swishTimer *BWDATA_gluCustmSwishController = (swishTimer*)0x0051A9F0;   // count: 5
  static swishTimer *BWDATA_gluCmpgnSwishController = (swishTimer*)0x00512B10;   // count: 2
  static swishTimer *BWDATA_gluScoreSwishController = (swishTimer*)0x0051A844;   // count: 1
  static swishTimer *BWDATA_gluChatSwishController  = (swishTimer*)0x0051A490;   // count: 5


  static u32    *BWDATA_playerStatusArray   = (u32*)      0x0057F0B8;
  /** QueueGameCommand data */
  static u32    *BWDATA_sgdwBytesInCmdQueue = (u32*)      0x00654AA0;
  static u8     *BWDATA_TurnBuffer          = (u8*)       0x00654880;
  static void  (*BWFXN_sendTurn)()          = (void(*)()) 0x00485A40;
  static u32    BWFXN_QueueCommand          =             0x00485BD0;

  /** Speed & Latency */
  static u32     OriginalSpeedModifiers[7]  =        { 167, 111, 83, 67, 56, 48, 42};
#ifdef _MAC
  static u32     *BWDATA_GameSpeedModifiers = (u32*) 0x0014BF3C;
  static u32     *BWDATA_GameSpeed          = (u32*) 0x00228458;
#else
  static u32     *BWDATA_GameSpeedModifiers = (u32*) 0x005124D8;
  static u32     *BWDATA_GameSpeed          = (u32*) 0x006CDFD4;
#endif

  static u32     *BWDATA_LatencyFrames      = (u32*) 0x0051CE70;
  static u32     *BWDATA_FramesUntilNextTurn = (u32*)0x0051CEA0;
  static int     *BWDATA_FrameSkip          = (int*) 0x005124D4;
  static u8      *BWDATA_Latency            = (u8*)  0x006556e4;
  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  /** Dialog and drawing offsets */
  static bool (__fastcall **BWDATA_GenericDlgInteractFxns)(dialog*,dlgEvent*)   = (bool (__fastcall**)(dialog*,dlgEvent*))    0x005014AC;
  static void (__fastcall **BWDATA_GenericDlgUpdateFxns)(dialog*,int,int,rect*) = (void (__fastcall**)(dialog*,int,int,rect*))0x00501504;

  static dialog  **BWDATA_DialogList       = (dialog**) 0x006D5E34;
  static fntHead **BWDATA_FontBase         = (fntHead**)0x006CE0F4;
  static bitmap  *BWDATA_GameScreenBuffer  = (bitmap*)  0x006CEFF0;
  static bitmap  *BWDATA_GameScreenConsole = (bitmap*)  0x00597240;
  
  static PALETTEENTRY        *BWDATA_GamePalette    = (PALETTEENTRY*)       0x006CE320;
  static LPDIRECTDRAWSURFACE *BWDATA_PrimarySurface = (LPDIRECTDRAWSURFACE*)0x006D5E00;
  static LPDIRECTDRAW        *BWDATA_DDInterface    = (LPDIRECTDRAW*)       0x006D5E08;
  static LPDIRECTDRAWPALETTE *BWDATA_PrimaryPalette = (LPDIRECTDRAWPALETTE*)0x006D5E0C;
  static LPDIRECTDRAWSURFACE *BWDATA_BackSurface    = (LPDIRECTDRAWSURFACE*)0x006D5E10;

  static void (__cdecl *BWFXN_DDrawDestroy)()   = (void(__cdecl*)())0x0041D8B0;
  static u32  BWDATA_DDrawInitCallPatch         = 0x004DB0A2;
  static void (__cdecl *BWFXN_UpdateBltMasks)() = (void(__cdecl*)())0x0041D470;

  static u8 *BWDATA_RefreshRegions = (u8*)0x006CEFF8;
  static u8 *BWDATA_PlayerColors   = (u8*)0x00581DD6;

  struct bounds
  {
    WORD  left;
    WORD  top;
    WORD  right;
    WORD  bottom;
    WORD  width;
    WORD  height;
  };

  struct layer
  {
    BYTE    buffers;
    BYTE    bits;
    WORD    left;
    WORD    top;
    WORD    width;
    WORD    height;
    WORD    alignment;
    bitmap  *pSurface;
    void (__stdcall *pUpdate)(bitmap *pSurface, bounds *pBounds);
  };
  static layer   *BWDATA_ScreenLayers = (layer*)0x006CEF50;
  extern void (__stdcall *pOldDrawGameProc)(BW::bitmap *pSurface, BW::bounds *pBounds);
  extern void (__stdcall *pOldDrawDialogProc)(BW::bitmap *pSurface, BW::bounds *pBounds);

  static RECT *BWDATA_ScrLimit = (RECT*)0x0051A15C;
  static RECT *BWDATA_ScrSize  = (RECT*)0x0051A16C;
  
  //------------------------------------------- CLIST DATA ---------------------------------------------------
  static Unit **BWDATA_UnitNodeList_VisibleUnit_First  = (Unit**)0x00628430;
  static Unit **BWDATA_UnitNodeList_HiddenUnit_First   = (Unit**)0x006283EC;
  static Unit **BWDATA_UnitNodeList_ScannerSweep_First = (Unit**)0x006283F4;
  static Unit *BWDATA_UnitNodeTable                    = (Unit*) 0x0059CCA8;

  static Bullet **BWDATA_BulletNodeTable_FirstElement = (Bullet**)0x0064DEC4;
  static Bullet *BWDATA_BulletNodeTable               = (Bullet*) 0x0064B2E8;

  struct unitFinder
  {
    int unitIndex;
    int searchValue;
  };
  // array size 3400 (1700 * 2) for each
  static unitFinder *BWDATA_UnitOrderingX             = (unitFinder*)0x0066FF78;
  static unitFinder *BWDATA_UnitOrderingY             = &BWDATA_UnitOrderingX[MAX_SEARCH];

  //------------------------------------------- DATA LEVEL ---------------------------------------------------
  /* Mode Stuff */
  static u8             *BWDATA_gameType       = (u8*)  0x00596820;
  static u32            *BWDATA_InGame         = (u32*) 0x006556E0;
  static u32            *BWDATA_InReplay       = (u32*) 0x006D0F14;
  static int            *BWDATA_NetMode        = (int*) 0x0059688C;
  static u32            *BWDATA_CountdownTimer = (u32*) 0x0058D6F4; // Countdown Timer (in seconds)
  static u32            *BWDATA_ElapsedTime    = (u32*) 0x0058D6F8; // Elapsed Game Time (in seconds)

  // note: unaligned because it's part of a structure that we're not using
  static u32            *BWDATA_ReplayFrames   = (u32*) 0x006D0F31;

  static u8             *BWDATA_GameState      = (u8*)  0x006D11EC;
  static u16            *BWDATA_gwNextGameMode = (u16*) 0x0051CE90;
  static u16            *BWDATA_gwGameMode     = (u16*) 0x00596904; // BW::GamePosition
  static u32            *BWDATA_glGluesMode    = (u32*) 0x006D11BC; // BW::MenuPosition

  static u32 *BWDATA_g_LocalHumanID = (u32*) 0x00512688;

  static void **BWDATA_customList_UIDlgData = (void**)0x0051A350;

  //--------------------------------------- FOR RESOLUTION HACK ----------------------------------------------
  static Sprite **BWDATA_spriteGroups = (Sprite**) 0x00629688;

  struct _gametext
  {
    char txt[218];
  };
  static _gametext *BWDATA_Chat_GameText   = (_gametext*)0x00640B60;
  static u8        *BWDATA_Chat_NextLine   = (u8*)       0x00640B58;
  static u8        *BWDATA_Chat_ColorBytes = (u8*)       0x00641674;
  static u32       *BWDATA_Chat_IncrementY = (u32*)      0x00640B20;

  static void (__cdecl *BWFXN_drawDragSelBox)()           = (void (__cdecl*)()) 0x00470040;
  static void (__cdecl *BWFXN_drawAllThingys)()           = (void (__cdecl*)()) 0x00488180;
  static void (__cdecl *BWFXN_drawMapTiles)()             = (void (__cdecl*)()) 0x0049C780;
  static void (__cdecl *BWFXN_blitMapTiles)()             = (void (__cdecl*)()) 0x0040C253;
  static void (__cdecl *BWFXN_drawAllSprites)()           = (void (__cdecl*)()) 0x00498D40;
  //static void (__cdecl *BWFXN_updateImageData)()          = (void (__cdecl*)()) 0x00498CF0;


  static void (__fastcall **BWDATA_InputProcs)(dlgEvent*) = (void (__fastcall**)(dlgEvent*))0x005968A0;
  static dialog **BWDATA_EventDialogs                     = (dialog**)0x006D5E40;

  static u8 *BWDATA_InputFlags = (u8*)0x006CDDC0;

  //----------------------------------------- FUNCTION LEVEL -------------------------------------------------
  static Unit **BWDATA_ClientSelectionGroup = (Unit**) 0x00597208;
  static u8   *BWDATA_ClientSelectionCount  = (u8*)    0x0059723D;
  static u32  BWDATA_PlayerSelection        =          0x006284E0;

  static u32 *BWDATA_isGamePaused           = (u32*)0x006509C4;
  static u32 BWFXN_P_IsGamePaused           = 0x004D974E;

  // These 2 can be merged;
  // void __usercall refundBuildingCost(unsigned __int16 unitType<ax>, unsigned __int8 playerID<cl>)
  // Function takes cost of unit type, multiplies it by 3/4, and adds it to the player's resources.
  static u32 BWFXN_RefundMinerals                 = 0x0042ce97;//building cancel
  static u32 BWFXN_RefundMineralsReturnAddress    = 0x0042ce9e;//building cancel
  static u32 BWFXN_RefundGas                      = 0x0042ceb8;//building cancel
  static u32 BWFXN_RefundGasReturnAddress         = 0x0042cebe;//building cancel

  static u32 BWFXN_RefundMinAndGas2               = 0x0042ced9;//morphing
  static u32 BWFXN_RefundMinAndGas2ReturnAddress  = 0x0042cedf;//morphing

  static u32 BWFXN_RefundMinAndGas5               = 0x00453E62;//research cancel
  static u32 BWFXN_RefundMinAndGas5ReturnAddress  = 0x00453E68;//research cancel

  // These 2 can be merged;
  // void __userpurge refundUpgrade(unsigned __int8 upgradeType<al>, unsigned __int8 playerID)
  // Function calculates the cost of the upgrade by taking the upgrade level, multiplying 
  // with cost factor, and adding base cost, then adding that to player's resources.
  static u32 BWFXN_RefundMin4                     = 0x004541C2;//upgrade cancel
  static u32 BWFXN_RefundMin4ReturnAddress        = 0x004541CD;//upgrade cancel
  static u32 BWFXN_RefundGas4                     = 0x004541FD;//upgrade cancel
  static u32 BWFXN_RefundGas4ReturnAddress        = 0x00454204;//upgrade cancel

  // A little trickier, but can also be merged
  static u32 BWFXN_RefundMin3                     = 0x0045D44D;//morphing
  static u32 BWFXN_RefundMin3ReturnAddress        = 0x0045D454;//morphing
  static u32 BWFXN_RefundGas3                     = 0x0045D46e;//morphing
  static u32 BWFXN_RefundGas3ReturnAddress        = 0x0045D474;//morphing

  static u32 BWFXN_SpendRepair                    = 0x00467006;
  static u32 BWFXN_SpendRepairReturnAddress       = 0x0046700D;

  static void (*BWDATA_DSoundDestroy)()  = (void (*)()) 0x004BC180;

  static u32 BWFXN_NetSelectReturnMenu   = 0x004DC5B0;

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl *BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;
  static u32            *BWDATA_MoveToX                  = (u32*)             0x0062848C;
  static u32            *BWDATA_MoveToY                  = (u32*)             0x006284A8;
  static Position       *BWDATA_MoveToTile               = (Position*)        0x0057F1D0;

  static POINT          *BWDATA_Mouse                    = (POINT*) 0x006CDDC4;
  static u32            *BWDATA_ScreenX                  = (u32*)   0x00628448;
  static u32            *BWDATA_ScreenY                  = (u32*)   0x00628470;

  static TilePosition   *BWDATA_MapSize                  = (TilePosition*) 0x0057F1D4;

  //--------------------------------------------- STRINGS ----------------------------------------------------
  static char           *BWDATA_CurrentMapFileName       = (char*) 0x0057FD3C;
  static char           *BWDATA_CurrentMapName           = (char*) 0x0057FE40;
  static char           *BWDATA_CurrentMapFolder         = (char*) 0x0059BB70;
  static char           *BWDATA_SaveGameFile             = (char*) 0x00685148;

  struct MapVectorEntry   // sizeof 1348
  {
    char  szEntryName[64];        // fixed entry name // 8
    BYTE  bUnknown_48;            // 72
    char  szMapName[32];          // 73
    char  szMapDescription[316];  // 105
    char  szTotalPlayers[35];     // 421
    char  szComputerSlots[35];    // 456
    char  szHumanSlots[35];       // 491
    char  szMapDimensions[35];    // 526
    char  szMapTileset[35];       // 561
    DWORD dwListEntryIndex;       // 596
    DWORD dwUnknown_258;          // 600  Computer slots?
    DWORD dwUnknown_25C;          // 604
    DWORD dwUnknown_260;          // 608
    DWORD dwUnknown_264;          // 612
    BYTE  bEntryFlags;            // 616
    char  szFullPath[MAX_PATH];   // 617
    char  szFileName[MAX_PATH];   // 877
    BYTE  bUnknown_471;           // 1137
    WORD  wMapWidth;              // 1138
    WORD  wMapHeight;             // 1140
    WORD  wMapTileset;            // 1142
    BYTE  bUnknown_478;           // 1144
    BYTE  bTotalPlayers;          // 1145
    BYTE  bComputerSlots;         // 1146
    BYTE  bHumanSlots;            // 1147
    BYTE  bUnknown_47C;           // 1148
    WORD  wUnknown_47D;           // 1149
    struct 
    {
      struct
      {
        BYTE  bCmd;             //  1151
        DWORD dwSeed;           //  1152
        BYTE  bPlayerBytes[8];  //  1156
      } gameInitCommand;
      struct
      {
        DWORD dwUnknownAlways0;   // 1164
        char  szPlayerName[24];   // 1168
        DWORD dwGameFlags;        // 1192
        WORD  wMapWidth;          // 1196
        WORD  wMapHeight;         // 1198
        BYTE  bActivePlayerCount; // 1200
        BYTE  bAvailableSlots;    // 1201
        BYTE  bGameSpeed;         // 1202
        BYTE  bGameState;         // 1203
        WORD  wGameType;          // 1204
        WORD  wGameSubType;       // 1206
        DWORD dwSeed;             // 1208
        WORD  wTileset;           // 1212
        BYTE  bAutosaved;         // set if it was saved as LastReplay    // 1214
        BYTE  bComputerPlayerCount; // 1215
      } gameData;
      char szGameName[25];  // 1216
      char szMapName[32];   // 1241
      struct
      {
        WORD  wGameType;              // 1273
        WORD  wGameSubType;           // 1275
        WORD  wSubTypeDisplay;        // 1277
        WORD  wSubTypeLabel;          // 1279
        BYTE  bVictoryCondition;      // 1281
        BYTE  bResourceType;          // 1282
        BYTE  bUseStandardUnitStats;  // 1283
        BYTE  bFogOfWar_Unused;       // 1284
        BYTE  bStartingUnits;         // 1285
        BYTE  bUseFixedPosition;      // 1286
        BYTE  bRestrictionFlags;      // 1287
        BYTE  bAlliesEnabled;         // 1288
        BYTE  bTeamsEnabled;          // 1289
        BYTE  bCheatsEnabled;         // 1290
        BYTE  bTournamentModeEnabled; // 1291
        DWORD dwVictoryConditionValue;  // 1292
        DWORD dwResourceMineralsValue;  // 1296
        DWORD dwResourceGasValue; // unused // 1300
        BYTE  __align_1; // 1304
      } gameTemplate;
    } partialReplayGameHeader;
    DWORD dwUnknown_519[7]; // 1305
    BYTE  bPlayerSlotEnabled[12];  // 1333
    BYTE  __align_3[3];   // 1345
  };  // 1348

  struct TransVectorEntry
  {
    HANDLE            hTrans;
    RECT              info;
    DWORD             dwReserved;
  };


  template <class _T>
  struct BlizzVectorEntry
  {
    BlizzVectorEntry<_T> *prev; // First entry points to begin in controller
    BlizzVectorEntry<_T> *next; // negative value indicates vector::end and points to &end in controller
    _T container;
  };

  template <class _T>
  struct BlizzVectorController // sizeof 12
  {
    int unknown_00;
    BlizzVectorEntry<_T> *end;
    BlizzVectorEntry<_T> *begin;
  };

  static BlizzVectorController<MapVectorEntry>    *BWDATA_MapListVector   = (BlizzVectorController<MapVectorEntry>*)    0x0051A274;
  static BlizzVectorController<TransVectorEntry>  *BWDATA_TransMaskVector = (BlizzVectorController<TransVectorEntry>*)  0x0051A334;
  static BlizzVectorController<Triggers::Trigger> *BWDATA_TriggerVectors  = (BlizzVectorController<Triggers::Trigger>*) 0x0051A280;

  static bool (__fastcall **BWDATA_TriggerActionFxnTable)(BW::Triggers::Action*) = (bool(__fastcall**)(BW::Triggers::Action*))0x00512800;

  const char            *GetStatString(int index);
  extern char           *BWDATA_StringTableOff;

  //------------------------------------------------ SUPPLIES ------------------------------------------------
  struct AllScores
  {
    s32 allUnitsTotal[PLAYER_COUNT];
    s32 allUnitsProduced[PLAYER_COUNT];
    s32 allUnitsOwned[PLAYER_COUNT];
    s32 allUnitsLost[PLAYER_COUNT];
    s32 allUnitsKilled[PLAYER_COUNT];
    s32 allUnitScore[PLAYER_COUNT];
    s32 allKillScore[PLAYER_COUNT];
    s32 allBuildingsTotal[PLAYER_COUNT];
    s32 allBuildingsConstructed[PLAYER_COUNT];
    s32 allBuildingsOwned[PLAYER_COUNT];
    s32 allBuildingsLost[PLAYER_COUNT];
    s32 allBuildingsRazed[PLAYER_COUNT];
    s32 allBuildingScore[PLAYER_COUNT];
    s32 allRazingScore[PLAYER_COUNT];
    s32 allFactoriesConstructed[PLAYER_COUNT];
    s32 allFactoriesOwned[PLAYER_COUNT];
    s32 allFactoriesLost[PLAYER_COUNT];
    s32 allFactoriesRazed[PLAYER_COUNT];

    /** Supply available, used, and maximum for all players and every race */
    struct SuppliesPerRace
    {
      s32 available[PLAYER_COUNT];
      s32 used[PLAYER_COUNT];
      s32 max[PLAYER_COUNT];
    };
    SuppliesPerRace supplies[RACE_COUNT];
    s32 customScore[PLAYER_COUNT];

    /** Unit counts: all, completed, killed, dead */
    struct Counts
    {
      s32 all[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 completed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 killed[UNIT_TYPE_COUNT][PLAYER_COUNT];
      s32 dead[UNIT_TYPE_COUNT][PLAYER_COUNT];
    };
    Counts unitCounts;
  };
  static AllScores *BWDATA_AllScores = (AllScores*) 0x00581DE4;

  //---------------------------------------------- UNIT DATA -------------------------------------------------
  static u8  *BWDATA_Unit_Graphic             = (u8*)  unitsDat[0].address;
  static u16 *BWDATA_Unit_SubUnit             = (u16*) unitsDat[1].address;
  static u32 *BWDATA_Unit_ConstructionGraphic = (u32*) unitsDat[4].address;
  static u8  *BWDATA_Unit_ShieldsEnabled      = (u8*)  unitsDat[6].address;
  static u16 *BWDATA_Unit_MaxShieldPoints     = (u16*) unitsDat[7].address;
  static s32 *BWDATA_Unit_MaxHitPoints        = (s32*) unitsDat[8].address;
  static u8  *BWDATA_Unit_Elevation           = (u8*)  unitsDat[9].address;
  static u8  *BWDATA_Unit_GroundWeapon        = (u8*)  unitsDat[17].address;
  static u8  *BWDATA_Unit_MaxGroundHits       = (u8*)  unitsDat[18].address;
  static u8  *BWDATA_Unit_AirWeapon           = (u8*)  unitsDat[19].address;
  static u8  *BWDATA_Unit_MaxAirHits          = (u8*)  unitsDat[20].address;
  static u32 *BWDATA_Unit_PrototypeFlags      = (u32*) unitsDat[22].address;
  static u8  *BWDATA_Unit_SeekRange           = (u8*)  unitsDat[23].address;
  static u8  *BWDATA_Unit_SightRange          = (u8*)  unitsDat[24].address;
  static u8  *BWDATA_Unit_ArmorUpgrade        = (u8*)  unitsDat[25].address;
  static u8  *BWDATA_Unit_SizeType            = (u8*)  unitsDat[26].address;
  static u8  *BWDATA_Unit_ArmorAmount         = (u8*)  unitsDat[27].address;
  static u16 *BWDATA_Unit_MineralCost         = (u16*) unitsDat[40].address;
  static u16 *BWDATA_Unit_GasCost             = (u16*) unitsDat[41].address;
  static u16 *BWDATA_Unit_TimeCost            = (u16*) unitsDat[42].address;
  static u8  *BWDATA_Unit_GroupFlags          = (u8*)  unitsDat[44].address;
  static u8  *BWDATA_Unit_SupplyProvided      = (u8*)  unitsDat[45].address;
  static u8  *BWDATA_Unit_SupplyRequired      = (u8*)  unitsDat[46].address;
  static u8  *BWDATA_Unit_SpaceRequired       = (u8*)  unitsDat[47].address;
  static u8  *BWDATA_Unit_SpaceProvided       = (u8*)  unitsDat[48].address;
  static u16 *BWDATA_Unit_BuildScore          = (u16*) unitsDat[49].address;
  static u16 *BWDATA_Unit_DestroyScore        = (u16*) unitsDat[50].address;
  static u16 *BWDATA_Unit_MapStringID         = (u16*) unitsDat[51].address;
  static u8  *BWDATA_Unit_BroodwarOnly        = (u8*)  unitsDat[52].address;

  /** Unit Placement Size */
  struct UnitPlacement_type
  {
    struct Placement_Internal_type
    {
      u16 height;
      u16 width;
    } unitType[UNIT_TYPE_COUNT];
  };
  static UnitPlacement_type *BWDATA_Unit_Placement = (UnitPlacement_type*) unitsDat[36].address;

  /** Unit Dimensions; The distance from the 1px 'center' of unit to each border */
  struct UnitsDimensions_type
  {
    struct UnitDimensions
    {
      u16 left;
      u16 up;
      u16 right;
      u16 down;
    } units[UNIT_TYPE_COUNT];
  };
  static UnitsDimensions_type *BWDATA_Unit_Dimensions = (UnitsDimensions_type*) unitsDat[38].address;

  struct _uavail
  {
    u8 available[12][228];
  };
  static _uavail *BWDATA_UnitAvailability = (_uavail*)0x0057F27C;

  //-------------------------------------------- FLINGY DATA -------------------------------------------------
  static u16 *BWDATA_Flingy_SpriteID        = (u16*) flingyDat[0].address;
  static u32 *BWDATA_Flingy_TopSpeed        = (u32*) flingyDat[1].address;
  static u16 *BWDATA_Flingy_Acceleration    = (u16*) flingyDat[2].address;
  static u32 *BWDATA_Flingy_HaltDistance    = (u32*) flingyDat[3].address;
  static u8  *BWDATA_Flingy_TurnRadius      = (u8*)  flingyDat[4].address;
  static u8  *BWDATA_Flingy_MovementControl = (u8*)  flingyDat[6].address;

  //-------------------------------------------- WEAPON DATA -------------------------------------------------
  static u16 *BWDATA_Weapon_Label              = (u16*) weaponsDat[0].address;
  static u32 *BWDATA_Weapon_Graphic            = (u32*) weaponsDat[1].address;
  static u16 *BWDATA_Weapon_TargetFlags        = (u16*) weaponsDat[3].address;
  static u32 *BWDATA_Weapon_MinRange           = (u32*) weaponsDat[4].address;
  static u32 *BWDATA_Weapon_MaxRange           = (u32*) weaponsDat[5].address;
  static u8  *BWDATA_Weapon_Upgrade            = (u8*)  weaponsDat[6].address;
  static u8  *BWDATA_Weapon_DamageType         = (u8*)  weaponsDat[7].address;
  static u8  *BWDATA_Weapon_Behavior           = (u8*)  weaponsDat[8].address;
  static u8  *BWDATA_Weapon_RemoveAfter        = (u8*)  weaponsDat[9].address;
  static u8  *BWDATA_Weapon_ExplosionType      = (u8*)  weaponsDat[10].address;
  static u16 *BWDATA_Weapon_InnerSplashRadius  = (u16*) weaponsDat[11].address;
  static u16 *BWDATA_Weapon_MedianSplashRadius = (u16*) weaponsDat[12].address;
  static u16 *BWDATA_Weapon_OuterSplashRadius  = (u16*) weaponsDat[13].address;
  static u16 *BWDATA_Weapon_DamageAmount       = (u16*) weaponsDat[14].address;
  static u16 *BWDATA_Weapon_DamageBonus        = (u16*) weaponsDat[15].address;
  static u8  *BWDATA_Weapon_DamageCooldown     = (u8*)  weaponsDat[16].address;
  static u8  *BWDATA_Weapon_DamageFactor       = (u8*)  weaponsDat[17].address;
  static u8  *BWDATA_Weapon_AttackDirection    = (u8*)  weaponsDat[18].address;
  static u8  *BWDATA_Weapon_LaunchSpin         = (u8*)  weaponsDat[19].address;
  static u8  *BWDATA_Weapon_XOffset            = (u8*)  weaponsDat[20].address;
  static u8  *BWDATA_Weapon_YOffset            = (u8*)  weaponsDat[21].address;

  //------------------------------------------- UPGRADE DATA -------------------------------------------------
  static u16            *BWDATA_Upgrade_MineralCostBase   = (u16*)  upgradesDat[0].address;
  static u16            *BWDATA_Upgrade_MineralCostFactor = (u16*)  upgradesDat[1].address;
  static u16            *BWDATA_Upgrade_GasCostBase       = (u16*)  upgradesDat[2].address;
  static u16            *BWDATA_Upgrade_GasCostFactor     = (u16*)  upgradesDat[3].address;
  static u16            *BWDATA_Upgrade_TimeCostBase      = (u16*)  upgradesDat[4].address;
  static u16            *BWDATA_Upgrade_TimeCostFactor    = (u16*)  upgradesDat[5].address;
  static u16            *BWDATA_Upgrade_Label             = (u16*)  upgradesDat[8].address;
  static u8             *BWDATA_Upgrade_Race              = (u8*)   upgradesDat[9].address;
  static u8             *BWDATA_Upgrade_MaxRepeats        = (u8*)   upgradesDat[10].address;

  struct _scUpgrs {    u8 level[PLAYER_COUNT][46];  };
  struct _bwUpgrs {    u8 level[PLAYER_COUNT][15];  };
  static _scUpgrs *BWDATA_UpgradeMaxSC   = (_scUpgrs*)0x0058D088;
  static _scUpgrs *BWDATA_UpgradeLevelSC = (_scUpgrs*)0x0058D2B0;
  static _bwUpgrs *BWDATA_UpgradeMaxBW   = (_bwUpgrs*)0x0058F24A;
  static _bwUpgrs *BWDATA_UpgradeLevelBW = (_bwUpgrs*)0x0058F32C;

  static u32 BWDATA_UpgradeProgress = 0x0058F3E0;

  //--------------------------------------------- TECH DATA --------------------------------------------------
  static u16 *BWDATA_Tech_MineralCost  = (u16*) techdataDat[0].address;
  static u16 *BWDATA_Tech_GasCost      = (u16*) techdataDat[1].address;
  static u16 *BWDATA_Tech_TimeCost     = (u16*) techdataDat[2].address;
  static u16 *BWDATA_Tech_EnergyCost   = (u16*) techdataDat[3].address;
  static u16 *BWDATA_Tech_LabelIndex   = (u16*) techdataDat[7].address;

  struct _scTechs {    u8 enabled[PLAYER_COUNT][24];  };
  struct _bwTechs {    u8 enabled[PLAYER_COUNT][20];  };
  static _scTechs *BWDATA_TechAvailableSC = (_scTechs*)0x0058CE24;
  static _scTechs *BWDATA_TechResearchSC  = (_scTechs*)0x0058CF44;
  static _bwTechs *BWDATA_TechAvailableBW = (_bwTechs*)0x0058F050;
  static _bwTechs *BWDATA_TechResearchBW  = (_bwTechs*)0x0058F140;

  static u32 BWDATA_ResearchProgress  = 0x0058F230;

  //------------------------------------------------ MAPPING -------------------------------------------------
  /** Higher 12 bits for tile group, lower 4 bits for variant of tile in the tile group. */
  typedef u16 TileID;
  extern TileID   *BWDATA_MapTileArray;
  extern TileType *BWDATA_TileSet;

  /** Direct mapping of minitile flags array */
  struct MiniTileMaps_type
  {
    struct MiniTileFlagArray
    {
      u16 miniTile[16];
    };
    MiniTileFlagArray tile[0x10000];
  };
  extern MiniTileMaps_type *BWDATA_MiniTileFlags;

  struct activeTile
  {
    u8 bVisibilityFlags;
    u8 bExploredFlags;
    u8 bWalkable          : 1; // Set on tiles that can be walked on
    u8 bUnknown1          : 1; // Unused?
    u8 bUnwalkable        : 1; // Set on tiles that can't be walked on
    u8 bUnknown2          : 3; // Unused?
    u8 bHasCreep          : 1; // Set when creep occupies the area
    u8 bAlwaysUnbuildable : 1; // always unbuildable, like water
    u8 bGroundHeight      : 3; // ground height
    u8 bCurrentlyOccupied : 1; // unbuildable but can be made buildable
    u8 bCreepReceeding    : 1; // Set when the nearby structure supporting the creep is destroyed
    u8 bCliffEdge         : 1; // Set if the tile is a cliff edge
    u8 bTemporaryCreep    : 1; // Set when the creep occupying the area was created. Not set if creep tiles were preplaced.
    u8 bUnknown3          : 1; // Unused?
  };
  extern activeTile        *BWDATA_ActiveTileArray;
  extern SAI_Paths         *BWDATA_SAIPathing;

  // EXPERIMENTAL
  //static int (__stdcall *BWFXN_getTileDistance)(int x, int y, int x2, int y2) = (int (__stdcall*)(int,int,int,int))0x00444100;
  
  struct baseLocation
  {
    /* 0x00 */BW::Position position;
    /* 0x04 */BYTE  mineralClusters;
    /* 0x05 */BYTE  gasGeysers;
    /* 0x06 */BYTE  isStartLocation; // The "base" is actually a Start Location
    /* 0x07 */BYTE  bFlags;
                    /*  0x01 - This base is currently occupied.
                        0x02 - An AI script has acquired this base as an expansion.
                    */
    /* 0x08 */DWORD remainingMinerals;
    /* 0x0C */DWORD remainingGas;
    /* 0x10 */DWORD unk_10[8];
  };

  // 250 entries max
  //static baseLocation *BWDATA_Bases = (baseLocation*)0x00692688;
};
#pragma pack()
