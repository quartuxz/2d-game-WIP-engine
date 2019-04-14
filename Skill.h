#pragma once
#include <string>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "unit.h"


enum failCastTypes {
	outOfMana
};

struct castParam {
	sf::Vector2f dirUnitVec;
};

struct skillParam {
	std::string skillType;
};

class Skill
{
private:
	skillParam m_skill;
	std::queue<castParam>  m_casts;
public:
	Skill();
	void setSkill(skillParam);
	failCastTypes cast(unit*, castParam);
	void update(unit*, Weapon*, std::vector<unit*>, std::vector<Weapon*>);
	~Skill();
};

