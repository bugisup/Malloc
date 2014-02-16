#include "World.h"
#include "tinyxml2.h"
#include "XMLParser.h"
#include "TextureStore.h"
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include "Tile.h"

bool World::loadWorldData( std::string file ) {
	static const int solidIds[] = { 0, 1, 2, 3 };
	int x = 0;
	int y = 0;

	auto mapHandler = [&](tinyxml2::XMLElement* e) {
		mWidth = e->IntAttribute("width");
		mHeight = e->IntAttribute("height");
		mTileSize = e->IntAttribute("tilewidth");
		mTileLayer.resize( mWidth, std::vector< Tile* >( mHeight, nullptr ) );
	};

	auto tilesetHandler = [&](tinyxml2::XMLElement* e) {
		unsigned int gid = e->IntAttribute("firstgid");
		tinyxml2::XMLElement* child = e->FirstChildElement();
		std::string textureFile = child->Attribute("source");
		textureFile = textureFile.substr( 0, textureFile.find_last_of('.') );
		mTileset.setTexture( TextureStore::getTexture( textureFile ) );
	};

	auto tileHandler = [&](tinyxml2::XMLElement* e) {

		/* This gid will represent the actual index of the texturerect. */
		unsigned int gid = e->IntAttribute("gid");
		if( gid == 0 ) {
			x++;
			if( x == mWidth ) {
				x = 0;
				y++;
			}
			return;
		}
		gid -= 1;

		bool solid = false;
		for( auto i : solidIds ) {
			if ( gid != i ) continue;
			solid = true;
			break;
		}

		mTileLayer[x][y] = new Tile( gid, solid );

		x++;
		if( x == mWidth ) {
			x = 0;
			y++;
		}
	};

	ch::XMLParser parser;
	parser.hook( "map", mapHandler );
	parser.hook( "tileset", tilesetHandler);
	parser.hook( "tile", tileHandler );
	return parser.parse( file );
}

unsigned int World::getTileId( unsigned int x, unsigned int y ) {
	return mTileLayer[x][y]->getID();
}

World::~World() {
	for( auto row : mTileLayer ) {
		for( auto tile : row ) {
			delete tile;
		}
	}

}

void World::draw( sf::RenderWindow& win ) {
	 /* Draw the tiles first */
	for( int y = 0; y < mHeight; ++y ) {
		for( int x = 0; x < mWidth; ++x ) {
			if( mTileLayer[x][y] == nullptr ) continue;

			unsigned int id = mTileLayer[x][y]->getID();
			int tilesetWidth = mTileset.getTexture()->getSize().x;
			int col = tilesetWidth / mTileSize;
			int tileX = id % col;
			int tileY = id / col;

			mTileset.setTextureRect( sf::IntRect( tileX*mTileSize , tileY*mTileSize, mTileSize, mTileSize ) );
			mTileset.setPosition( x * mTileSize, y * mTileSize );
			win.draw( mTileset );
		}
	}
}