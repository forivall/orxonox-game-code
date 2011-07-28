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

#ifndef _OutputListener_H__
#define _OutputListener_H__

#include "util/UtilPrereqs.h"

#include <vector>

#include "OutputDefinitions.h"

namespace orxonox
{
    class _UtilExport OutputListener
    {
        public:
            OutputListener();
            virtual ~OutputListener();

            void setLevelMax(OutputLevel max);
            void setLevelRange(OutputLevel min, OutputLevel max);
            void setLevelMask(OutputLevel mask);

            inline OutputLevel getLevelMask() const
                { return this->levelMask_; }

            void setContextMask(OutputContext mask);

            inline OutputContext getContextMask() const
                { return this->contextMask_; }

            inline bool acceptsOutput(OutputLevel level, OutputContext context) const
                { return ((this->levelMask_ & level) && (this->contextMask_ & context)); }

            inline void unfilteredOutput(OutputLevel level, OutputContext context, const std::vector<std::string>& lines)
                { if (this->acceptsOutput(level, context)) this->output(level, context, lines); }

        protected:
            virtual void output(OutputLevel level, OutputContext context, const std::vector<std::string>& lines) = 0;

        private:
            OutputLevel   levelMask_;
            OutputContext contextMask_;
    };
}

#endif /* _OutputListener_H__ */
