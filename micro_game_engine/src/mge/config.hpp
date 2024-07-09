namespace config {
    #include <string>

    const std::string MGE_MODEL_PATH ("Data/models/");
    const std::string MGE_TEXTURE_PATH ("Data/textures/");
    const std::string MGE_SHADER_PATH ("Data/shaders/");
    const std::string MGE_FONT_PATH ("Data/fonts/");
	const std::string MGE_SOUND_PATH("Data/sounds/");
	const std::string MGE_MENU_HUD("Data/UI_MAIN_MENU_ASSETS/");

//--------------------------------------------------------------------------------------------------
	// Strings for designers
	// NO, you cannot add more for now
	// Rotation is only on the Y axis
	// Order could matter

	// CAMERA CONTROLS: [Orbit: LMB + Drag] [Zoom: Scroll wheel]

	const std::string MESH_ONE("Car Reference.obj");	// Name of file
	const std::string TEXTURE_ONE("02Player1.jpg");		// Name of file
	const float POSITION_ONE[] = { 0, 1, 0 };			// Position in x, y, z
	const float SCALE_ONE[] = { 1, 1, 1 };				// Scale in x, y, z
	const float ROTATION_ONE = 0;						// Rotation in degrees
	const bool ADD_ONE = true;							// Should the object be added to the scene?

	const std::string MESH_TWO("Highway_normalroad.obj");
	const std::string TEXTURE_TWO("Gradient test.png");
	const float POSITION_TWO[] = { 0, 0, 49 };
	const float SCALE_TWO[] = { 1, 1, 1 };
	const float ROTATION_TWO = 0;
	const bool ADD_TWO = true;

	const std::string MESH_THREE("Car Reference.obj");
	const std::string TEXTURE_THREE("02Player2.jpg");
	const float POSITION_THREE[] = { 3, 1, 0 };
	const float SCALE_THREE[] = { 1, 1, 1 };
	const float ROTATION_THREE = 0;
	const bool ADD_THREE = true;

	const std::string FONT("coolvetica_rg.ttf");
}
