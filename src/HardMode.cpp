/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */
#include "Log.h"
#include "HardMode.h"

ChallengeModes* ChallengeModes::instance() {
    static ChallengeModes instance;
    return &instance;
}

class ChallengeModes_WorldScript : public WorldScript {
public:
    ChallengeModes_WorldScript() : WorldScript("ChallengeModes_WorldScript") {}

    void OnBeforeConfigLoad(bool /*reload*/) override {
        LoadConfig();
    }

private:
    static void LoadConfig() {
        sChallengeModes->challengesEnabled = sConfigMgr->GetOption<bool>("HardMode.Enable", false);
        if (sChallengeModes->enabled()) {
	    LOG_INFO ("server.loading", "\n\rLoading mod-hard-modes...");
            sChallengeModes->serverHardMode               = sConfigMgr->GetOption<uint32>("HardMode.Mode", 0);
            sChallengeModes->serverReset                  = sConfigMgr->GetOption<bool>("HardMode.Reset", false);
            sChallengeModes->serverSelfCraftedEnable      = sConfigMgr->GetOption<bool>("HardMode.SelfCrafted", false);
            sChallengeModes->serverItemQualityLevelEnable = sConfigMgr->GetOption<bool>("HardMode.ItemQualityLevel", false);
            sChallengeModes->serverXPGain                 = sConfigMgr->GetOption<uint32>("HardMode.XPGain", 0);
            sChallengeModes->serverXPRate                 = sConfigMgr->GetOption<float>("HardMode.XPRate", 1.0f);
            
            LOG_INFO ("server.loading", sChallengeModes->serverHardMode == 1 ? ">> Server is Hardcore" : 
                sChallengeModes->serverHardMode == 2 ? ">> Server is Semi-HardCore" : 
                sChallengeModes->serverHardMode == 3 ? ">> Server is IronMan" : ">> Server Hardcore is disabled");
            if (sChallengeModes->serverSelfCraftedEnable) LOG_INFO ("server.loading", ">> Self Crafting restriction enabled");
            if (sChallengeModes->serverItemQualityLevelEnable) LOG_INFO ("server.loading", ">> Item Quality restriction enabled");
            LOG_INFO ("server.loading", sChallengeModes->serverXPGain == 0 ? ">> XP rate is set by XPRate" : ">> XP rate is only gained through quests");

	    LOG_INFO ("server.loading", "\n\r");
        }
    }
};

class HardMode : public PlayerScript {
    private:
        uint32 HARDCORE_DEAD = 0;

    public:
        explicit HardMode() : PlayerScript("HardMode") {}

