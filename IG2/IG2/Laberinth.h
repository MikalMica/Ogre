#pragma once
#include "IG2Object.h"
#include <OgreTrays.h>
#include "AliveEntity.h"

//<3
class Hero;
class EnemyBase;

class Laberinth : public IG2Object
{
	Ogre::SceneNode* mLightNode = nullptr;

	Hero* hero;
	double wallSize;
	vector<vector<bool>> map;
	vector<EnemyBase*> enemies;
	OgreBites::TextBox* info;
	std::string floorMat;
	std::string wallMat;
public:
	Laberinth(Vector3 initPos, SceneNode* node, SceneManager* sceneMng, Hero* h, OgreBites::TextBox* text, std::string Fmat = "FloorMat", std::string Wmat = "WallMat")
		: IG2Object(initPos, node, sceneMng), hero(h), wallSize(0), floorMat(Fmat), wallMat(Wmat), info(text)
	{
	};
	~Laberinth() { delete info; info = nullptr; }

	void GenerateLaberinth(std::string in);
	bool isPositionWalkable(float fil, float col);
	bool isPositionWalkable(Vector2 pos);
	inline vector<EnemyBase*> getEnemies() { return enemies; }
	inline double getWallSize() { return wallSize; }
	bool checkCollisions(AliveEntity& entity, bool isHero);
	void resetGame();
	void updateInfoText();
	double normalizeSize(Vector3 const& sizeBox);
	void correctPosition(AliveEntity& entity);
	Vector2 getLaberinthPosition(Vector2 const& pos);
};

