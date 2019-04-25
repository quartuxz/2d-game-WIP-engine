#include "Animator.h"
#include <Python.h>
#include <boost/python.hpp>
#include <cstdlib>

Animator::~Animator()
{
	std::map<unsigned int, sf::Texture*>::iterator it;
	for (it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		delete it->second;
	}

}

std::queue<AnimatorSprite> Animator::m_updateAnimRecursive(std::queue<AnimatorSprite> animation, float timeDelta, AnimatorSprite offset)
{

	AnimatorSprite tempASprite = animation.front();

	if (tempASprite.timeDisplayed > tempASprite.timeElapsed) {
		AnimatorSprite lastTempSprite;
		if (offset.offsets) {
			lastTempSprite = tempASprite;
			tempASprite.position = offset.position;
			tempASprite.rotation = offset.rotation;
		}
		addOneFrameSprite(tempASprite);
		if (offset.offsets) {
			tempASprite = lastTempSprite;
		}
		tempASprite.timeElapsed += timeDelta;
		animation.front() = tempASprite;
	}
	else {
		animation.pop();
		animation = m_updateAnimRecursive(animation, tempASprite.timeElapsed- tempASprite.timeDisplayed);
	}

	return animation;
}

sf::Sprite Animator::m_getSprite(AnimatorSprite aSprite)
{
	sf::Sprite tempSprite;
	tempSprite.setTexture(*m_textures[aSprite.textureID]);
	if (aSprite.originToCenter) {
		tempSprite.setOrigin(sf::Vector2f(tempSprite.getLocalBounds().width / 2, tempSprite.getLocalBounds().height / 2));
	}
	tempSprite.setPosition(aSprite.position);
	tempSprite.setScale(sf::Vector2f(aSprite.scale, aSprite.scale));
	tempSprite.setRotation(aSprite.rotation);

	return tempSprite;
}

void Animator::clearTextures()
{
	m_textures.clear();
}

void Animator::setWindow(sf::RenderWindow *window)
{
	m_window = window;
}

sf::Texture* Animator::getTexture(unsigned int ID)
{
	return m_textures[ID];
}

unsigned int Animator::addTexture(std::string fileName)
{
	sf::Texture *tempTex = new sf::Texture();
	tempTex->loadFromFile(fileName);
	m_fileIDs[fileName] = m_TextureIDCounter;
	m_fileNames[m_TextureIDCounter] = fileName;
	m_textures[m_TextureIDCounter++] = tempTex;
	return (m_TextureIDCounter-1);
}

void Animator::addOneFrameSprite(AnimatorSprite aSprite)
{
    if (m_spritesToDraw.size() < (aSprite.drawLayer + 1)) {
		m_spritesToDraw.resize(aSprite.drawLayer + 1);
	}
	if (aSprite.isActive) {
		m_spritesToDraw[aSprite.drawLayer].push(m_getSprite(aSprite));
	}

}

void Animator::addOneFrameSprite(ToolTip *toolTip)
{

	m_toolTipsToDraw.push_back(toolTip);
}

void Animator::update(float timeDelta)
{

	std::list<AnimatorSprite>::iterator it2;
	for (it2 = m_decals.begin(); it2 != m_decals.end(); ++it2)
	{
		if (it2->timeDisplayed == 0 || it2->timeElapsed < it2->timeDisplayed) {
			it2->timeElapsed += timeDelta;
		}
	}

	std::list<std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>>::iterator it = m_animations.begin();
	while (it != m_animations.end())
	{
		if (it->first.empty()) {
			delete it->second;
			m_animations.erase(it++);
		}
		else {
			it->first = m_updateAnimRecursive(it->first, timeDelta, *it->second);
			++it;
		}
	}
}

void Animator::addDecal(AnimatorSprite decal)
{
	m_decals.push_back(decal);
}

std::string Animator::getTextureFileName(unsigned int textureID)
{
	return m_fileNames[textureID];
}

unsigned int Animator::getTextureID(std::string fileName)
{
	return m_fileIDs[fileName];
}

sf::FloatRect Animator::getTextureLocalBounds(unsigned int textureID)
{
	sf::Sprite tempSprite;
	tempSprite.setTexture(*m_textures[textureID]);
	return tempSprite.getLocalBounds();
}

AnimatorSprite* Animator::playAnimation(std::queue<AnimatorSprite> animation)
{
	AnimatorSprite *controller = new AnimatorSprite(animation.front());
	controller->offsets = true;
	m_animations.push_back(std::pair<std::queue<AnimatorSprite>, AnimatorSprite*>(animation, controller));
	return controller;
}

AnimatorSprite* Animator::playAnimation(unsigned int animationPresetID)
{
	return playAnimation(m_animationPresets[animationPresetID]);
}

unsigned int Animator::addAnimationPreset(std::queue<AnimatorSprite> animationPreset)
{
	m_animationPresets[m_animationPresetIDCounter++] = animationPreset;
	return m_animationPresetIDCounter;
}

void Animator::draw()
{
	std::list<AnimatorSprite>::iterator it;
	for (it = m_decals.begin(); it != m_decals.end(); ++it)
	{
		addOneFrameSprite(*it);
		if (it->timeDisplayed != 0 || it->timeElapsed > it->timeDisplayed) {
			m_decals.erase(it);
		}
	}
	for (size_t i = 0; i < m_spritesToDraw.size(); i++)
	{
		while (!m_spritesToDraw[i].empty()) {
			m_window->draw(m_spritesToDraw[i].top());
			m_spritesToDraw[i].pop();
		}
	}
	m_spritesToDraw.clear();

	for (size_t i = 0; i < m_toolTipsToDraw.size(); i++)
	{
		m_toolTipsToDraw[i]->draw(*m_window);
	}
	m_toolTipsToDraw.clear();
}

void Animator::instantDraw(AnimatorSprite aSprite)
{
	m_window->draw(m_getSprite(aSprite));
}

