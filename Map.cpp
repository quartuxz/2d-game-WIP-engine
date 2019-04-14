#include "Map.h"
#include <iostream>



Map::Map()
{
}

void Map::update(std::vector<unit*> units)
{
	for (size_t i = 0; i < units.size(); i++)
	{
		if (units[i]->isDead) {
			continue;
		}
		update(units[i]);
	}
}

void Map::update(unit *units)
{
	for (size_t o = 0; o < walls.size(); o++)
	{
		auto body = units->getBody();
		for (size_t p = 0; p < body.size(); p++)
		{
			sf::Vector2f proj;
			float dist = minimum_distance(walls[o].wall.first, walls[o].wall.second, body[p].first, &proj);
			//this if checks for collision with wall
			if (dist < body[p].second) {
				//this if checks if the object is on the correct side of the wall, else, it transports it to the correct side upon collsiion
				if (length_squared(walls[o].inside, body[p].first) < length_squared(walls[o].getOutside(), body[p].first)) {
					float dist2 = sqrt(pow(body[p].first.x - proj.x, 2) + pow(body[p].first.y - proj.y, 2));
					units->move(sf::Vector2f(((body[p].first.x - proj.x) / dist2) * (body[p].second - dist), ((body[p].first.y - proj.y) / dist2) * (body[p].second - dist)));
					//units->hitsWall(sf::Vector2f(((body[p].first.x - proj.x) / dist2) * (body[p].second - dist), ((body[p].first.y - proj.y) / dist2) * (body[p].second - dist)));
				}
				else {
					sf::Vector2f reflected = reflect(walls[o].wall.first, walls[o].wall.second, body[p].first);
					units->move(reflected - body[p].first);
					//units->hitsWall(reflected - body[p].first);
				}

			}
		}
	}
}

bool Map::collides(unit *units)
{
	for (size_t o = 0; o < walls.size(); o++)
	{
		auto body = units->getBody();
		for (size_t p = 0; p < body.size(); p++)
		{
			sf::Vector2f proj;
			float dist = minimum_distance(walls[o].wall.first, walls[o].wall.second, body[p].first, &proj);
			if (dist < body[p].second) {
				return true;
			}
		}
	}
	return false;
}

void Map::addWall(sf::Vector2f first, sf::Vector2f second, bool clockWiseFromFirst)
{
	Wall wall;
	wall.wall = std::pair<sf::Vector2f, sf::Vector2f>(first, second);
	wall.inside = rotate90(sf::Vector2f((first.x + second.x) / 2, (first.y + second.y) / 2), first);

	float dist = distance(wall.inside - (sf::Vector2f((first.x + second.x) / 2, (first.y + second.y) / 2)), sf::Vector2f((first.x + second.x) / 2, (first.y + second.y) / 2));
	std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	wall.inside.x -= (first.x + second.x) / 2;
	wall.inside.y -= (first.y + second.y) / 2;
	std::cout << (first.x + second.x) / 2 << std::endl;
	std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	wall.inside.x = wall.inside.x / dist;
	wall.inside.y = wall.inside.y / dist;
	std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	wall.inside.x += (first.x + second.x) / 2;
	wall.inside.y += (first.y + second.y) / 2;
	std::cout << wall.inside.x << ", " << wall.inside.y << std::endl;
	walls.push_back(wall);
}


std::vector<Wall> Map::getWalls() const
{
	return walls;
}

Map::~Map()
{
}

float length_squared(sf::Vector2f v, sf::Vector2f w)
{
	return pow(v.x - w.x, 2) + pow(v.y - w.y, 2);
}

float dot(sf::Vector2f v, sf::Vector2f w)
{
	return v.x * w.x + v.y * w.y;
}

float minimum_distance(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p, sf::Vector2f *proj)
{
	// Return minimum distance between line segment vw and point p
	const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return distance(p, v);   // v == w case
											// Consider the line extending the segment, parameterized as v + t (w - v).
											// We find projection of point p onto the line. 
											// It falls where t = [(p-v) . (w-v)] / |w-v|^2
											// We clamp t from [0,1] to handle points outside the segment vw.
	const float t = std::max(static_cast<float>(0), std::min(static_cast<float>(1), dot((p - v), (w - v)) / l2));
	const sf::Vector2f projection = v + t * (w - v);  // Projection falls on the segment
	if (proj != nullptr) {
		*proj = projection;
	}
	return distance(p, projection);
}

sf::Vector2f reflect(sf::Vector2f v, sf::Vector2f w, sf::Vector2f p)
{
	float s = 0;
	float c = -1;

	sf::Vector2f proj;
	minimum_distance(v, w, p, &proj);

	// translate point back to origin:
	p.x -= proj.x;
	p.y -= proj.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + proj.x;
	p.y = ynew + proj.y;
	return p;
}

sf::Vector2f rotate90(sf::Vector2f o, sf::Vector2f p)
{
	float s = 1;
	float c = 0;

	// translate point back to origin:
	p.x -= o.x;
	p.y -= o.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + o.x;
	p.y = ynew + o.y;
	return p;
}

sf::Vector2f Wall::getOutside()
{
	return reflect(wall.first, wall.second, inside);
}
