/*
 * Copyright (C) 2023+ Firelands Core Project , released under AGPL v3 license: https://github.com/FirelandsProject/firelands-cata/blob/master/LICENSE
 */

#include "Language.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"
#include "AccountMgr.h"
#include "Chat.h"
#include "Config.h"
#include "LoginDatabase.h"
#include "Player.h"
#include "ScriptMgr.h"

enum RAFTexts
{
    HELLO_RAF = 40000,
    RAF_DISABLED,
    RAF_ALREADY_HAVE_RECRUITED,
    RAF_SUCCESS,
    RAF_RESET_SUCCESS,
    RAF_TARGET_ONESELF,
    RAF_NAMES,
    RECRUIT_VIEW_EMPTY
};

class RAFAnnouncer : public PlayerScript
{
  public:
    RAFAnnouncer() : PlayerScript("RAFAnnouncer") {}

    void OnLogin(Player* player, bool fistLogin) override
    {
        if (sConfigMgr->GetBoolDefault("RAF.ShowMessage", true))
        {
            ChatHandler(player->GetSession()).PSendSysMessage(HELLO_RAF);
        }
    }
};

static void GetTargetAccountIdByName(std::string& name, uint32& accountId)
{
    QueryResult result = CharacterDatabase.Query(Firelands::StringFormat("SELECT `account` FROM `characters` WHERE `name`='%s'", name).c_str());
    accountId = (*result)[0].GetInt32();
}

using namespace Firelands::ChatCommands;

void RegisterInfo(ChatHandler* handler, const char* commandType)
{
    uint32 myAccId = handler->GetSession()->GetAccountId();
    std::string accName;
    AccountMgr::GetName(myAccId, accName);
    std::string characterName = handler->GetSession()->GetPlayerName();
    std::string ipAccount = handler->GetSession()->GetRemoteAddress();
    // clang-format off
    QueryResult info = LoginDatabase.Query(
        Firelands::StringFormat("INSERT INTO `raf_info` (`accountId`, `accountName`, `characterName`, `ip`, `command`) VALUES (%u, '%s', '%s', '%s', '%s')",
         myAccId, accName.c_str(), characterName.c_str(), ipAccount.c_str(), commandType).c_str()
    );
    // clang-format on
}

class RAFCommandScript : public CommandScript
{
  public:
    RAFCommandScript() : CommandScript("RAFCommand") {}

    std::vector<ChatCommand> GetCommands() const override
    {
        // clang-format off
        static std::vector<ChatCommand> rafCommandTable =
        {
            { "info",    rbac::RBAC_PERM_SEC_PLAYER,       false,    &HandleRAFInfoCommand,     "" },
            { "reset",   rbac::RBAC_PERM_SEC_PLAYER,      false,    &HandleRAFResetCommand,    "" },
            { "add",     rbac::RBAC_PERM_SEC_PLAYER,        false,    &HandleRAFAddCommand,      "" },
        };
        static std::vector<ChatCommand> commandTable =
        {
            { "raf",     rbac::RBAC_PERM_SEC_PLAYER,    false,    nullptr,                   "", rafCommandTable },
        };
        // clang-format on
        return commandTable;
    }

    static bool HandleRAFInfoCommand(ChatHandler* handler)
    {
        uint32 myAccountId = handler->GetSession()->GetAccountId();

        RegisterInfo(handler, "info");

        QueryResult result = LoginDatabase.Query(Firelands::StringFormat("SELECT `recruiter` FROM `account` WHERE `id`=%u", myAccountId).c_str());

        if (result)
        {
            QueryResult resultCharacters = CharacterDatabase.Query(Firelands::StringFormat("SELECT `name` FROM `characters` WHERE `account`=%u", (*result)[0].GetUInt32()).c_str());
            if (resultCharacters)
            {
                do
                {
                    handler->PSendSysMessage(RAF_NAMES, (*resultCharacters)[0].GetString());
                } while (resultCharacters->NextRow());
            }
            else
                ChatHandler(handler->GetSession()).SendSysMessage(RECRUIT_VIEW_EMPTY);
        }

        return true;
    }

    static bool HandleRAFResetCommand(ChatHandler* handler)
    {
        uint32 myAccountId = handler->GetSession()->GetAccountId();

        RegisterInfo(handler, "reset");
        QueryResult result = LoginDatabase.Query(Firelands::StringFormat("UPDATE `account` SET `recruiter`=0 WHERE `id`=%u", myAccountId).c_str());
        ChatHandler(handler->GetSession()).SendSysMessage(RAF_RESET_SUCCESS);

        return true;
    }

    static bool HandleRAFAddCommand(ChatHandler* handler, std::string characterName)
    {
        if (characterName.empty())
            return false;

        Player* target = nullptr;

        std::string playerName;
        uint32 targetAccountId;

        if (!handler->extractPlayerTarget(characterName.data(), &target, nullptr, &playerName))
        {
            return false;
        }

        if (target)
        {
            targetAccountId = target->GetSession()->GetAccountId();
        }
        else
        {
            GetTargetAccountIdByName(playerName, targetAccountId);
        }

        if (targetAccountId == 0)
        {
            handler->PSendSysMessage(LANG_ACCOUNT_NOT_EXIST, characterName.c_str());
            handler->SetSentErrorMessage(true);
            return false;
        }

        uint32 myAccountId = handler->GetSession()->GetAccountId();

        RegisterInfo(handler, "add");

        QueryResult result = LoginDatabase.Query(Firelands::StringFormat("SELECT * FROM `account` WHERE `recruiter` <> 0 AND `id`=%u", myAccountId).c_str());

        if (result)
        {
            ChatHandler(handler->GetSession()).SendSysMessage(RAF_SUCCESS);
        }
        else if (targetAccountId != myAccountId)
        {
            result = LoginDatabase.Query(Firelands::StringFormat("UPDATE `account` SET `recruiter`=%u WHERE `id`=%u", targetAccountId, myAccountId).c_str());
            ChatHandler(handler->GetSession()).SendSysMessage(RAF_SUCCESS);
        }
        else
        {
            ChatHandler(handler->GetSession()).SendSysMessage(RAF_TARGET_ONESELF);
        }
        return true;
    }
};

// Add all scripts in one
void AddRecruitAFriendCommandScripts()
{
    if (sConfigMgr->GetBoolDefault("RAF.Enabled", true))
    {
        new RAFAnnouncer();
        new RAFCommandScript();
    }
}