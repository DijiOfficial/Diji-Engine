# Pacman and the Diji-Engine

**Diji-Engine is a lightweight C++ engine built with SDL and GLM, with a Pacman demo that showcases its capabilities, implemented following the C++ Core Guidelines.**

# Overview

I based it of the [Unity Pipeline]() simplifying it for an easier use of handling graphics, inputs and basic game-loop logic, allowing me to focus on building gameplay systems while allowing easy expansions on needed engine systems. Applying modern practices, it serves both as a teaching tool and as a basis for future custom game engine. One such Expansion is my [custom SFML engine]() which I greatly encourage you to check out as it is simply better in every way possible.

The Pacman demo included in this repo serves as proof of the engine's capabilites. A complete, playable example that ties the engine’s core systems together.

# Contents

- [The Engine](#the-tngine)
    - [Features](#features)
    - [Paterns]()
    - [Pipeline]()
    - [Getting Started]()
    - [Expansions Todo]
- [Pacman]()
    - [intro]()
    - Demo overview — what the Pacman project demonstrates about the engine
    - Gameplay & controls — player controls, win/lose conditions, UI
    - Level & asset structure — tilemap usage and asset layout used by the demo
    - AI & systems used — how ghost behavior, collisions, and state transitions are implemented
    - Running the demo — quick steps to build and run the Pacman example   

# The Engine

## Features && Architecture

### Core Engine Systems
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

### Component System
- **GameObject Architecture:** GameObject system allowing modular game object composition with Components. Focusing the game towards Composition over Inheritence.
- **Built-in Components:** Default components including Transform, Render, Text, Texture2D, Collider, Controller, and more.
- **Component Lifecycle:** Managed by their Owning GameObjects themselves managed by the scene.

### Graphics & Rendering
- **2D Rendering Pipeline:** OpenGL-accelerated SDL2 renderer with texture rendering, sprite animation, and rotation support.
- **Texture Management:** Advanced texture rendering with support for sprite sheets, scaling, rotation, and animation frames.
- **ImGui Integration:** Built-in immediate mode GUI system for debug interfaces and development tools.

### Audio System
- **Multi-format Audio:** Support for music and sound effects through SDL_mixer integration.
- **Audio Management:** Separate Music and SoundEffect classes with mute/volume control capabilities.

### Input & Control
- **Input Management:** Centralized InputManager with support for keyboard and controller input.
- **Command Pattern:** Built-in command system for handling player actions and game commands.
- **Controller Support:** Game controller integration for multiple input devices.

### Advanced Features
- **Transform Hierarchy:** Parent-child GameObject relationships with world/local position management and dirty flag optimization.
- **Collision System:** Basic collision detection with Collider components for game physics.
- **Observer Pattern:** Event system with Subject/Observer implementation.
- ~~**Steam Integration:** Steam Achievements system for Steam platform integration.~~ Since removed as it causes issues with controller inputs.

### Development Tools
- **SVG Parser:** Custom SVG parsing capabilities for vector graphics importation.
- **FPS Counter:** Built-in frame rate monitoring and display.
- **Visual Leak Detector:** Memory leak detection integration for development builds. (may incorrectly report leaks with static objects)

This overall Architecture provides a solid foundation for a 2D game engine. Which can easily be expanded upon as shown in my [SFML Engine]().

## Game Programming Patterns
Drawing inspiration from [Robert Nystrom's "Game Programming Patterns"](), I implemented several key design patterns that provide structure and flexibility for game development.

### Command Pattern
Created a  base Command class providing a pure virtual Execute() method, allowing me to encapsulate input requests as objects. This allows me to map inputs to actions easily like in Unreal Engine.

### Observer Pattern
I implemented a default system using IObserver and Subject classes. Allowing me to decouple object further by using messages as events, all components are both observers and subjects, allowing each components to listen for events/messages and execute code accordingly. 

My current Implementation has one big weakness and that is it doesn't allow events to pass information with it. This could be fixed with templates, however in my newer [SFML Engine]() I completely changed this system to mimick the [Unreal Engine's event system]() using templates, making it much easier and cleaner to use.

### Singleton Pattern
I made a template-based Singleton implementation used throughout the codebase. While I'm aware of the pattern's controversial nature, it's use can far outweight its downsides if used correctly as presented in the engine.

### Game Loop
The main loop handles timing, input processing, and separates game logic updates from rendering with fixed updates as well, ensuring consistent gameplay across different hardware.

#### Update Method
I used a simple update method pattern with multiple phases:

`Init() - Component initialization`, While initialy not present within the game loop, it was quickly added as it would GREATLY simplify initialization for gameobejcts and their compoentns.

`Update() - Regular frame updates` In my case using a TimeSingleton to hold DeltaTime instead of passing it dowm. A design choice I'm still debating to this day!

`FixedUpdate() - Fixed timestep physics/logic updates`

`LateUpdate() - Post-update update to ensure that dependant components have the correct updated data for their own updates`

### Double Buffer
Used in rendering with SDL2's built-in double buffering system. having no other uses it is currently it's only usage.

### Component Pattern
Composition over Inheritance! Featuring a complete Entity-Component system where GameObjects are composed of various components. This promotes modularity and reusability, allowing for flexible game object creation without deep inheritance hierarchies.
In my version GameObjects come bare without any components. Altough this was changed midway through development the Transform was moved outside the GameObject into it's own component. It has remained the same in my [SFML Engine](). I marked it asa future improvement that every gameobject upon creation will come with a transform component. As 99% of uses need one.

### Dirty Flag
Used in different context, as well as the Transform component and GameObject hierarchy system. used to optimize expensive operations like world position calculations, only recalculating when necessary.

### State Pattern
Not found in the engine as I found that simpler GameState did the trick much better, however it is extensively used in the Pac-Man example through the ghost AI system. simplifying the headache that is  the state transitions and behavior management of the ghost AI.

### Event Queue Pattern
The audio system implements an event queue through the SDLISoundSystem class. Sound requests are queued using `std::queue` and processed asynchronously on a separate thread.

### Service Locator Pattern
The audio system also implements the Service Locator pattern through the ServiceLocator class, providing global access to audio services while maintaining flexibility in implementation choice.

### Hierarchy with Parent-Child Relationships
GameObjects support parent-child relationships with proper world/local position management, implementing a scene graph pattern common in game engines.

These patterns allow me to provides a clean, maintainable code architecture.







4. Project Structure

A brief explanation of the directory layout (e.g., src/, examples/, build/, docs/). Use a tree snippet or simple list.

5. Getting Started / Usage

Steps to compile/build quickly.

How to run example projects if any.

Dependencies (like SFML version).

6. How It Works / Architecture

Short narrative explaining core parts (e.g., how the rendering loop is structured, how modules interact).

Provide an overview of data flow or main classes.

7. Examples / Demos

Mention any sample projects included (e.g., Pac-Man clone, demo scenes).

Screenshots or links to visuals (if applicable).

8. Planned Improvements / Roadmap

Features you intend to add later (e.g., physics integration, scripting, editor tools).

Invite contributions if it's open to it.

9. About / Credits

Your name, background, motivation.

Reference your other projects for context or style (WaveFunctionCollapse, CursedResurrection, Astyanax).
