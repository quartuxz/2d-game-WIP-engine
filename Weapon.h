#pragma once
#include <list>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include "unit.h"
#include "Bullet.h"

enum failCastTypes {
	outOfStamina, outOfMana, notFail
};

enum skillTypes {
	dash
};

struct skillParam {
	float castDelay = 0;
	sf::Vector2f dirUnitVec;
	skillTypes sType;
	float strenght = 3;
	float manaCost = 0;
	float staminaCost = 0;
};



class Weapon
{
private:
	sf::Sprite m_bulletTex;

	AnimatorSprite m_bulletATex;

	std::list<Bullet*> m_bullets;
	unit *m_wieldingUnit;
	float m_fireRate = 5;
	float m_bulletSpeed = 1;

    unsigned int m_magBulletsUsed = 0;

	void m_fire(sf::Vector2f);

	float m_accumulatedFireTime = 0;
public:

	void setBulletTex(sf::Sprite&);
	void setbulletAnimatorTex(AnimatorSprite);

    void reload();

    bool canFire();

	sf::Vector2f getUnitVecTo(sf::Vector2f);

	Weapon(unit*);

	void fire(sf::Vector2f);

	failCastTypes useSkill(skillParam);

	std::vector<unsigned int> update(float, std::vector<unit*>);
	std::vector<unsigned int> update(float, std::queue<unit*>);

	std::list<Bullet*> getBullets()const;
	void displayToWindow(sf::RenderWindow&);
	~Weapon();
};

