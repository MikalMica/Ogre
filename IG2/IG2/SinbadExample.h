#ifndef __SINBADEXAMPLE_H__
#define __SINBADEXAMPLE_H__

#include <OgreApplicationContext.h>
#include <OgreSceneManager.h>
#include <OgreRTShaderSystem.h>       
#include <OgreSceneNode.h>
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include <OgreEntity.h>
#include <OgreInput.h>
#include <OgreMeshManager.h>
#include <sstream>
#include <iostream>
#include <string>
#include "Ogre.h"
#include <OgreWindowEventUtilities.h>
#include <SDL_keycode.h>
class Hero;
class Laberinth;


class SinbadExample: public OgreBites::ApplicationContext, OgreBites::InputListener {

    const size_t SCALE = 1;

public:
    explicit SinbadExample() : OgreBites::ApplicationContext("SinbadExample") {};
    virtual ~SinbadExample() {};

protected:
    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
    virtual void setup();
    virtual void shutdown();
    virtual void setupScene();
    void loadLaberinth(Laberinth* lab, std::string const& stage);


    Ogre::SceneNode* mSinbadNode = nullptr;

    Ogre::SceneManager* mSM = nullptr;
    OgreBites::TrayManager* mTrayMgr = nullptr;

    Ogre::Light* light = nullptr;
    Ogre::SceneNode* mLightParent = nullptr;

    Ogre::SceneNode* mCamNode = nullptr;
    OgreBites::CameraMan* mCamMgr = nullptr;

    OgreBites::Label* mStageLabel = nullptr;
    OgreBites::TextBox* mInfoText = nullptr;

    Hero* mHero = nullptr;
};

#endif
