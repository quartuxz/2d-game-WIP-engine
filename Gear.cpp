#include "Gear.h"
#include <fstream>
#include "Animator.h"


Gear::Gear()
{
}

void Gear::addCard(AttackCard aCard, std::string gearPieceName)
{
	m_gearItems[gearPieceName].aModule.attackCards.push_back(aCard);
}

void Gear::addCard(DefenceCard dCard, std::string gearPieceName)
{
	m_gearItems[gearPieceName].dModule.defenceCards.push_back(dCard);
}

void Gear::saveGearToFile(std::string fileName)
{
	std::ofstream editFile(fileName, std::ios::trunc);

	std::map<std::string, GearPiece>::iterator it;
	for (it = m_gearItems.begin(); it != m_gearItems.end(); it++)
	{
		editFile << "gearPiece" << ";" << 0 << ";" << 0 << ";"
			<< it->first << ";"
			<< it->second.dModule.hitPoinCap << ";"
			<< it->second.dModule.moveSpeed << ";"
			<< it->second.aModule.bulletDuration << ";"
			<< it->second.aModule.bulletSpeed << ";"
			<< it->second.aModule.fireRate << ";"
			<< it->second.aModule.inaccuracy << ";"
			<< it->second.aModule.pushBackStrength << ";"
			<< it->second.aModule.bulletSize << ";"
			<< it->second.aModule.magSize << ";"
			<< it->second.aModule.reloadTime << ";"
            << it->second.aModule.igniteDamage << ";"
            << it->second.aModule.frozenAttackCardsIntroduced << ";"
            << it->second.aModule.moveSpeedReduction << ";"
			<< it->second.goldValue << ";"
			<< it->second.name << ";"
			<< Animator::getInstance().getTextureFileName(it->second.tex.textureID) << ";"
			<< it->second.tex.scale << ";";
		editFile << std::endl;
		for (size_t o = 0; o < it->second.aModule.attackCards.size(); o++)
		{
			editFile << "attackCard" << ";"
				<< it->first << ";"
				<< it->second.aModule.attackCards[o].damage << ";"
				<< it->second.aModule.attackCards[o].damageDoneToSelf << ";"
				<< it->second.aModule.attackCards[o].deletesAfterUse << ";"
				<< it->second.aModule.attackCards[o].pen << ";"
				<< it->second.aModule.attackCards[o].ignites << ";"
				<< it->second.aModule.attackCards[o].freezes << ";"
				<< it->second.aModule.attackCards[o].roots << ";";
			auto tempAttackbehaviours = it->second.aModule.attackCards[o].getAttackBehaviours();
			for (size_t i = 0; i < tempAttackbehaviours.size(); i++)
			{
				editFile << tempAttackbehaviours[i].name << ",;";
				//TODO: add if else statements for each beahviour, taking data from the attackBehaviours struct and writing it accordingly(as data is loaded only if needed,
				//i.e if behaviour A needs 3 ints and 5 ints are provided in the field the last two ints would not be loaded in to the struct object, regardless of wether the struct has and need 5 fields to be initialized)
			}
			editFile << std::endl;
		}
		size_t p = 0;
		for (p = 0; p < it->second.dModule.defenceCards.size(); p++)
		{
			editFile << "defenceCard" << ";"
				<< it->first << ";"
				<< it->second.dModule.defenceCards[p].addedDamage << ";"
				<< it->second.dModule.defenceCards[p].armor << ";"
				<< it->second.dModule.defenceCards[p].copiesCreatedAfterUse << ";"
				<< it->second.dModule.defenceCards[p].damageReduction << ";"
				<< it->second.dModule.defenceCards[p].deletesAfterUse << ";"
				<< it->second.dModule.defenceCards[p].duplicatesAttacks << ";"
				<< it->second.dModule.defenceCards[p].extraAttack << ";"
				<< it->second.dModule.defenceCards[p].healingOnHit << ";"
				<< it->second.dModule.defenceCards[p].isRemovable << ";"
				<< it->second.dModule.defenceCards[p].mitigatedDamage << ";"
				<< it->second.dModule.defenceCards[p].removedCardsAfterUse << ";";
			editFile << std::endl;
		}

	}
	editFile.close();
}

void Gear::addGearPiece(GearPiece gearPiece)
{
	m_gearItems[gearPiece.type] = gearPiece;
}

std::map<std::string, GearPiece> Gear::getGearItems() const
{
	return m_gearItems;
}

