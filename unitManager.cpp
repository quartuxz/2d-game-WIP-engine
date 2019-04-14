#include "unitManager.h"
#include <iostream>
#include <map>
#include "Animator.h"


const float UnitManager::m_itemPickupCooldown = 2;


void MarketMenu::createStaticMenuLayout()
{
	//

	MenuItem tempMenuItem = createMenuItem(sf::Vector2f(0.15, 0.05), sf::Vector2f(0.1, 0.05));
	behaviourParameters tempBParam;
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "chestPiece";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.35, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "helmet";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.55, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "boots";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.75, 0.05), sf::Vector2f(0.1, 0.05));
	tempBParam.behaviourName = removesGearPiece;
	tempBParam.gearPieceRemoved = "weapon";
	tempBParam.goldCost = m_playerGearCost[tempBParam.gearPieceRemoved];
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);

	tempMenuItem = createMenuItem(sf::Vector2f(0.15, 0.7),sf::Vector2f(0.1, 0.1));
	tempBParam.behaviourName = buysItem;
	tempBParam.itemBought = "healthPotion";
	tempBParam.goldCost = 10;
	tempMenuItem.addBehviourParameters(tempBParam);
	addMenuItem(tempMenuItem);
}

void MarketMenu::onDraw(bool beforeDraw, sf::Vector2f viewDisplacement)
{
	if (beforeDraw) {
		for (size_t i = 0; i < m_playerGear.size(); i++)
		{
			sf::Vector2f initialPosition = m_playerGear[i].getPosition();
			m_playerGear[i].setPosition(initialPosition + viewDisplacement);
			m_playerGear[i].draw(*m_window);
			m_playerGear[i].setPosition(initialPosition);
		}
	}
}

void MarketMenu::addToolTips(Gear playerGear)
{

	m_playerGear.clear();

	AnimatorSprite tempASprite;
	tempASprite.textureID = Animator::getInstance().getTextureID("tooltip.png");
	tempASprite.originToCenter = false;
	tempASprite.scale = 6;

	sf::Vector2f scaleFactor =  getScaleFactor(sf::Vector2f(Animator::getInstance().getTexture(Animator::getInstance().getTextureID("tooltip.png"))->getSize()) * tempASprite.scale, sf::Vector2f(0.2, 0.4));

	ToolTip tempToolTip;



	tempToolTip.setScale(scaleFactor.x);
	tempToolTip.setTexture(tempASprite);

	auto gearItems = playerGear.getGearItems();
	std::map<std::string, GearPiece>::iterator it;
	for (it = gearItems.begin(); it != gearItems.end(); it++){
		tempToolTip.clearText();
		tempToolTip.makeTooltipForGear(it->second);
		m_playerGearCost[it->first] = it->second.goldValue;
		if (it->first == "chestPiece") {
			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.1, 0.15)));
		}

		if (it->first == "helmet") {
			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.3, 0.15)));
		}

		if (it->first == "boots") {
			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.5, 0.15)));
		}

		if (it->first == "weapon") {
			tempToolTip.setPosition(getPixelCoordinate(sf::Vector2f(0.7, 0.15)));
		}
		m_playerGear.push_back(tempToolTip);
	}
}

void MarketMenu::update(updateEvent)
{
}

void UnitManager::m_updateAIWeapons(std::vector<Weapon*> AIWeapons)
{
	for (size_t i = 0; i < AIWeapons.size(); i++)
	{
		std::list<Bullet*> AIWeaponBullets = AIWeapons[i]->getBullets();
		std::list<Bullet*>::iterator it2;
		for (it2 = AIWeaponBullets.begin(); it2 != AIWeaponBullets.end(); ++it2)
		{
			if (distance((*it2)->getBody()->getBody()[0].first, m_player->getBody()[0].first) < renderDistance) {
				if (m_map->collides((*it2)->getBody())) {

					(*it2)->onWallHit();
				}
			}
		}
	}
}

void UnitManager::m_updateAI(float timeDelta, std::vector<unit*> AIsSubset, std::vector<unit*> nonPlayerUnits)
{
	for (size_t i = 0; i < AIsSubset.size(); i++)
	{
		AIsSubset[i]->update(timeDelta, nonPlayerUnits);
		m_map->update(AIsSubset[i]);
	}
}

