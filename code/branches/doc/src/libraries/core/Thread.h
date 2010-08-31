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
 *      Oliver Scheuss
 *   Co-authors:
 *      ...
 *
 */

#ifndef _Thread_H__
#define _Thread_H__

#include "CorePrereqs.h"

namespace boost{
  class recursive_mutex;
}

 namespace orxonox
{
    class _CoreExport Thread
    {
    public:
        Thread();
        virtual ~Thread();

        bool isWorking();
        void waitUntilFinished();
        bool evaluateExecutor( const ExecutorPtr& executor );

    private:
        void            threadLoop();

        ExecutorPtr     executor_;
        bool            isWorking_;
        bool            stopThread_;
        boost::thread*  workerThread_;
        boost::mutex*   executorMutex_;
        boost::mutex*   isWorkingMutex_;
        boost::mutex*   stopThreadMutex_;
    };

 }

#endif /* _Thread_H__ */
