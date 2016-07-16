SDLMain.c:Update_Input()

TouchChanged()
TouchIsDown()
TouchIsUp()
Touching()

PollEvents()

SDLMain.c:main()->
	SDLMain.c:ScreenWidth
	SDLMain.c:ScreenHeight
	SDLMain.c:ScreenScalar
	
	game.h:Game_Initialize()->
		Graphics.h:InitGraphics()
		Graphics.h:LoadBallGraphic()->
			BallData.c:BallData[]->
				PVRTexture.h
		game.c:InitBallPhysics()
		
	WindowsTime.h:SetFramesPerSecond()
	WindowsTime.h:GetTimeNow()
	WindowsTime.h:SubtractTime()
	WindowsTime.h:GetFrames()
	
	SDLMain.c:PollEvents()
	SDLMain.c:Update_Input()
	
	game.h:Game_Work()->
		game.c:DoInput()->
			SDLMain.c:TouchIsDown()
			SDLMain.c:TouchIsUp()
		game.c:StepBallPhysics()
		
	WindowsTime.h:AddFrame()
	
	game.h:Game_Draw()->
		Graphics.h:ClearBackground()
		game.c:DrawBalls()->
			Graphics.h:DrawBall()
		Graphics.h:DrawArrow()
		
	game.h:Game_Exit()->
		Graphics.h:FreeBallGraphic()
		
	




