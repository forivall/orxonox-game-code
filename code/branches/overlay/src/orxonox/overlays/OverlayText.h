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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

#ifndef _OverlayText_H__
#define _OverlayText_H__

#include "OrxonoxPrereqs.h"

#include <string>
#include <OgrePrerequisites.h>
#include <OgreTextAreaOverlayElement.h>
#include "OrxonoxOverlay.h"

namespace orxonox
{
    class _OrxonoxExport OverlayText : public OrxonoxOverlay
    {
    public:
        OverlayText();
        virtual ~OverlayText();

        virtual void XMLPort(Element& xmlElement, XMLPort::Mode mode);

        void setCaption(const std::string& caption) { this->text_->setCaption(caption); }
        std::string getCaption() const              { return this->text_->getCaption(); }

        void setFont(const std::string& font);
        const std::string& getFont() const;

        void setTextSize(float size) { this->setSize(Vector2(size, size)); }
        float getTextSize() const    { return this->getSize().y; }

    protected:
        virtual void sizeChanged();

    private:

        Ogre::TextAreaOverlayElement* text_;
    };
}
#endif /* _OverlayText_H__ */