void UnitManager::m_updatePlayer(float timeDelta ,std::vector<unit*> nonPlayerUnits)
{
	m_player->update(timeDelta, nonPlayerUnits);
	m_map->update(m_player);
}

Gear UnitManager::getGear() const
{
	return m_gear;
}

void UnitManager::addHealthPotions(unsigned int amount)
{
	m_healthPotions += amount;
}

void UnitManager::addGold(unsigned int amount)
{
	m_gold += amount;
}

bool UnitManager::subtractGold(int amount)
{
	if (m_gold >= amount) {
		m_gold -= amount;
		return true;
	}
	return false;
}

bool UnitManager::removeGearPiece(std::string gearPieceName)
{
	return m_gear.removeGearPiece(gearPieceName);
}

void UnitManager::drinkPotion()
{
	if (m_healthPotions > 0) {
		m_player->Dmodule->hitPoints += 100;
		m_healthPotions--;
	}

}

void UnitManager::setToolTipsShow(bool toolTipsShown)
{
	m_showToolTips = toolTipsShown;
}

bool UnitManager::areToolTipsShown() const
{
	return m_showToolTips;
}

void UnitManager::removeToolTip(unsigned int ID)
{
	m_toolTips[ID].second = false;
}

void UnitManager::setProgressionFile(std::string fileName)
{
	m_progressionFileName = fileName;
}

void UnitManager::loadGearProgression()
{
	std::string line;
	std::ifstream fileRead(m_progressionFileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			std::vector<AttackCard*> tempGearPieceAttackCards;
			std::vector<DefenceCard*> tempGearPieceDefenceCards;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}

			if (tokens[0] == "bag") {
				m_healthPotions = std::atoi(tokens[1].c_str());
				m_gold = std::atoi(tokens[2].c_str());
			}

			if (tokens[0] == "attackCard") {
				AttackCard tempACard = AttackCard::makeAttackCard(tokens);
				for (size_t i = AttackCard::maxFields; i < tokens.size(); i++)
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
					//tempACard.addBehaviour(behaviour);
				}
				m_gear.addCard(tempACard, tokens[1]);
			}
			else
			if (tokens[0] == "defenceCard") {
				//std::cout << tokens[11] << std::endl;
				//system("pause");
				DefenceCard tempDCard = DefenceCard::makeDefenceCard(tokens);
				m_gear.addCard(tempDCard, tokens[1]);

			}else
			if (tokens[0] == "gearPiece") {
				GearPiece tempGearPiece = GearPiece::makeGearPiece(tokens);
				m_gear.addGearPiece(tempGearPiece);
			}
		}
		fileRead.close();
	}

	assignPlayerGear(true);

}

void UnitManager::saveGearProgression()
{
	m_gear.saveGearToFile(m_progressionFileName);
	std::ofstream editFile(m_progressionFileName, std::ios::app);
	editFile << "bag;" << m_healthPotions << ";" << m_gold << ";" << std::endl;
	editFile.close();
}

endLevelTypes UnitManager::hasLevelEnded() const
{
	return m_levelHasEnded;
}

void UnitManager::startLevel()
{
	assignPlayerGear();
	m_needsAnUpdate = true;
}

void UnitManager::endLevel()
{
	m_needsAnUpdate = false;
}

float UnitManager::getDistanceToPlayer(sf::Vector2f pos) const
{
	return distance(pos, m_player->getBody()[0].first);
}

void UnitManager::addInteractable(Menu *menu, sf::Vector2f pos)
{
	interactable tempInteractable;
	tempInteractable.menu = menu;
	tempInteractable.position = pos;
	m_interactables.push_back(tempInteractable);
}

Menu * UnitManager::interact()
{
	for (size_t i = 0; i < m_interactables.size(); i++)
	{
		if (distance(m_interactables[i].position, m_player->getBody()[0].first) < m_interactDistance) {
			return m_interactables[i].menu;
		}
	}
	return nullptr;
}

UnitManager::UnitManager()
{
}

UnitManager::UnitManager(Map *map):
	m_map(map)
{
}

void UnitManager::setMap(Map *map)
{
	m_map = map;
}

void UnitManager::setPlayerWeapon(Weapon *weapon)
{
	m_playerWeapon = weapon;
}

unsigned int UnitManager::getHealthPotions()const{
    return m_healthPotions;
}

