# <ins>**Pac-Man and the Diji-Engine**</ins>

**Diji-Engine is a lightweight C++ engine built with [SDL](https://github.com/libsdl-org/SDL) and [GLM](https://github.com/g-truc/glm), with a [Pac-Man demo](#pac-man) that showcases its capabilities, implemented following the C++ Core Guidelines.**

# <ins>**Overview**</ins>

I based it of the [**Unity Pipeline**](https://docs.unity3d.com/6000.2/Documentation/Manual/execution-order.html) simplifying it for my use case, allowing me to focus on building simple and efficient gameplay systems while allowing easy expansions on needed engine systems. Applying modern practices, it served both as a learning tool and as a basis for future custom game engines. One such Expansion is my [**custom SFML engine**](https://github.com/DijiOfficial/SFML-Engine) which I highly encourage you to check out as it is simply better in every way possible.

The [**Pac-Man demo**](#pac-man) included in this repo serves as proof of the engine's capabilites. A complete, [**playable example**](https://dijiofficial.github.io/MyPortfolio/Projects/Pacman.html#status) that ties the engine’s core systems together.

# <ins>**Contents**</ins>

- [**The Engine**](#the-engine)
    - [**Features**](#features--architecture)
    	- [**Core Engine Systems**](#core-engine-systems)
     	- [**Component System**](#component-system)
      	- [**Graphics & Rendering**](#graphics--rendering)
      	- [**Audio System**](#audio-system)
      	- [**Input & Controls**](#input--controls)
      	- [**Advanced Features**](#advanced-features)
      	- [**Development Tools**](#development-tools)
    - [**Game Programming Patterns**](#game-programming-patterns)
    	- [**Command Pattern**](#command-pattern) 
    	- [**Observer Pattern**](#observer-pattern) 
    	- [**Singleton Pattern**](#singleton-pattern) 
    	- [**Game Loop**](#game-loop) 
    	- [**Update Method**](#update-method) 
    	- [**Component Pattern**](#component-pattern) 
    	- [**Dirty Flag**](#dirty-flag) 
    	- [**Event Queue Pattern**](#event-queue-pattern) 
    	- [**Service Locator Pattern**](#service-locator-pattern) 
    	- [**GameObject Parent-Child Relationship**](#gameobjects-parent-child-relationships) 
    - [**Pipeline**](#pipeline)
    - [**Improvements & todos**](#improvements--todos)
- [**Pac-Man**](#pac-man)
    - [**Intro**](#intro)
    - [**Overview**](#overview-1)
    - [**Controls**](#controls)
    - [**Single Player**](#single-player)
    	- [**AI**](#ai) 
    - [**CO-OP**](#co-op)
    - [**VERSUS**](#versus)
    	- [**FREEMODE**](#freemode)
    	- [**GHOST RULES**](#ghost-rules)
     - [**Create Level**](#create-level)
     - [**Play The Game**](#play-the-game)
  - [**Conclusion**](#conclusion)
    - [**References**](#references) 

# <ins>The Engine</ins>

<details>
	 <summary>Click to expand engine details</summary>
	
## <ins>Features & Architecture</ins>

### <ins>Core Engine Systems</ins>
- **SDL2 Integration:** Setup with [**SDL2**](https://github.com/libsdl-org/SDL), **SDL_image**, **SDL_ttf**, and **SDL_mixer** for graphics, audio, and multimedia support.
- **Game Loop Architecture:** Composed of **Initialization**, **Input Processing**, **FixedUpdate** for physics, **Update**, **LateUpdate**, and **Rendering**.

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

- **Scene Management:** Global System for managing scenes. 
- **Resource Management:** Global **ResourceManager** for handling textures, fonts, sounds, and other game assets for **optimal memory usage**.

### <ins>Component System</ins>
- **GameObject Architecture:** Modular **GameObject** system using [**Components**](#component-pattern). Focusing the game towards **Composition over Inheritence**.
- **Built-in Components:** Includes **Transform, Render, Text, Texture2D, Collider, Controller**, and more.
- **Component Lifecycle:** Managed by their owning GameObjects, which are managed by the scene.

### <ins>Graphics & Rendering</ins>
- **2D Rendering Pipeline:** Simple [**SDL2**](https://github.com/libsdl-org/SDL) renderer with support for **texture rendering**, **sprite animation**, **rotation**, **various shapes** and more.
- **ImGui Integration:** Built-in **ImGui** system for debug interfaces and development tools.

### <ins>Audio System</ins>
- **Multi-format Audio:** Support for **music** and **sound effects** using **SDL_mixer**.
- **Audio Management:** Separate Music and SoundEffect classes with mute and volume control.

### <ins>Input & Controls</ins>
- **Input Management:** Global **InputManager** on separate thread with support for **keyboard** and **controller** input.
- **Command Pattern:** Built-in system to map player inputs to commands.

### <ins>Advanced Features</ins>
- **Transform Hierarchy:** **Parent-child GameObject relationships** with world/local position management and dirty flag optimization.
- **Collision System:** Basic **collision** detection with **Collider components** inspired by [**Unreal Engine Collision System**](https://dev.epicgames.com/documentation/en-us/unreal-engine/collision-in-unreal-engine---overview).
- **Observer Pattern:** Event system with Subject/Observer.
- ~~**Steam Integration:** Steam Achievements system wit Steam API.~~ Since removed due to conflicts with controller inputs.

### <ins>Development Tools</ins>
- **SVG Parser:** Custom **SVG** support to import vector graphics.
- **FPS Counter:** Built-in **frame rate monitoring**.

This architecture provides a **solid foundation** for a **2D game engine**. Which can easily be **expanded upon** as shown in my [**SFML Engine**](https://github.com/DijiOfficial/SFML-Engine).
</details>

## <ins>Game Programming Patterns</ins>
Inspired from [**Robert Nystrom's "Game Programming Patterns"**](https://gameprogrammingpatterns.com), I implemented several key design patterns that provide structure and flexibility.

<details>
	 <summary>Click to expand Game Programming Patterns Section</summary>

I have linked each pattern to the book’s reference at the end of the section.

### <ins>Command Pattern</ins>
Created a base Command class with a pure `virtual Execute()` method, allowing me to encapsulate input requests as objects. This allows me to map inputs to actions like [**Unreal Engine Input System**](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine).

[**Command Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/command.html)

### <ins>Observer Pattern</ins>
I implemented IObserver and Subject classes. Components can send and receive messages/events, though currently events can’t carry data.
This could be fixed with templates, however in my newer [**SFML Engine**](https://github.com/DijiOfficial/SFML-Engine) I completely changed this system inspired by the [**Unreal Engine's Event System**](https://dev.epicgames.com/documentation/en-us/unreal-engine/custom-events-in-unreal-engine) using templates, making it much cleaner and easier to use, decoupling it even further.

[**Observer Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/observer.html)

### <ins>Singleton Pattern</ins>
I made a template-based Singleton implementation used throughout the engine. While I'm aware of the pattern's controversial nature, its use can far outweight its downsides if used correctly as presented in the engine.

[**Singleton Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/singleton.html)

### <ins>Game Loop</ins>
Handles timing, input, logic updates, and rendering. Includes `FixedUpdate` and `LateUpdate` for consistent physics and dependencies.

[**Game Loop Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/game-loop.html)

### <ins>Update Method</ins>
I reused the pattern for different methods to simplify lifecycle management:
- `Init()` - While initialy not present within the game loop, it was quickly added as it would GREATLY simplify initialization for gameobejcts and their compoentns.
- `Update()` In my case using a TimeSingleton to hold DeltaTime instead of passing it dowm. A design choice I'm still debating to this day!
- `FixedUpdate()` Runs at a fixed interval for consistent physics.
- `LateUpdate()` - For post-update logic.

[**Update Method Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/update-method.html)

### <ins>Component Pattern</ins>
**Composition over Inheritance!** Featuring a complete Entity-Component system where GameObjects are composed by and handle various components. This makes for simple and flexible game object creation.

Transform was moved outside the GameObject into it's own component. It has remained the same in my [**SFML Engine**](https://github.com/DijiOfficial/SFML-Engine). Though I marked it as a future improvement that every GameObject will come with a Transform component.

[**Component Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/component.html)

### <ins>Dirty Flag</ins>
Mainly used for optimization in transform hierarchy to avoid unnecessary recalculations.

[**Dirty Flag Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/dirty-flag.html)

### <ins>State Pattern</ins>
Not found in the engine as it would not simplify already simple systems, however it is extensively used in the [**Pac-Man**](#pac-man) example through the [**Ghost AI System**](#ai).

[**State Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/state.html)

### <ins>Event Queue Pattern</ins>
Implemented in the SDLISoundSystem class. Sound requests are queued using `std::queue` and processed asynchronously.

[**Event Queue Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/event-queue.html)

### <ins>Service Locator Pattern</ins>
The audio system also implements the Service Locator Pattern, providing global access while maintaining flexibility in implementation choice.

[**Service Locator Pattern by Robert Nystrom**](https://gameprogrammingpatterns.com/event-queue.html)

### <ins>GameObjects Parent-Child Relationships</ins>
GameObjects support parent-child relationships with proper world/local position management, implementing a scene graph common in game engines.

These patterns allow me to provides a clean and maintainable code architecture.

</details>

## <ins>Pipeline</ins>

As mentioned before, I kept the pipeline quite simple not needing complex collision, rendering and such.

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/Pipeline.png)

In my pipeline, I check for scene changes after the `Update` method. This isn’t ideal, as it effectively skips a frame. I addressed this in my [SFML Engine](https://github.com/DijiOfficial/SFML-Engine), moving the check to the end of the game loop for a more robust and less error-prone system..

## <ins>Improvements & todos</ins>

If you’ve gotten this far, you’ve seen the [**SFML Engine**](https://github.com/DijiOfficial/SFML-Engine) mentioned a couple of times. That engine is an upgraded version building on top of this one's framework, with many improvements made to the engine itself as well as more modernized code. So if you haven't already I invite to go over the the [**SFML Engine GitHub Page**](https://github.com/DijiOfficial/SFML-Engine) to find out about the improved version of the engine!

With all of that said, let's get into the example usage of this engine.

# <ins>Pac-Man</ins>

The Following sections will guide you through my implementation of the original [**Pac-Man**](#pac-man) arcade game!

## <ins>Intro</ins>

I made [**Pac-Man**](#pac-man) alongside the [**Engine**](#the-engine) as both a showcase and a design reference for the engine. Developing it in parallel helped me shape key engine systems, and expose the potential flaws in the design.

It’s a project I’m particularly proud of. The gameplay is a faithful replica of the original as I spent countless hours researching [**the original Arcade Game**](https://en.wikipedia.org/wiki/Pac-Man) exploring old research papers and abandonned threads on obscure forums to find the information I needed!

I'm confident my game showcases my capabilities as a programmer so let me present it to you!

## <ins>Overview</ins>

Once again won't be showing much code as it would make for a very boring read. But feel free to have a read through the code or look at my [**SFML Engine**](https://github.com/DijiOfficial/SFML-Engine) for a more up-to-date code style.

I first tried to replicate the intro closely to the original. You can find a [**video of the arcade game**](https://youtu.be/dScq4P5gn4A) for reference. However the first "boot up" was hard to replicate so I started right after that on the classic start menu.

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/PacmanIntroGif.gif)

Here's a fun fact, while researching the menu for pacman I found that depending on the version or rather location of the game the names on display would vary.
After pressing start, is where you will find the biggest difference with the original! An actual Menu!

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/MenuGif.gif)

Here you will find multiple options:
- [**1 player**](#single-player)
- **2 players**
	- [**COOP**](#co-op)
	- [**VERSUS**](#versus)
 		- [**FREEMODE**](#freemode)
   		- [**GHOST RULES**](#ghost-rules) 
- [CREATE LEVEL](#create-level)
- OPTIONS
- HIGHSCORES
- EXIT

I had planned to include a settings menu with graphics and audio options, but I ran out of time and decided to cut it from the game. It’s still present in the codebase if I decide to revisit it.
The highscoresm though currently brokenm should take you to the end screen where all the highscores are displayed!

## <ins>Controls</ins>

| Keyboard          | Controller 				  | Action                       |  
|-------------------|-----------------------------|------------------------------|  
| **W, A, S, D**    | **D-PAD**/**LEFT JOYSTICK** | Move                         |  
| **ENTER**  		| **BUTTON DOWN**			  | Select / Next  		         |  

## <ins>Single Player</ins>

I won’t go into too much detail about the single-player mode. It’s a close replica, but not an exact one.

Some of the details I didn’t get around to:
- **Varying Speeds**
	- In the original ghosts speed increase depending on the level, then decreases around level 17 and stabilizes onwards.
 	- Pac-Man’s speed also appears inconsistent. It turns out he stops for 1 frame every time he eats a pellet!
- **Level Transitions**
	- In the original, the screen flashes after completing a level. My version loads the next level immediately.
 
### <ins>AI</ins>

The AI, the whole reason for this Pacman section!
I implemented an exact replica of the original AI, following [Retro Game Mechanics Explained by Retro Game Mechanics Explained](https://youtu.be/ataGotQ7ir8) as a reference for the [**Ghost AI**](#ai). I used the state pattern combined with some components and commands.

I won't go in too much details about the AI itself, rather I'll show the structure of it. Below you can find the "**Brain**" of the AI.
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
The ghosts are handled by a GhostAI component, which manages the GhostState component and others. If a state switch condition is met, it’s also handled by GhostAI through the state pattern.

Each ghost has a unique GhostAI component for their unique chase behaviours.

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/GhostAICode.png)

Each Ghost State is then inherited from the GhostState Template using the state pattern.

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/GhostAIStateCode.png)

Finally, all states are managed by the GhostAI.

![Picture of the Pipeline](https://github.com/DijiOfficial/Diji-Engine/blob/master/GitHubAssets/GhostAIStateSwitch.png)

Comprehensible, flexible, and expandable!

## <ins>CO-OP</ins>

The co-op mode showcases how the engine handles multiple inputs and how composition made this an easy process since all necessary building blocks were already present.

For co-op, I decided to count both players as one. The biggest issue I wanted to avoid was having one player die and being forced to watch the other continue.

I also wanted to avoid competition. Both players work together to complete the game. If one dies, both die. Points and lives are shared.

(video demo)

## <ins>VERSUS</ins>

I made two versus modes. Though similar, they help demonstrate the engine’s capabilities and allow some balance testing.

### <ins>FREEMODE</ins>

Freemode is the first option, allowing the second player to control the red ghost. At the time I didn't think of removing the other 3 ghosts for balance. So allowing the ghost to be controlled by a player made it very unfair for player 1.

(video demo)
### <ins>GHOST RULES</ins>
I then made Ghost Rules, where the second player still controls the red ghost but it must abide by ghost rules. Forcing movement at all times and restricted directions.

(video demo)

## <ins>Create Level</ins>

While developing the engine and the game, I quickly realized the potential for a level editor.

Since composition is like a puzzle you can arrange however you want, I imagined cutting up sections of the original map to rearrange into any shape—placing upgrades, pellets, or whatever else was needed. With file I/O, it would have been easy to save and share maps.

Unfortunately, it was too ambitious and never made it into the final version. I’ll likely never get around to it, but it may serve as inspiration for future games. Perhaps I could even incorporate some [**WFC**](https://github.com/DijiOfficial/WaveFunctionCollapse) to generate random maps each time!

## <ins>Play The Game</ins>

You can [**play a web version of the game here**](https://dijiofficial.github.io/MyPortfolio/Projects/Pacman.html#status) 

Or [**download the C++ executable here**](https://github.com/DijiOfficial/Diji-Engine/releases/tag/v1.0).

# <ins>Conclusion</ins>

This Pac-Man project was more than just a fun remak. It was a design challenge and a learning tool through the creation of an engine.

Recreating the original Pac-Man pushed me to stress-test the engine in a real setting. It forced me to avoid shortcuts and quick fixes, and instead build proper, flexible, and comprehensible systems that could handle real gameplay demands. Copying an existing, well-understood game also meant I couldn't hide behind vague design decisions, it either worked like the original or it didn’t.

Though some features didn’t make it in due to time, the result is something I’m proud of. It shows my attention to detail, ability to problem-solve, and my commitment to building complete and playable experiences.

If you’ve read this far, thank you. I hope this gave you a clear picture of what I’ve built, how.

## <ins>References</ins>

- [**Pac-Man (Arcade Game)**](https://en.wikipedia.org/wiki/Pac-Man)  
- [**Original Pac-Man Gameplay (Video)**](https://youtu.be/dScq4P5gn4A)  
- [**Retro Game Mechanics Explained – Pac-Man Ghost AI Explained**](https://youtu.be/ataGotQ7ir8)  
- [**Retro Game Mechanics Explained – How Frightened Ghosts Decide Where to Go**](https://youtu.be/eFP0_rkjwlY?list=PLHQ0utQyFw5IQyWhyR5Ec8_aQ3-BrLJZ9 )  
- [**Unity Execution Order (Reference for Engine Pipeline)**](https://docs.unity3d.com/6000.2/Documentation/Manual/execution-order.html)  
- [**Unreal Engine Doc**](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-6-documentation)
- [**Unity Doc**](https://docs.unity.com/en-us)

