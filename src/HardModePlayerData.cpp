/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "HardModePlayerData.h"

PlayerData* PlayerData::instance() {
    static PlayerData instance;
    return &instance;
}

void PlayerData::Insert(Player* player, uint32 deaths, uint32 reprieves, uint32 currency) {
    QueryResult result = CharacterDatabase.Query("INSERT INTO mod_hard_modes VALUES ('{}', '{}', '{}', '{}', '{}')",
        player->GetSession()->GetAccountId(), player->GetGUID().GetCounter(), deaths, reprieves, currency);

}

uint32 PlayerData::GetDeaths(Player* player) {
    QueryResult result = CharacterDatabase.Query("SELECT deaths FROM mod_hard_modes WHERE account={} AND GUID={}",
        player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (result && result->GetRowCount() != 0) {
        Field* fields = result->Fetch();
        return fields[0].Get <uint32>();
    }
    Insert(player, 0, 0, 0);
    return 0;
}

void PlayerData::AddDeath(Player* player, uint32 deaths) {
    QueryResult result = CharacterDatabase.Query("UPDATE mod_hard_modes SET deaths={} WHERE account={} AND GUID={}",
        GetDeaths(player) + deaths, player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (!result || result->GetRowCount() == 0) Insert(player, deaths, 0, 0);
}

void PlayerData::Died(Player * player) {
    AddDeath(player, 1);
}

uint32 PlayerData::GetReprieves(Player* player) {
    QueryResult result = CharacterDatabase.Query("SELECT reprieves FROM mod_hard_modes WHERE account={} AND GUID={}",
        player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (result && result->GetRowCount() != 0) {
        Field* fields = result->Fetch();
        return fields[0].Get <uint32> ();
    }
    Insert(player, 0, 0, 0);
    return 0;
}

void PlayerData::AddReprieve(Player* player) {
    QueryResult result = CharacterDatabase.Query("UPDATE mod_hard_modes SET reprieves={} WHERE account={} AND GUID={}",
        GetReprieves(player) + 1, player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (!result || result->GetRowCount() == 0) Insert(player, 0, 1, 0);
}

uint32 PlayerData::GetCurrency(Player* player) {
    QueryResult result = CharacterDatabase.Query("SELECT currency FROM mod_hard_modes WHERE account={} AND GUID={}",
        player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (result && result->GetRowCount() != 0) {
        Field* fields = result->Fetch();
        return fields[0].Get <uint32> ();
    }
    Insert(player, 0, 0, 0);
    return 0;
}

void PlayerData::AddCurrency(Player* player, uint32 amountToAdd) {
    QueryResult result = CharacterDatabase.Query("UPDATE mod_hard_modes SET currency={} WHERE account={} AND GUID={}",
        GetCurrency(player) + amountToAdd, player->GetSession()->GetAccountId(), player->GetGUID().GetCounter());
    if (!result || result->GetRowCount() == 0) Insert(player, 0, 0, amountToAdd);
}

bool PlayerData::CanPlay(Player* player, uint32 maxDeaths) {
    return GetDeaths(player) <= maxDeaths;
}

bool PlayerData::CanPurchaseResurrect(Player* player, uint32 currencyCost) {
    return GetCurrency(player) >= currencyCost;
}

void PlayerData::PurchaseResurrect(Player* player, uint32 currencyCost, bool resetDeaths) {
    if (CanPurchaseResurrect (player, currencyCost)) {
        AddCurrency(player, -currencyCost);
        AddReprieve(player);
        if (resetDeaths) AddDeath(player, -GetDeaths(player));
        else AddDeath(player, -1);
    }
}
