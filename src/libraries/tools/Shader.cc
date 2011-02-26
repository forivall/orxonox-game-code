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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#include "Shader.h"

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>
#include <OgrePlugin.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreMaterialManager.h>

#include "core/CoreIncludes.h"
#include "core/GameMode.h"
#include "core/GraphicsManager.h"

namespace orxonox
{
    bool Shader::bLoadedCgPlugin_s = false;
    Shader::MaterialMap Shader::parameters_s;

    Shader::Shader(Ogre::SceneManager* scenemanager) : compositorInstance_(0)
    {
        RegisterObject(Shader);

        this->scenemanager_ = scenemanager;
        this->bVisible_ = true;
        this->bLoadCompositor_ = GameMode::showsGraphics();

        static bool hasCgProgramManager = Shader::hasCgProgramManager();
        Shader::bLoadedCgPlugin_s = hasCgProgramManager;

        this->bLoadCompositor_ &= Shader::bLoadedCgPlugin_s;
    }

    Shader::~Shader()
    {
        if (this->compositorInstance_ && GraphicsManager::getInstance().getViewport())
            Ogre::CompositorManager::getSingleton().removeCompositor(GraphicsManager::getInstance().getViewport(), this->compositorName_);
    }

    void Shader::cameraChanged(Ogre::Viewport* viewport, Ogre::Camera* oldCamera)
    {
        if (!this->bLoadCompositor_ || !this->scenemanager_)
            return;

        // load the compositor if not already done
        if (!this->compositorName_.empty() && !this->compositorInstance_)
            this->changedCompositorName(viewport);

        // update compositor in viewport (shader should only be active if the current camera is in the same scene as the shader)

        // Note:
        // The shader needs also to be switched off and on after changing the camera in the
        // same scene to avoid weird behaviour with active compositors while switching the
        // camera (like freezing the image)
        //
        // Last known Ogre version needing this workaround:
        // 1.4.8
        // 1.7.2

        if (oldCamera && this->scenemanager_ == oldCamera->getSceneManager())
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, false);

