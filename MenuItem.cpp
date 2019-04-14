#include "MenuItem.h"
#include <iostream>


std::string behaviourNameToString(behaviourNames behaviourName)
{
	switch (behaviourName)
	{
	case opensMenu:
		return "opensMenu";
		break;
	case resumesGame:
		return "resumesGame";
		break;
	default:
	    return "";
		break;
	}
}

behaviourNames stringToBehaviourName(std::string name)
{
	if (name == "opensMenu") {
		return opensMenu;
	}
	else if (name == "resumesGame") {
		return resumesGame;
	}
	return resumesGame;
}

void MenuItem::setClickCooldown(float clickCooldown){
    m_clickCooldown = clickCooldown;
}

float MenuItem::getClickCooldown()const{
    return m_clickCooldown;
}

std::vector<behaviourParameters> MenuItem::click(bool clicked){
    std::vector<behaviourParameters> retVal;
    for(size_t i = 0; i < m_behaviours.size();i++){
        if(m_behaviours[i].activatesWhenClicked && clicked){
            retVal.push_back(m_behaviours[i]);
            continue;
        }
        if(!m_behaviours[i].activatesWhenClicked && !clicked){
            retVal.push_back(m_behaviours[i]);
        }
    }
    if(!clicked){
        return retVal;
    }
    if(m_clickTimer.getElapsedTime().asSeconds() > m_clickCooldown || m_firstClick){
        m_firstClick = false;
        m_clickTimer.restart();
        return retVal;
    }
    return std::vector<behaviourParameters>();
}

MenuItem::MenuItem(sf::Rect<float> dim) :
	m_item(dim)
{
        m_clickTimer.restart();
}

void MenuItem::addbehaviourFromString(std::vector<std::string> behaviours)
{
	behaviourParameters tempBehaviourParam;
	tempBehaviourParam.behaviourName = stringToBehaviourName(behaviours[0]);

	switch (tempBehaviourParam.behaviourName)
	{
	case opensMenu:
		tempBehaviourParam.menuTo = behaviours[1];
		break;
	case resumesGame:
		break;
	default:
		break;
	}

	m_behaviours.push_back(tempBehaviourParam);
}

void MenuItem::addBehviourParameters(behaviourParameters behaviourParam)
{
	m_behaviours.push_back(behaviourParam);
}

void MenuItem::move(sf::Vector2f delta)
{
	moveTransform += delta;
}

void MenuItem::setPosition(sf::Vector2f pos)
{
	moveTransform.x = -m_item.left;
	moveTransform.y = -m_item.top;
	moveTransform += pos;
}

sf::Vector2f MenuItem::getPosition() const
{
	return sf::Vector2f(m_item.left, m_item.top);
}

sf::Vector2f MenuItem::getDimension() const
{
	return sf::Vector2f(m_item.width, m_item.height);
}

void MenuItem::reestablishInitialPostion()
{
	moveTransform = sf::Vector2f(0,0);
}

void MenuItem::setTexture(sf::Sprite tex)
{
	m_tex = tex;
	m_tex.setPosition(sf::Vector2f(m_item.left, m_item.top));
	m_tex.setScale(sf::Vector2f(m_item.width/m_tex.getGlobalBounds().width, m_item.height / m_tex.getGlobalBounds().height));
}

void MenuItem::draw(sf::RenderWindow *window, sf::Vector2f viewDisplacement)
{
	//temporary texture for menuItem, a blue square the size of the target bounding rectangle.
	if (m_tex.getTexture() == nullptr) {
		sf::RectangleShape tempRectS;
		tempRectS.setFillColor(sf::Color(55, 10,0, 150));
		tempRectS.setSize(sf::Vector2f(m_item.width, m_item.height));
		tempRectS.setPosition(sf::Vector2f(m_item.left, m_item.top));
		tempRectS.setOutlineThickness(10);
		tempRectS.setOutlineColor(sf::Color(10,10,10,100));
		tempRectS.move(viewDisplacement + moveTransform);
		window->draw(tempRectS);
	}
	else {

		m_tex.move(viewDisplacement + moveTransform);
		window->draw(m_tex);
		m_tex.move(-(viewDisplacement +moveTransform));
	}
}

bool MenuItem::isClicked(sf::Vector2f mousePos)
{
	bool retVal = false;
	m_item.left += moveTransform.x;
	m_item.top += moveTransform.y;
	if (m_item.contains(mousePos)) {
        retVal = true;
	}
	m_item.left -= moveTransform.x;
	m_item.top -= moveTransform.y;
	return retVal;
}

std::vector<behaviourParameters> MenuItem::getBehaviour()const
{
	return m_behaviours;
}

MenuItem::~MenuItem()
{
}

