#pragma once

#include <Util/Types.h>

#include <BWAPI/UnitType.h>
#include <BWAPI/Position.h>
#include <BWAPI/Unit.h>

#include "PositionUnitTarget.h"
#include "Race.h"
#include <BW/TilePosition.h>

namespace BWAPI { class UnitImpl; }
#pragma pack(1)
namespace BW
{
  /** Set of classes that can pass to IssueCommand function. */
  namespace Orders
  {
    /** Attack Location command in bw, can target either location or target. */
    class Attack
    {
      public :
        /** Attack Location on position. */
        Attack(const BW::Position& target, int order, bool queued = false);
        Attack(int x, int y, int order, bool queued = false);
        Attack(const BWAPI::Position& target, int order, bool queued = false);
        /** Attack Location on unit. */
        Attack(BWAPI::Unit* target, int order, bool queued = false);
        /** Attack Location on general target. */
        Attack(const PositionUnitTarget& target, int order, bool queued = false);
      private :
        /** 0x15 = Attack Location command-code in bw */
        u8 always0x15;
        BW::PositionUnitTarget target;
        u16 always0xe4;
        u8 order;
        u8 type;
    };
    /** Right click command in bw, can target either location or target. */
    class RightClick
    {
      public :
        /** Right-click on position. */
        RightClick(const BW::Position& target, bool queued = false);
        RightClick(int x, int y, bool queued = false);
        /** Right-click on unit. */
        RightClick(BWAPI::Unit* target, bool queued = false);
        /** Right-click on general target. */
        RightClick(const PositionUnitTarget& target, bool queued = false);
      private :
        /** 0x14 = Rightclick command-code in bw */
        u8 always0x14;
        BW::PositionUnitTarget target;
        u16 always0xe4;
        u8  type;
    };
    /** Selection addition in bw */
    class SelectAdd
    {
      public :
        SelectAdd(int count, BWAPI::UnitImpl **units);
        SelectAdd(int count, BW::Unit **units);
        /** 0x0A = Shift-Select command-code in bw */
        u8         always0x0A;
        u8         targCount;
        UnitTarget targets[12];
        u32        size;
    };
    /** Selection command in bw */
    class Select
    {
      public :
        Select(int count, BWAPI::UnitImpl **units);
        Select(int count, BW::Unit **units);
        Select(const std::vector<BW::Unit*> &vUnits);
        Select(const std::vector<BWAPI::UnitImpl*> &vUnits);
        /** 0x09 = Select command-code in bw */
        u8         always0x09;
        u8         targCount;
        UnitTarget targets[12];
        u32        size;
    };
    /** Train unit command in bw. */
    class TrainUnit
    {
      public :
        TrainUnit(int type);
        /** 0x1f = Train Command-code in bw */
        u8 always0x1f;
        /** Type of unit to train */
        u16 type;
    };
    /** Train unit command in bw. */
    class TrainFighter
    {
      public :
        TrainFighter();
        /** 0x27 = Train fighter Command-code in bw */
        u8 always0x27;
    };
    /** Make building. */
    class MakeBuilding
    {
      public :
        MakeBuilding(BW::TilePosition position, int type);
        MakeBuilding(int tileX, int tileY, int type);
        /** 0x0c = make building Command-code in bw */
        u8 always0x0c;
        /** Specifies race of builder: zerg = 0x19, terran = 1e, toss = 1f */
        u8 raceDependant;
        BW::TilePosition position;
        /** Type of building to make */
        u16 type;
    };

    /** Place COP */
    class PlaceCOP
    {
      public :
        PlaceCOP(BW::TilePosition position, int type);
        PlaceCOP(int x, int y, int type);
        /** 0x0c = make building Command-code in bw */
        u8 always0x0c;
        u8 always0x9B;
        BW::TilePosition position;
        /** Type of building to make */
        u16 type;
    };

