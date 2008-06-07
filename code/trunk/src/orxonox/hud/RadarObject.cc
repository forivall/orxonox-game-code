/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Felix Schulthess
 *   Co-authors:
 *      ...
 *
 */

#include "OrxonoxStableHeaders.h"
#include "RadarObject.h"

#include <OgreOverlayManager.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

#include "GraphicsEngine.h"
#include "util/Convert.h"

namespace std
{
    template <>
    class less<orxonox::ColourValue>
    {
        public:
            bool operator()(const orxonox::ColourValue& __x, const orxonox::ColourValue& __y)
            {
                if (__x.r == __y.r)
                {
                    if (__x.g == __y.g)
                    {
                        if (__x.b == __y.b)
                        {
                            return __x.a < __y.a;
                        }
                        return __x.b < __y.b;
                    }
                    return __x.g < __y.g;
                }
                return __x.r < __y.r;
            }
    };
}

namespace orxonox
{
    unsigned int RadarObject::count_s = 0;
    std::map<std::string, std::map<ColourValue, std::string> > RadarObject::materials_s;

    RadarObject::RadarObject(Ogre::OverlayContainer* container, Ogre::SceneNode* node, const ColourValue& colour, const std::string& texturename)
    {
        this->colour_ = colour;
        this->texturename_ = texturename;

        this->container_ = container;
        this->node_ = node;

        this->panel_ = static_cast<Ogre::PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement("Panel", "RadarObject" + getConvertedValue<unsigned int, std::string>(RadarObject::count_s)));
        this->setMaterial(colour, texturename);

        this->panel_->setDimensions(3,3);
        this->panel_->setMetricsMode(Ogre::GMM_PIXELS);
        this->panel_->show();

        this->index_ = count_s++;
        this->container_->addChild(panel_);
    }

    RadarObject::~RadarObject()
    {
        delete panel_;
    }

    void RadarObject::setMaterial(const ColourValue& colour, const std::string& texturename)
    {
        std::map<ColourValue, std::string>& colourmap = this->materials_s[texturename];
        std::map<ColourValue, std::string>::const_iterator it = colourmap.find(colour);
        std::string materialname;

        if (it == colourmap.end())
        {
            materialname = "radarmaterial" + getConvertedValue<unsigned int, std::string>(RadarObject::count_s);
            Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().create(materialname, "General");
            Ogre::TextureUnitState* textureunitstate = material->getTechnique(0)->getPass(0)->createTextureUnitState();
            textureunitstate->setTextureName(texturename);
            textureunitstate->setColourOperationEx(Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, colour);
            colourmap[colour] = materialname;
        }
        else
        {
            materialname = (*it).second;
        }

        this->panel_->setMaterialName(materialname);
    }

    Vector3 RadarObject::getPosition()
    {
        return node_->getPosition();
    }

    Ogre::SceneNode* RadarObject::getNode()
    {
        return node_;
    }
}

