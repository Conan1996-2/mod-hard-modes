#ifndef AZEROTHCORE_HARDMODES_H
#define AZEROTHCORE_HARDMODES_H

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "SpellMgr.h"
#include "Item.h"
#include "ItemTemplate.h"
#include "GameObjectAI.h"
#include "Pet.h"
#include <map>

enum AllowedProfessions
{
    RUNEFORGING    = 53428,
    POISONS        = 2842,
    BEAST_TRAINING = 5149
};


class ChallengeModes {
public:
    static ChallengeModes* instance();

    bool challengesEnabled, serverReset, serverSelfCraftedEnable, serverItemQualityLevelEnable;
    uint32 serverHardMode, serverXPGain;
    float serverXPRate;

    [[nodiscard]] bool enabled() const { return challengesEnabled; }
};

#define sChallengeModes ChallengeModes::instance()

#endif //AZEROTHCORE_CHALLENGEMODES_H
