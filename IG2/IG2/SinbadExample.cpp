#include "SinbadExample.h"
#include "Laberinth.h"
#include "Hero.h"
#include "ComplexEnemy.h"
#include <iostream>
#include <cmath>

using namespace std;
using namespace Ogre;


void
SinbadExample::loadLaberinth(Laberinth* lab, string const& stage) {
    lab->GenerateLaberinth(stage);
    mStageLabel->setCaption(stage);
}

bool SinbadExample::keyPressed(const OgreBites::KeyboardEvent& evt) {

    // ESC key finished the rendering...
    if (evt.keysym.sym == SDLK_ESCAPE) {
        getRoot()->queueEndRendering();
    }

    return true;
}


void SinbadExample::shutdown() {

    mShaderGenerator->removeSceneManager(mSM);
    mSM->removeRenderQueueListener(mOverlaySystem);

    mRoot->destroySceneManager(mSM);

    delete mTrayMgr;  mTrayMgr = nullptr;
    delete mCamMgr; mCamMgr = nullptr;

    // do not forget to call the base 
    OgreBites::ApplicationContext::shutdown();
}

void SinbadExample::setup(void) {

    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();

    // Create the scene manager
    mSM = mRoot->createSceneManager();

    // Register our scene with the RTSS
    mShaderGenerator->addSceneManager(mSM);
        
    mSM->addRenderQueueListener(mOverlaySystem);
    //mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow.render);
    mTrayMgr = new OgreBites::TrayManager("TrayGUISystem", getRenderWindow());
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);

    mStageLabel = mTrayMgr->createLabel(OgreBites::TL_BOTTOMRIGHT, "GameInfo", "Stage: 1", 400.0);
    mInfoText = mTrayMgr->createTextBox(OgreBites::TL_BOTTOMRIGHT, "Info", "Game Info here!", Ogre::Real(400), Ogre::Real(200));
    mInfoText->appendText("lives = 3\n");
    mInfoText->appendText("points = 0");

    addInputListener(mTrayMgr);

    // Adds the listener for this object
    addInputListener(this);
    setupScene();
}
//hola chat.
void SinbadExample::setupScene(void) {

    //------------------------------------------------------------------------
    // Creating the camera

    Camera* cam = mSM->createCamera("Cam");
    cam->setNearClipDistance(1);
    cam->setFarClipDistance(10000);
    cam->setAutoAspectRatio(true);
    //cam->setPolygonMode(Ogre::PM_WIREFRAME);

    mCamNode = mSM->getRootSceneNode()->createChildSceneNode("nCam");
    mCamNode->attachObject(cam);

    mCamNode->setPosition(0, -1500, 0);
    mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

    // and tell it to render into the main window
    Viewport* vp = getRenderWindow()->addViewport(cam);

    mCamMgr = new OgreBites::CameraMan(mCamNode);
    addInputListener(mCamMgr);
    mCamMgr->setStyle(OgreBites::CS_ORBIT);

    //------------------------------------------------------------------------
    // Creating Sinbad

    auto m_node = mSM->getRootSceneNode()->createChildSceneNode("laberinth");
    auto h_node = mSM->getRootSceneNode()->createChildSceneNode("hero");
    mHero = new Hero(Vector3(0, 0, 0), h_node, mSM);
    Laberinth* laberint = new Laberinth(Vector3(0, 0, 0), m_node, mSM, mHero, mInfoText);
    mHero->setLaberinth(laberint);
    loadLaberinth(laberint, "stage1");
    addInputListener(mHero);

    auto enemies = laberint->getEnemies();

    for (auto enemy : enemies) {
        addInputListener(enemy);
    }

}