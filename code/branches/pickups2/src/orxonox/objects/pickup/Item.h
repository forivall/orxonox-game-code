#ifndef _Item_H__
#define _Item_H__

#include "core/BaseObject.h"
#include "OrxonoxPrereqs.h"

namespace orxonox
{
	class _OrxonoxExport Item : public BaseObject
	{
	public:
	Item( BaseObject* creator);
	virtual ~Item();

	virtual void XMLPort(Element& xmlelement, XMLPort::Mode mode);

	virtual bool pickedUp(Pawn* player) { return true; }
	virtual bool dropped(Pawn* player) { return true; } //erst bei festen Items
	void addTo(Pawn* player);

	inline void setPlayerBaseClass(Identifier* identifier)
		{ this->playerBaseClass_ = identifier; }
	inline Identifier* getPlayerBaseClass() const
		{ return this->playerBaseClass_; }
	
	private:
	void setPlayerBaseClassName(const std::string& name);
	const std::string& getPlayerBaseClassName() const;

	Identifier* playerBaseClass_; //class(...)
	};
}
	
#endif /* _Item_H__ */
