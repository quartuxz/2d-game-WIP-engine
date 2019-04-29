#include "AttackAndDefence.h"
#include <iostream>


AttackModule AttackModule::makeAttackModule(std::string)
{
	return AttackModule();
}

AttackModule::AttackModule()
{
}

AttackModule::AttackModule(bool)
{
	fireRate = 0;
	bulletDuration = 0;
	bulletSpeed = 0;
	inaccuracy = 0;
	pushBackStrength = 0;
	bulletSize = 0;
	magSize = 0;
	reloadTime = 0;
}


AttackModule::~AttackModule()
{
}


DefenceModule::DefenceModule()
{
}

DefenceModule::DefenceModule(bool)
{
	hitPoinCap = 0;
	hitPoints = 0;
	manaCap = 0;
	mana = 0;
	moveSpeed = 0;
	damageTakenPerSecond = 0;
}

void DefenceModule::processEffects(float timeDelta, AttackModule *aModule) {
	for (size_t i = 0; i < effects.size(); i++)
	{
		float minDuration = std::min(timeDelta, effects[i].duration);
		switch (effects[i].eType)
		{
		case staminaRegenEffect:
			stamina += effects[i].amount * minDuration;
			if (stamina > 100) {
				stamina = 100;
			}
			break;
		case damageOverTimeEffect:
			hitPoints -= effects[i].amount * minDuration;
			break;
		case rootEffect:
			if (!effects[i].active) {
				effects[i].originalAmount = moveSpeed;
				moveSpeed -= effects[i].amount;
				effects[i].active = true;
			}
			
			
			break;
		default:
			break;
		}
		if (aModule != nullptr) {

		}
		effects[i].duration -= timeDelta;
		if (effects[i].duration <= 0) {
			if (effects[i].eType == rootEffect) {
				moveSpeed = effects[i].originalAmount;
			}
			effects.erase(effects.begin() + i);
		}
	}
	
}

DefenceModule::~DefenceModule()
{
}

void AttackCard::onAttack(DefenceModule *module, AttackModule *amodule)
{
	//std::cout << module->hitPoints << ", " << damage << std::endl;
	module->hitPoints -= damage;
}

std::vector<attackBehaviours> AttackCard::getAttackBehaviours() const
{
	return m_aBehaviours;
}

void AttackCard::addBehaviour(std::vector<std::string> behaviour)
{
	if (behaviour[0] == "destroy") {

	}
	else if(behaviour[0] == "anhilate"){

	}
	//std::cout << behaviour[0].c_str() << std::endl;
	//std::cout << "BEHAVIOUR ADDED" << std::endl;
}

void AttackCard::attack(DefenceModule *module, AttackModule *amodule)
{
	DefenceCard temp;

	int initialDamage = damage;
	//std::cout << damage << std::endl;
	bool extraAttack = true;
	bool extraAttackDone = false;
	while (extraAttack) {
		int index = rand() % module->defenceCards.size();
		temp = module->defenceCards[index];
		extraAttack = false;
		if (pen >= temp.armor) {
			//damage logic:
			extraAttack = temp.extraAttack;
			damage += temp.addedDamage;
			damage -= temp.mitigatedDamage;
			if (damage < 0) {
				damage = 0;
			}
			damage *= temp.damageReduction;
			onAttack(module, amodule);
			if (temp.duplicatesAttacks) {
				onAttack(module, amodule);
			}

			//after use logic:
			for (size_t i = 0; i </* temp.defenceCardsInsertedAfterUse.first*/ 0; i++)
			{
				//module->defenceCards.push_back(temp.defenceCardsInsertedAfterUse.second);
			}

			for (size_t i = 0; i < temp.removedCardsAfterUse; i++)
			{
				int removableCardsLeft = module->defenceCards.size();
				for (size_t  o = 0; o < module->defenceCards.size(); o++)
				{

					if (module->defenceCards[i].isRemovable) {
						module->defenceCards.erase(module->defenceCards.begin() + i);
						break;
					}
					else {
						--removableCardsLeft;
					}
					if (removableCardsLeft <= 0) {
						goto stopLoop;
					}
				}
			}
			stopLoop:


			for (size_t i = 0; i < temp.copiesCreatedAfterUse; i++)
			{
				module->defenceCards.push_back(temp);
			}
			if (temp.deletesAfterUse) {
				module->defenceCards.erase(module->defenceCards.begin() + index);
			}
		}
		damage = initialDamage;
		if (extraAttackDone) {
			break;
		}
		if (extraAttack) {
			extraAttackDone = true;
		}
	}
	//std::cout << damage << std::endl;

}

