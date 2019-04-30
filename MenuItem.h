#pragma once
#include <SFML/Graphics.hpp>
#include "Serializable.h"


enum behaviourNames {
	opensMenu, resumesGame, removesGearPiece, buysItem, sendsMessage
};

std::string behaviourNameToString(behaviourNames);
behaviourNames stringToBehaviourName(std::string);

struct behaviourParameters {
	behaviourNames behaviourName;
	std::string menuTo;

    bool activatesWhenClicked = true;

	unsigned int goldCost;

	decomposedData messageData;

	std::string itemBought;
	std::string gearPieceRemoved;
};


class MenuItem
{
private:
	sf::Rect<float> m_item;

	sf::Vector2f moveTransform;

	sf::Text m_buttonText;

	bool m_buttonTextSet = false;

    bool m_firstClick = true;

	sf::Sprite m_tex;
	std::vector<behaviourParameters> m_behaviours;
	sf::Clock m_clickTimer;
	float m_clickCooldown = 2;
public:
	MenuItem(sf::Rect<float>);

	void addbehaviourFromString(std::vector<std::string>);
	void addBehviourParameters(behaviourParameters);

    void setClickCooldown(float);
    float getClickCooldown()const;

	void move(sf::Vector2f);

	void setPosition(sf::Vector2f);
	sf::Vector2f getPosition()const;
	sf::Vector2f getDimension()const;

	void setButtonText(std::string, float, sf::Color, unsigned int);

	void reestablishInitialPostion();

    std::vector<behaviourParameters> click(bool);

	void setTexture(sf::Sprite);
	void draw(sf::RenderWindow*, sf::Vector2f viewDisplacement = sf::Vector2f(0,0));
	bool isClicked(sf::Vector2f);
	std::vector<behaviourParameters> getBehaviour()const;
	~MenuItem();
};

