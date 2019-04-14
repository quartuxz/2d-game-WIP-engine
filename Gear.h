#pragma once
#include "AttackAndDefence.h"
#include "unit.h"
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

struct GearPiece {
	size_t ID = reinterpret_cast<size_t>(this);

	static constexpr unsigned int maxFields = 21;//plus 3 from aModule

	unsigned int goldValue = 10;
	std::string type;
	std::string name;
	AttackModule aModule;
	DefenceModule dModule;
	AnimatorSprite tex;
	AnimatorSprite toolTipTexture;
	std::string toolTipBackGroundTex;
	bool healsOnPickup = false;


	~GearPiece();

	static GearPiece makeGearPiece(std::vector<std::string>);

	struct GearPieceCompare
	{
		bool operator() (const GearPiece& lhs, const GearPiece& rhs) const
		{
			return lhs.ID < rhs.ID;
		}
	};

};



class Gear
{
private:
	std::map<std::string, GearPiece> m_gearItems;
public:
	Gear();
	void addCard(AttackCard, std::string);
	void addCard(DefenceCard, std::string);
	void saveGearToFile(std::string);
	void addGearPiece(GearPiece);

	std::map<std::string, GearPiece> getGearItems()const;
	bool removeGearPiece(std::string);

	void assignGear(unit*, bool heal = true);
	~Gear();
};

