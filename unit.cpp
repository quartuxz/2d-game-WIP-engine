#include "unit.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#include "Animator.h"
using namespace std;


int unit::ownedByIDTracker = 0;

unit::unit(std::vector<std::pair<sf::Vector2f, float>> m_body)
{
	static int IDCounter = 0;
	body = m_body;
	//texture.setRadius(m_body[0].second);
	//texture.setOrigin(sf::Vector2f(m_body[0].second, m_body[0].second));
	//texture.setPosition(m_body[0].first);
	//texture.setFillColor(sf::Color(255, 0, 0));
	ID = IDCounter++;
}

std::vector<std::pair<sf::Vector2f, float>> unit::getBody() const
{
	return body;
}

void unit::stopMovement()
{
	velocity = sf::Vector2f(0,0);
	acceleration = sf::Vector2f(0,0);
}

sf::Vector2f unit::getVelocity() const
{
	return velocity;
}

void unit::applyMoveSpeed(sf::Vector2f unitVec)
{
	applyInstantVelocity(unitVec, Dmodule->moveSpeed);
}

void unit::applyInstantVelocity(sf::Vector2f unitVec, float mag)
{
	applyInstantVelocity(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::applyInstantVelocity(sf::Vector2f vel)
{
	velocity = vel;
}

void unit::addVelocity(sf::Vector2f unitVec, float mag)
{
	addVelocity(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::addVelocity(sf::Vector2f vel)
{
	velocity += vel;
}

void unit::applyForce(sf::Vector2f unitVec, float mag)
{
	applyForce(sf::Vector2f(unitVec.x * mag, unitVec.y * mag));
}

void unit::applyForce(sf::Vector2f force)
{
	force = sf::Vector2f(force.y /actualWeight, force.y / actualWeight);
	acceleration += force;
}

AnimatorSprite * unit::getAnimationController() const
{
	return animationConroller;
}

void unit::setAnimationController(AnimatorSprite *aSprite)
{
	animationConroller = aSprite;
}

bool unit::isMoving(float threshHold) const
{
	return ((abs(velocity.x)+abs(velocity.y))> threshHold);
}

AnimatorSprite unit::getAnimatorSprite()
{
	animatorSprite.position = body[0].first;
	return animatorSprite;
}

void unit::setAnimatorSprite(AnimatorSprite aSprite, float additionalScale)
{
	aSprite.scale = body[0].second / (Animator::getInstance().getTexture(aSprite.textureID)->getSize().x / 2);
	aSprite.scale += additionalScale;
	animatorSprite = aSprite;
}

sf::Vector2f unit::predictNextPos(float timeDelta)
{
	return body[0].first + (velocity*timeDelta);
}

sf::Vector2f unit::predictNextPos()
{
	return body[0].first + (velocity*lastTime);
}

void unit::move(sf::Vector2f delta)
{
    lastLastPos = lastPos;
    lastPos = body[0].first;
	currentPos += delta;
	texture.move(delta);
	for (size_t i = 0; i < body.size(); i++)
	{
		body[i].first += delta;
	}
	lastMove = delta;

}

sf::Vector2f unit::getPosition() const
{
	return currentPos;
}

float unit::getVelocityMag() const
{
	return sqrt(pow(velocity.x,2) + pow(velocity.y, 2));
}

sf::Vector2f unit::getUnitVecTo(sf::Vector2f pos)
{
	sf::Vector2f dist = sf::Vector2f(pos) - (body[0].first);
	float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
	return sf::Vector2f(dist.x / mag, dist.y / mag);
}

void unit::kill(){
    isDead = true;
    move(-body[0].first);
    move(graveyard);
}

void unit::update(float seconds, std::vector<unit*> colliders)
{

	if (seconds > 1) {
		velocity += acceleration;
		acceleration = sf::Vector2f(0,0);
	}
	else {
		velocity += acceleration * seconds;
		acceleration -= acceleration * seconds;
	}

	float accelerationMagnitude = sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2));
	if (accelerationMagnitude < 0.0001) {
		acceleration = sf::Vector2f(0, 0);
	}

	move(velocity * seconds);
	if (Dmodule != nullptr) {
		if (Dmodule->push.second > 0) {
			move(Dmodule->push.first * std::min<float>(seconds, Dmodule->push.second));
			Dmodule->push.second -= seconds;
		}
	}




	if (velocity.x == 0 && velocity.y == 0) {

	}
	else {
		animatorSprite.rotation = getAngleInDegrees(getUnitVec(sf::Vector2f(0, 0), velocity*seconds));
	}

	if (animationConroller != nullptr) {
		animationConroller->position = body[0].first;
		animationConroller->rotation = animatorSprite.rotation;
	}

	for (size_t i = 0; i < colliders.size(); i++)
	{
		if (colliders[i]->getID() != ID) {
			sf::Vector2f evadePos;
			float collisionVal = collides(*colliders[i], &evadePos);
			if (collisionVal != -1) {
				//std::cout << "asd" << std::endl;
				//direction-of-movement based collision avoidance
				//evadePos = sf::Vector2f(0, 0);
				float temp = collisionVal / sqrt(pow(evadePos.x, 2) + pow(evadePos.y, 2));
				evadePos *= temp;
				//weight based momentum transfer
				//colliders[i]->actions.push(std::pair<actionsOnUnit, metaActionData>(moves, -evadePos));
				colliders[i]->move(-evadePos*actualWeight/(actualWeight + colliders[i]->actualWeight));
				move(evadePos* colliders[i]->actualWeight / (actualWeight + colliders[i]->actualWeight));
			}
		}
	}

	if (Dmodule != nullptr) {
		Dmodule->hitPoints -= (Dmodule->damageTakenPerSecond *seconds);
		if(Dmodule->hitPoints <= 0 && recentlyDead){
            kill();
		}
		if (Dmodule->hitPoints > Dmodule->hitPoinCap) {
			Dmodule->hitPoints = Dmodule->hitPoinCap;
		}
	}


	lastTime = seconds;

}

sf::Sprite unit::getTexture() const
{
	return texture;
}

void unit::setTexture(sf::Sprite &tex, bool scaleToMatch)
{
	texture = tex;
	texture.setOrigin(sf::Vector2f(texture.getLocalBounds().width / 2, texture.getLocalBounds().height / 2));
	texture.setPosition(body[0].first);
	if (scaleToMatch) {
		texture.scale(sf::Vector2f(body[0].second /(texture.getLocalBounds().width/2), body[0].second / (texture.getLocalBounds().height/2)));
		texture.setOrigin(sf::Vector2f(texture.getLocalBounds().width / 2, texture.getLocalBounds().height / 2));
	}

}

float unit::getWeight() const
{
	return actualWeight;
}

void unit::setWeight(float newWeight)
{
	actualWeight = newWeight;
}

int unit::getID() const
{
	return ID;
}

float unit::collides(const unit &collider, sf::Vector2f *evadePos)
{
	for (size_t i = 0; i < body.size(); i++)
	{
		for (size_t o = 0; o < collider.body.size(); o++)
		{
			//std::cout << body[i].first.x << ", " << body[i].first.y << std::endl;
			//std::cout << collider.body[o].first.x << ", " << collider.body[o].first.y << std::endl;

			float xDis = body[i].first.x - collider.body[o].first.x;
			float yDis = body[i].first.y - collider.body[o].first.y;
			float xSq = xDis * xDis;
			float ySq = yDis * yDis;

			float temp = sqrt(xSq + ySq);

			//cout << temp << ", " << (body[i].second + collider.body[o].second) << std::endl;
			if (temp < (body[i].second + collider.body[o].second)) {
				if (evadePos != nullptr) {
					*evadePos = body[i].first - collider.body[i].first;
				}
				return (body[i].second + collider.body[o].second) - temp;
			}
		}
	}
	return -1;
}


unit::~unit()
{
	delete Amodule;
	delete Dmodule;
}

sf::Vector2f rotateByAngle(sf::Vector2f o, sf::Vector2f p, float angle)
{
	float s = sin(angle * (M_PI/180));
	float c = cos(angle * (M_PI/180));;

	// translate point back to origin:
	p.x -= o.x;
	p.y -= o.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + o.x;
	p.y = ynew + o.y;
	return p;
}

float getAngleInDegrees(sf::Vector2f unitVec)
{
	return std::atan2(unitVec.x, -unitVec.y) * 180 / M_PI;
}

sf::Vector2f getUnitVec(sf::Vector2f pos1, sf::Vector2f pos2)
{
	sf::Vector2f dist = sf::Vector2f(pos2 - pos1);
	float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
	//std::cout << newUnit.getBody()[0].first.x << ", " << newUnit.getBody()[0].first.y << std::endl;
	return sf::Vector2f(sf::Vector2f(dist.x / mag, dist.y / mag));
}


float distance(sf::Vector2f v, sf::Vector2f w)
{
	return std::sqrt(pow(v.x - w.x, 2) + pow(v.y - w.y, 2));
}
