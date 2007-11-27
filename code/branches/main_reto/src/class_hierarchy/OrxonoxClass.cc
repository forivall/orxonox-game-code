#include "ClassHierarchy.h"
#include "OrxonoxClass.h"

namespace orxonox
{
    OrxonoxClass::OrxonoxClass()
    {
        this->identifier_ = 0;
        this->parents_ = 0;
    }

    OrxonoxClass::~OrxonoxClass()
    {
        if (this->parents_)
            delete this->parents_;
    }
}
