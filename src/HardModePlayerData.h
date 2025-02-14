#ifndef AZEROTHCORE_HARD_MODE_DATABASE_H
#define AZEROTHCORE_HARD_MODE_DATABASE_H

#include "DatabaseEnv.h"
#include "Player.h"
#include "Chat.h"

class PlayerData {
public:
    static PlayerData* instance();
    
    void Insert (Player* player, uint32 deaths, uint32 reprieves, uint32 currency);
    uint32 GetDeaths (Player* player);
    void AddDeath (Player* player, uint32 deaths);
    void Died (Player* player);
    uint32 GetReprieves (Player* player);
    void AddReprieve (Player* player);
    uint32 GetCurrency (Player* player);
    void AddCurrency (Player* player, uint32 amountToAdd);    
    bool CanPlay (Player* player, uint32 maxDeaths);
    bool CanPurchaseResurrect (Player* player, uint32 numberCurrency);
    void PurchaseResurrect(Player* player, uint32 numberCurrency, bool resetDeaths);
};


#define sPlayerData PlayerData::instance()

#endif //AZEROTHCORE_HARD_MODE_DATABASE_H
