#include "GameMain.h"
#include <iostream>
#include <sstream>
#include "Animator.h"
#include "ToolTip.h"




void HUDMenu::createStaticMenuLayout()
{
	MenuItem tempMenuItem(sf::Rect<float>(sf::Vector2f(0 * m_window->getSize().x, 1 * m_window->getSize().y), sf::Vector2f(0.2 * m_window->getSize().x, 0.5 * m_window->getSize().y)));
	m_menuItems.push_back(tempMenuItem);

	//m_availablePotions.setTexture();
	m_updatePotions(0);
}

void HUDMenu::m_updatePotions(unsigned int availablePotions){
    sf::Text tempText;
    std::stringstream ss;
    m_availablePotions.clearText();
    ss << "available potions: " << availablePotions << std::endl;
    tempText.setString(ss.str());
    tempText.setPosition(sf::Vector2f(100,100));
    tempText.setScale(sf::Vector2f(1,1));
    tempText.setFillColor(sf::Color(0,0,0,255));
    m_availablePotions.addText(tempText);
}

void HUDMenu::onDraw(bool beforeDraw, sf::Vector2f viewDisplacement){
	if (beforeDraw) {
        sf::Vector2f initialPosition = m_availablePotions.getPosition();
        m_availablePotions.setPosition(initialPosition + viewDisplacement);
        m_availablePotions.draw(*m_window);
        m_availablePotions.setPosition(initialPosition);
	}
}

void HUDMenu::update(updateEvent uEvent)
{
	switch (uEvent.updateEventType)
	{
	case lostLife:
		m_menuItems[0].reestablishInitialPostion();
		//std::cout << "currentLife: " << uEvent.currentLife << ", maxLife: " << uEvent.maxLife << ", percentage: " << (uEvent.currentLife / uEvent.maxLife) << std::endl;
		m_menuItems[0].move(sf::Vector2f(0 ,-m_menuItems[0].getDimension().y * (uEvent.currentLife / uEvent.maxLife)));
		//m_menuItems[0].move(sf::Vector2f(0, (m_menuItems[0].getDimension().y/1) * (uEvent.currentLife/uEvent.maxLife)));
		break;
    case lostPotions:
        m_updatePotions(uEvent.availablePotions);
        break;
	default:
		break;
	}
}

void GameMain::m_setActiveLevel()
{


	m_currentLevel->createFromFile(m_gameLevels[m_activeLevel]);
	m_currentLevel->setProgressionFile(m_progressionFile);
	m_currentLevel->loadGearProgression();

    std::string line;
	std::ifstream fileRead(m_gameLevels[m_activeLevel]);
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

			if(tokens[0] == "interactable"){
                if(tokens[1] == "market"){
                    MarketMenu *tempMarketMenu = new MarketMenu(&m_window);
                    tempMarketMenu->addToolTips(m_currentLevel->getGear());
                    tempMarketMenu->createStaticMenuLayout();
                    m_currentLevel->addInteractable(tempMarketMenu, sf::Vector2f(std::atof(tokens[2].c_str()) , std::atof(tokens[3].c_str())));
			    }
			}
        }
    }
    fileRead.close();
}

void GameMain::spawnWindow(std::string fontFile)
{
	m_window.create(sf::VideoMode(1200, 1000), "SFML works!");
	m_window.setFramerateLimit(60);
	//Animator::getInstance().setWindow(&m_window);
	//Animator::getInstance().addTexture("player.png");
	m_gameMenus["HUD"] = new HUDMenu(&m_window);
	m_gameMenus["HUD"]->createStaticMenuLayout();
	Animator::getInstance().setWindow(&m_window);
	ToolTip::init(fontFile);
}

void GameMain::startLevel(unsigned int activeLevel)
{
	m_activeLevel = activeLevel;
	m_setActiveLevel();
	MarketMenu *tempMenu = new MarketMenu(&m_window);
    tempMenu->addToolTips(m_currentLevel->getGear());
	tempMenu->createStaticMenuLayout();
	m_currentLevel->addInteractable(tempMenu, sf::Vector2f(0,0));
}

void GameMain::m_create(std::vector<std::string> gameLevels)
{
	srand(static_cast <unsigned> (time(0)));
	m_gameLevels = gameLevels;
}

GameMain::GameMain(std::vector<std::string> gameLevels)
{
	m_create(gameLevels);
}

GameMain::GameMain(std::string fileName)
{
	std::string line;
	std::ifstream fileRead(fileName);
	std::vector<std::string> gameLevelNames;
	if (fileRead.is_open()) {
		while (getline(fileRead, line)) {
			std::vector<std::string> tokens;
			std::string tempLine = line;
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

			}
			else if (tokens[0] == "//") {
				continue;
			}
			gameLevelNames.push_back(tempLine);
		}
		m_create(gameLevelNames);
		std::cout << "READ FILE, SIZE: " << m_gameLevels.size() << std::endl;
		//system("pause");
		fileRead.close();
	}
}

void GameMain::createUIFromFile(std::string fileName)
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


			m_gameMenus[tokens[1]] = new Menu(&m_window);
			m_gameMenus[tokens[1]]->createMenuFromFile(tokens[0]);
		}
		fileRead.close();
	}
}

