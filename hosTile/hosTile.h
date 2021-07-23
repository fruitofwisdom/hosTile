/* Benjamin Ward Childress
   childb@rpi.edu
   hosTile 2d graphics engine (last modified 10/5/2003)
   hosTile.h
   Version 0.91
   This is a 2d tile-based graphics engine.  You need to include the library
   files d3d8.lib, d3dx8.lib, dxguid.lib, and dinput8.lib as well.  The
   hosTile class is the main graphics handling portion that interfaces with
   DirectX.  It contains an array of cMaps, each of which holds multiple
   layers of data.  Each layer has a two-dimensional array of indices into
   the cTileSet class.  This class holds the textures and can draw each
   texture (or part) to the screen, though the RenderMap function will draw
   them all automatically.  Also, a collection of sObjects are kept that can
   be used to hold player graphics, enemy graphics, etc.
   Copyright 2003 by Benjamin Ward Childress 
   This code is free and covered under the GNU GPL.  It may be edited, copied,
   and distributed as allowed therein. */
#ifndef _HOSTILE_H_
#define _HOSTILE_H_

// The hosTile class prototype
class hosTile;

// The object struct contains all necessary information for drawing an
// individual sprite (object) to the screen.  It will draw the sprite with size
// taken from the tileSet specified, offset into the tileSet as specified by
// tile, and at coordinates (xPos, yPos).  Use the functions in the main class
// to access and draw these objects.
struct sObject
{
	long xPos, yPos;											// X and Y coordinates
	long tileSet;												// Tile set to draw from
	long tile;													// Tile to draw
};

// The tile set class contains the texture data and information about how the
// textures should be split up.  cTileSet::Draw() is the main texture rendering
// function.  No function here needs to be called by the user; instead all the
// interaction takes place in the hosTile functions.
class cTileSet
{
	private:
		hosTile *mHosTile;										// A pointer back to the hosTile instance
		IDirect3DTexture8 *mTexture;							// The texture
		long mHeight;											// Texture height (in pixels)
		long mWidth;											// Texture width (in pixels)

	public:
		cTileSet();												// Constructor
		~cTileSet();											// Destructor

		// Draw a particular tileNum with the texture having numCols of columns
		// and each tile being tileHeight x tileWidth in size.  Draw out to
		// (screenX, screeny) with applied color (use
		// D3DCOLOR_RGBA( 255, 255, 255, 255 ) for default color) and scaled by
		// scaleX and scaleY.  Let hosTile::RenderMap() take care of everything
		// for you.
		BOOL Draw( long tileNum, long numCols, long tileHeight, long tileWidth, long screenX, long screenY, D3DCOLOR color, float xScale = 1, float yScale = 1 );
		// Free the texture from memory
		BOOL FreeTileSet();
		// Get height and width
		long GetHeight();
		long GetWidth();
		// Load the tile fileName into memory and use transparent as the alpha
		// color with format (D3DFMT_A8R8G8B8 is default 32-bit color).
		BOOL Load( hosTile *ourHosTile, char *fileName, D3DCOLOR transparent, D3DFORMAT format );
};

// The map class contains the layer data and layer tile set data.  This is all
// the information about what tile to draw where in each map.  It may have
// multiple layers in which case each subsequent layer is contained immediately
// after the previous in the data structure.  E.g., mLayerData is actually a
// flexible, multi-dimensional array.  The number of layers is contained in
// mNumLayers and the number of tiles in the map is stored in mHeight and
// mWidth.
class cMap
{
	private:
		long *mLayerData;										// Array of layer data
		long *mLayerTileSet;									// Array of layer tile set indices
		long mNumLayers;										// Number of layers
		long mHeight;											// Map height (in tiles)
		long mWidth;											// Map width (in tiles)

	public:
		cMap();													// Constructor
		~cMap();												// Destructor

		// Allocate room for the map with numLayers and width x height tiles
		BOOL CreateMap( long numLayers, long width, long height );
		// Free map data
		BOOL FreeMap();
		// Get layer and tile set data
		long *GetLayerData( long layerNum );
		long GetLayerTileSet( long layerNum );
		// Get the number of layers, height, and width
		long GetNumLayers();
		long GetHeight();
		long GetWidth();
		// Set a layer's data
		BOOL SetMapData( long layerNum, long *data );
		// Specify that a layer uses a tile set
		BOOL UseTileSet( long layerNum, long tileSetNum );
};

// The hosTile class contains all the information for the graphics engine.
// This includes basic DirectX interfaces, font rendering attributes, object
// data, map data, and everything in between.  This is the class the programmer
// interfaces with.
class hosTile
{
	private:
		// Display data
		BOOL mError;											// Print error messages?
		BOOL mHAL;												// Use hardware acceleration?
		BOOL mWindowed;											// Windowed?
		D3DDISPLAYMODE md3ddm;									// The Direct3D display mode
		HWND mHWnd;												// The window handle

		// Interface data
		IDirect3D8 *mpD3D;										// The Direct3D8 interface
		IDirect3DDevice8 *mpD3DDevice;							// The Direct3DDevice8 interface
		ID3DXFont *mpFont;										// The Direct3DX font interface
		ID3DXSprite *mpSprite;									// The Direct3DX sprite interface

