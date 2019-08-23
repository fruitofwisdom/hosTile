/* Benjamin Ward Childress
   childb@rpi.edu
   hosTile 2d graphics engine (last modified 10/5/2003)
   hosTile.cpp
   Version 0.91
   These are the definitions of functions used by the hosTile classes (look in
   hosTile.h for declarations).
   Copyright 2003 by Benjamin Ward Childress
   This code is free and covered under the GNU GPL.  It may be edited, copied,
   and distributed as allowed therein. */
#include "globals.h"
#include <stdio.h>

/******************************
 * cTileSet class definitions *
 ******************************/

// Constructor
cTileSet::cTileSet()
{
	mHosTile = NULL;
	mTexture = NULL;
	mHeight = mWidth = 0;
}

// Destructor
cTileSet::~cTileSet()
{
	FreeTileSet();
}

// Draw a particular tileNum with the texture having numCols of columns and
// each tile being tileHeight x tileWidth in size.  Draw out to (screenX,
// screeny) with applied color (use D3DCOLOR_RGBA( 255, 255, 255, 255 ) for
// default color) and scaled by scaleX and scaleY.  Let hosTile::RenderMap()
// take care of everything for you.
BOOL cTileSet::Draw( long tileNum, long numCols, long tileHeight, long tileWidth, long screenX, long screenY, D3DCOLOR color, float xScale, float yScale )
{
	ID3DXSprite *pSprite;
	long srcX, srcY;
	RECT rect;

	// Error checking
	if( mHosTile == NULL )
		return FALSE;
	if( mTexture == NULL )
		return FALSE;
	if( ( pSprite = mHosTile->GetSpriteCOM() ) == NULL )
		return FALSE;

	// Calculate the source tile coordinates from texture
	if( numCols == 0 )	// If numCols is 0 just use the origin to draw from
	{
		srcX = srcY = 0;
	}
	else
	{
		// For example:  tileNum = 18, numCols = 16, and tileWidth = 64.
		// To calculate the srcX we'd get position 2 * 64 = 128 pixel offset
		// in the X direction.  Same idea for the Y offset.
		srcX = ( tileNum % numCols ) * tileWidth;
		srcY = ( tileNum / numCols ) * tileHeight;
	}

	// Make the bounding box
	rect.left = srcX;
	rect.top = srcY;
	rect.right = rect.left + tileWidth;
	rect.bottom = rect.top + tileHeight;

	// Make sure our box doesn't start on the edge of the texture
	if( rect.left >= mWidth )
		return FALSE;
	if( rect.top >= mHeight )
		return FALSE;

	// Now tell DirectX to draw
	if( FAILED( pSprite->Draw( mTexture,										// IDirect3DTexture8 object
			                   &rect,											// The bounding box that we want drawn
							   &D3DXVECTOR2( xScale, yScale ),					// How much to scale by
							   NULL,											// Rotation info we don't want
							   0.0f,											// Rotation info we don't want
							   &D3DXVECTOR2( (float)screenX, (float)screenY ),	// Where to draw on the screen
							   color ) ) )										// What color to draw with
		return FALSE;

	return TRUE;
}

// Free the texture from memory
BOOL cTileSet::FreeTileSet()
{
	mHosTile = NULL;
	if( mTexture )
	{
		mTexture->Release();
		mTexture = NULL;
	}
	mHeight = mWidth = 0;

	return TRUE;
}

// Get height
long cTileSet::GetHeight()
{
	D3DSURFACE_DESC d3dsd;

	// Error check
	if( mTexture == NULL )
		return 0;

	// Pull the height from the texture (not mHeight)
	if( FAILED( mTexture->GetLevelDesc( 0, &d3dsd ) ) )
	{
		ErrorMessage( "mTexture->GetLevelDesc() failed!" );
		return 0;
	}

	return d3dsd.Height;
}

// Get width
long cTileSet::GetWidth()
{
	D3DSURFACE_DESC d3dsd;

	// Error check
	if( mTexture == NULL )
		return 0;

	// Pull the width from the texture (not mWidth)
	if( FAILED( mTexture->GetLevelDesc( 0, &d3dsd ) ) )
	{
		ErrorMessage( "mTexture->GetLevelDesc() failed!" );
		return 0;
	}

	return d3dsd.Width;
}

