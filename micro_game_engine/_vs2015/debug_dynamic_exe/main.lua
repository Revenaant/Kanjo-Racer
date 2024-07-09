io.stdout:setvbuf('no') -- enable flushed printing, slower, but usefull 'no', 'line', 'full'

if debugging then
  print("Lua in Debug mode, is slower")
  require("mobdebug").start() 
end

print( 'Lua starting' )
local HUD =
{
    Play = "MAIN_MENU/MAINMENU_START.png",
    Options = "MAIN_MENU/MAINMENU_OPTIONS.png",
    Credits = "MAIN_MENU/MAINMENU_CREDITS.png",
    Exit = "MAIN_MENU/MAINMENU_EXIT.png",
    
    InsideOptions = "OPTIONS/OPTIONS.png",
    InsideCredits = "CREDITS/CREDITS.png",
    
    LevelSelectMountain = "STAGE_SELECT/STAGE_SELECT_MOUNTAIN_EXPRESSWAY.png",
    LevelSelectBeach = "STAGE_SELECT/STAGE_SELECT_ROUTE58.png",
    LevelSelectKanjo = "STAGE_SELECT/STAGE_SELECT_KANJO.png",
    LevelSelectForest = "STAGE_SELECT/STAGE_SELECT_HIGASHI_FOREST.png",
    
    CarSelect = "CAR_SELECT/CAR_SELECT.png",
    
    Story = "INTRO.png",
    
    LoadMountain = "LOADING_SCREEN/LOADING_MOUNTAIN_EXPRESSWAY.png",
    LoadBeach = "LOADING_SCREEN/LOADING_ROUTE58.png",
    LoadKanjo = "LOADING_SCREEN/LOADING_KANJOLOOP.png",
    LoadForest = "LOADING_SCREEN/LOADING_HIGASHI_FOREST.png",

    Game = "IN_GAME/INGAME_HUD.png"
}

local ResolutionScreen = 
{
    Player1NoFlame = "RESOLUTION_SCREEN/P1_1st_rating",
    Player1Ok      = "RESOLUTION_SCREEN/P1_1st_rating",
    Player1Nice    = "RESOLUTION_SCREEN/P1_1st_rating",
    Player1wow     = "RESOLUTION_SCREEN/P1_1st_rating",
    Player1Lit     = "RESOLUTION_SCREEN/P1_1st_rating",
    Player1LitAF   = "RESOLUTION_SCREEN/P1_1st_rating",
    
    Player2NoFlame = "RESOLUTION_SCREEN/P2_1st_rating",
    Player2Ok      = "RESOLUTION_SCREEN/P2_1st_rating",
    Player2Nice    = "RESOLUTION_SCREEN/P2_1st_rating",
    Player2wow     = "RESOLUTION_SCREEN/P2_1st_rating",
    Player2Lit     = "RESOLUTION_SCREEN/P2_1st_rating",
    Player2LitAF   = "RESOLUTION_SCREEN/P2_1st_rating"
}
--print(Menu.Play)
--MainMenu.drawHUD(Menu.Play)

local starting = true
local inGame = false
local timer = 0
local timerCD = 0.1
local state = "Play"        --I realize how gay this is, but honestly i cant be bothered doing a c++ function for this atm
local tileSet = ""
local resolutionScreen = false
local litness = 0
local stopRandomShit = false

function update()
    if starting then MainMenu.drawHUD(HUD.Play) starting = false end
    timer = timer + MainMenu.menuPickCDTimer()
    if inGame == false then
    --MENU MOVE UP
    if MainMenu.menuUp() == true and timer > timerCD and state == "Play" then MainMenu.drawHUD(HUD.Exit) timer = 0 state = "Exit"
elseif MainMenu.menuUp() == true and timer > timerCD and state == "Exit" then MainMenu.drawHUD(HUD.Credits) timer = 0 state = "Credits"
elseif MainMenu.menuUp() == true and timer > timerCD and state == "Credits" then MainMenu.drawHUD(HUD.Options) timer = 0 state = "Options"
elseif MainMenu.menuUp() == true and timer > timerCD and state == "Options" then MainMenu.drawHUD(HUD.Play) timer = 0 state = "Play"
else  end

    --MENU MOVE DOWN
    if MainMenu.menuDown() == true and timer > timerCD and state == "Play" then MainMenu.drawHUD(HUD.Options) timer = 0 state = "Options"
