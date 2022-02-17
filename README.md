# hosTile
A 2D sprite-based graphics engine written in modern C++ using DirectX 11. 

Many, many years ago, I had written a 2D graphics engine in C++ and DirectX 8. This is a relaunch of that engine, but modernized with current C++ language features and the Direct3D 11 graphics pipeline.

As of version 0.5.0-beta, this rebuild is basically feature complete compared to the original version. It includes support for:
* Simple sprites that render a provided image.
* Tilesets of varying dimensions that can be referenced by...
* Tile sprites that render a specific tile from a tileset.
* Maps that describe a large area made of many tiles from a tileset.
* Fonts that can be layed out arbitrary to be referenced by...
* Text boxes that can render text from a font into a specified region.
* And many other helpful features including support for exported tilesets and maps from Tiled.

This project includes a sample "game" called Futile that shows off many of the above features. This game uses assets from Kenney's free game assets. Links to those and Tiled are below. All hosTile-specific content is contained within the hosTile folder while content within the Game folder is intended to be replaced by any game implementation.

Kenney's Micro Roguelike: https://www.kenney.nl/assets/micro-roguelike
Kenney's Fonts: https://www.kenney.nl/assets/kenney-fonts
Tiled: https://www.mapeditor.org/