// Load the tile fileName into memory and use transparent as the alpha color
// with format (D3DFMT_A8R8G8B8 is default 32-bit color).
BOOL cTileSet::Load( hosTile *ourHosTile, char *fileName, D3DCOLOR transparent, D3DFORMAT format )
{
	char buffer[ 128 ];

	FreeTileSet();

	// Error check
	if( ( mHosTile = ourHosTile ) == NULL )
		return FALSE;
	if( mHosTile->GetDeviceCOM() == NULL )
		return FALSE;
	if( fileName == NULL )
		return FALSE;

	// Now tell DirectX to load the file
	if( FAILED( D3DXCreateTextureFromFileEx( ourHosTile->GetDeviceCOM(),	// IDirect3DDevice interface
											 fileName,						// File to load
											 D3DX_DEFAULT, D3DX_DEFAULT,	// Get width and height from the texture
											 D3DX_DEFAULT,					// Get the number of mip levels from the texture
											 0,								// Default usage
											 format,						// D3DFORMAT (8, 16, 24, or 32-bit)
											 D3DPOOL_MANAGED,				// What memory class should this be placed in?
											 D3DX_FILTER_TRIANGLE,			// Filtering
											 D3DX_FILTER_TRIANGLE,			// Filtering
											 transparent,					// D3DCOLOR for transparency
											 NULL,							// Default mage info
											 NULL,							// Default palette
											 &mTexture ) ) )				// IDirect3DTexture8 object
	{
		// If this didn't work we need to tell the user
		sprintf( buffer, "D3DXCreateTextureFromFileEx( %s ) failed!", fileName );
		ErrorMessage( buffer );
		return FALSE;
	}

	// Get the width and height from the texture
	mWidth = GetWidth();
	mHeight = GetHeight();

	return TRUE;
}

/**************************
 * cMap class definitions *
 **************************/

// Constructor
cMap::cMap()
{
	mLayerData = NULL;
	mLayerTileSet = 0;
	mNumLayers = mHeight = mWidth = 0;
}

// Destructor
cMap::~cMap()
{
	FreeMap();
}

// Allocate room for the map with numLayers and width x height tiles
BOOL cMap::CreateMap( long numLayers, long width, long height )
{
	FreeMap();

	// Store map variables
	mNumLayers = numLayers;
	mHeight = height;
	mWidth = width;

	// Allocate and clear out the room for the map data
	if( ( mLayerData = new long[ mNumLayers * mWidth * mHeight ] ) == NULL )
		return FALSE;
	ZeroMemory( mLayerData, mNumLayers * mWidth * mHeight );

	// Allocate and clear layer tile set data
	if( ( mLayerTileSet = new long[ mNumLayers ] ) == NULL )
		return FALSE;
	ZeroMemory( mLayerTileSet, mNumLayers );

	return TRUE;
}

// Free map data
BOOL cMap::FreeMap()
{
	delete [] mLayerData;
	delete [] mLayerTileSet;

	mLayerData = NULL;
	mLayerTileSet = 0;
	mNumLayers = mHeight = mWidth = 0;

	return TRUE;
}

// Get layer data
long *cMap::GetLayerData( long layerNum )
{
	// Error check
	if( mLayerData == NULL || layerNum >= mNumLayers )
		return FALSE;

	return &mLayerData[ layerNum ];
}

// Get tile set data
long cMap::GetLayerTileSet( long layerNum )
{
	// Error check
	if( mLayerTileSet == NULL || layerNum >= mNumLayers )
		return FALSE;

	return mLayerTileSet[ layerNum ];
}

// Get the number of layers
long cMap::GetNumLayers()
{
	return mNumLayers;
}

// Get height
long cMap::GetHeight()
{
	return mHeight;
}

// Get width
long cMap::GetWidth()
{
	return mWidth;
}

// Set a layer's data
BOOL cMap::SetMapData( long layerNum, long *data )
{
	// Error check
	if( mLayerData == NULL )
		return FALSE;

	// Copy over the memory.  Note to self:  it is possible to set individual
	// layer data using mLayerData + layerNum * mWidth * mHeight as an offset.
	// This could be a possible function in the future (in hosTile of course).
	memcpy( &mLayerData[ layerNum * mWidth * mHeight ], data, sizeof( long ) * mWidth * mHeight );

	return TRUE;
}

// Specify that a layer uses a tile set
BOOL cMap::UseTileSet( long layerNum, long tileSetNum )
{
	// Error check
	if( mLayerTileSet == NULL )
		return FALSE;

	// D'uh
	mLayerTileSet[ layerNum ] = tileSetNum;

	return TRUE;
}

/*****************************
 * hosTile class definitions *
 *****************************/

