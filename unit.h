#pragma once
#include <vector>
#include <map>
#include <tuple>
#include <functional>
#include <random>
#include <stack>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "AttackAndDefence.h"
#include "AnimatorSprite.h"

const sf::Vector2f graveyard = sf::Vector2f(-10000, -10000);

float distance(sf::Vector2f v, sf::Vector2f w);
sf::Vector2f rotateByAngle(sf::Vector2f, sf::Vector2f, float);

float getAngleInDegrees(sf::Vector2f);
sf::Vector2f getUnitVec(sf::Vector2f, sf::Vector2f);

enum unitType {
	playerType, bulletType, defaultType
};

class unit
{
private:

	std::vector<std::pair<sf::Vector2f, float>> body;
	sf::Sprite texture;
	float actualWeight = 100;
	sf::Vector2f lastMove;

	float lastTime = 0;

	AnimatorSprite animatorSprite;

	sf::Vector2f currentPos;


	sf::Vector2f acceleration;
	sf::Vector2f velocity = sf::Vector2f(0,0);

	int ID;

	AnimatorSprite *animationConroller = nullptr;



public:
	static int ownedByIDTracker;

	unsigned int walkingAnimation;

    sf::Vector2f lastPos;
    sf::Vector2f lastLastPos;

	bool needsAnUpdate = false;

	AnimatorSprite *getAnimationController()const;
	void setAnimationController(AnimatorSprite*);

	AttackModule *Amodule = nullptr;
	DefenceModule *Dmodule = nullptr;

	bool isMoving(float threshHold = 0.1)const;

	//gets the animator Sprite and updates is values
	AnimatorSprite getAnimatorSprite();
	void setAnimatorSprite(AnimatorSprite, float additionalScale = 0);

    void kill();

	sf::Vector2f predictNextPos(float);
	sf::Vector2f predictNextPos();

	int ownedByID;
	float hitPoints = 100;
	bool isDead = false;
	bool recentlyDead = true;
	unitType typeOfUnit = defaultType;

	void move(sf::Vector2f);
	unit(std::vector<std::pair<sf::Vector2f, float>>);

	std::vector<std::pair<sf::Vector2f, float>> getBody()const;

	void stopMovement();

	sf::Vector2f getVelocity()const;

	void applyMoveSpeed(sf::Vector2f);
	void applyInstantVelocity(sf::Vector2f, float);
	void applyInstantVelocity(sf::Vector2f);
	void addVelocity(sf::Vector2f, float);
	void addVelocity(sf::Vector2f);
	void applyForce(sf::Vector2f, float);
	void applyForce(sf::Vector2f);

	sf::Vector2f getPosition()const;

	float getVelocityMag()const;

	sf::Vector2f getUnitVecTo(sf::Vector2f);

	//updates the position in accordance to the surroundings
	void update(float, std::vector<unit*> colliders = std::vector<unit*>());
	sf::Sprite getTexture()const;
	void setTexture(sf::Sprite&, bool scaleToMatch = false);

	float getWeight()const;
	void setWeight(float);

	int getID()const;

	//calculates the margin by which to spheres collide, without giving the most optimal movement for de-collision, too much math, collision avoidance is handled by the update function.
	float collides(const unit&, sf::Vector2f *evadePos = nullptr);

	~unit();
};

