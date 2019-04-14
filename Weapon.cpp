#include "Weapon.h"
#include <iostream>
#include "Animator.h"

void Weapon::setBulletTex(sf::Sprite &circleTex)
{
	m_bulletTex = circleTex;
}

void Weapon::setbulletAnimatorTex(AnimatorSprite aSprite)
{
	m_bulletATex = aSprite;
}

sf::Vector2f Weapon::getUnitVecTo(sf::Vector2f posTo)
{
	return m_wieldingUnit->getUnitVecTo(posTo);
}

Weapon::Weapon(unit *wieldingUnit):
m_wieldingUnit(wieldingUnit)
{
}

void Weapon::m_fire(sf::Vector2f unitVec)
{
	std::cout << m_wieldingUnit->Amodule->bulletSpeed << std::endl;
	//system("pause");
	float r3 = -(m_wieldingUnit->Amodule->inaccuracy / 2) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((m_wieldingUnit->Amodule->inaccuracy / 2) - -(m_wieldingUnit->Amodule->inaccuracy / 2))));
	//float r3 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / Amodule->inaccuracy));
	//std::cout << r3 << std::endl;
	if (r3 < 0) {
		r3 += 360;
	}
	//std::cout << unitVec.x << ", " << unitVec.y << std::endl;
	unitVec = rotateByAngle(sf::Vector2f(0,0), unitVec, r3);
	//std::cout << unitVec.x << ", " << unitVec.y  << std::endl;
	m_bullets.push_back(new Bullet(m_wieldingUnit->getBody()[0].first + (unitVec * m_wieldingUnit->getBody()[0].second), unitVec, m_wieldingUnit->Amodule));
	m_bullets.back()->Amodule = m_wieldingUnit->Amodule;
	m_bullets.back()->Dmodule = m_wieldingUnit->Dmodule;
	m_bullets.back()->getBody()->setTexture(m_bulletTex);
	m_bullets.back()->getBody()->setAnimatorSprite(m_bulletATex);
	m_bullets.back()->getBody()->addVelocity(m_wieldingUnit->getVelocity());
}

void Weapon::fire(sf::Vector2f unitVec)
{
    if(canFire()){
		m_fire(unitVec);
		m_accumulatedFireTime = 0;
		m_magBulletsUsed++;
	}else if(m_magBulletsUsed >= m_wieldingUnit->Amodule->magSize){
        if(m_accumulatedFireTime >= m_wieldingUnit->Amodule->reloadTime){
            m_magBulletsUsed = 0;

        }
	}
}

std::vector<unsigned int> Weapon::update(float timeDelta, std::vector<unit*> units)
{
	std::vector<unsigned int> retVal;
	std::list<Bullet*>::iterator it = m_bullets.begin();
	while (it != m_bullets.end()) {
		(*it)->update(timeDelta*m_bulletSpeed);
		int tempID = (*it)->hits(units);
		if (tempID != -1) {
			retVal.push_back(tempID);
		}
		if ((*it)->getBody()->isDead) {
			delete Bullet::getAllBullets()[(*it)->getID()];
			m_bullets.erase(it++);
		}
		else {
			++it;
		}
	}
	m_accumulatedFireTime += timeDelta;
	return retVal;
}

std::vector<unsigned int> Weapon::update(float timeDelta, std::queue<unit*> units)
{
	std::vector<unit*> updateUnits;
	while(!units.empty()){
		updateUnits.push_back(units.front());
		units.pop();
	}
	return update(timeDelta, updateUnits);
}

std::list<Bullet*> Weapon::getBullets()const
{
	return m_bullets;
}

void Weapon::reload(){
    m_magBulletsUsed = m_wieldingUnit->Amodule->magSize;
}

void Weapon::displayToWindow(sf::RenderWindow &window)
{
	std::list<Bullet*>::iterator it;
	for (it = m_bullets.begin(); it != m_bullets.end(); ++it) {
		//window.draw((*it)->getBody()->getTexture());
		Animator::getInstance().addOneFrameSprite((*it)->getBody()->getAnimatorSprite());
	}
}


bool Weapon::canFire(){
    float fireCooldown = 1/ m_wieldingUnit->Amodule->fireRate;
	//m_accumulatedFireTime += fireTime;
	if ( (m_accumulatedFireTime > fireCooldown) && (m_magBulletsUsed < m_wieldingUnit->Amodule->magSize) ) {
        return true;
    }
    return false;
}

Weapon::~Weapon()
{
	std::list<Bullet*>::iterator it;
	for (it = m_bullets.begin(); it != m_bullets.end(); ++it) {
		delete (*it);
	}
}
