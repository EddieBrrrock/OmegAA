# OmegA Changelog

## 3.2
- removing the ratmod delag code
- Keep jumping when holding jump with `pmove_autohop`
- new ammo boxes
- new sound when flag is taken
- scoreboard close immediatly when releasing the tab key
- new flags
- no more upscaled weapon for space saving
- servers are displayed with uppercase and lowercase
- more resolutions in the menu settings
- `g_allowDuplicateGuid`
- mute spec with `g_muteSpectators`
- when someone join a team it's printed in chat instead of displayed in the center
- bot can be muted with `g_botChat` instead of `bot_nochat` who was in the engine code

## 3.2.2
- `g_ambientSound`
- pass through invisible / clip walls (ty ratmod) with `clipWalls`
- show spawn during warmup (ty ratmod again) with `cg_showSpawns`
- better pul1ctf textures (ty ratmod again again)
- better scoreboard
- headshot bug fix
- small rocket trail and bullet impact changes imported from OA 0.6.0
- `g_startWhenReady` don't listen to bot anymore. It means if you are not ready and there are bots, it will not start the game.
- small ui changes (map select and map loading levelshot)
- `cg_brightPlayers` has now 2 types of skins which can be set with 1 or 2
- new skull icons (displayed in frag messages)

## 3.2.3
- show to spectators when the people they watch is zooming  with `cg_showSpecZoom`
- display in the scoreboard when someone is dead or typing
- small particle effect with the shotgun, disable with `cg_shotgunSparks`
- be able to choose if you want to play with only one weapon with `g_weaponArena`, it's displayed when you want to create a server instead of `g_rockets`
- new blood effect and fix `cg_leiEnhancement`
- when `g_botchat` = 0, bots don't even stop to type an empty message
- new numbers
- Server name, map name, gametype and lockteam when teams are locked are displayed in the scoreboard
- new shell shaders for quaddamage and battlesuit powerup (we can see the bright outline now)
- be able to search servers by mod, no more OmegA in the gametype filter
- add a Local+Internet filter for servers

## 3.2.4
- remove `clipWalls` as it have bugs
- `g_railgunSpeed` -> `g_railgunFireRate`
- add `rocketFireRate`
- `g_airControl` does the CPM air control instead of just control the movement in air
- some textures alpha channels fix (like some OmegA logos who have the OpenArena logo hidden in the alpha channel)
- add credits.txt

## 3.2.5
- higher quality menu buttons
- `g_headDamageMultiplicator` if you shoot in the head, damage will be multiplicated by the value you've set
- `cg_killSound` you can now disable the high pitch hitsound when you frag someone

## 3.2.6
- new blood effects (screen blood and hit blood)
- `cg_bobgun` activated by default
- fix "Headshot!" message position
- fix SinglePlayer stuck at tutorial
- when you're spawnprotected, you have a battlesuit shell
- `cg_brightPlayers` now deactivable

## 3.2.7
- no more orbital view when spectating just thirdperson
- clean a lot of warnings (during compiling)
- `cg_trueLightning` enabled by default

## 3.2.8
- new health models/effects
- shotgun laser animation
- new pm skins
- change the impact of effect color bars (first one is for railgun color, second one is for pm skins color)
- better `cg_wallhack`
- `r_ext_supersample` available in graphic settings as "supersampling"
- teleport missile sound

## 3.2.9
- `s_worldSound`, `cg_hitSound`, `cg_killSound` are in the sound settings menu now
- `cg_transparentGun`, `cg_drawGun` in the preferences menu
- `r_drawflat`, `r_mode -2` in the graphic settings menu
- `fwd_use` and `fwd_address` in the network settings menu
- `g_chaos` set every weapon fire-rate to maximum
- `g_railThroughwalls` railgun can kill through a wall
- dead cpm skins become grey
- update defaults
- fix flares with Quake3e/Omega-engine
- `cg_shadows` have now 4 different types
- cleanup (- 4 Mo)
- switch up to openarena with a button in the main menu
- remove statistics in the menu
- new machinegun skin
- sarge/neon neons colors change like cpm
- new health models
- `cg_switchCycle` if set to 0, weapon switch will not cycle

## 3.3.0
- better chat balloon
- bind zoom to mouse2 by default
- fix runes pickup sound
- `cg_forceTeamSkins` if set to 0, you can choose the character color you want in team gametypes
- drop flag keybind in the menu
- fix battlesuit
- fix runes effect with `cg_brightPlayers` enabled
- `g_spawnProtect` is 1 second by default now

## 3.3.1
- dmflags settings in the multiplayer menu
- `g_bluerespawtime` and `g_redrespawntime` for team specific respawn time
- new reward system imported from ratmod
- `g_elimination_items` if set to 1, enable items pickup in elimination modes

## 3.3.2
- ui updates
- desktop resolution setting fix
- save favorite server button
- menu music