elseif MainMenu.menuDown() == true and timer > timerCD and state == "Options" then MainMenu.drawHUD(HUD.Credits) timer = 0 state = "Credits"
elseif MainMenu.menuDown() == true and timer > timerCD and state == "Credits" then MainMenu.drawHUD(HUD.Exit) timer = 0 state = "Exit"
elseif MainMenu.menuDown() == true and timer > timerCD and state == "Exit" then MainMenu.drawHUD(HUD.Play) timer = 0 state = "Play"
else  end

    --MENU ACCEPT
    if MainMenu.menuAccept() == true and timer > timerCD and state == "Play" then MainMenu.drawHUD(HUD.LevelSelectBeach) timer = 0 state = "LevelSelectBeach"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Options" then MainMenu.drawHUD(HUD.InsideOptions) timer = 0 state = "InsideOptions"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Credits" then MainMenu.drawHUD(HUD.InsideCredits) timer = 0 state = "InsideCredits"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Exit" then MainMenu.drawHUD(HUD.Play) timer = 0

    --TILESET SELECT
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "LevelSelectBeach" then MainMenu.drawHUD(HUD.CarSelect) timer = 0 state = "CarSelect" MainMenu.loadBeach() tileSet = "beach"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "LevelSelectMountain" then MainMenu.drawHUD(HUD.CarSelect) timer = 0 state = "CarSelect" MainMenu.loadMountain() tileSet = "mountain"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "LevelSelectKanjo" then MainMenu.drawHUD(HUD.CarSelect) timer = 0 state = "CarSelect" MainMenu.loadKanjoEnvironment() tileSet = "kanjo"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "LevelSelectForest" then MainMenu.drawHUD(HUD.CarSelect) timer = 0 state = "CarSelect" MainMenu.loadForest() tileSet = "forest"

elseif MainMenu.menuAccept() == true and timer > timerCD and state == "CarSelect" then MainMenu.drawHUD(HUD.Story) timer = 0 state = "Story"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "CarSelect" then MainMenu.drawHUD(HUD.Story) timer = 0 state = "Story"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "CarSelect" then MainMenu.drawHUD(HUD.Story) timer = 0 state = "Story"
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "CarSelect" then MainMenu.drawHUD(HUD.Story) timer = 0 state = "Story"

elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Story" and tileSet == "beach" then MainMenu.drawHUD(HUD.LoadBeach) MainMenu.startGame(true) inGame = true
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Story" and tileSet == "mountain" then MainMenu.drawHUD(HUD.LoadMountain) MainMenu.startGame(true) inGame = true
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Story" and tileSet == "kanjo" then MainMenu.drawHUD(HUD.LoadKanjo) MainMenu.startGame(true) inGame = true
elseif MainMenu.menuAccept() == true and timer > timerCD and state == "Story" and tileSet == "forest" then MainMenu.drawHUD(HUD.LoadForest) MainMenu.startGame(true) inGame = true

else  end
    
    --MENU BACK
    if MainMenu.menuBack() == true and timer > timerCD and (state == "LevelSelectBeach" or state == "LevelSelectCity" or state == "LevelSelectKanjo")
    then MainMenu.drawHUD(HUD.Play) timer = 0 state = "Play"
elseif MainMenu.menuBack() == true and timer > timerCD and state == "InsideOptions" then MainMenu.drawHUD(HUD.Options) timer = 0 state = "Options"
elseif MainMenu.menuBack() == true and timer > timerCD and state == "InsideCredits" then MainMenu.drawHUD(HUD.Credits) timer = 0 state = "Credits"
elseif MainMenu.menuBack() == true and timer > timerCD and state == "CarSelect" then MainMenu.drawHUD(HUD.LevelSelectBeach) timer = 0 state = "LevelSelectBeach"
else  end

    --MENU LEFT
    if MainMenu.menuLeft() == true and timer > timerCD and state == "LevelSelectBeach" then MainMenu.drawHUD(HUD.LevelSelectForest) timer = 0 state = "LevelSelectForest"
elseif MainMenu.menuLeft() == true and timer > timerCD and state == "LevelSelectKanjo" then MainMenu.drawHUD(HUD.LevelSelectMountain) timer = 0 state = "LevelSelectMountain"
elseif MainMenu.menuLeft() == true and timer > timerCD and state == "LevelSelectMountain" then MainMenu.drawHUD(HUD.LevelSelectBeach) timer = 0 state = "LevelSelectBeach"
elseif MainMenu.menuLeft() == true and timer > timerCD and state == "LevelSelectForest" then MainMenu.drawHUD(HUD.LevelSelectKanjo) timer = 0 state = "LevelSelectKanjo"
else  end

    --MENU RIGHT
    if MainMenu.menuRight() == true and timer > timerCD and state == "LevelSelectBeach" then MainMenu.drawHUD(HUD.LevelSelectMountain) timer = 0 state = "LevelSelectMountain"
elseif MainMenu.menuRight() == true and timer > timerCD and state == "LevelSelectMountain" then MainMenu.drawHUD(HUD.LevelSelectKanjo) timer = 0 state = "LevelSelectKanjo"
elseif MainMenu.menuRight() == true and timer > timerCD and state == "LevelSelectKanjo" then MainMenu.drawHUD(HUD.LevelSelectForest) timer = 0 state = "LevelSelectForest"
elseif MainMenu.menuRight() == true and timer > timerCD and state == "LevelSelectForest" then MainMenu.drawHUD(HUD.LevelSelectBeach) timer = 0 state = "LevelSelectBeach"
else end
end

resolutionScreen = MainMenu.getResolutionScreen()
if resolutionScreen == true and stopRandomShit == false then 
    litness = MainMenu.randomNumber()
    stopRandomShit = true
end

if stopRandomShit == true and Game.winnerN() == 1 then MainMenu.drawResolutionScreen(ResolutionScreen.Player1NoFlame, litness)
elseif stopRandomShit == true and Game.winnerN() == 2 then MainMenu.drawResolutionScreen(ResolutionScreen.Player2NoFlame, litness) end

end