// Constructor
hosTile::hosTile( long maxObjects, BOOL error )
{
	// Initialize display data
	mError = error;
	mHAL = FALSE;
	mWindowed = TRUE;
	mHWnd = NULL;

	// Initialize interface data
	mpD3D = NULL;
	mpD3DDevice = NULL;
	mpFont = NULL;
	mpSprite = NULL;

	// Initialize window data
	mWidth = mHeight = mBPP = 0;

	// Initialize font data
	mFontBold = mFontItalic = mFontStrikeout = mFontUnderlined = FALSE;
	mFontWeight = 0;

	// Initialize object data
	mObjects = (struct sObject *)malloc( sizeof( struct sObject ) * maxObjects );
	mMaxObjects = maxObjects;
	mNumObjects = 0;

	// Initialize tile set data
	mTileSets = NULL;
	mTileColumns = mTileHeights = mTileWidths = NULL;
	mNumTileSets = 0;

	// Initialize map data
	mMaps = NULL;
	mNumMaps = 0;
}

// Destructor
hosTile::~hosTile()
{
	Shutdown();

	// Free object data
	FreeObjects();
}

// Add an object to be drawn at (xPos, yPos) using the tile from tileSet.
BOOL hosTile::AddObject( long xPos, long yPos, long tileSet, long tile )
{
	char buffer[ 128 ];

	// If we haven't used up all our allocated object space
	if( mNumObjects < mMaxObjects )
	{
		mObjects[ mNumObjects ].tile = tile;
		mObjects[ mNumObjects ].tileSet = tileSet;
		mObjects[ mNumObjects ].xPos = xPos;
		mObjects[ mNumObjects ].yPos = yPos;
		mNumObjects++;

		return TRUE;
	}

	// Print error message
	if( mError )
	{
		sprintf( buffer, "Object space is full!  Try increasing maxObjects beyond %d.", mMaxObjects );
		ErrorMessage( buffer );
	}
	return FALSE;
}