        if (viewport->getCamera() && this->scenemanager_ == viewport->getCamera()->getSceneManager())
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, this->isVisible());
    }

    void Shader::changedCompositorName()
    {
        // For the moment, we get the viewport always from the graphics manager
        // TODO: Try to support multiple viewports - note however that scenemanager_->getCurrentViewport() returns NULL
        //       after switching to a camera in a different scene (only for the first time this scene is displayed though)
        this->changedCompositorName(GraphicsManager::getInstance().getViewport());
    }

    void Shader::changedCompositorName(Ogre::Viewport* viewport)
    {
        if (this->bLoadCompositor_)
        {
            assert(viewport);
            if (!this->oldcompositorName_.empty())
            {
                Ogre::CompositorManager::getSingleton().removeCompositor(viewport, this->oldcompositorName_);
                this->compositorInstance_ = 0;
            }
            if (!this->compositorName_.empty())
            {
                this->compositorInstance_ = Ogre::CompositorManager::getSingleton().addCompositor(viewport, this->compositorName_);
                if (!this->compositorInstance_)
                    COUT(2) << "Warning: Couldn't load compositor with name \"" << this->compositorName_ << "\"." << std::endl;
                else if (viewport->getCamera())
                    Ogre::CompositorManager::getSingleton().setCompositorEnabled(viewport, this->compositorName_, this->isVisible() && viewport->getCamera() && this->scenemanager_ == viewport->getCamera()->getSceneManager());
            }
            this->oldcompositorName_ = this->compositorName_;
        }
    }

    void Shader::updateVisibility()
    {
        if (this->compositorInstance_)
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, this->isVisible());
    }

    void Shader::setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value)
    {
        if (Shader::_setParameter(material, technique, pass, parameter, value))
        {
            if (this->compositorInstance_ && this->isVisible())
            {
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, false);
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, true);
            }
        }
    }

    void Shader::setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value)
    {
        if (Shader::_setParameter(material, technique, pass, parameter, value))
        {
            if (this->compositorInstance_ && this->isVisible())
            {
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, false);
                Ogre::CompositorManager::getSingleton().setCompositorEnabled(GraphicsManager::getInstance().getViewport(), this->compositorName_, true);
            }
        }
    }

    /* static */ bool Shader::_setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, float value)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
            {
                if ((*static_cast<float*>(pointer->second)) != value)
                {
                    (*static_cast<float*>(pointer->second)) = value;
                    return true;
                }
            }
            else
            {
                if ((*static_cast<int*>(pointer->second)) != static_cast<int>(value))
                {
                    (*static_cast<int*>(pointer->second)) = static_cast<int>(value);
                    return true;
                }
            }
        }
        return false;
    }

    /* static */ bool Shader::_setParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter, int value)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
            {
                if ((*static_cast<float*>(pointer->second)) != static_cast<float>(value))
                {
                    (*static_cast<float*>(pointer->second)) = static_cast<float>(value);
                    return true;
                }
            }
            else
            {
                if ((*static_cast<int*>(pointer->second)) != value)
                {
                    (*static_cast<int*>(pointer->second)) = value;
                    return true;
                }
            }
        }
        return false;
    }

    /* static */ float Shader::getParameter(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
        {
            if (pointer->first)
                return (*static_cast<float*>(pointer->second));
            else
                return static_cast<float>(*static_cast<int*>(pointer->second));
        }
        else
            return 0;
    }

    /* static */ bool Shader::getParameterIsFloat(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
            return pointer->first;
        else
            return false;
    }

    /* static */ bool Shader::getParameterIsInt(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        ParameterPointer* pointer = Shader::getParameterPointer(material, technique, pass, parameter);
        if (pointer)
            return (!pointer->first);
        else
            return false;
    }

    /* static */ Shader::ParameterPointer* Shader::getParameterPointer(const std::string& material, size_t technique, size_t pass, const std::string& parameter)
    {
        if (!GameMode::showsGraphics() || !Shader::bLoadedCgPlugin_s)
            return 0;

        MaterialMap::iterator material_iterator = Shader::parameters_s.find(material);
        if (material_iterator != Shader::parameters_s.end())
        {
            TechniqueVector& technique_vector = material_iterator->second;
            if (technique < technique_vector.size())
            {
                PassVector& pass_vector = technique_vector[technique];
                if (pass < pass_vector.size())
                {
                    ParameterMap& parameter_map = pass_vector[pass];
                    ParameterMap::iterator parameter_iterator = parameter_map.find(parameter);

                    if (parameter_iterator != parameter_map.end())
                        return (&parameter_iterator->second);
                    else
                        COUT(2) << "Warning: No shader parameter \"" << parameter << "\" in pass " << pass << " in technique " << technique << " in material \"" << material << "\"." << std::endl;
                }
                else
                    COUT(2) << "Warning: No pass " << pass << " in technique " << technique << " in material \"" << material << "\" or pass has no shader." << std::endl;
            }
            else
                COUT(2) << "Warning: No technique " << technique << " in material \"" << material << "\" or technique has no pass with shader." << std::endl;
        }
        else
        {
            bool foundAtLeastOneShaderParameter = false;
            Ogre::MaterialManager::ResourceMapIterator iterator = Ogre::MaterialManager::getSingleton().getResourceIterator();
            Ogre::Material* material_pointer = 0;

            while (iterator.hasMoreElements())
            {
                Ogre::Resource* resource = iterator.getNext().get();
                if (resource->getName() == material)
                    material_pointer = (Ogre::Material*)resource;
            }

            if (!material_pointer)
            {
                COUT(2) << "Warning: No material with name \"" << material << "\" found." << std::endl;
                return 0;
            }

            for (unsigned int t = 0; t < material_pointer->getNumTechniques(); ++t)
            {
                Ogre::Technique* technique_pointer = material_pointer->getTechnique(t);
                if (!technique_pointer)
                    continue;

                for (unsigned int p = 0; p < technique_pointer->getNumPasses(); ++p)
                {
                    Ogre::Pass* pass_pointer = technique_pointer->getPass(p);
                    if (!pass_pointer)
                        continue;

                    if (!pass_pointer->getFragmentProgramName().empty())
                    {
                        Ogre::GpuProgramParameters* parameter_pointer = pass_pointer->getFragmentProgramParameters().get();
                        if (!parameter_pointer)
                            continue;

                        const Ogre::GpuConstantDefinitionMap& constant_definitions = parameter_pointer->getConstantDefinitions().map;
                        for (Ogre::GpuConstantDefinitionMap::const_iterator definition_iterator = constant_definitions.begin(); definition_iterator != constant_definitions.end(); ++definition_iterator)
                        {
                            void* temp = (definition_iterator->second.isFloat())
                                            ? static_cast<void*>(parameter_pointer->getFloatPointer(definition_iterator->second.physicalIndex))
                                            : static_cast<void*>(parameter_pointer->getIntPointer(definition_iterator->second.physicalIndex));
                            ParameterPointer parameter_pointer = ParameterPointer(definition_iterator->second.isFloat(), temp);

                            TechniqueVector& technique_vector = Shader::parameters_s[material];
                            technique_vector.resize(technique + 1);
                            PassVector& pass_vector = technique_vector[technique];
                            pass_vector.resize(pass + 1);
                            pass_vector[pass][definition_iterator->first] = parameter_pointer;
                            foundAtLeastOneShaderParameter = true;
                        }
                    }
                }
            }

            // recursive call if the material was added to the map
            if (foundAtLeastOneShaderParameter)
                return Shader::getParameterPointer(material, technique, pass, parameter);
        }
        return 0;
    }

    /* static */ bool Shader::hasCgProgramManager()
    {
        if (Ogre::Root::getSingletonPtr())
        {
            const Ogre::Root::PluginInstanceList& plugins = Ogre::Root::getSingleton().getInstalledPlugins();
            for (size_t i = 0; i < plugins.size(); ++i)
                if (plugins[i]->getName() == "Cg Program Manager")
                    return true;
        }
        return false;
    }
}