		// Window data
		char mBPP;												// Window bits-per-pixel (8, 16, 24, or 32)
		long mHeight;											// Window height (in pixels)
		long mWidth;											// Window width (in pixels)

		// Font data
		BOOL mFontBold;											// Bold?
		BOOL mFontItalic;										// Italic?
		BOOL mFontStrikeout;									// Strikeout?
		BOOL mFontUnderlined;									// Underlined?
		long mFontWeight;										// Font weight

		// Object data
		sObject *mObjects;										// Object array
		long mMaxObjects;										// Max number of objects
		long mNumObjects;										// Number of objects

		// Tile set data
		cTileSet *mTileSets;									// Tile set array
		long *mTileColumns;										// Number of columns in textures
		long *mTileHeights;										// Number of tiles high array
		long *mTileWidths;										// Number of tiles wide array
		long mNumTileSets;										// Number of tile sets

		// Map data
		cMap *mMaps;											// Array of maps
		long mNumMaps;											// Number of maps

		// Get BPP
		char GetFormatBPP( D3DFORMAT format );

	public:
		hosTile( long maxObjects = 1024, BOOL error = TRUE );	// Constructor
		~hosTile();												// Destructor

		// Get device and sprite COM
		IDirect3DDevice8 *GetDeviceCOM();
		ID3DXSprite *GetSpriteCOM();

		// Add an object to be drawn at (xPos, yPos) using the tile from
		// tileSet.
		BOOL AddObject( long xPos, long yPos, long tileSet, long tile );
		// Declare it time to begin the font sequence
		BOOL BeginFont();
		// Declare it time to begin the scene
		BOOL BeginScene();
		// See if video card supports hardware acceleration
		BOOL CheckFormat( D3DFORMAT format, BOOL windowed, BOOL HAL );
		// Create a font with name, size, and desired attributes
		BOOL CreateFont( char *name, long size, BOOL bold, BOOL italic, BOOL strikeout, BOOL underline );
		// Allocate room for the maps with numLayers and width x height tiles
		BOOL CreateMaps( long numMaps, long numLayers, long width, long height );
		// Allocate room for the tile sets
		BOOL CreateTiles( long numTileSets );
		// Display the graphics
		BOOL Display();
		// Do or don't display error message
		void DisplayErrors( BOOL error );
		// Declare it time to end the font sequence
		BOOL EndFont();
		// Declare it time to end the scene
		BOOL EndScene();
		// Free tile set data
		BOOL Free();
		BOOL Free( long tileSet );
		// Free a font
		BOOL FreeFont();
		// Free all map data
		BOOL FreeMaps();
		// Free object data
		BOOL FreeObjects();
		// Get what global tile is at a particular coordinate (row * col + col)
		long GetGlobalTileAt( long mapNum, long layerNum, long xPos, long yPos );
		// Get number of objects allowed
		long GetMaxObjects();
		// Get number of objects used
		long GetNumObjects();
		// Get what tile is at a particular coordinate
		long GetTileAt( long mapNum, long layerNum, long xPos, long yPos );
		// Initialize the engine
		BOOL Init();
		// Load the tile set with fileName that is width x height pixels in
		// size.  Make transparent transparent and default to 32-bit format.
		BOOL LoadTiles( long tileSet, char *fileName, long width, long height, D3DCOLOR transparent = 0, D3DFORMAT format = D3DFMT_A8R8G8B8 );
		// Render the font with text at (xPos, yPos) in the block width x
		// height (0 for as much as needed) using color and format (left
		// justified default).
		BOOL RenderFont( char *text, long xPos, long yPos, long width = 0, long height = 0, D3DCOLOR color = D3DCOLOR_RGBA( 255, 255, 255, 255 ), DWORD format = DT_LEFT);
		// Render the map mapNum with the offset (mapXOffset, mapYOffset) with
		// numRows and numCols being the total number of rows and cols in the
		// map.  Use color (D3DCOLOR_RGBA( 255, 255, 255, 255 ) default).
		BOOL RenderMap( long mapNum, long mapXOffset, long mapYOffset, long numRows, long numCols, D3DCOLOR color = D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		// Render an object with a color
		BOOL RenderObject( long object, D3DCOLOR color = D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
		// Set a map and layer's data
		BOOL SetMapData( long mapNum, long layerNum, long *data );
		// Set the display mode with window handle hWnd with attributes such as
		// windowed, useZBuffer, width x height (in pixels), and bpp (0 for the
		// display default).
		BOOL SetMode( HWND hWnd, BOOL windowed = TRUE, BOOL useZBuffer = FALSE, long width = 0, long height = 0, char bpp = 0 );
		// Set an object's tile
		BOOL SetObjectTile( long object, long tile );
		// Set an object's tile set (-1 for tile will keep the same tile)
		BOOL SetObjectTileSet( long object, long tileSet, long tile = -1 );
		// Set an object's coordinates
		BOOL SetObjectXY( long object, long xPos, long yPos );
		// Shutdown the engine
		BOOL Shutdown();
		// Tell a map and layer to use a particular tile set
		BOOL UseTiles( long mapNum, long layerNum, long tileSetNum );
};

// Error message dialog box
static long ErrorMessage( char *message );

#endif