// Declare it time to begin the font sequence
BOOL hosTile::BeginFont()
{
	// Error check
	if( mpFont == NULL )
		return FALSE;

	if( FAILED( mpFont->Begin() ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpFont->Begin() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Declare it time to begin the scene
BOOL hosTile::BeginScene()
{
	// Error check
	if( mpD3DDevice == NULL )
		return FALSE;

	if( FAILED( mpD3DDevice->BeginScene() ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->BeginScene() failed!" );
		return FALSE;
	}

	return TRUE;
}

// See if video card supports hardware acceleration
BOOL hosTile::CheckFormat( D3DFORMAT format, BOOL windowed, BOOL HAL )
{
	if( FAILED( mpD3D->CheckDeviceType( D3DADAPTER_DEFAULT, ( HAL == TRUE ) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF, format, format, windowed ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3D->CheckDeviceType() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Create a font with name, size, and desired attributes
BOOL hosTile::CreateFont( char *name, long size, BOOL bold, BOOL italic, BOOL strikeout, BOOL underline )
{
	LOGFONT lf;

	// Error check
	if( name == NULL )
		return FALSE;
	if( mpD3DDevice == NULL )
		return FALSE;

	// Clear out the font structure
	ZeroMemory( &lf, sizeof( LOGFONT ) );

	// Assign attributes
	strcpy( lf.lfFaceName, name );
	lf.lfHeight = -size;
	lf.lfWeight = ( bold == TRUE ) ? 700 : 0;
	lf.lfItalic = italic;
	lf.lfUnderline = underline;
	lf.lfStrikeOut = strikeout;

	// Create the font object
	if( FAILED( D3DXCreateFontIndirect( mpD3DDevice, &lf, &mpFont ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "D3DXCreateFontIndirect() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Allocate room for the maps with numLayers and width x height tiles
BOOL hosTile::CreateMaps( long numMaps, long numLayers, long width, long height )
{
	// Free in case of existing data
	FreeMaps();

	// Error check
	if( ( mNumMaps = numMaps ) == NULL )
		return FALSE;

	// Allocate maps
	if( ( mMaps = new cMap[ mNumMaps ] ) == NULL )
		return FALSE;
	for( short i = 0; i < numMaps; i++ )
		mMaps[ i ].CreateMap( numLayers, width, height );

	return TRUE;
}

// Allocate room for the tile sets
BOOL hosTile::CreateTiles( long numTileSets )
{
	// Free in case of existing data
	Free();

	// Error check
	if( ( mNumTileSets = numTileSets ) == NULL )
		return FALSE;

	// Allocate tile sets
	if( ( mTileSets = new cTileSet[ mNumTileSets ] ) == NULL )
		return FALSE;

	// Allocate width, height, and column count arrays
	mTileColumns = new long[ mNumTileSets ];
	mTileHeights = new long[ mNumTileSets ];
	mTileWidths = new long[ mNumTileSets ];

	return TRUE;
}

// Display the graphics
BOOL hosTile::Display()
{
	// Error check
	if( mpD3DDevice == NULL )
		return FALSE;

	// Present the rendered objects
	if( FAILED( mpD3DDevice->Present( NULL, NULL, NULL, NULL ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->Present() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Do or don't display error message
void hosTile::DisplayErrors( BOOL error )
{
	mError = error;
}

// Declare it time to end font sequence
BOOL hosTile::EndFont()
{
	// Error check
	if( mpFont == NULL )
		return FALSE;

	if( FAILED( mpFont->End() ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpFont->End() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Declare it time to end the scene
BOOL hosTile::EndScene()
{
	// Error check
	if( mpD3DDevice == NULL )
		return FALSE;

	mpD3DDevice->SetTexture( 0, NULL );

	if( FAILED( mpD3DDevice->EndScene() ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->EndScene() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Free tile set data
BOOL hosTile::Free()
{
	// Free all tile sets
	if( mNumTileSets )
	    for( short i = 0; i < mNumTileSets; i++ )
	      mTileSets[ i ].FreeTileSet();
	delete [] mTileSets;
	mTileSets = NULL;

	// Free width, height, and column arrays
	delete [] mTileColumns;
	delete [] mTileHeights;
	delete [] mTileWidths;

	mTileColumns = mTileHeights = mTileWidths = NULL;
	mNumTileSets = 0;

	return TRUE;
}

BOOL hosTile::Free( long tileSet )
{
	// Error check
	if( tileSet >= mNumTileSets || mTileSets == NULL )
		return FALSE;

	// Free a single texture resource
	mTileSets[ tileSet ].FreeTileSet();

	return TRUE;
}

// Free object data
BOOL hosTile::FreeObjects()
{
	delete [] mObjects;

	mObjects = NULL;
	mNumObjects = 0;

	return TRUE;
}

// Free a font
BOOL hosTile::FreeFont()
{
	if( mpFont )
	{
		mpFont->Release();
		mpFont = NULL;
	}

	return TRUE;
}

// Free all map data
BOOL hosTile::FreeMaps()
{
	// Free all maps
	if( mNumMaps )
		for( short i = 0; i < mNumMaps; i++ )
			mMaps[ i ].FreeMap();
	delete [] mMaps;

	mMaps = NULL;
	mNumMaps = 0;

	return TRUE;
}

// Get device COM
IDirect3DDevice8 *hosTile::GetDeviceCOM()
{
	return mpD3DDevice;
}

// Get BPP
char hosTile::GetFormatBPP( D3DFORMAT format )
{
	switch( format )
	{
		// 32 bit modes
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
			return 32;
			break;

		// 24 bit modes
		case D3DFMT_R8G8B8:
			return 24;
			break;

		// 16 bit modes
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
			return 16;
			break;

		// 8 bit modes
		case D3DFMT_A8P8:
		case D3DFMT_P8:
			return 8;
			break;	

		default:
			return 0;
	}
}

// Get what global tile is at a particular coordinate (row * col + col)
long hosTile::GetGlobalTileAt( long mapNum, long layerNum, long xPos, long yPos )
{
	char message[ 128 ];
	long mapWidth, mapX, mapY, tileSet;

	// Error check
	if( mapNum > mNumMaps )
		return NULL;
	if( layerNum > mMaps[ mapNum ].GetNumLayers() )
		return NULL;

	// Get tile set pointer
	tileSet = mMaps[ mapNum ].GetLayerTileSet( layerNum );

	// Is the width 0?  We don't want to divide by 0.
	if( mTileWidths[ tileSet ] == 0 )
	{
		// Warn but keep going
		if( mError )
		{
			sprintf( message, "Divide by zero in hosTile::GetTileAt():  mTileWidths[ %d ] = 0", tileSet );
			ErrorMessage( message );
		}
		mapX = xPos;
	}
	else
		mapX = xPos / mTileWidths[ tileSet ];

	// Is the height 0?  We don't want to divide by 0.
	if( mTileHeights[ tileSet ] == 0 )
	{
		// Warn but keep going
		if( mError )
		{
			sprintf( message, "Divide by zero in hosTile::GetTileAt():  mTileHeights[ %d ] = 0", tileSet );
			ErrorMessage( message );
		}
		mapY = yPos;
	}
	else
		mapY = yPos / mTileHeights[ tileSet ];

	// Get map pointer
	mapWidth = mMaps[ mapNum ].GetWidth();

	return mapY * mapWidth + mapX;
}

// Get number of objects allowed
long hosTile::GetMaxObjects()
{
	return mMaxObjects;
}

// Get number of objects used
long hosTile::GetNumObjects()
{
	return mNumObjects;
}

// Get sprite COM
ID3DXSprite *hosTile::GetSpriteCOM()
{
	return mpSprite;
}

// Get what tile is at a particular coordinate
long hosTile::GetTileAt( long mapNum, long layerNum, long xPos, long yPos )
{
	char message[ 128 ];
	long *mapPtr, mapWidth, mapX, mapY, ptrIn, tileSet;

	// Error check
	if( mapNum > mNumMaps )
		return NULL;
	if( layerNum > mMaps[ mapNum ].GetNumLayers() )
		return NULL;

	// Get map and tile set pointers
	mapPtr = mMaps[ mapNum ].GetLayerData( layerNum );
	tileSet = mMaps[ mapNum ].GetLayerTileSet( layerNum );

	// Is the width 0?  We don't want to divide by 0.
	if( mTileWidths[ tileSet ] == 0 )
	{
		// Warn but keep going
		if( mError )
		{
			sprintf( message, "Divide by zero in hosTile::GetTileAt():  mTileWidths[ %d ] = 0", tileSet );
			ErrorMessage( message );
		}
		mapX = xPos;
	}
	else
		mapX = xPos / mTileWidths[ tileSet ];

	// Is the height 0?  We don't want to divide by 0.
	if( mTileHeights[ tileSet ] == 0 )
	{
		// Warn but keep going
		if( mError )
		{
			sprintf( message, "Divide by zero in hosTile::GetTileAt():  mTileHeights[ %d ] = 0", tileSet );
			ErrorMessage( message );
		}
		mapY = yPos;
	}
	else
		mapY = yPos / mTileHeights[ tileSet ];

	// Get map pointer
	mapWidth = mMaps[ mapNum ].GetWidth();
	ptrIn = layerNum * mapWidth + mapY * mapWidth + mapX;

	return mapPtr[ ptrIn ];
}

// Initialize the engine
BOOL hosTile::Init()
{
	// Make sure the system isn't already running
	Shutdown();

	if( ( mpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) == NULL )
	{
		// Print error message
		if( mError )
			ErrorMessage( "Direct3DCreate8() returned NULL!" );
		return FALSE;
	}

	return TRUE;
}

// Load the tile set with fileName that is width x height pixels in size.  Make
// transparent transparent and default to 32-bit format.
BOOL hosTile::LoadTiles( long tileSet, char *fileName, long width, long height, D3DCOLOR transparent, D3DFORMAT format )
{
	char message[ 128 ];

	// Error check
	if( tileSet >= mNumTileSets || mTileSets == NULL )
		return FALSE;

	Free( tileSet );

	// Load the texture
	if( mTileSets[ tileSet ].Load( this, fileName, transparent, format ) == FALSE )
		return FALSE;

	// Store height value (number of pixels across)
	mTileWidths[ tileSet ] = width;

	// Store height value (number of pixels high)
	mTileHeights[ tileSet ] = height;

	// Calculate how many columns of tiles there are in the texture. This is used to speed up calculations when drawing tiles.
	if( mTileWidths[ tileSet ] == 0 )
	{
		// Print error message
		if( mError )
		{
			sprintf( message, "Divide by zero in hosTile::LoadTiles():  mTileWidths[ %d ] = 0", tileSet );
			ErrorMessage( message );
		}
		return FALSE;
	}
	mTileColumns[ tileSet ] = mTileSets[ tileSet ].GetWidth() / mTileWidths[ tileSet ];

	return TRUE;
}

// Render the font with text at (xPos, yPos) in the block width x height (0 for
// as much as needed) using color and format (left justified default).
BOOL hosTile::RenderFont( char *text, long xPos, long yPos, long width, long height, D3DCOLOR color, DWORD format )
{
	RECT rect;

	// Error check
	if( mpFont == NULL )
		return FALSE;

	// Max out width and height if the programmer doesn't care
	if( width == 0 )
		width = 65535;
	if( height == 0 )
		height = 65536;

	// Make bounding box
	rect.left = xPos;
	rect.top = yPos;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	if( FAILED( mpFont->DrawText( text, -1, &rect, format, color ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpFont->DrawText() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Render the map mapNum with the offset (mapXOffset, mapYOffset) with numRows
// and numCols being the total number of rows and cols in the map.  Use color
// (D3DCOLOR_RGBA( 255, 255, 255, 255 ) default).
BOOL hosTile::RenderMap( long mapNum, long mapXOffset, long mapYOffset, long numRows, long numCols, D3DCOLOR color )
{
	char message[ 128 ];
	// A lot of longs (haha, I'm funny)
	long column, layer, *mapPtr, mapWidth, mapX, mapY, numLayers, ptrIn, row, tileHeight, tileNum, tileSet, tileWidth, xOffset, yOffset;

	// Error check
	if( mMaps == NULL || mTileSets == NULL )
		return FALSE;

	// Loop through each layer
	numLayers = mMaps[ mapNum ].GetNumLayers();
	for( layer = 0; layer < numLayers; layer++ )
	{
		// Get map and tile set pointers
		mapPtr = mMaps[ mapNum ].GetLayerData( layer );
		tileSet = mMaps[ mapNum ].GetLayerTileSet( layer );

		// Calculate smooth scrolling variables
		if( mTileWidths[ tileSet ] == 0 )
		{
			if( mError )
			{
				sprintf( message, "Divide by zero in hosTile::RenderMap( %d ):  mTileWidths[ %d ] = 0", mapNum, tileSet );
				ErrorMessage( message );
			}
			return FALSE;
		}
		mapX = mapXOffset / mTileWidths[ tileSet ];
		if( mTileHeights[ tileSet ] == 0 )
		{
			if( mError )
			{
				sprintf( message, "Divide by zero in hosTile::RenderMap( %d ):  mTileHeights[ %d ] = 0", mapNum, tileSet );
				ErrorMessage( message );
			}
			return FALSE;
		}
		mapY = mapYOffset / mTileHeights[ tileSet ];

		// Calculate x and y offsets
		xOffset = mapXOffset % mTileWidths[ tileSet ];
		yOffset = mapYOffset % mTileHeights[ tileSet ];

		// Loop for each row and column
		for( row = 0; row < numRows + 1; row++ )
			for( column = 0; column < numCols + 1; column++ )
			{
				tileHeight = mTileHeights[ tileSet ];
				tileWidth = mTileWidths[ tileSet ];

				// Get map pointer
				mapWidth = mMaps[ mapNum ].GetWidth();
				ptrIn = layer * mapWidth + ( column + mapY ) * mapWidth + ( row + mapX );
				tileNum = mapPtr[ ptrIn ];
				mTileSets[ tileSet ].Draw( tileNum,										// Which tile to draw
										   numCols,										// Number of columns in tile set
										   tileHeight,									// Height of the tile (in pixels)
										   tileWidth,									// Width of the tile (in pixels)
										   row * mTileWidths[ tileSet ] - xOffset,		// Screen X coordinate
										   column * mTileHeights[ tileSet ] - yOffset,	// Screen Y coordinate
										   color );										// What color to draw
			}
	}

	return TRUE;
}

// Render an object with a color
BOOL hosTile::RenderObject( long object, D3DCOLOR color )
{
	long screenX, screenY, tileHeight, tileWidth;

	// Error check
	if( object > mNumObjects || mObjects == NULL || mTileSets == NULL )
		return FALSE;

	screenX = mObjects[ object ].xPos;
	screenY = mObjects[ object ].yPos;
	tileHeight = mTileHeights[ mObjects[ object ].tileSet ];
	tileWidth = mTileWidths[ mObjects[ object ].tileSet ];

	mTileSets[ mObjects[ object ].tileSet ].Draw( mObjects[ object ].tile,						// Which tile to draw
												  mTileColumns[ mObjects[ object ].tileSet ],	// Number of columns in tile set
												  tileHeight, tileWidth,						// Height and width (in pixels)
												  screenX, screenY,								// Screen coordinates
												  color );										// What color to draw

	return TRUE;
}

// Set a map and layer's data
BOOL hosTile::SetMapData( long mapNum, long layerNum, long *data )
{
	// Error check
	if( mapNum >= mNumMaps )
		return FALSE;
	if( mMaps == NULL )
		return FALSE;

	return mMaps[ mapNum ].SetMapData( layerNum, data );
}

// Set the display mode with window handle hWnd with attributes such as
// windowed, useZBuffer, width x height (in pixels), and bpp (0 for the display
// default).
BOOL hosTile::SetMode( HWND hWnd, BOOL windowed, BOOL useZBuffer, long width, long height, char bpp )
{
	D3DFORMAT format, altFormat;	// Direct3D formats
	D3DXMATRIX matProjection;		// Projection matrix
	D3DPRESENT_PARAMETERS d3dpp;	// Presentation structure
	float aspect;					// Aspect ratio
	long WindowHeight, WindowWidth;	// Window coordinates
	RECT ClientRect;				// A rectangle to hold client coordinates
	RECT WindowRect;				// A rectangle to hold window coordinates

	// Error check
	if( ( mHWnd = hWnd ) == FALSE )
		return FALSE;
	if( mpD3D == NULL )
		return FALSE;

	// Get the current display format
	if( FAILED( mpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &md3ddm ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3D->GetAdapterDisplayMode() failed!" );
		return FALSE;
	}

	// Configure width
	if( !width )
	{
		// Default to screen width if fullscreen
		if( windowed == FALSE )
		{
			mWidth = md3ddm.Width;
		}
		else
		{
			GetClientRect( mHWnd, &ClientRect );
			mWidth = ClientRect.right;
		}
	}
	else
	{
		mWidth = width;
	}

	// Configure height
	if( !height )
	{
		// Default to screen height if fullscreen
		if( windowed == FALSE )
		{
			mHeight = md3ddm.Height;
		}
		else
		{
			GetClientRect( mHWnd, &ClientRect );
			mHeight = ClientRect.bottom;
		}
	}
	else
	{
		mHeight = height;
	}

	// Configure BPP
	if( !( mBPP = bpp ) || windowed == TRUE )
	{
		if( !( mBPP = GetFormatBPP( md3ddm.Format ) ) )
		{
			// Print error message
			if( mError )
				ErrorMessage( "GetFormatBPP() failed!" );
			return FALSE;
		}
	}

	// Resize client window if windowed
	if( windowed == TRUE )
	{
		GetWindowRect( mHWnd, &WindowRect );
		GetClientRect( mHWnd, &ClientRect );

		WindowWidth = ( WindowRect.right - ( ClientRect.right - mWidth ) ) - WindowRect.left;
		WindowHeight = ( WindowRect.bottom - ( ClientRect.bottom - mHeight ) ) - WindowRect.top;

		MoveWindow( mHWnd, WindowRect.left, WindowRect.top, WindowWidth, WindowHeight, TRUE );
	}

	// Clear presentation structure
	ZeroMemory( &d3dpp, sizeof( D3DPRESENT_PARAMETERS ) );

	// Default to no hardware acceleration
	mHAL = FALSE;

	// Setup windowed or fullscreen usage
	if( ( mWindowed = windowed ) == TRUE )
	{
		d3dpp.BackBufferFormat = md3ddm.Format;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed = TRUE;

		// See if video card supports hardware acceleration
		if( CheckFormat( md3ddm.Format, windowed, TRUE ) == TRUE )
			mHAL = TRUE;
		else if( CheckFormat( md3ddm.Format, windowed, FALSE ) == FALSE  )
		{
			// Print error message
			if( mError )
				ErrorMessage( "CheckFormat() returned FALSE!" );
			return FALSE;
		}
	}
	else
	{
		d3dpp.BackBufferHeight = mHeight;
		d3dpp.BackBufferWidth = mWidth;
		d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.Windowed = FALSE;

		// Figure display format to use
		if( mBPP == 32 )
		{
			format = D3DFMT_X8R8G8B8;
			altFormat = D3DFMT_X8R8G8B8;
		}
		if( mBPP == 24 )
		{
			format = D3DFMT_R8G8B8;
			altFormat = D3DFMT_R8G8B8;
		}
		if( mBPP == 16 )
		{
			format = D3DFMT_R5G6B5;
			altFormat = D3DFMT_X1R5G5B5;
		}
		if( mBPP == 8 )
		{
			format = D3DFMT_P8;
			altFormat = D3DFMT_P8;
		}

		// Check for hardware accelerated device
		if( CheckFormat( format, FALSE, TRUE ) == TRUE )
			mHAL = TRUE;
		else
			if( CheckFormat( altFormat, FALSE, TRUE ) == TRUE )
			{
				mHAL = TRUE;
				format = altFormat;
			}
			else
				if( CheckFormat( format, FALSE, FALSE ) == FALSE )
					if( CheckFormat( altFormat, FALSE, FALSE ) == FALSE )
					{
						// Print error message
						if( mError )
							ErrorMessage( "CheckFormat() returned FALSE!" );
						return FALSE;
					}
					else
						format = altFormat;

		d3dpp.BackBufferFormat = format;
	}

	// Setup ZBuffer format - 16 bit
	if( useZBuffer == TRUE )
	{
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else
		d3dpp.EnableAutoDepthStencil = FALSE;

	// Create the Direct3D device object and set render states
	HRESULT hr = mpD3D->CreateDevice( D3DADAPTER_DEFAULT, ( mHAL == TRUE ) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &mpD3DDevice );
	if( hr != D3D_OK )
	{
		// Print an appropriate error message
		if( mError )
		{
			// INVALIDCALL can arise from an improper resolution.  Try a
			// "normal" resolution like 640x480 if you're getting this error
			// and your height and width are "crazy."
			if( hr == D3DERR_INVALIDCALL )
				ErrorMessage( "mpD3D->CreateDevice() failed:  D3DERR_INVALIDCALL!" );
			else if( hr == D3DERR_NOTAVAILABLE )
				ErrorMessage( "mpD3D->CreateDevice() failed:  D3DERR_NOTAVAILABLE!" );
			else if( hr == D3DERR_OUTOFVIDEOMEMORY )
				ErrorMessage( "mpD3D->CreateDevice() failed:  D3DERR_OUTOFVIDEOMEMORY!" );
			else
				ErrorMessage( "mpD3D->CreateDevice() failed:  Don't know why..." );
		}
		return FALSE;
	}
	if( mpD3DDevice == NULL )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice == NULL!" );
		return FALSE;
	}
	if( FAILED( mpD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetRenderState( D3DRS_LIGHTING ) failed!" );
		return FALSE;
	}
	if( FAILED( mpD3DDevice->SetRenderState( D3DRS_ZENABLE, ( useZBuffer == TRUE ) ? D3DZB_TRUE : D3DZB_FALSE ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetRenderState( D3DRS_ZENABLE ) failed!" );
		return FALSE;
	}
	if( FAILED( mpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE ) failed!" );
		return FALSE;
	}
	if( FAILED( mpD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE ) failed!" );
		return FALSE;
	}

	// Enable texture rendering stages and filter types
	mpD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	mpD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	mpD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	mpD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	mpD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

	// Set default ambient color to white
	if( FAILED( mpD3DDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 255, 255, 255 ) ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetRenderState( D3DRS_AMBIENT ) failed!" );
		return FALSE;
	}

	// Set proper aspect ratio
	aspect = (float)mHeight / (float)mWidth;
	D3DXMatrixPerspectiveFovLH( &matProjection, D3DX_PI / 4, aspect, 1.0f, 10000.0f );
	if( FAILED( mpD3DDevice->SetTransform( D3DTS_PROJECTION, &matProjection ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "mpD3DDevice->SetTransform() failed!" );
		return FALSE;
	}

	// Create a sprite interface
	if( FAILED( D3DXCreateSprite( mpD3DDevice, &mpSprite ) ) )
	{
		// Print error message
		if( mError )
			ErrorMessage( "D3DXCreateSprite() failed!" );
		return FALSE;
	}

	return TRUE;
}

// Set an object's tile
BOOL hosTile::SetObjectTile( long object, long tile )
{
	// Error check
	if( object > mNumObjects )
		return FALSE;

	mObjects[ object ].tile = tile;

	return TRUE;
}

// Set an object's tile set (-1 for tile will keep the same tile)
BOOL hosTile::SetObjectTileSet( long object, long tileSet, long tile )
{
	// Error check
	if( object > mNumObjects )
		return FALSE;

	mObjects[ object ].tileSet = tileSet;
	if( tile != -1 )
		mObjects[ object ].tile = tile;

	return TRUE;
}

// Set an object's coordinates
BOOL hosTile::SetObjectXY( long object, long xPos, long yPos )
{
	// Error check
	if( object > mNumObjects )
		return FALSE;

	mObjects[ object ].xPos = xPos;
	mObjects[ object ].yPos = yPos;

	return TRUE;
}

// Shutdown the engine
BOOL hosTile::Shutdown()
{
	if( mpSprite )
	{
		mpSprite->Release();
		mpSprite = NULL;
	}
	if( mpD3DDevice )
	{
		mpD3DDevice->Release();
		mpD3DDevice = NULL;
	}
	if( mpD3D )
	{
		mpD3D->Release();
		mpD3D = NULL;
	}

	Free();
	FreeFont();
	FreeMaps();

	return TRUE;
}

// Tell a map and layer to use a particular tile set
BOOL hosTile::UseTiles( long mapNum, long layerNum, long tileSetNum )
{
	// Error check
	if( mapNum >= mNumMaps )
		return FALSE;
	if( mMaps == NULL )
		return FALSE;

	return mMaps[ mapNum ].UseTileSet( layerNum, tileSetNum );
}

// Error message dialog box
static long ErrorMessage( char *message )
{
	return MessageBox( NULL, message, "Error", MB_OK | MB_ICONEXCLAMATION );
}