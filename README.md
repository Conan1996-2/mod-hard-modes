# AzerothCore Server Wide Hard Modes
Hard Modes Module for AzerothCore

This module adds the following hard modes:

- **Hardcore** - Players who die are permanently ghosts and can never be revived^.
- **Semi-Hardcore** - Players who die lose all worn equipment and carried gold.
- **Iron Man Mode** - Enforces the [Iron Man Ruleset](https://wowchallenges.com/challangeinfo/iron-man/)^

^ - Players can be revived if they have whats called currency/token. Tokens can be given by a GM or via web interface and added to the database table.
    Number of deaths can also be configured in the .conf file (default 0) before use of the currency/token is needed to res.

This module adds the following attributes when enabled:
 
- **Self Crafted** - Players can only wear equipment that they have crafted.
- **Item Quality Level** - Players can only wear equipment that is of Normal or Poor quality.
- **XP Gain** - Choose if XP will be either a **XP Rate** or **Quest XP Only**.
- **XP Rate** - Players receive a percentage of the normal amount of XP, with 1 being 100%.
- **Quest XP Only** - Players can receive XP only from quests. 

Console Commands that can be used (At any time you can type the command to see whats available):
* note that some commands are only available to GM or higher level accounts.
* When dead, use the /guild channel to use the commands (works even when you're not in a guild). This gets around the "can't chat when dead" issue.
  
- **.hm or .hardmode** - This will access the following mods commands.
   - **status** - get a printout of you or [GM Only] targeted players status (player currency, total account currency, deaths, reprieves).
   - **currency** -
      - **add #** - [GM only] currently adds # currency to the targeted player. # can be a negative (-) to remove currency from the account.
      - **account** -
         - **balance** - Displays your account total (all players on this account) or [GM only] targeted players total account balance.
      - **balance** - Displays yours or [GM only] targeted players balance.
   - **deaths** - 
      - **balance** - Displays yours or [GM only] targeted players balance.
      - **clear** - [GM Only] Erases targeted players death count for free and will resurect them if dead.
      - **resurrect** - Will resurrect you or [GM Only] targeted player at the cost of currency.
   - **reprieves** -
      - **balance** - Displays yours or [GM only] targeted players balance.
  

Players do not have the ability to enable or disable the hard modes or attributes.

Only one hard mode can be activated on a server, but multiple attributes can be activated.

There are no rewards for using a hard mode, its a hard mode for a reason. Take comfort in knowing you achieved something great.

* Credit, This idea was taken from mod-challenge-modes @ https://github.com/ZhengPeiRu21/mod-challenge-modes
