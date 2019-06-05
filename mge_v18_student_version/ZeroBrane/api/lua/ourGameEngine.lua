local Game = { -- following the order of the docs
	type = "lib",
	description = "picoGameEngine 0.01 API",
	childs = {
		-- Game
		getDistance = {
			type = "function",
			description = "gets the traveled distance",
			args = "( player )",
			returns = "distance",
		},
		winnerN = {
			type = "function",
			description = "returns the winner of the game",
			args = "()",
			returns = "winner",
		},
	},
}
local MainMenu = { -- following the order of the docs
	type = "lib",
	description = "picoGameEngine 0.01 API",
	childs = {
		-- Game
		menuAccept = {
			type = "function",
			description = "accepts selected shit, A button",
			args = "()",
			returns = "Accept",
		},
		menuBack = {
			type = "function",
			description = "goes back in the menu, B button",
			args = "()",
			returns = "Back",
		},
		menuUp = {
			type = "function",
			description = "plz dont make me explain what this does, plz",
			args = "()",
			returns = "Up",
		},
		menuDown = {
			type = "function",
			description = "plz dont make me explain what this does, plz",
			args = "()",
			returns = "Down",
		},
		menuLeft = {
			type = "function",
			description = "plz dont make me explain what this does, plz",
			args = "()",
			returns = "Left",
		},
		menuRight = {
			type = "function",
			description = "plz dont make me explain what this does, plz",
			args = "()",
			returns = "Right",
		},
		changeCar = {
			type = "function",
			description = "changes between cars, i think this is going to use the x axis on the joystick",
			args = "( carMesh )",
			returns = "",
		},
		loadLevel = {
			type = "function",
			description = "loads the level",
			args = "( level )",
			returns = "",
		},
		setTileSet = {
			type = "function",
			description = "sets the tileset we use for generating the level",
			args = "( tileset )",
			returns = "",
		},
		switchMusic = {
			type = "function",
			description = "changes the music",
			args = "( music )",
			returns = "",
		},
		setSound = {
			type = "function",
			description = "sets sounds",
			args = "( sounds )",
			returns = "",
		},
		drawHUD = {
			type = "function",
			description = "draws sprites on the screen",
			args = "( image )",
			returns = "",
		},
		menuPickCDTimer = {
			type = "function",
			description = "really gay cooldown between switching images",
			args = "()",
			returns = "time",
		},
		exitGame = {
			type = "function",
			description = "exits the game",
			args = "()",
			returns = "",
		},
		startGame = {
			type = "function",
			description = "starts the game",
			args = "( start )",
			returns = "shouldStart",
		},
		loadNormal = {
			type = "function",
			description = "really gay cooldown between switching images",
			args = "()",
			returns = "time",
		},
		loadMountain = {
			type = "function",
			description = "exits the game, Mountain environment",
			args = "()",
			returns = "",
		},
		loadBeach = {
			type = "function",
			description = "starts the game Beach environment",
			args = "( start )",
			returns = "shouldStart",
		},
		loadForest = {
			type = "function",
			description = "starts the game Forest environment",
			args = "( start )",
			returns = "shouldStart",
		},
		loadVillage = {
			type = "function",
			description = "starts the game Village environment",
			args = "( start )",
			returns = "shouldStart",
		},
		loadCity = {
			type = "function",
			description = "starts the game City environment",
			args = "( start )",
			returns = "shouldStart",
		},
		loadKanjoEnvironment = {
			type = "function",
			description = "starts the game Kanjo environment",
			args = "()",
			returns = "",
		},
		loadCars = {
			type = "function",
			description = "starts the game",
			args = "( start )",
			returns = "shouldStart",
		},
		loadResolutionScreen = {
			type = "function",
			description = "resolves the game",
			args = "()",
			returns = "resolutionScreen",
		},
		randomNumber = {
			type = "function",
			description = "returns random number between 1 and 5",
			args = "()",
			returns = "litness",
		},
		getResolutionScreen = {
			type = "function",
			description = "returns the state of the resolutionScreen",
			args = "()",
			returns = "isIt",
		},
		drawResolutionScreen = {
			type = "function",
			description = "returns the state of the resolutionScreen",
			args = "( image, litness)",
			returns = "isIt",
		},
	},
}

return {
  Game = Game,
	MainMenu = MainMenu,
}