bool Gear::removeGearPiece(std::string gearPieceType)
{
	bool retVal;
	if (m_gearItems.find(gearPieceType) == m_gearItems.end()) {
		// not found
		retVal = false;
	}
	else {
		retVal = true;
		// found
	}
	m_gearItems.erase(gearPieceType);
	return retVal;

}


void Gear::assignGear(unit *aunit, bool heal)
{
	AttackModule *Amodule = new AttackModule(true);
	DefenceModule *Dmodule = new DefenceModule(true);

	std::map<std::string, GearPiece>::iterator it;
	for (it = m_gearItems.begin(); it != m_gearItems.end(); it++)
	{
		for (size_t o = 0; o < it->second.aModule.attackCards.size(); o++)
		{
			Amodule->attackCards.push_back(it->second.aModule.attackCards[o]);
		}
		for (size_t o = 0; o < it->second.dModule.defenceCards.size(); o++)
		{
			Dmodule->defenceCards.push_back(it->second.dModule.defenceCards[o]);
		}
		Amodule->bulletDuration += it->second.aModule.bulletDuration;
		Amodule->bulletSpeed += it->second.aModule.bulletSpeed;
		Amodule->fireRate += it->second.aModule.fireRate;
		Amodule->inaccuracy += it->second.aModule.inaccuracy;
		Amodule->pushBackStrength += it->second.aModule.pushBackStrength;
		Amodule->bulletSize += it->second.aModule.bulletSize;
        Amodule->magSize += it->second.aModule.magSize;
        Amodule->reloadTime += it->second.aModule.reloadTime;
        Amodule->fireRate += it->second.aModule.fireRate;

        Amodule->igniteDamage += it->second.aModule.igniteDamage;
        Amodule->frozenAttackCardsIntroduced += it->second.aModule.frozenAttackCardsIntroduced;
        Amodule->moveSpeedReduction += it->second.aModule.moveSpeedReduction;

		Dmodule->damageTakenPerSecond += it->second.dModule.damageTakenPerSecond;
		Dmodule->hitPoinCap += it->second.dModule.hitPoinCap;
		Dmodule->moveSpeed += it->second.dModule.moveSpeed;
	}
	if (heal) {
		Dmodule->hitPoints = Dmodule->hitPoinCap;
		Dmodule->damageTakenPerSecond = 0;
	}

	if (aunit->Amodule != nullptr) {
		delete aunit->Amodule;
	}
	if (aunit->Dmodule != nullptr) {
		if (!heal) {
			Dmodule->hitPoints = aunit->Dmodule->hitPoints;
			Dmodule->damageTakenPerSecond = aunit->Dmodule->damageTakenPerSecond;
		}

		delete aunit->Dmodule;
	}

	aunit->Amodule = Amodule;
	aunit->Dmodule = Dmodule;
}

Gear::~Gear()
{
}

GearPiece::~GearPiece()
{
}

GearPiece GearPiece::makeGearPiece(std::vector<std::string> tokens)
{

	DefenceModule Dmodule = DefenceModule(true);
	AttackModule Amodule = AttackModule(true);
	Dmodule.hitPoinCap = std::atof(tokens[4].c_str());
	Dmodule.hitPoints = 0;
	Dmodule.moveSpeed = std::atof(tokens[5].c_str());
	Amodule.bulletDuration = std::atof(tokens[6].c_str());
	Amodule.bulletSpeed = std::atof(tokens[7].c_str());
	Amodule.fireRate = std::atof(tokens[8].c_str());
	Amodule.inaccuracy = std::atof(tokens[9].c_str());
	Amodule.pushBackStrength = std::atof(tokens[10].c_str());
	Amodule.bulletSize =std::atof(tokens[11].c_str());
	Amodule.magSize = std::atof(tokens[12].c_str());
	Amodule.reloadTime = std::atof(tokens[13].c_str());

	Amodule.igniteDamage = std::atof(tokens[14].c_str());
	Amodule.frozenAttackCardsIntroduced = std::atoi(tokens[15].c_str());
	Amodule.moveSpeedReduction = std::atof(tokens[16].c_str());

	GearPiece tempGearPiece;
	tempGearPiece.aModule = Amodule;
	tempGearPiece.dModule = Dmodule;
	tempGearPiece.type = tokens[3];
	tempGearPiece.goldValue = std::atoi(tokens[17].c_str());

	if (tokens.size() > 18) {
		tempGearPiece.name = tokens[18];
	}
	if (tokens.size() > 19) {
		tempGearPiece.tex.textureID = Animator::getInstance().getTextureID(tokens[19]);
	}
	if (tokens.size() > 20) {
		tempGearPiece.tex.scale = std::atof(tokens[20].c_str());
	}
	return tempGearPiece;
}
