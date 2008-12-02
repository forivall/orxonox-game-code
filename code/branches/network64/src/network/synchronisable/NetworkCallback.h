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
 *      Oliver Scheuss, (C) 2008
 *   Co-authors:
 *      ...
 *
 */


#ifndef _NETWORK_CALLBACK__
#define _NETWORK_CALLBACK__

#include "network/NetworkPrereqs.h"
#include "NetworkCallbackManager.h"

namespace orxonox{
  
  class _NetworkExport NetworkCallbackBase
  {
    public:
      virtual void call() = 0;
      NetworkCallbackBase(){ NetworkCallbackManager::registerCallback( this ); }
      virtual ~NetworkCallbackBase() {}
  };

  template <class T>
  class NetworkCallback: public NetworkCallbackBase
  {
    public:
      NetworkCallback(T* object, void (T::*function) (void)) : object_(object), function_(function) {}
	  virtual ~NetworkCallback() {}
      virtual void call()
        { (this->object_->*function_)(); }

    private:
      T* object_;
      void (T::*function_) (void);
  };

}


#endif
