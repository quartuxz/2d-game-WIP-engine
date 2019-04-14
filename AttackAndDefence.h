#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class DefenceModule;

class DefenceCard;

class DefenceCard {
protected:

public:
	float damageReduction = 1;
	int armor = 1;
	int healingOnHit = 0;
	int mitigatedDamage = 0;
	bool duplicatesAttacks = false;
	bool extraAttack = false;
	bool deletesAfterUse = false;
	int copiesCreatedAfterUse = 0;
	//std::pair<int, DefenceCard> defenceCardsInsertedAfterUse;
	bool isRemovable = false;
	int removedCardsAfterUse = 0;
	int addedDamage = 0;


	static constexpr unsigned int maxFields = 13;
	static DefenceCard makeDefenceCard(std::vector<std::string>);

};



class DefenceModule
{
public:
	float hitPoinCap = 100;
	float hitPoints = hitPoinCap;
	float moveSpeed;
	float damageTakenPerSecond;

	std::pair<sf::Vector2f, float> push;

	std::vector<DefenceCard> defenceCards;

	DefenceModule();
	DefenceModule(bool);
	~DefenceModule();
};

class AttackCard;


struct attackParameters {
	sf::Vector2f bulletDir = sf::Vector2f(0, 0);
};

class AttackModule
{
protected:

public:
	std::vector<AttackCard> attackCards;
	void attack(DefenceModule*, AttackModule*, DefenceModule*, attackParameters aParams = attackParameters());
	float fireRate = 0.1;
	float bulletDuration = 2;
	float bulletSpeed = 10;
	float inaccuracy = 20;
	float bulletSize = 10;
	float pushBackStrength = 0.05;
	unsigned int magSize = 10;
	float reloadTime = 3;

    float igniteDamage = 0;
    unsigned int frozenAttackCardsIntroduced = 0;
    float moveSpeedReduction = 0;

	static constexpr unsigned int maxFields = 13;//should be 13 with the three above
	static AttackModule makeAttackModule(std::string);

	AttackModule();
	AttackModule(bool);
	~AttackModule();

};


struct attackBehaviours {
	std::string name;
};

class AttackCard {
protected:
	void onAttack(DefenceModule*, AttackModule*);
	std::vector<attackBehaviours> m_aBehaviours;


public:
    bool ignites = false;
    bool freezes = false;
    bool roots = false;
	int pen = 2;
	float damage = 50;
	int damageDoneToSelf = 0;
	bool deletesAfterUse = false;
	std::vector<attackBehaviours> getAttackBehaviours()const;
	void addBehaviour(std::vector<std::string>);
	void attack(DefenceModule*, AttackModule*);
	static AttackCard makeAttackCard(std::vector<std::string>);

	static constexpr unsigned int maxFields = 9;//should be 9
};