    void OnLogin(Player* player) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverHardMode == 1) ChatHandler(player->GetSession()).PSendSysMessage("Server wide Hard mode is Enabled, All deaths are permanent.");
            if (sChallengeModes->serverHardMode == 2) ChatHandler(player->GetSession()).PSendSysMessage("Server wide Semi-Hard mode is Enabled, Equiped equipment and all money is lost on death.");
            if (sChallengeModes->serverHardMode == 3) ChatHandler(player->GetSession()).PSendSysMessage("Server wide IronMan is Enabled, Iron Man rules apply.");
            
            if (sChallengeModes->serverReset) ChatHandler(player->GetSession()).PSendSysMessage("Server Reset is Enabled, All characters that log in will have the perminant death removed");

            if (sChallengeModes->serverSelfCraftedEnable) ChatHandler(player->GetSession()).PSendSysMessage("Server wide Self Crafted restriction is enabled.");
            if (sChallengeModes->serverItemQualityLevelEnable) ChatHandler(player->GetSession()).PSendSysMessage("Server wide Item Quality Level is Enabled.");
            if (sChallengeModes->serverXPGain == 0) ChatHandler(player->GetSession()).PSendSysMessage("Server wide XP Gain is set by a rate.");
            if (sChallengeModes->serverXPGain == 1) ChatHandler(player->GetSession()).PSendSysMessage("Server wide XP Gain is set for quests only.");
            
            if ((sChallengeModes->serverHardMode == 1 || sChallengeModes->serverHardMode == 3) && player->GetPlayerSetting("mod-hard-modes", HARDCORE_DEAD).value == 1 && sChallengeModes->serverReset) {
                player->UpdatePlayerSetting("mod-hard-modes", HARDCORE_DEAD, 0);
                player->GetSession()->KickPlayer("Hardcore character died");
            }
        }
    }
     
    void OnPlayerReleasedGhost(Player* player) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverReset) return;
            if (sChallengeModes->serverHardMode == 1 || sChallengeModes->serverHardMode == 3) {
                player->UpdatePlayerSetting("mod-hard-modes", HARDCORE_DEAD, 1);
                player->GetSession()->KickPlayer("Hardcore character died");
            }
        }
    }
    
    void OnPlayerResurrect(Player* player, float /*restore_percent*/, bool /*applySickness*/) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverReset) return;
            if (sChallengeModes->serverHardMode == 1 || sChallengeModes->serverHardMode == 3) {
                player->UpdatePlayerSetting("mod-challenge-modes", HARDCORE_DEAD, 1);
                player->KillPlayer();
                player->GetSession()->KickPlayer("Hardcore character died");
            }
        }
    }

    void OnPVPKill(Player* /*killer*/, Player* killed) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverReset) return;
            if (sChallengeModes->serverHardMode == 1 || sChallengeModes->serverHardMode == 3) {
                killed->UpdatePlayerSetting("mod-hard-modes", HARDCORE_DEAD, 1);
            }
        }
    }
   
    void OnPlayerKilledByCreature(Creature* /*killer*/, Player* killed) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverReset) return;
            if (sChallengeModes->serverHardMode == 1 || sChallengeModes->serverHardMode == 3) { // Hard Mode
                killed->UpdatePlayerSetting("mod-hard-modes", HARDCORE_DEAD, 1);
            } else if (sChallengeModes->serverHardMode == 2) { // Semi Hard Mode
		for (uint8 i = 0; i < EQUIPMENT_SLOT_END; ++i) {
		    if (Item* pItem = killed->GetItemByPos(INVENTORY_SLOT_BAG_0, i)) {
		        if (pItem->GetTemplate() && !pItem->IsEquipped()) continue;
		        uint8 slot = pItem->GetSlot();
		        ChatHandler(killed->GetSession()).PSendSysMessage("|cffDA70D6You have lost your |cffffffff|Hitem:%d:0:0:0:0:0:0:0:0|h[%s]|h|r", pItem->GetEntry(), pItem->GetTemplate()->Name1.c_str());
		        killed->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
		    }
		}
		killed->SetMoney(0);
            }
        }
    }

    bool CanEquipItem(Player* player, uint8 /*slot*/, uint16& /*dest*/, Item* pItem, bool /*swap*/, bool /*not_loading*/) override {
        bool useable = true;
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverItemQualityLevelEnable || sChallengeModes->serverHardMode == 3) useable &= pItem->GetTemplate()->Quality <= ITEM_QUALITY_NORMAL;
            if (sChallengeModes->serverSelfCraftedEnable) {
                if (!pItem->GetTemplate()->HasSignature()) return false;
                useable &= pItem->GetGuidValue(ITEM_FIELD_CREATOR) == player->GetGUID();
            }
        }
        return useable;
    }
   
    void OnLevelChanged(Player* player, uint8 /*oldlevel*/) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverHardMode == 3) player->SetFreeTalentPoints(0);
        }
    }

    void OnTalentsReset(Player* player, bool /*noCost*/) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverHardMode == 3) player->SetFreeTalentPoints(0); // Remove all talent points
        }
    }

    void OnLearnSpell(Player* player, uint32 spellID) override {
        if (sChallengeModes->serverHardMode == 3) {
            switch (spellID) {
                case RUNEFORGING:
                case POISONS:
                case BEAST_TRAINING:
                    return;
                default:
                    break;
            }
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
            if (!spellInfo) return;
            bool shouldForget = false;
            for (uint8 i = 0; i < 3; i++)
                if (spellInfo->Effects[i].Effect == SPELL_EFFECT_TRADE_SKILL) shouldForget = true;
            if (shouldForget) player->removeSpell(spellID, SPEC_MASK_ALL, false);
        }
    }

    bool CanUseItem(Player* /*player*/, ItemTemplate const* proto, InventoryResult& /*result*/) override {
        if (sChallengeModes->serverHardMode == 3) {
            // Do not allow using elixir, potion, or flask
            if (proto->Class == ITEM_CLASS_CONSUMABLE && (proto->SubClass == ITEM_SUBCLASS_POTION || proto->SubClass == ITEM_SUBCLASS_ELIXIR || proto->SubClass == ITEM_SUBCLASS_FLASK)) return false;
            // Do not allow food that gives food buffs
            if (proto->Class == ITEM_CLASS_CONSUMABLE && proto->SubClass == ITEM_SUBCLASS_FOOD) {
                for (const auto & Spell : proto->Spells) {
                    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(Spell.SpellId);
                    if (!spellInfo) continue;

                    for (uint8 i = 0; i < 3; i++)
                        if (spellInfo->Effects[i].ApplyAuraName == SPELL_AURA_PERIODIC_TRIGGER_SPELL) return false;
                }
            }
        }
        return true;
    }

    bool CanApplyEnchantment(Player* /*player*/, Item* /*item*/, EnchantmentSlot /*slot*/, bool /*apply*/, bool /*apply_dur*/, bool /*ignore_condition*/) override {
        return sChallengeModes->serverHardMode != 3;
    }

    bool CanGroupInvite(Player* /*player*/, std::string& /*membername*/) override {
        return sChallengeModes->serverHardMode != 3;
    }

    bool CanGroupAccept(Player* /*player*/, Group* /*group*/) override {
        return sChallengeModes->serverHardMode != 3;
    }

    void OnGiveXP(Player* player, uint32& amount, Unit* victim, uint8 xpSource) override {
        if (sChallengeModes->enabled()) {
            if (sChallengeModes->serverXPGain == 0) amount *= sChallengeModes->serverXPRate;
            else if (sChallengeModes->serverXPGain == 1 && victim) {
	        Pet* pet = player->GetPet();
	        if (pet && xpSource == XPSOURCE_KILL) pet->GivePetXP(player->GetGroup() ? amount / 2 : amount);
                amount = 0;
            }
        }
    }
};

// Add all scripts in one
void AddSC_mod_challenge_modes()
{
    new ChallengeModes_WorldScript();
    new HardMode();
}