AttackCard AttackCard::makeAttackCard(std::vector<std::string> tokens)
{
	AttackCard tempACard;
	tempACard.damage = std::atof(tokens[2].c_str());
	tempACard.damageDoneToSelf = std::atof(tokens[3].c_str());
	tempACard.deletesAfterUse = std::atoi(tokens[4].c_str());
	tempACard.pen = std::atoi(tokens[5].c_str());
	tempACard.ignites = std::atoi(tokens[6].c_str());
    tempACard.freezes = std::atoi(tokens[7].c_str());
    tempACard.roots = std::atoi(tokens[8].c_str());
	for (size_t i = maxFields; i < tokens.size(); i++)
	{
		std::vector<std::string> behaviour;

		std::string delimiter = ",";

		size_t pos = 0;
		pos = tokens[i].find(delimiter);
		while (pos != std::string::npos) {
			behaviour.push_back(tokens[i].substr(0, pos));
			tokens[i].erase(0, pos + delimiter.length());
			pos = tokens[i].find(delimiter);
		}

		if (behaviour[0] == "addList") {

		}
		else {
			tempACard.addBehaviour(behaviour);
		}
	}

	return tempACard;
}


void AttackModule::attack(DefenceModule *module, AttackModule *amodule, DefenceModule *ownModule, attackParameters aParams)
{
	module->pushes.push_back(std::pair<sf::Vector2f, float>(aParams.bulletDir, pushBackStrength));
	int index = rand() % attackCards.size();
	AttackCard temp = attackCards[index];
	if(temp.ignites){
		effect tempEffect;
		tempEffect.amount = igniteDamage;
		tempEffect.duration = 5;
        module->effects.push_back(tempEffect);
	}
	if(temp.freezes){
        AttackCard frozenAttackCard;
        frozenAttackCard.deletesAfterUse = true;
        frozenAttackCard.pen = 0;
        for(unsigned int i = 0; i < frozenAttackCardsIntroduced; i++){
            amodule->attackCards.push_back(frozenAttackCard);
        }
	}
	if(temp.roots){
		effect tempEffect;
		tempEffect.eType = rootEffect;
		tempEffect.amount = moveSpeedReduction;
		tempEffect.duration = 3;
        module->effects.push_back(tempEffect);
        
	}
	temp.attack(module, amodule);
	ownModule->hitPoints -= temp.damageDoneToSelf;
	if (temp.deletesAfterUse) {
		attackCards.erase(attackCards.begin() +  index);
	}
}

DefenceCard DefenceCard::makeDefenceCard(std::vector<std::string> tokens)
{
	DefenceCard tempDCard;
	tempDCard.addedDamage = std::atof(tokens[2].c_str());
	tempDCard.armor = std::atoi(tokens[3].c_str());
	tempDCard.copiesCreatedAfterUse = std::atoi(tokens[4].c_str());
	tempDCard.damageReduction = std::atof(tokens[5].c_str());
	tempDCard.deletesAfterUse = std::atoi(tokens[6].c_str());
	tempDCard.duplicatesAttacks = std::atoi(tokens[7].c_str());
	tempDCard.extraAttack = std::atoi(tokens[8].c_str());
	tempDCard.healingOnHit = std::atoi(tokens[9].c_str());
	tempDCard.isRemovable = std::atoi(tokens[10].c_str());
	tempDCard.mitigatedDamage = std::atof(tokens[11].c_str());
	tempDCard.removedCardsAfterUse = std::atoi(tokens[12].c_str());
	for (size_t i = maxFields; i < tokens.size(); i++)
	{
		std::vector<std::string> behaviour;

		std::string delimiter = ",";

		size_t pos = 0;
		pos = tokens[i].find(delimiter);
		while (pos != std::string::npos) {
			behaviour.push_back(tokens[i].substr(0, pos));
			tokens[i].erase(0, pos + delimiter.length());
			pos = tokens[i].find(delimiter);
		}
	}
	return tempDCard;
}