unit *UnitManager::getClosestAIUnit()const{

    return m_closestAIUnit;
}

//TODO: create a layer render system, to combine effects over the player/bullet etc, integrate with worldTextures and animation
void UnitManager::update(float timeDelta, sf::RenderWindow &window)
{


    std::vector<unit*> killedUnits;
	if (m_player->isDead) {
		m_levelHasEnded = playerDied;
		return;
	}

	for (size_t i = 0; i < m_worldTextures.size(); i++)
	{
		window.draw(m_worldTextures[i]);
	}
	//std::cout << "bullet speed: " << m_player->Amodule->bulletSpeed << ", bullet duration: " << m_player->Amodule->bulletDuration << ", fire rate: " << m_player->Amodule->fireRate << std::endl;

	std::vector<std::thread*> threads;
	int numberOfThreads = 1;

	std::vector<unit*> nonPlayerUnits;
	std::vector<Weapon*> AIWeapons;
	std::vector<std::vector<unit*>> nonPlayerUnitsThreaded;
	nonPlayerUnitsThreaded.resize(numberOfThreads);
	std::vector<std::vector<Weapon*>> AIWeaponsThreaded;
	AIWeaponsThreaded.resize(numberOfThreads);

    float minDistance = std::numeric_limits<float>::max();


	for (size_t i = 0; i < m_AIs.size(); i++)
	{
	    float tempDistance = distance(m_AIs[i]->getUnit()->getBody()[0].first, m_player->getBody()[0].first);
		if ( (tempDistance < renderDistance) && !m_AIs[i]->getUnit()->isDead) {
            if(minDistance > tempDistance){
                m_closestAIUnit = m_AIs[i]->getUnit();
                minDistance = tempDistance;
            }
			m_AIs[i]->update(timeDelta, std::vector<unit*>(1, m_player));

			m_AIs[i]->drawBullets(window);
			//window.draw(m_AIs[i]->getUnit()->getTexture());
			nonPlayerUnitsThreaded[i%numberOfThreads].push_back(m_AIs[i]->getUnit());
			nonPlayerUnits.push_back(m_AIs[i]->getUnit());
			AIWeaponsThreaded[i%numberOfThreads].push_back(m_AIs[i]->getWeapon());
			AIWeapons.push_back(m_AIs[i]->getWeapon());
		}else if(m_AIs[i]->getUnit()->recentlyDead && m_AIs[i]->getUnit()->isDead){
            m_AIs[i]->getUnit()->recentlyDead = false;
            killedUnits.push_back(m_AIs[i]->getUnit());
        }
	}

	//std::thread tempThread(&UnitManager::m_updateAI, this, timeDelta, nonPlayerUnitsThreaded[0], nonPlayerUnits);

	for (size_t i = 0; i < numberOfThreads; i++)
	{
		//threads.push_back(new std::thread(&UnitManager::m_updateAI, this, timeDelta, nonPlayerUnitsThreaded[i], nonPlayerUnits));
		//threads.push_back(new std::thread(&UnitManager::m_updateAIWeapons, this, AIWeaponsThreaded[i]));
	}



	for (size_t i = 0; i < threads.size(); i++)
	{
		//threads[i]->join();
	}

	m_updateAI(timeDelta, nonPlayerUnits, nonPlayerUnits);
    for (size_t i = 0; i < m_AIs.size(); i++){

    }
	m_updateAIWeapons(AIWeapons);
	m_updatePlayer(timeDelta, nonPlayerUnits);

    m_playerWeapon->update(timeDelta, nonPlayerUnits);



	for (size_t i = 0; i < killedUnits.size(); i++)
	{
		while (true) {
			if (m_lootTable.empty()) {
				break;
			}
			unsigned int lootIndex = (rand() % m_lootTable.size());
			if (m_lootTable[lootIndex].second > rand()%10000) {
				placeGearOnMap(killedUnits[i]->lastLastPos, m_lootTable[lootIndex].first);
				break;
			}
		}
	}
	std::list<Bullet*> playerWeaponBullets = m_playerWeapon->getBullets();
	std::list<Bullet*>::iterator it2;
	for (it2 = playerWeaponBullets.begin(); it2 != playerWeaponBullets.end(); ++it2)
	{
		if (distance((*it2)->getBody()->getBody()[0].first, m_player->getBody()[0].first) < renderDistance) {
			if (m_map->collides((*it2)->getBody())) {
				(*it2)->onWallHit();
			}
		}
	}

	//tempThread.join();

	for (size_t i = 0; i < threads.size(); i++)
	{
		//threads[i]->join();
	}


	for (size_t i = 0; i < nonPlayerUnits.size(); i++)
	{
		Animator::getInstance().addOneFrameSprite(nonPlayerUnits[i]->getAnimatorSprite());
		//window.draw(nonPlayerUnits[i]->getTexture());
	}
	//window.draw(m_player->getTexture());
	Animator::getInstance().addOneFrameSprite(m_player->getAnimatorSprite());
	m_playerWeapon->displayToWindow(window);
	for (size_t i = 0; i < m_mapGearPieces.size(); i++)
	{
		//m_mapGearPieces[i].second.tex.textureID = Animator::getInstance().getTextureID("chestpiece.png");
		m_mapGearPieces[i].second.tex.drawLayer = 0;
		m_mapGearPieces[i].second.tex.position = m_mapGearPieces[i].first;
		m_mapGearPieces[i].second.tex.scale = 0.5;
		Animator::getInstance().addOneFrameSprite(m_mapGearPieces[i].second.tex);
	}

	if (m_showToolTips) {
		for (size_t i = 0; i < m_toolTips.size(); i++)
		{
			if (m_toolTips[i].second && (distance(m_toolTips[i].first->getPosition(), m_player->getBody()[0].first) < toolTipReadDistance)) {
				Animator::getInstance().addOneFrameSprite(m_toolTips[i].first);
			}
		}
	}


	////map updating

}

