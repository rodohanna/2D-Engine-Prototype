# 2D Engine Prototype

This project is the result of a rabbit hole I went down when I wanted to learn more about 2D graphics and game engines.

Disclaimer: I use the term "engine" loosely because I've learned that the real engines that are used to ship complex games are lightyears more advanced than anything I have here.

<video width="320" height="240" controls>
  <source src="level-editor-demo.mp4" type="video/mp4">
</video>

### Tech

- [SDL2](https://www.libsdl.org/) - Low-level access to mouse, keyboard, and graphics hardware
- [picojson](https://github.com/kazuho/picojson) - header-only JSON library
- [C++](https://www.cplusplus.com/)

### Installation

Requires [SDL2_Image](https://www.libsdl.org/projects/SDL_image/) and [SDL2_Ttf](https://www.libsdl.org/projects/SDL_ttf/).

The Makefiles make some assumptions about where SLD2 and its auxilary libraries are stored.

On Mac:

```sh
$ make -f Makefile.mac
```

On Windows (requires MinGW).

```sh
$ make
```

## Architecture

The engine is a purely event-driven program, meaning that to do anything interesting you usually must buffer an event somewhere.

### Initialization

When the engine boots up it initializes SDL, figures out the monitor refresh rate and sets the timestep accordingly, sets up the world & GUI cameras, initializes the input system, and finally loads resources from JSON config files.

### Engine Loop

- Input events are collected and buffered
- The GUI manager processes new events and then updates (may generate new events)
- The ECS processes events new events and then updates (may generate new events)
- The frame rate is syncronized
- The render system performs a render of any queued render events

### Assets

Assets are handled very simply. An `asset-manifest.json` is used to tell the asset loader where assets are and how they should be loaded. The supported asset types are `sprites` and `fonts`. They are turned into [textures](https://wiki.libsdl.org/SDL_Texture) and stored in an asset table to be used by the renderer. Entities never handle assets directly, they are only ever given a handle to an asset that they give to the renderer when they want to be drawn.

### Entity Component System

The entity manager is organized based on the [Entity Component System](https://en.wikipedia.org/wiki/Entity_component_system) architecture.
My system is set up as follows:

- A component is a struct that contains (among other things) an enum type and a [union](https://www.tutorialspoint.com/cprogramming/c_unions.htm) member called "data" that is a union of all possible component data. Ex. A position component contains an x and y value that can be used to identify where an entity should be drawn.
- An entity contains a component array as well as an integer field that is used as a bitmask to quickly identify which components an entity has.
- A system is just a function that takes in an entity and some context data and produces an event(s). Ex. the render system takes in an entity, checks to see if it has the necessary components to be drawn, and generates a render event.

### Renderer

Entites can't control when they are rendered. They can only buffer a render event telling the renderer where, how, and on which layer they want to be drawn. This allows for a lot of flexibility because render events can be preprocessed before rendering (like y and z-index sorting). The renderer processes buffered render events by:

- Separating events into a world buffer and a GUI buffer.
- Initializing a blank texture and setting it as the render target.
- Sorting world-render events by z-index
- Retrieving their textures from the texture table.
- Rendering them accordingly.
- Doing the same thing for the GUI render events.
- Flipping the render buffer to actually draw stuff to the screen.

### Save/Load

The state of the ECS can be persisted by writing a JSON representation of the entities and their components to disk. The engine will automatically look for a save file when booting up. The current way to save your state is by pressing `q` and then clicking the `left mouse button`. It's weird but it works. ¯\\_(ツ)_/¯

## License

MIT
