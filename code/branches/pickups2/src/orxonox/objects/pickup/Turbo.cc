#include "Turbo.h"

#include "objects/worldentities/pawns/SpaceShip.h"
#include "core/Executor.h"
#include "core/CoreIncludes.h"
#include "core/XMLPort.h"

namespace orxonox
{
	

	CreateFactory(Turbo);

	Turbo::Turbo(BaseObject* creator) : Item(creator)
	{
		RegisterObject(Turbo);

		this->boost_ = 0;
		this->duration_ = 0;
		this->accboost_ = 1;
	}

	Turbo::~Turbo()
	{
	}

    void Turbo::XMLPort(Element& xmlelement, XMLPort::Mode mode)
    {
        SUPER(Turbo, XMLPort, xmlelement, mode);

        XMLPortParam(Turbo, "boost", setBoost, getBoost, xmlelement, mode);
        XMLPortParam(Turbo, "duration", setDuration, getDuration, xmlelement, mode);
        XMLPortParam(Turbo, "accboost", setAccBoost, getAccBoost, xmlelement, mode);
    }

	bool Turbo::pickedUp(Pawn* player)
	{
		if(player-> isA(this->getPlayerBaseClass()))
			{
			SpaceShip* ship = dynamic_cast <SpaceShip*>(player);
			this->setSpeedBoost(ship);
			if(duration_==0)
			addTo(player);
			return true;
			}
		return false;

	}
	
	
	void Turbo::unsetSpeedBoost(SpaceShip* ship)
	{
	ship->setMaxSpeed( ship->getMaxSpeed() - this->boost_);
	ship->setTransAcc( ship->getTransAcc()/this->accboost_);
	COUT(3)<<"PickUp Timer expired"<<std::endl;
	}

	void Turbo::setSpeedBoost(SpaceShip* ship)
	{
	ship->setMaxSpeed( ship->getMaxSpeed() + this->boost_);
	ship->setTransAcc( ship->getTransAcc()*this->accboost_);
	if( this->duration_ != 0)
	{
		ExecutorMember<Turbo>* executor = createExecutor(createFunctor(&Turbo::unsetSpeedBoost));
		executor->setDefaultValues(ship);
		turbotimer_.setTimer(this->duration_, false, this, executor);
	}
	
	}
	bool Turbo::dropped(Pawn* player)
	{
		if (this->duration_ == 0)
		{
			//player->Equipment.erase ( std::pair<std::string, Item*>(this->itemname,this) );
			SpaceShip* ship = dynamic_cast <SpaceShip*>(player);
			this->unsetSpeedBoost(ship);
		}
		return true;
	}
	
}
/*<Template baseclass="Turbo" name=turboitem>
      <Turbo playerclass="SpaceShip" boost=150 duration=10 accboost=10 />
    </Template>

    <PickupSpawner item=turboitem>
      <attached>
        <Billboard material="Examples/Flare" scale=0.2 colour="0.0, 0.0, 1.0, 1.0" />
      </attached>
    </PickupSpawner>*/

	
	
	
	