    /** Invent tech. */
    class Invent
    {
      public :
        Invent(int type);
        /** 0x30 = invent tech Command-code in bw */
        u8 always0x30;
        /** Type of tech to invent */
        u8 tech;
    };
    class Upgrade
    {
      public :
        Upgrade(int upgrade);
        /** 0x32 = upgrade Command-code in bw */
        u8 always0x32;
        /** Type of upgrade */
        u8 upgrade;
    };
    class MakeAddon
    {
      public :
        MakeAddon(BW::TilePosition position, int type);
        MakeAddon(BWAPI::TilePosition position, int type);
        MakeAddon(int tileX, int tileY, int type);
        /** 0x0c = make building Command-code in bw */
        u8 always0x0c;
        /** 1e for terran 1f for protoss*/
        u8 always0x24;
        BW::TilePosition position;
        /** Type of building to make */
        u16 type;
    };
    class MakeNydusExit
    {
      public :
        MakeNydusExit(BW::TilePosition position);
        MakeNydusExit(int tileX, int tileY);
        /** 0x0c = make building Command-code in bw */
        u8 always0x0c;
        /** 1e for terran 1f for protoss*/
        u8 always0x2E;
        BW::TilePosition position;
        /** Type of building to make */
        u16 type;
    };
    /** Change slot command in bw. */
    class ChangeSlot
    {
      public :
      typedef enum Slot
        {
          Computer = 0,
          Open = 1,
          Closed = 2
        };
        ChangeSlot(Slot slot, int slotID);
      private :
        /** 0x44 = Change slot command-code in bw */
        u8 always0x44;
        /** Order of the slot to change (0 for the 1st slot) */
        u8 slotID;
        /** Target slot state. */
        Slot slot;
    };
    /** Change race command in bw. */
    class RequestChangeRace
    {
      public :
        RequestChangeRace(int slot, int slotID);
      private :
        /** 0x41 = Command code for change race in bw. */
        u8 always0x41;
        /** Order of the slot to change (0 for the 1st slot). */
        u8 slotID;
        /** Target slot race. */
        u8 race;
    };
    /** Lobby slot alteration (sent by host only). */
    class UpdateSlot
    {
    public:
      UpdateSlot(int slot, int stormPlayerID, int owner, int newRace, int team);
    private:
      /** 0x3E = Command code */
      u8 always0x3E;
      /** Order of the slot to change (0 for the 1st slot). */
      u8 bSlot;
      /** Storm ID of the player to map the slot to. */
      u8 bStormPlayerID;
      /** Player type. */
      u8 nType;
      /** Player's race. */
      u8 bNewRace;
      /** Player's force. */
      u8 nTeam;
    };
    /** Starts game in the pre-game lobby. */
    class StartGame
    {
      public :
        StartGame();
      private :
        /** 0x3c = Command code for start game. */
        u8 always0x3c;
    };
    /** Pauses the game. */
    class PauseGame
    {
      public :
        PauseGame();
      private :
        /** 0x10 = Command code for pause game. */
        u8 always0x10;
    };
    class ResumeGame
    {
      public :
        ResumeGame();
      private :
        /** 0x11 = Command code for unpause game. */
        u8 always0x11;
    };
    class LeaveGame
    {
      public :
        LeaveGame(int type);
      private :
        /** 0x57 = Command code for unpause game. */
        u8 always0x57;
        u8 type;
    };

    class MergeDarkArchon
    {
      public :
        MergeDarkArchon();
      private :
        /** 0x5A = Command code for Merge Dark Archon. */
        u8 always0x5A;
    };

    class MergeArchon
    {
      public :
        MergeArchon();
      private :
        /** 0x2A = Command code for Merge Archon. */
        u8 always0x2A;
    };

    class MinimapPing
    {
      public :
        MinimapPing(BW::Position position);
        MinimapPing(BWAPI::Position position);
        MinimapPing(int x, int y);
      private :
        /** 0x58 = Command code for Minimap Ping. */
        u8 always0x58;
        BW::Position position;
    };