void GameMain::onProgramEnd()
{
	m_currentLevel->saveGearProgression();
}

void GameMain::setProgressionFile(std::string fileName)
{
	m_progressionFile = fileName;
}

void GameMain::gameLoop()
{
	MarketMenu tempMenu =  MarketMenu(&m_window);
	tempMenu.addToolTips(m_currentLevel->getGear());
	tempMenu.createStaticMenuLayout();
	m_window.setFramerateLimit(120);

	sf::Vector2f lastPlayerPos = m_currentLevel->getPlayer()->getPosition();
	sf::View view;

	view.setSize(sf::Vector2f(m_window.getSize()));
	view.setCenter(sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2));

	view.move(-sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2));
	view.move(m_currentLevel->getPlayer()->getBody()[0].first);

	m_viewDisplacement += -sf::Vector2f(m_window.getSize().x / 2, m_window.getSize().y / 2);
	m_viewDisplacement += m_currentLevel->getPlayer()->getBody()[0].first;
	sf::Vector2i mousePosition;


	sf::Time currentTime;

	sf::Image lastWindowState;

	while (m_window.isOpen())
	{
		mousePosition = sf::Mouse::getPosition(m_window);
		bool mouseClick = false;

		sf::Vector2f dist = sf::Vector2f(mousePosition + sf::Vector2i(m_viewDisplacement)) - (m_currentLevel->getPlayer()->getBody()[0].first);
		float mag = sqrt(pow(dist.x, 2) + pow(dist.y, 2));
		//std::cout << newUnit.getBody()[0].first.x << ", " << newUnit.getBody()[0].first.y << std::endl;
		sf::Vector2f mouseUnitVec = sf::Vector2f(dist.x / mag, dist.y / mag);

		lastPlayerPos = m_currentLevel->getPlayer()->getPosition();
		if (m_isPaused) {
			currentTime = sf::Time::Zero;
			m_clock.restart();
		}
		else {
			currentTime = m_clock.restart();
		}




		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			//std::cout << "w" << std::endl;
			m_currentLevel->getPlayer()->applyMoveSpeed(mouseUnitVec);
		}
		else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			std::cout << "a" << std::endl;
			if (mouseUnitVec.y > 0) {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
			}
			else {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
			}

		}
		else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			std::cout << "s" << std::endl;
			m_currentLevel->getPlayer()->applyMoveSpeed(sf::Vector2f(-mouseUnitVec.x, -mouseUnitVec.y));
		}
		else
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			std::cout << "d" << std::endl;
			if (mouseUnitVec.y > 0) {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), mouseUnitVec));
			}
			else {
				m_currentLevel->getPlayer()->applyMoveSpeed(rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), rotate90(sf::Vector2f(0, 0), mouseUnitVec))));
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			std::cout << "gearPickedUP!" << std::endl;
			m_currentLevel->pickUpGear();
		}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		    unit *tempUnit = m_currentLevel->getClosestAIUnit();
		    if(tempUnit != nullptr){
                m_currentLevel->getWeapon()->fire(m_currentLevel->getPlayer()->getUnitVecTo(tempUnit->getBody()[0].first));
		    }

		}

		else
		{
			//unitManager.getPlayer()->stopMovement();
		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//UnitManagerSingleton::getInstance().requestUpdate();
			m_currentLevel->getWeapon()->fire(mouseUnitVec);
			mouseClick = true;
		}





		sf::Event event;
		while (m_window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				m_window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					std::cout << "gearPickedUP!" << std::endl;
					m_currentLevel->pickUpGear();
				}if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					if (m_activeMenu != "mainMenu") {
						m_activeMenu = "mainMenu";
						//lastWindowState = m_window.capture();
					}
					else {
						m_activeMenu = "";
					}

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
					if (m_activeMenu != "inventory") {
						m_activeMenu = "inventory";
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
					if (m_activeMenu != "mainMenu") {
						Menu *tempMenu = m_currentLevel->interact();
						if (tempMenu != nullptr) {
							if (m_activeMenu == "interactable") {
								m_activeMenu = "";
							}
							else {
								m_activeMenu = "interactable";
								m_gameMenus[m_activeMenu] = tempMenu;
								//lastWindowState = m_window.capture();

							}
						}
					}

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
					//std::cout << "gearPickedUP!" << std::endl;
					m_currentLevel->setToolTipsShow(!m_currentLevel->areToolTipsShown());
				}
			}

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                //UnitManagerSingleton::getInstance().requestUpdate();
                m_currentLevel->drinkPotion();
            }

			if (event.type == sf::Event::KeyReleased) {
				m_currentLevel->getPlayer()->stopMovement();
			}
		}

        //std::cout << 1/currentTime.asSeconds() << std::endl;

		m_window.clear(sf::Color::White);

		if (m_isPaused) {
			sf::Texture tempTexture;
			tempTexture.loadFromImage(lastWindowState);
			sf::Sprite tempSprite;
			tempSprite.move(m_viewDisplacement);
			tempSprite.setTexture(tempTexture);
			sf::RectangleShape tempRect;
			tempRect.setFillColor(sf::Color(100,100,100, 100));
			tempRect.setSize(sf::Vector2f(m_window.getSize()));
			tempRect.move(m_viewDisplacement);
			m_window.draw(tempSprite);
			m_window.draw(tempRect);
		}
		m_isPaused = runOnce(currentTime.asSeconds() , mousePosition, mouseClick);


		view.move(m_currentLevel->getPlayer()->getPosition() - lastPlayerPos);
		m_viewDisplacement += (m_currentLevel->getPlayer()->getPosition() - lastPlayerPos);
		m_window.setView(view);

		//AnimatorSprite tempSprite = AnimatorSprite::AnimatorSprite(0);
		//tempSprite.scale = 10;
		//tempSprite.rotation = 60;

		//Animator::getInstance().addOneFrameSprite(tempSprite);

		Animator::getInstance().draw();

		//m_gameMenus["HUD"]->draw(m_viewDisplacement);


		m_window.display();
	}

}