unsigned int UnitManager::addToolTip(ToolTip *toolTip)
{
	m_toolTips.push_back(std::pair<ToolTip*, bool>(toolTip, true));
	return (m_toolTips.size() - 1);
}

void UnitManager::loadLootTable(std::string fileName)
{
	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}
			if (tokens[0] == "attackCard") {
				AttackCard tempACard = AttackCard::makeAttackCard(tokens);
				m_lootTable[std::atoi(tokens[1].c_str())].first.aModule.attackCards.push_back(tempACard);

			}
			else
			if (tokens[0] == "defenceCard") {
				std::cout << tokens[11] << std::endl;
				//system("pause");
				DefenceCard tempDCard = DefenceCard::makeDefenceCard(tokens);
				m_lootTable[std::atoi(tokens[1].c_str())].first.dModule.defenceCards.push_back(tempDCard);

			}
			else if (tokens[0] == "gearPiece") {

				GearPiece tempGearPiece = GearPiece::makeGearPiece(tokens);
				m_lootTable.push_back(std::pair<GearPiece, unsigned int>(tempGearPiece, std::atoi(tokens[GearPiece::maxFields].c_str())));
			}
		}
		fileRead.close();
	}

}

Weapon * UnitManager::getWeapon()
{
	return m_playerWeapon;
}

unit * UnitManager::getPlayer()
{
	return m_player;
}

void UnitManager::placeGearOnMap(sf::Vector2f pos, GearPiece gearPiece)
{
	gearPiece.tex.rotation = rand() % 360;
	m_mapGearPieces.push_back(std::pair<sf::Vector2f, GearPiece>(pos, gearPiece));
	ToolTip *tempToolTip = new ToolTip();
	AnimatorSprite tempAnimatorSprite;
	tempAnimatorSprite.textureID = Animator::getInstance().getTextureID("tooltip.png");
	tempAnimatorSprite.scale = 5;
	tempAnimatorSprite.originToCenter = false;
	tempToolTip->setTexture(tempAnimatorSprite);
	tempToolTip->makeTooltipForGear(gearPiece);
	tempToolTip->setPosition(pos);
	m_itemToolTipID[gearPiece] = addToolTip(tempToolTip);
	//m_toolTips.back().first.position.y += 20;
}

void UnitManager::pickUpGear()
{
	if (m_itemPickupClock.getElapsedTime().asSeconds() > m_itemPickupCooldown) {
		for (size_t i = 0; i < m_mapGearPieces.size(); i++)
		{
			std::cout << "size of gear: " << m_mapGearPieces.size() << std::endl;
			if (distance(m_mapGearPieces[i].first, m_player->getBody()[0].first) < m_pickUpDistance) {
				m_mapGearPieces[i].second.tex.rotation = 0;
				addPlayerGear(m_mapGearPieces[i].second);
				m_toolTips[m_itemToolTipID[m_mapGearPieces[i].second]].second = false;
				m_mapGearPieces.erase(m_mapGearPieces.begin() + i);
				break;
			}
		}
		m_itemPickupClock.restart();
	}
}

