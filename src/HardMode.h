#ifndef AZEROTHCORE_HARD_MODE_H
#define AZEROTHCORE_HARD_MODE_H

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "SpellMgr.h"
#include "Pet.h"
#include "Log.h"
#include "DatabaseEnv.h"
#include "HardModePlayerData.h"

enum AllowedProfessions
{
    RUNEFORGING    = 53428,
    POISONS        = 2842,
    BEAST_TRAINING = 5149
};


class ChallengeModes {
public:
    static ChallengeModes* instance();

    bool challengesEnabled, serverReset, serverSelfCraftedEnable, serverItemQualityLevelEnable, serverResetDeathOnCurrencyUse;
    uint32 serverHardMode, serverXPGain, serverCurrencyCostToRes, serverNumberOfDeaths;
    float serverXPRate;
    
    [[nodiscard]] bool enabled() const { return challengesEnabled; }
};

#define sChallengeModes ChallengeModes::instance()

#endif //AZEROTHCORE_HARD_MODE_H