void GameMain::updateUI(std::string UIName, sf::Vector2i mousePos, bool mouseClicked){
    updateEvent tempEvent;
    tempEvent.updateEventType = lostLife;
    tempEvent.currentLife = m_currentLevel->getPlayer()->Dmodule->hitPoints;
    tempEvent.maxLife = m_currentLevel->getPlayer()->Dmodule->hitPoinCap;
    m_gameMenus[UIName]->update(tempEvent);
    tempEvent.updateEventType = lostPotions;
    tempEvent.availablePotions = m_currentLevel->getHealthPotions();
    m_gameMenus[UIName]->update(tempEvent);

    m_gameMenus[UIName]->draw(m_viewDisplacement);

    std::vector<behaviourParameters> tempBehaviourParam = m_gameMenus[UIName]->onClick(sf::Vector2f(mousePos), mouseClicked);
    for (size_t i = 0; i < tempBehaviourParam.size(); i++)
    {
        switch (tempBehaviourParam[i].behaviourName)
        {
        case opensMenu:
            m_activeMenu = tempBehaviourParam[i].menuTo;

            break;
        case resumesGame:
            m_activeMenu = "";
            break;
        case buysItem:
            if (tempBehaviourParam[i].itemBought == "healthPotion" && mouseClicked) {
                if (m_currentLevel->subtractGold(tempBehaviourParam[i].goldCost)) {
                    m_currentLevel->addHealthPotions(1);
                }
            }
            break;
        case removesGearPiece:
            if (mouseClicked) {
                if (m_currentLevel->removeGearPiece(tempBehaviourParam[i].gearPieceRemoved)) {
                    m_currentLevel->addGold(tempBehaviourParam[i].goldCost);
                    m_currentLevel->assignPlayerGear(false);
                }
            }
        default:
            break;
        }
    }
}

bool GameMain::runOnce(float timeDelta, sf::Vector2i mousePos, bool mouseClicked)
{
	if (m_activeMenu.empty()) {
		m_currentLevel->update(timeDelta, m_window);
		if (m_currentLevel->hasLevelEnded() == playerDied) {
			m_currentLevel->setProgressionFile(m_progressionFile);
			m_currentLevel->saveGearProgression();
			delete m_currentLevel;
			m_currentLevel = new UnitManager();
			if (m_levelJump) {
				m_setActiveLevel();
				m_levelJump = false;
			}
			else {
				m_setActiveLevel();
			}

		}
        //TODO: move UI updates into their own function
		if (m_HUDActive) {
            updateUI("HUD", mousePos, mouseClicked);
		}

		return false;
	}
	else {

		updateUI(m_activeMenu, mousePos, mouseClicked);
		return true;
	}
}

//old UI update code
//m_gameMenus[m_activeMenu]->draw(m_viewDisplacement);
//		std::vector<behaviourParameters> tempBehaviourParam = m_gameMenus[m_activeMenu]->onClick(sf::Vector2f(mousePos), mouseClicked);
//		for (size_t i = 0; i < tempBehaviourParam.size(); i++)
//		{
//			switch (tempBehaviourParam[i].behaviourName)
//			{
//			case opensMenu:
//                m_activeMenu = tempBehaviourParam[i].menuTo;
//
//				break;
//			case resumesGame:
//                m_activeMenu = "";
//				break;
//			case buysItem:
//				if (tempBehaviourParam[i].itemBought == "healthPotion" && mouseClicked) {
//					if (m_currentLevel->subtractGold(tempBehaviourParam[i].goldCost)) {
//						m_currentLevel->addHealthPotions(1);
//					}
//				}
//				break;
//			case removesGearPiece:
//				if (mouseClicked) {
//					if (m_currentLevel->removeGearPiece(tempBehaviourParam[i].gearPieceRemoved)) {
//						m_currentLevel->addGold(tempBehaviourParam[i].goldCost);
//						m_currentLevel->assignPlayerGear(false);
//					}
//				}
//			default:
//				break;
//			}
//		}


GameMain::~GameMain()
{

	delete m_currentLevel;
}
