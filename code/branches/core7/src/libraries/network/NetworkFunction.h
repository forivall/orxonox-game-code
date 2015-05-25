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

#ifndef _NetworkFunction_H__
#define _NetworkFunction_H__

#include "NetworkPrereqs.h"

#include <cassert>
#include <cstring>
#include <map>
#include <string>

#include "core/command/Functor.h"
#include "FunctionCallManager.h"
#include "synchronisable/Synchronisable.h"

namespace orxonox
{

#if defined(ORXONOX_COMPILER_GCC) && defined(ORXONOX_ARCH_32)
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 8;
#else
static const unsigned int MAX_FUNCTION_POINTER_SIZE = 16;
#endif //ORXONOX_COMPILER_GCC
static const unsigned int MAX_FUNCTION_POINTER_INTS = (MAX_FUNCTION_POINTER_SIZE-1)/4+1;

struct _NetworkExport NetworkFunctionPointer {
  uint32_t pointer[MAX_FUNCTION_POINTER_INTS];
  bool operator<(const NetworkFunctionPointer& b) const
  {
#if defined(ORXONOX_COMPILER_GCC) && defined(ORXONOX_ARCH_32)
    return pointer[0]<b.pointer[0] ? true : pointer[1]<b.pointer[1];
#else //ORXONOX_COMPILER_GCC
    return pointer[0]<b.pointer[0] ? true : ( pointer[1]<b.pointer[1] ? true : ( pointer[2]<b.pointer[2] ? true : pointer[3]<b.pointer[3] ) );
#endif //ORXONOX_COMPILER_GCC
  }
};





class _NetworkExport NetworkFunctionBase {
  public:
    NetworkFunctionBase(const std::string& name, const NetworkFunctionPointer& p);
    virtual ~NetworkFunctionBase() {}

    void setNetworkID(uint32_t id);
    inline uint32_t     getNetworkID() const            { return this->networkID_; }
    inline const std::string& getName() const           { return name_; }

  private:
    uint32_t networkID_;
    std::string name_;

};


class _NetworkExport NetworkFunctionStatic: public NetworkFunctionBase {
  public:
    NetworkFunctionStatic(const FunctorStaticPtr& functor, const std::string& name, const NetworkFunctionPointer& p)
        : NetworkFunctionBase(name, p)
        , functor_(functor)
    { }

    inline void call(){ (*this->functor_)(); }
    inline void call(const MultiType& mt1){ (*this->functor_)(mt1); }
    inline void call(const MultiType& mt1, const MultiType& mt2){ (*this->functor_)(mt1, mt2); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3){ (*this->functor_)(mt1, mt2, mt3); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4){ (*this->functor_)(mt1, mt2, mt3, mt4); }
    inline void call(const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5){ (*this->functor_)(mt1, mt2, mt3, mt4, mt5); }

  private:
    FunctorStaticPtr functor_;

};


class _NetworkExport NetworkMemberFunctionBase: public NetworkFunctionBase {
  public:
    NetworkMemberFunctionBase(const std::string& name, const NetworkFunctionPointer& p)
        : NetworkFunctionBase(name, p)
    { }

    //
    virtual bool call(uint32_t objectID)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)=0;
    virtual bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)=0;
};


template <class T> class NetworkMemberFunction: public NetworkMemberFunctionBase {
  public:
    NetworkMemberFunction(const FunctorMemberPtr<T>& functor, const std::string& name, const NetworkFunctionPointer& p)
        : NetworkMemberFunctionBase(name, p)
        , functor_(functor)
    { }

    inline bool call(uint32_t objectID)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)));
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4);
        return true;
      }
      else
        return false;
    }
    inline bool call(uint32_t objectID, const MultiType& mt1, const MultiType& mt2, const MultiType& mt3, const MultiType& mt4, const MultiType& mt5)
    {
      if ( Synchronisable::getSynchronisable(objectID)!=0 )
      {
        (*this->functor_)(orxonox_cast<T*>(Synchronisable::getSynchronisable(objectID)), mt1, mt2, mt3, mt4, mt5);
        return true;
      }
      else
        return false;
    }

  private:
    FunctorMemberPtr<T> functor_;
};

template<class T> inline void copyPtr( T ptr, NetworkFunctionPointer& destptr)
{
  if( sizeof(NetworkFunctionPointer)-sizeof(T) > 0)
    memset((uint8_t*)&destptr + sizeof(T), 0, sizeof(NetworkFunctionPointer)-sizeof(T));
  T p2 = ptr;
  memcpy( &destptr, &p2, sizeof(T) );
//   for(unsigned int i=0; i<(sizeof(T)-1/4)+1; i++)
//     *((uint32_t*)destptr+i) = p2>>32*i;
}

}

#endif /* _NetworkFunction_H__ */
