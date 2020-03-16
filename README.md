
Game engine project based on code from VoxelRPG

Developed by Adrian Grannas and Patrick Dahlin

# Note
The project cannot be compile as it's missing dependencies and assets.

# About
The game engine framework is split into multiple dynamic libraries that handle a specific task in the game engine.
### Library
The library project contains common code used in all other projects such as a debug console, asserts, math functions and a few data structures.
### Render Engine
The Render Engine project handles window management and rendering of 3D and 2D graphics and optionally provides a pixel perfect mouse pick function that uses the 3D rendered world to identify what object was clicked. The render engine also bundles ImGui (https://github.com/ocornut/imgui) for use as debug UI during development.
### Physics engine
The Physics project provides a physics engine to implement physics into a game. This is achieved by wrapping the bullet physics library (https://github.com/bulletphysics/bullet3) and exposing it through API functions 
### Networking and UI
Both the networking and UI projects are currently a work in progress and doesn't work with the rest of the framework.
### Framework
The framework project is an optional library that can be used that binds all other projects together into a game engine framework. It provides a concept of Levels and entities with functionality implemented in components that can be attached to entities, which tries to avoid a large inheritance tree from the base entity class. The framework library also handles initialization of the other libraries such as the physics engine and render engine.
