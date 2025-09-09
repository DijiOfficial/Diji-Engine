# <ins>Pacman and the Diji-Engine</ins>

**Diji-Engine is a lightweight C++ engine built with SDL and GLM, with a Pacman demo that showcases its capabilities, implemented following the C++ Core Guidelines.**

# <ins>Overview</ins>

I based it of the [Unity Pipeline]() simplifying it for an easier use of handling graphics, inputs and basic game-loop logic, allowing me to focus on building gameplay systems while allowing easy expansions on needed engine systems. Applying modern practices, it serves both as a learning tool and as a basis for future custom game engine. One such Expansion is my [custom SFML engine]() which I greatly encourage you to check out as it is simply better in every way possible.

The Pacman demo included in this repo serves as proof of the engine's capabilites. A complete, playable example that ties the engine’s core systems together.

# <ins>Contents</ins>

- [The Engine](#the-engine)
    - [Features](#features--architecture)
    	- [Core Engine Systems](#core-engine-systems)
     	- [Component System](#component-system)
      	- [Graphics & Rendering](#graphics--rendering)
      	- [Audio System](#audio-system)
      	- [Input & Controls](#input--controls)
      	- [Advanced Features](#advanced-features)
      	- [Development Tools](#development-tools)
    - [Game Programming Patterns](#game-programming-patterns)
    	- [Command Pattern](#command-pattern) 
    	- [Observer Pattern](#observer-pattern) 
    	- [Singleton Pattern](#singleton-pattern) 
    	- [Game Loop](#game-loop) 
    	- [Update Method](#update-method) 
    	- [Double Buffer](#double-buffer) 
    	- [Component Pattern](#component-pattern) 
    	- [Dirty Flag](#dirty-flag) 
    	- [Event Queue Pattern](#event-queue-pattern) 
    	- [Service Locator Pattern](#service-locator-pattern) 
    	- [GameObject Parent-Child Relationship](#gameobjects-parent-child-relationships) 
    - [Pipeline](#pipeline)
    - [Improvements & todos](#improvements--todos)
- [Pacman](#pacman)
    - [Intro](#intro)
    - [Overview](#overview)
    - [Controls](#controls)
    - [Single Player](#single-player)
    	- [AI](#ai) 
    - [CO-OP](#co-op)
    - [VERSUS](#versus)
    	- [FREEMODE](#freemode)
    	- [GHOST RULES](#ghost-rules)
     - [Create Level](#create-level)
     - [Play The Game](#play-the-game)
  - [Conclusion](#conclusion)
    - [References](#references) 

# <ins>The Engine</ins>

## <ins>Features & Architecture</ins>

### <ins>Core Engine Systems</ins>
- **SDL2 Integration:** Setup with SDL2, SDL_image, SDL_ttf, and SDL_mixer for graphics, audio, and multimedia support.
- **Game Loop Architecture:** Comprised of an Initialization phase, Input processing, FixedUpdate for physics, Update, LateUpdate and Rendering pass.

```c++
	sceneManager.Init();

	while (true)
	{
		GUI::GetInstance().NewFrame();

		const auto currentTime{ std::chrono::high_resolution_clock::now() };
		const float deltaTime{ std::chrono::duration<float>(currentTime - lastFrameTime).count() };
		lastFrameTime = currentTime;
		lag += deltaTime;

		TimeSingleton::GetInstance().SetDeltaTime(deltaTime);
		if(not input.ProcessInput())
			break;

		while (lag >= FIXED_TIME_STEP)
		{
			sceneManager.FixedUpdate();
			lag -= FIXED_TIME_STEP;
		}

		sceneManager.Update();
		sceneManager.LateUpdate();
		renderer.Render();
	}
```

- **Scene Management:** General System for managing scenes. 
- **Resource Management:** Centralized ResourceManager for handling textures, fonts, sounds, and other game assets to keep memory usage optimal.

### <ins>Component System</ins>
- **GameObject Architecture:** GameObject system allowing modular game object composition with Components. Focusing the game towards Composition over Inheritence.
- **Built-in Components:** Default components including Transform, Render, Text, Texture2D, Collider, Controller, and more.
- **Component Lifecycle:** Managed by their Owning GameObjects themselves managed by the scene.

### <ins>Graphics & Rendering</ins>
- **2D Rendering Pipeline:** OpenGL-accelerated SDL2 renderer with texture rendering, sprite animation, and rotation support.
- **Texture Management:** Advanced texture rendering with support for sprite sheets, scaling, rotation, and animation frames.
- **ImGui Integration:** Built-in immediate mode GUI system for debug interfaces and development tools.

### <ins>Audio System</ins>
- **Multi-format Audio:** Support for music and sound effects through SDL_mixer integration.
- **Audio Management:** Separate Music and SoundEffect classes with mute/volume control capabilities.

### <ins>Input & Controls</ins>
- **Input Management:** Centralized InputManager with support for keyboard and controller input.
- **Command Pattern:** Built-in command system for handling player actions and game commands.
- **Controller Support:** Game controller integration for multiple input devices.

### <ins>Advanced Features</ins>
- **Transform Hierarchy:** Parent-child GameObject relationships with world/local position management and dirty flag optimization.
- **Collision System:** Basic collision detection with Collider components for game physics.
- **Observer Pattern:** Event system with Subject/Observer implementation.
- ~~**Steam Integration:** Steam Achievements system for Steam platform integration.~~ Since removed as it causes issues with controller inputs.

### <ins>Development Tools</ins>
- **SVG Parser:** Custom SVG parsing capabilities for vector graphics importation.
- **FPS Counter:** Built-in frame rate monitoring and display.
- **Visual Leak Detector:** Memory leak detection integration for development builds. (may incorrectly report leaks with static objects)

This overall Architecture provides a solid foundation for a 2D game engine. Which can easily be expanded upon as shown in my [SFML Engine]().

## <ins>Game Programming Patterns</ins>
Drawing inspiration from [Robert Nystrom's "Game Programming Patterns"](), I implemented several key design patterns that provide structure and flexibility for game development.

### <ins>Command Pattern</ins>
Created a  base Command class providing a pure virtual Execute() method, allowing me to encapsulate input requests as objects. This allows me to map inputs to actions easily like in Unreal Engine.

### <ins>Observer Pattern</ins>
I implemented a default system using IObserver and Subject classes. Allowing me to decouple object further by using messages as events, all components are both observers and subjects, allowing each components to listen for events/messages and execute code accordingly. 

My current Implementation has one big weakness and that is it doesn't allow events to pass information with it. This could be fixed with templates, however in my newer [SFML Engine]() I completely changed this system to mimick the [Unreal Engine's event system]() using templates, making it much easier and cleaner to use.

### <ins>Singleton Pattern</ins>
I made a template-based Singleton implementation used throughout the codebase. While I'm aware of the pattern's controversial nature, it's use can far outweight its downsides if used correctly as presented in the engine.

### <ins>Game Loop</ins>
The main loop handles timing, input processing, and separates game logic updates from rendering with fixed updates as well, ensuring consistent gameplay across different hardware.

### <ins>Update Method</ins>
I used a simple update method pattern with multiple phases:

`Init() - Component initialization`, While initialy not present within the game loop, it was quickly added as it would GREATLY simplify initialization for gameobejcts and their compoentns.

`Update() - Regular frame updates` In my case using a TimeSingleton to hold DeltaTime instead of passing it dowm. A design choice I'm still debating to this day!

`FixedUpdate() - Fixed timestep physics/logic updates`

`LateUpdate() - Post-update update to ensure that dependant components have the correct updated data for their own updates`

### <ins>Double Buffer</ins>
Used in rendering with SDL2's built-in double buffering system. having no other uses it is currently it's only usage.

### <ins>Component Pattern</ins>
Composition over Inheritance! Featuring a complete Entity-Component system where GameObjects are composed of various components. This promotes modularity and reusability, allowing for flexible game object creation without deep inheritance hierarchies.
In my version GameObjects come bare without any components. Altough this was changed midway through development the Transform was moved outside the GameObject into it's own component. It has remained the same in my [SFML Engine](). I marked it asa future improvement that every gameobject upon creation will come with a transform component. As 99% of uses need one.

### <ins>Dirty Flag</ins>
Used in different context, as well as the Transform component and GameObject hierarchy system. used to optimize expensive operations like world position calculations, only recalculating when necessary.

### <ins>State Pattern</ins>
Not found in the engine as I found that simpler GameState did the trick much better, however it is extensively used in the Pac-Man example through the ghost AI system. simplifying the headache that is  the state transitions and behavior management of the ghost AI.

### <ins>Event Queue Pattern</ins>
The audio system implements an event queue through the SDLISoundSystem class. Sound requests are queued using `std::queue` and processed asynchronously on a separate thread.

### <ins>Service Locator Pattern</ins>
The audio system also implements the Service Locator pattern through the ServiceLocator class, providing global access to audio services while maintaining flexibility in implementation choice.

### <ins>GameObjects Parent-Child Relationships</ins>
GameObjects support parent-child relationships with proper world/local position management, implementing a scene graph pattern common in game engines.

These patterns allow me to provides a clean, maintainable code architecture.

## <ins>Pipeline</ins>

As Mentionned before I kept the pipeline quite simple, not needing complex collision, rendering and such.

(insert pipeline diagram)

Now that you've got acquainted with the engine, let's see its flaws and avenues for improvements. 

## <ins>Improvements & todos</ins>

This section is dedicated to potential improvements for the engine. However if you've gotten this far you probably saw the [SFML Engine]() mentionned a couple of times. That engine is an upgraded version building on top of this one's framework, with many improvements made to the engine itself as well as more modernized code. So if you haven't already I invite to go over the the [SFML Engine GitHub Page]() to find out about the improved version of the engine!

With all of that said, let's get into the example usage of this engine.

# <ins>Pacman</ins>

The Following sections will guide you through my implementation of the original Pacman arcade game that I made!

## <ins>Intro</ins>

I made Pacman alongside the Engine as both a showcase and a design reference for the engine. Developing it in parallel helped me shape key engine systems, and expose the potential flaws in the design.

It’s a project I’m particularly proud of, the gameplay is a faithful replica of the original as I spent countless hours researching the original Arcade game exploring old research papers and abandinned threads on unkown forums to find the information I needed!

I'm confident my game showcases my capabilities as a programmer so let me present it to you!

## <ins>Overview</ins>

I once again won't be showing much code as it would make for a very boring read. But feel free to have a read through the code or look at my [SFML Engine]() for a more up-to-date code style.
With that out of the way let's get to the intersting part. The game Itself!

I first tried to replicate the intro closely to the original. You can find a video of the arcade game [here]() for a reference. However the first "boot up" section seemed hard to replicate as it almost looks like it's clearing the memory and using that to display visuals on the screen. I ended up starting right after that on the classic start menu.

(insert gif)

Here's a fun fact while researching the menu for pacman I found that depending on the version or rather location of the game the names on display would vary.
After pressing start, is where you will find the bigges difference with the original! An actual Menu!

(insert menu images)

Here you will find multiple options:
- [**1 player**](#single-player)
- **2 players**
	- [**COOP**](#co-op)
	- [**VERSUS**](#versus)
 		- [**FREEMODE**](#freemode)
   		- [**GHOST RULES**](#ghost-rules) 
- [CREATE LEVEL]()
- OPTIONS
- HIGHSCORES
- EXIT

I had planned for an option settings allowing some graphics and audio settings, but ran out of time and decided to cut it from the game. It is still present if I want to revisit it.
The highscores though currently broken should take you to the end screen where all the highscores are displayed!

## <ins>Gameplay & Controls</ins>

| Keyboard          | Controller 				  | Action                       |  
|-------------------|-----------------------------|------------------------------|  
| **W, A, S, D**    | **D-PAD**/**LEFT JOYSTICK** | Move                         |  
| **ENTER**  		| **BUTTON DOWN**			  | Select / Next  		         |  

## <ins>Single Player</ins>

I won't go in too much details about the singleplayer, it's a close replica but not an exact one. 
Some of the details I didn't get around to are:
- varying speeds
	- In the original ghosts speed increase depending on the level, then decreases around level 17 and stabilizes onwards.
 	- In the original the player speed seems very inconsistent. It turns out that the player stops for 1frame every time he eats a pellet! (I may have added that I'm not sure)
- Level Transitions
	- When completing a level, the level flashes for a bit before reseting. My version loads the next level immideatly.
 
### <ins>AI</ins>

The AI, the whole reason for this Pacman section!
The AI is an exact replica of the original, I followed [this video by ...]() as a reference for the ghost AI. As for the code itself I used the state Pattern combined with some components and commands.

I won't go in too much details about the AI itself, rather I'll show the structure of it. Below you can find the "Brain" of the AI.
```
	                                                                      If Power                         
	                                    If In Chase Mode    ┌───────┐   Pellet Eaten                       
	                               ┌────────────────────────► Chase ◄──────────────────┐                   
	                               │                        └───▲───┘   Power Ran Out  │                   
	                               │                            │                      │                   
	                               │                            │                      │                   
	                               │                          If│Time            ┌─────▼──────┐            
	                               │                          To│Switch          │ Frightened ├──┐         
	                               │                            │                └─────▲──────┘  │         
	                               │                            │                      │         │         
	               Complex         │            If In           │         If Power     │         │         
	 ┌─────────┐  Algorithm  ┌─────┴──────┐  Scatter Mode  ┌────▼────┐  Pellet Eaten   │         │         
	 │ Waiting ├─────────────► Enter Maze ├────────────────► Scatter ◄─────────────────┘         │If       
	 └─────────┘             └─────▲──────┘                └─────────┘  Power Ran Out            │Colliding
	                               │                                                             │With     
	                      Personnal│Spawn                                                        │Player   
	                         Is Reached                                                          │         
	                               │                                  Update Paused For          │         
	                          ┌────┴────┐  If Spawn Reached  ┌───────┐  For 2 Seconds ┌───────┐  │         
	                          │ Respawn ◄────────────────────┤ Eaten ◄────────────────┤ Dying ◄──┘         
	                          └─────────┘                    └───────┘                └───────┘
```
The ghosts are then Handled by a GhostAI component, The GhostAI then manages the GhostState Component amongst other things. If a state switch condition is met it is then handled by the GhostAI as well.

Each ghost as a Unique GhostAI component for their unique Chase states
(insert image)
Each Ghost State is then Inherited from the GhostState Template using the state pattern.
(insert image)
Finally all of the states are handled by the GhostAI
(insert image)

Comprehesible, flexibeal and expandable!


## <ins>CO-OP</ins>

The Coop mode was a way to showcase how the engine handles multiple inputs as well as how composition made this an easy process since all the necessary building blocks where already present.

For Coop I decided to count both players as one, the biggest issue I wanted to avoid was having one player die and having to watch the other keep playing. 
I also wanted to avoid competition as well so both players work together to complete the game! If one dies both die, points and lives are shared!

(video demo)

## <ins>VERSUS</ins>

I made two versus mode, although similar it was so I could both show the capacities of the engine as well as some balance.

### <ins>FREEMODE</ins>

Freemode is the first option, allowing the second player to control the red ghost. At the time I didn't think of removing the other 3 ghosts for balance but simply put allowing the ghost to be controlled by a player made it already very unfair for player 1.

(video demo)
### <ins>GHOST RULES</ins>
And so I made Ghost Rules, where the second player still controls the red ghost but it must abide by ghost rules. forcing movement at all times as well as not allowing certain movements.

(video demo)


## <ins>Create Level</ins>

As I was making the game and the engine I quickly realised the potential it had, with composition being a puzzle you arrange the way you want it I thought I could make a level editor. With pieces and sections of the original map cut up to be rearranged into any shape you desire, plopping upgrades and pellets and whatever else you need how you want it. With file IO it would be easy to save and share those maps. Unfortunately it was very ambitious and never made it in. Likely I will never get around to making it but it may serve as an idea for future games. Perhaps I could even incorporate some [WFC]() into it, have random maps every time!


## <ins>Play The Game</ins>

You can [play a web version of the game here](https://dijiofficial.github.io/MyPortfolio/Projects/Pacman.html#status) or [download the C++ version here]().

# <ins>Conclusion</ins>

## <ins>References</ins>


9. About / Credits

Your name, background, motivation.

Reference your other projects for context or style (WaveFunctionCollapse, CursedResurrection, Astyanax).
