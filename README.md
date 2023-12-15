# hosTile
A 2D sprite-based graphics engine written in modern C++ using DirectX 11. 

Many, many years ago, I had written a 2D graphics engine in C++ and DirectX 8. This is a relaunch of that engine, but modernized with current C++ language features and the Direct3D 11.3 graphics API. It also uses features of the DirectXTK tool kit to speed up development and for debugging. As of version 0.6.0-beta, this rebuild is feature complete compared to the original version. It includes support for:
* Simple sprites that render a provided image.
* Tilesets of varying dimensions that can be referenced by...
* Tile sprites that render a specific tile from a tileset.
* Maps that describe a large area made of many tiles from a tileset.
* Fonts that can be layed out arbitrary to be referenced by...
* Text boxes that can render text from a font into a specified region.
* Debug rendering of quads using a separate debug pipeline.
* And many other helpful features including support for exported tilesets and maps from Tiled.

This project includes a sample "game" called Futile that shows off the above features. This game uses assets from Foozle and Kenney's free game assets. Links to those and Tiled are below. All hosTile-specific content is contained within the hosTile folder while game content is in the Futile folder.

Foozle's game assets: https://foozlecc.itch.io/
Kenney's Fonts: https://www.kenney.nl/assets/kenney-fonts
Tiled: https://www.mapeditor.org/
