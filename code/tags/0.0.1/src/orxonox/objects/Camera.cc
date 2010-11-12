
#include "OrxonoxStableHeaders.h"

#include <string>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include "util/tinyxml/tinyxml.h"
#include "util/Tokenizer.h"
#include "util/String2Number.h"
#include "util/Math.h"
#include "../core/Debug.h"
#include "../core/CoreIncludes.h"
#include "../Orxonox.h"
#include "../GraphicsEngine.h"

#include "Camera.h"

namespace orxonox
{
    CreateFactory(Camera);

    Camera::Camera()
    {
        RegisterObject(Camera);
    }

    Camera::~Camera()
    {
    }

    void Camera::loadParams(TiXmlElement* xmlElem)
    {
      Ogre::SceneManager* mgr = orxonox::Orxonox::getSingleton()->getSceneManager();

      if (xmlElem->Attribute("name") && xmlElem->Attribute("pos") && xmlElem->Attribute("lookat") && xmlElem->Attribute("node"))
      {
        //    <Camera name="Camera" pos="0,0,-250" lookat="0,0,0" />

        std::string name = xmlElem->Attribute("name");
        std::string pos = xmlElem->Attribute("pos");
        std::string lookat = xmlElem->Attribute("lookat");

        Ogre::Camera *cam = mgr->createCamera(name);

        float x, y, z;
        std::vector<std::string> posVec = tokenize(xmlElem->Attribute("pos"),",");
        String2Number<float>(x, posVec[0]);
        String2Number<float>(y, posVec[1]);
        String2Number<float>(z, posVec[2]);

        cam->setPosition(Vector3(x,y,z));

        posVec = tokenize(xmlElem->Attribute("lookat"),",");
        String2Number<float>(x, posVec[0]);
        String2Number<float>(y, posVec[1]);
        String2Number<float>(z, posVec[2]);

        cam->lookAt(Vector3(x,y,z));

        std::string node = xmlElem->Attribute("node");

        Ogre::SceneNode* sceneNode = (Ogre::SceneNode*)mgr->getRootSceneNode()->createChildSceneNode(node); //getChild(node);
        sceneNode->attachObject((Ogre::MovableObject*)cam);

        // FIXME: unused var
        Ogre::Viewport* vp = orxonox::Orxonox::getSingleton()->getOgrePointer()->getRoot()->getAutoCreatedWindow()->addViewport(cam);


        COUT(4) << "Loader: Created camera "<< name  << std::endl << std::endl;
      }
   }
}