    class UseStimPack
    {
      public :
        UseStimPack();
      private :
        /** 0x36 = Command code for Stim */
        u8 always0x36;
    };

    class BuildingMorph
    {
      public :
        BuildingMorph(int type);
        /** 0x35 = Building Morph for zerg */
        u8 always0x35;
        /** Type of unit to train */
        u16 type;
    };

    class CancelAddon
    {
      public :
        CancelAddon();
      private :
        u8 always0x34;
    };

    class CancelUpgrade
    {
      public :
        CancelUpgrade();
      private :
        u8 always0x33;
    };

    class CancelResearch
    {
      public :
        CancelResearch();
      private :
        u8 always0x31;
    };

    class CancelNuke
    {
      public :
        CancelNuke();
      private :
        u8 always0x2E;
    };

    class Lift
    {
      public :
        Lift();
      private :
        u8 always0x2F;
        u32 unknown;
    };

    class Land
    {
      public :
        Land(BW::TilePosition position, int type);
        Land(int x, int y, int type);
      private :
        u8 always0x0C;
        u8 always0x47;
        BW::TilePosition position;
        u16 type;
    };

    class Burrow
    {
      public :
        Burrow();
      private :
        u8 always0x2C;
        u8 unused;
    };

    class Unburrow
    {
      public :
        Unburrow();
      private :
        u8 always0x2D;
        u8 unused;
    };

    class HoldPosition
    {
      public :
        HoldPosition(bool queued = false);
      private :
        u8 always0x2B;
        u8 type;
    };

    class UnloadUnit
    {
      public :
        UnloadUnit(BWAPI::Unit* unload);
        /** 0x29 = Unload Unit */
        u8 always0x29;
        /** The unit to unload bw index */
        UnitTarget target;
    };

    class UnloadAll
    {
      public :
        UnloadAll();
      private :
        u8 always0x28;
        u8 unused;
    };

    class Siege
    {
      public :
        Siege();
      private :
        u8 always0x26;
        u8 unused;
    };

    class Unsiege
    {
      public :
        Unsiege();
      private :
        u8 always0x25;
        u8 unused;
    };

    class UnitMorph
    {
      public :
        UnitMorph(int type);
        /** 0x23 = Unit Morph (Zerg) Command-code in bw */
        u8 always0x23;
        /** Type of unit to train */
        u16 type;
    };

    class Cloak
    {
      public :
        Cloak();
      private :
        u8 always0x21;
        u8 unused;
    };

    class Decloak
    {
      public :
        Decloak();
      private :
        u8 always0x22;
        u8 unused;
    };

    class ReturnCargo
    {
      public :
        ReturnCargo(bool queued = false);
      private :
        u8 always0x1E;
        u8 type;
    };

    class Stop
    {
      public :
        Stop(bool queued = false);
      private :
        u8 always0x1A;
        u8 type;
    };

    class ReaverStop
    {
      public :
        ReaverStop();
      private :
        u8 always0x1C;
    };

    class CarrierStop
    {
      public :
        CarrierStop();
      private :
        u8 always0x1B;
    };

    class CancelUnitMorph
    {
      public :
        CancelUnitMorph();
      private :
        u8 always0x19;
    };

    class CancelConstruction
    {
      public :
        CancelConstruction();
      private :
        u8 always0x18;
    };

    class CancelTrain
    {
      public :
        CancelTrain(int slot = -2);
      private :
        u8 always0x20;
        s8 slot;
        u8 unknown;
    };

    class UseCheat
    {
      public :
        UseCheat(u32 flags);
      private :
        u8 always0x12;
        u32 flags;
    };

    class RestartGame
    {
      public :
        RestartGame();
      private :
        u8 always0x08;
    };

    class SetAllies
    {
      public :
        SetAllies(u32 allies);
      private :
        u8  always0x0E;
        u32 allies;
    };

    class SetVision
    {
      public :
        SetVision(u16 vision);
      private :
        u8  always0x0D;
        u16 vision;
    };
  } // namespace orders
}; // namespace BW
#pragma pack()
