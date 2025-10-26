#include "Laberinth.h"
#include "Wall.h"
#include "NullNode.h"
#include "Hero.h"
#include "ComplexEnemy.h"
#include <fstream>

double 
Laberinth::normalizeSize(Vector3 const& sizeBox) {
	double max = std::max(sizeBox.x, std::max(sizeBox.y, sizeBox.z));
	return wallSize / max;
}


void 
Laberinth::GenerateLaberinth(std::string in)
{
	std::string txt = in + ".txt";
	ifstream stage(txt);

	if (!stage) return;

	int fils, cols;
	string lightType;

	stage >> fils >> cols >> wallMat >> floorMat >> lightType;

	mSM->setAmbientLight(ColourValue(0.25, 0.25, 0.25));
	Light* luz = mSM->createLight("Luz");
	luz->setDiffuseColour(1, 1, 1);
	
	if (lightType == "directional") {

		luz->setType(Ogre::Light::LT_DIRECTIONAL);

		mLightNode = mSM->getRootSceneNode()->createChildSceneNode("nLuz");
		mLightNode->attachObject(luz);
		mLightNode->setDirection(Ogre::Vector3(-1, -1, 10));
	}
	else if (lightType == "spotlight") {
		
		luz->setType(Ogre::Light::LT_SPOTLIGHT);

		luz->setSpotlightInnerAngle(Ogre::Degree(5.0f));
		luz->setSpotlightOuterAngle(Ogre::Degree(45.0f));
		luz->setSpotlightFalloff(1.0f);

		mLightNode = hero->createChildSceneNode();
		mLightNode->attachObject(luz);
		mLightNode->setDirection(Ogre::Vector3(0, -1, 0));
		mLightNode->setPosition(Vector3(0, 50, 0));
	}
	else if (lightType == "point") {
		luz->setType(Ogre::Light::LT_POINT);

		mLightNode = hero->createChildSceneNode();
		mLightNode->attachObject(luz);
	}

	map = vector<vector<bool>>(fils);
	for (vector<bool>& col : map) {
		col = vector<bool>(cols);
	}

	Wall* baby = new Wall(Vector3(0, 0, 0), createChildSceneNode(), mSM);
	wallSize = baby->calculateBoxSize().x;
	delete baby;

	Ogre::MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(Vector3::UNIT_Y, Vector3(0, -wallSize / 2.0, 0)), wallSize * cols, wallSize * fils, 100, 80, true, 1, cols/3, fils/3, Vector3::UNIT_Z);
	Ogre::Entity* plane = mSM->createEntity("floor");
	plane->setMaterialName(floorMat);
	SceneNode* floor = createChildSceneNode();
	floor->attachObject(plane);
	floor->setPosition(Vector3(wallSize * cols / 2, 0, wallSize * fils / 2 ));

	char z;
	for (int i = 0; i < fils; ++i) {
		stage.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for (int c = 0; c < cols; c++) {
			stage.get(z);
			auto node = createChildSceneNode();
			Vector3 pos = Vector3(c * wallSize, 0, i * wallSize);
			if (z == 'x') {
				Wall* wall = new Wall(pos, node, mSM);
				wall->setMaterialName(wallMat);
				map[i][c] = false;
			}
			else if(z == 'o'){
				NullNode* null = new NullNode(pos, node, mSM);
				map[i][c] = true;
			}
			else if (z == 'h') {
				double scale = normalizeSize(hero->calculateBoxSize());
				hero->setScale(Vector3(scale, scale, scale));

				hero->setPosition(pos);
				hero->setInitialPosition(pos);
				map[i][c] = true;
			}
			else if (z == 'v') {
				ComplexEnemy* enemy = new ComplexEnemy(pos, node->getParentSceneNode()->createChildSceneNode(), mSM, 150.0f);
				double scale = normalizeSize(enemy->calculateBoxSize());
				enemy->setScale(Vector3(scale, scale, scale));

				enemy->setLaberinth(this);
				enemies.push_back(enemy);
				map[i][c] = true;
			}
		}
	}
}

bool
Laberinth::checkCollisions(AliveEntity& entity, bool isHero) {
	if (isHero) {
		bool col = false;
		for (int i = 0; i < enemies.size() && !col; ++i) {
			col = hero->getAABB().intersects(enemies[i]->getAABB());
		}
		return col;
	}
	else return entity.getAABB().intersects(hero->getAABB());
}


bool 
Laberinth::isPositionWalkable(float x, float y) {
	int fil, col;
	fil = std::round(y / wallSize);
	col = std::round(x / wallSize);

	return map[fil][col];
}

bool
Laberinth::isPositionWalkable(Vector2 pos) {
	return isPositionWalkable(pos.x, pos.y);
}

void
Laberinth::resetGame() {
	for (auto enemy : enemies) {
		enemy->resetPos();
	}
}

void 
Laberinth::updateInfoText() {
	info->setText("points: " + to_string(hero->getPoints()) + "\nlives: " + to_string(hero->getLives()));
}

Vector2
Laberinth::getLaberinthPosition(Vector2 const& pos) {
	int fil, col;
	fil = std::round(pos.y / wallSize);
	col = std::round(pos.x / wallSize);

	return Vector2(col * wallSize, fil * wallSize);
}


void 
Laberinth::correctPosition(AliveEntity& entity) {
	Vector2 eDir = entity.getDirection();

	Vector2 labPos = getLaberinthPosition({ entity.getPosition().x, entity.getPosition().z });

	if (eDir.x != 0) {
		entity.setPosition(Vector3(entity.getPosition().x, 0, labPos.y));
	}
	else entity.setPosition(Vector3(labPos.x, 0, entity.getPosition().z));
	
}