void UnitManager::assignPlayerGear(bool heal)
{
	m_gear.assignGear(m_player, heal);
	std::cout << "bullet speed: " << m_player->Amodule->bulletSpeed << ", bullet duration: " << m_player->Amodule->bulletDuration << ", fire rate: " << m_player->Amodule->fireRate << std::endl;
}

void UnitManager::addPlayerGear(GearPiece gearPiece)
{
	m_gear.addGearPiece(gearPiece);
	assignPlayerGear(gearPiece.healsOnPickup);
}

void UnitManager::addAI(EnemyAI *newAI)
{
	m_AIs.push_back(newAI);
}

void UnitManager::setPlayer(unit *playerUnit)
{
	m_player = playerUnit;
}

void UnitManager::createFromFile(std::string fileName)
{
	m_map = new Map();

	std::string line;
	std::ifstream fileRead(fileName);
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;

			std::string tempLine;

			std::vector<AttackCard*> tempGearPieceAttackCards;
			std::vector<DefenceCard*> tempGearPieceDefenceCards;

			if (tokens.empty()) {
				std::string delimiter = ";";

				size_t pos = 0;
				pos = line.find(delimiter);
				while (pos != std::string::npos) {
					tokens.push_back(line.substr(0, pos));
					line.erase(0, pos + delimiter.length());
					pos = line.find(delimiter);
				}
			}

			if (tokens.empty()) {
				continue;
			}
			else if (tokens[0] == "//") {
				continue;
			}

			if (tokens[0] == "wall") {
				//std::cout << tokens.size() << std::endl;
				//system("pause");
				m_map->addWall(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())), (sf::Vector2f(std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str()))));
			}else
			if (tokens[0] == "unit") {
				EnemyAI *tempAI = new EnemyAI();
                std::vector<std::pair<sf::Vector2f, float>> unitBody;
				unitBody.push_back(std::pair<sf::Vector2f, float>(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())), std::atof(tokens[3].c_str())));
				unit *tempUnit = new unit(unitBody);
				tempUnit->typeOfUnit = defaultType;
				tempUnit->setWeight(std::atof(tokens[4].c_str()));

				tempAI->setUnit(tempUnit);
				m_AIs.push_back(tempAI);
				if (tokens.size() > 5) {
					AnimatorSprite tempASprtie;
					tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[5]);

					tempAI->getUnit()->setAnimatorSprite(tempASprtie);
				}
				if(tokens.size() > 6){
                    tempAI->createFromFile(tokens[6]);
				}
				if(tokens.size() > 7){
                    tempAI->setHead(m_AIs[(m_AIs.size() -1) - std::atoi(tokens[7].c_str())]->getUnit(), sf::Vector2f(std::atof(tokens[8].c_str()), std::atof(tokens[9].c_str())));
				}

			}
			else if (tokens[0] == "tooltip") {
				ToolTip *toolTip = new ToolTip();
				AnimatorSprite tempASprite;
				tempASprite.textureID = Animator::getInstance().getTextureID(tokens[1]);
				toolTip->setTexture(tempASprite);
				toolTip->setPosition(sf::Vector2f(std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str())));
				toolTip->setScale(std::atof(tokens[4].c_str()));
				addToolTip(toolTip);

			}
			else
			if (tokens[0] == "player") {
				std::vector<std::pair<sf::Vector2f, float>> unitBody;
				unitBody.push_back(std::pair<sf::Vector2f, float>(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())), std::atof(tokens[3].c_str())));
				m_player = new unit(unitBody);
				m_player->typeOfUnit = playerType;
				m_player->setWeight(std::atof(tokens[4].c_str()));
				if (tokens.size() > 5) {
					AnimatorSprite tempASprtie;
					tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[5]);

					m_player->setAnimatorSprite(tempASprtie);
				}

			}else
			if (tokens[0] == "bulletTexture") {
				AnimatorSprite tempASprtie;
				tempASprtie.textureID = Animator::getInstance().getTextureID(tokens[1]);

				for (size_t i = 2; i < tokens.size(); i++)
				{
					if (std::atoi(tokens[i].c_str()) == -1) {
						m_playerWeapon->setbulletAnimatorTex(tempASprtie);
					}
					else {
						m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[i].c_str())]->getWeapon()->setbulletAnimatorTex(tempASprtie);
					}
				}


			}
			else
			if (tokens[0] == "gear") {
				unit *tempUnit;
				if (std::atoi(tokens[1].c_str()) == -1) {
					tempUnit = m_player;
				}
				else {
					tempUnit = m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->getUnit();
				}

				GearPiece tempGearPiece = GearPiece::makeGearPiece(tokens);
				tempGearPiece.dModule.hitPoints = tempGearPiece.dModule.hitPoinCap;

				tempUnit->Dmodule = new DefenceModule(tempGearPiece.dModule);
				tempUnit->Amodule = new AttackModule(tempGearPiece.aModule);

				if(std::atoi(tokens[1].c_str()) == -1) {
					m_playerWeapon = new Weapon(tempUnit);
				}else{
					m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->setWeapon(new Weapon(tempUnit));
				}


				for (size_t i = GearPiece::maxFields; i < tokens.size(); i++)
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

					if (behaviour.empty()) {
						continue;
					}

					if (behaviour[0] == "addList") {
						for (size_t i = 1; i < behaviour.size(); i++)
						{
							if (std::atoi(behaviour[i].c_str()) == -1) {
								tempUnit = m_player;
							}
							else {
								tempUnit = m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->getUnit();
							}

							tempUnit->Dmodule = new DefenceModule(tempGearPiece.dModule);
							tempUnit->Amodule = new AttackModule(tempGearPiece.aModule);


							if (std::atoi(behaviour[i].c_str()) == -1) {
								m_playerWeapon = new Weapon(tempUnit);
							}
							else {
								m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->setWeapon(new Weapon(tempUnit));
							}
						}
					}
				}


			}else
			if (tokens[0] == "attackCard") {
				AttackCard tempACard = AttackCard::makeAttackCard(tokens);
				for (size_t i = AttackCard::maxFields; i < tokens.size(); i++)
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
						for (size_t i = 1; i < behaviour.size(); i++)
						{
							if (std::atoi(behaviour[i].c_str()) == -1) {
								m_player->Amodule->attackCards.push_back(tempACard);
							}
							else if (std::atoi(behaviour[i].c_str()) < -1) {
								int tempIndex = abs(std::atoi(behaviour[i].c_str())) - 2;
								m_mapGearPieces[tempIndex].second.aModule.attackCards.push_back(tempACard);
							}
							else {
								m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->getUnit()->Amodule->attackCards.push_back(tempACard);
							}
						}
					}
				}
				if (std::atoi(tokens[1].c_str()) == -1) {
					m_player->Amodule->attackCards.push_back(tempACard);
				}
				else if (std::atoi(tokens[1].c_str()) < -1) {
					m_mapGearPieces[abs(std::atoi(tokens[1].c_str()))-2].second.aModule.attackCards.push_back(tempACard);
				}
				else {
					m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->getUnit()->Amodule->attackCards.push_back(tempACard);
				}

			}else
			if (tokens[0] == "defenceCard") {
				std::cout << tokens[11] << std::endl;
				//system("pause");
				DefenceCard tempDCard = DefenceCard::makeDefenceCard(tokens);
				for (size_t i = DefenceCard::maxFields; i < tokens.size(); i++)
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
						for (size_t i = 1; i < behaviour.size(); i++)
						{
							if (std::atoi(behaviour[i].c_str()) == -1) {
								m_player->Dmodule->defenceCards.push_back(tempDCard);
							}
							else if (std::atoi(behaviour[i].c_str()) < -1) {
								m_mapGearPieces[abs(std::atoi(behaviour[i].c_str()))-2].second.dModule.defenceCards.push_back(tempDCard);
							}
							else {
								m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->getUnit()->Dmodule->defenceCards.push_back(tempDCard);
							}
						}
					}
				}
				if (std::atoi(tokens[1].c_str()) == -1) {
					m_player->Dmodule->defenceCards.push_back(tempDCard);
				}
				else if (std::atoi(tokens[1].c_str()) < -1) {
					m_mapGearPieces[abs(std::atoi(tokens[1].c_str()))-2].second.dModule.defenceCards.push_back(tempDCard);
				}
				else {
					m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->getUnit()->Dmodule->defenceCards.push_back(tempDCard);
				}

			}else
			if (tokens[0] == "gearPiece") {

				GearPiece tempGearPiece = GearPiece::makeGearPiece(tokens);
				placeGearOnMap(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())), tempGearPiece);
			}else
			if (tokens[0] == "texture") {
				sf::Texture *tempTexture = new sf::Texture();
				tempTexture->loadFromFile(tokens[3]);
				m_toDeleteTextures.push_back(tempTexture);
				sf::Sprite tempSprite;
				tempSprite.setTexture(*tempTexture);
				//if the 3rd value equals 0 then set the textures for the units themselves
				if (std::atoi(tokens[2].c_str()) == 0) {
					if (std::atoi(tokens[1].c_str()) == -1) {
						m_player->setTexture(tempSprite, true);
					}
					else {
						m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->getUnit()->setTexture(tempSprite,true);
					}
				}
				//else, set them for the bullets
				else{

					if (std::atoi(tokens[1].c_str()) == -1) {
						m_playerWeapon->setBulletTex(tempSprite);
					}
					else {
						m_AIs[(m_AIs.size() - 1) - std::atoi(tokens[1].c_str())]->getWeapon()->setBulletTex(tempSprite);;
					}
				}

				for (size_t i = 4; i < tokens.size(); i++)
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
						for (size_t i = 1; i < behaviour.size(); i++)
						{
							if (std::atoi(tokens[2].c_str()) == 0) {
								if (std::atoi(behaviour[i].c_str()) == -1) {
									m_player->setTexture(tempSprite, true);
								}
								else {
									m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->getUnit()->setTexture(tempSprite, true);
								}
							}
							else {
								if (std::atoi(behaviour[i].c_str()) == -1) {
									m_playerWeapon->setBulletTex(tempSprite);
								}
								else {
									m_AIs[(m_AIs.size() - 1) - std::atoi(behaviour[i].c_str())]->getWeapon()->setBulletTex(tempSprite);;
								}
							}

						}
					}
				}



			}else
			if (tokens[0] == "worldTextureTile") {
				sf::Texture *tempTexture = new sf::Texture();
				tempTexture->loadFromFile(tokens[7]);
				m_toDeleteTextures.push_back(tempTexture);
				sf::Sprite tempSprite;
				tempSprite.setTexture(*tempTexture);
				if (std::atof(tokens[1].c_str())< 0) {
					tempSprite.setOrigin(sf::Vector2f(tempSprite.getLocalBounds().width/2, tempSprite.getLocalBounds().height/2));
				}
				else {
					tempSprite.setOrigin(sf::Vector2f(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str())));
				}

				tempSprite.setPosition(sf::Vector2f(std::atof(tokens[3].c_str()), std::atof(tokens[4].c_str())));
				tempSprite.setRotation(std::atof(tokens[5].c_str()));
				tempSprite.scale(std::atof(tokens[6].c_str()), std::atof(tokens[6].c_str()));
				m_worldTextures.push_back(tempSprite);
			}
			else if (tokens[0] == "animatorTexture") {
				Animator::getInstance().addTexture(tokens[1]);
			}
			else if (tokens[0] == "loadLootTable") {
				loadLootTable(tokens[1]);
			}
		}
		fileRead.close();
		//m_player->getTexture().setTexture();
	}

}

UnitManager::~UnitManager()
{
	for (size_t i = 0; i < m_AIs.size(); i++)
	{
		delete m_AIs[i];
	}
	for (size_t i = 0; i < m_toDeleteACards.size(); i++)
	{
		delete m_toDeleteACards[i];
	}
	for (size_t i = 0; i < m_toDeleteDCards.size(); i++)
	{
		delete m_toDeleteDCards[i];
	}
	for (size_t i = 0; i < m_toDeleteTextures.size(); i++)
	{
		delete m_toDeleteTextures[i];
	}
	for (size_t i = 0; i < m_interactables.size(); i++)
	{
		delete m_interactables[i].menu;
	}
	for (size_t i = 0; i < m_toolTips.size(); i++)
	{
		delete m_toolTips[i].first;
	}
}
