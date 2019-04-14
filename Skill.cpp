#include "Skill.h"



Skill::Skill()
{
}

void Skill::setSkill(skillParam sParam)
{
	m_skill = sParam;
}

failCastTypes Skill::cast(unit *, castParam)
{
	return failCastTypes();
}

void Skill::update(unit *player, Weapon *playerWeapon, std::vector<unit*> AIunits, std::vector<Weapon*> AIWeapons)
{
}


Skill::~Skill()
{
}
