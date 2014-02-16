#include "PlayState.h"
#include "SFML\Graphics\RenderWindow.hpp"
#include "Player.h"
#include "DireRat.h"
PlayState::PlayState(StateList &owner) : State(owner), mData() {
	mData.w.loadWorldData("test.tmx");
	mData.player = new Player(mData.w.getStartPos());
	mData.gameObjects.push_back(new DireRat(sf::Vector2i(8, 8)));
}


PlayState::~PlayState() {
}

void PlayState::draw(sf::RenderWindow &win) {
	mData.w.draw(win);
	for(auto it = mData.gameObjects.begin(); it != mData.gameObjects.end(); it++) {
		(*it)->draw(win);
	}
	mData.player->draw(win);
}

void PlayState::update(GameData &gData) {
	mData.player->update(gData, mData);
}