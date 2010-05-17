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
 *      Johannes Ritz
 *   Co-authors:
 *      ...
 *
 */
//TODO:
//pig punkte vergeben pro Zeit!
//killerfarbe schwarz; evtl. eigenes Raumfahrzeug;
//Low; Codeoptimierung und Dokumentation

/*
short gaming manual:
There are three different parties a player can belong to: victim, chaser or killer
Every player starts as chaser. As long as there are not enough victims and killers, you can change your and other player's parties by shooting them.
In order to win you have to earn as much points as possible:
- as victim by escaping the chasers
- as chaser by shooting the victim
- as killer by killing the chasers


What you shouldn't do is shooting at players of your own party. By doing so your score will decrease.
P.S: If you don't want to be a victim: Get rid of your part by shooting a chaser.
*/
#include "Dynamicmatch.h"

#include "util/Convert.h"
#include "core/CoreIncludes.h"
#include "network/Host.h"
#include "infos/PlayerInfo.h"
#include "worldentities/pawns/Pawn.h"
#include "worldentities/pawns/SpaceShip.h"
#include "core/ConfigValueIncludes.h"
#include "interfaces/TeamColourable.h"
#include "items/Engine.h"
#include "tools/Timer.h"
#include "worldentities/TeamSpawnPoint.h"

namespace orxonox
{
    CreateUnloadableFactory(Dynamicmatch);

    Dynamicmatch::Dynamicmatch(BaseObject* creator) : Gametype(creator)
    {
        RegisterObject(Dynamicmatch);
        this->gameTime_ = 180;
        this->setConfigValues();
        this->chaser=0;
        this->piggy=1;
        this->killer=2;
        this->notEnoughPigs=false;
        this->notEnoughKillers=false;
        this->notEnoughChasers=false;
        this->gameEnded_ =false;
        this->timesequence_ = static_cast<int>(this->gameTime_);
        this->friendlyfire=true;
        this->numberOf[chaser]=0;
        this->numberOf[piggy]=0;
        this->numberOf[killer]=0;
        this->tutorial=true;
	this->pointsPerTime=0.0f;
        this->setHUDTemplate("DynamicmatchHUD");

    }

    void Dynamicmatch::setConfigValues()
    {
        SetConfigValue(gameTime_, 180);
        SetConfigValue(friendlyfire, true);
        SetConfigValue(tutorial, true);
        static ColourValue colours[] =
        {
            ColourValue(0.3f, 0.3f, 1.0f),	//chasercolour
            ColourValue(1.0f, 0.3f, 0.3f),	//piggycolour
            ColourValue(0.3f, 1.0f, 0.3f)	//killercolour  what about black: 0.0f, 0.0f, 0.0f
            
        };
        static std::vector<ColourValue> defaultcolours(colours, colours + sizeof(colours) / sizeof(ColourValue));

        SetConfigValue(partyColours_, defaultcolours);
    }

    bool Dynamicmatch::allowPawnDamage(Pawn* victim, Pawn* originator)
    {	//TODO: static and fading message for the "human" player's
        if (!originator||!victim)
            return false;
        if (!!originator->getPlayer()||!victim->getPlayer())
            return false;
        if (victim && victim->getPlayer()) //&& originator && originator->getPlayer() ??
        {
        int target= playerParty_[victim->getPlayer()];
        int source= playerParty_[originator->getPlayer()];

            //Case: Not Enough Pigs: party change (= party management)
            if (notEnoughPigs)
            {
                numberOf[target]--;				//decrease numberof victims's party
                playerParty_[victim->getPlayer()]=piggy;	//victim's new party: pig
                setPlayerColour(victim->getPlayer());		//victim's new colour
                numberOf[piggy]++; 				//party switch: number of players is not affected (decrease and increase)
                    if(tutorial)				//announce party switch
                    {
                         std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                         if (it2 != this->players_.end())
                         {
                              this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it2->first->getClientID());
                         }
                    }
                    if (notEnoughKillers) 			//reward the originator
                    {
                        numberOf[source]--;			//decrease numberof originator's party
                        playerParty_[originator->getPlayer()]=killer;		//originator's new party: killer
                        setPlayerColour(originator->getPlayer());		//originator's new colour
                        numberOf[killer]++;
                        if(tutorial)				//announce party switch
                        {
                             std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                             if (it3 != this->players_.end())
                             {
                                  this->gtinfo_->sendStaticMessage("Take the chasers down.",it3->first->getClientID());
                             }
                        }
                    }
                evaluatePlayerParties(); 			//check if the party change has to trigger futher party changes
			
                //Give new pig boost
                SpaceShip* spaceship = dynamic_cast<SpaceShip*>(victim);
                if (spaceship && spaceship->getEngine())
                {
                    spaceship->getEngine()->setSpeedFactor(5);
                    WeakPtr<Engine>* ptr = new WeakPtr<Engine>(spaceship->getEngine());
                    new Timer(10, false, &createExecutor(createFunctor(&Dynamicmatch::resetSpeedFactor, this))->setDefaultValue(0, ptr), true);
                }
            }

            //Case: notEnoughKillers: party change
            else if (notEnoughKillers)
            {
                numberOf[target]--; 	//decrease numberof victims's party
                playerParty_[victim->getPlayer()]=killer; 	//victim's new party: killer
                setPlayerColour(victim->getPlayer()); 		//victim colour
                numberOf[killer]++;				//party switch: number of players is not affected (decrease and increase)
                if(tutorial)				//announce party switch
                {
                     std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                     if (it3 != this->players_.end())
                     {
                          this->gtinfo_->sendStaticMessage("Take the chasers down.",it3->first->getClientID());
                     }
                }
                evaluatePlayerParties();			//check if the party change has to trigger futher party changes
            }
            //Case: notEnoughChasers: party change
            else if (notEnoughChasers)
            {
                numberOf[target]--; 	//decrease numberof victims's party
                playerParty_[victim->getPlayer()]=chaser; 	//victim's new party: chaser
                setPlayerColour(victim->getPlayer()); 		//victim colour
                numberOf[chaser]++;				//party switch: number of players is not affected (decrease and increase)
                if(tutorial)					//announce party switch
                {
                     std::map<PlayerInfo*, Player>::iterator it3 = this->players_.find(originator->getPlayer());
                     if (it3 != this->players_.end())
                     {
                          if (numberOf[killer]>0)
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it3->first->getClientID());
                          else
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it3->first->getClientID());
                     }
                }
                evaluatePlayerParties();			//check if the party change has to trigger futher party changes
            }

            //Case: chaser vs. killer
            else if ((source == killer && target == chaser)||(target == killer && source == chaser ))
            {
                return true;
            }

            //Case: a chaser hits piggy
            else if ((source==chaser)&&(target==piggy))
            {
                std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                if (it != this->players_.end())
                    {
                        it->second.frags_++;
                    }
            }
            //Case: piggy hits chaser
            else if (source==piggy&&target==chaser)
            {
                //partyswitch: victim bcomes piggy and the originator(piggy) becomes chaser
                playerParty_[victim->getPlayer()]=piggy;
                playerParty_[originator->getPlayer()]=chaser;

                //party switch -> colour switch		
                setPlayerColour(victim->getPlayer()); //victim colour
                setPlayerColour(originator->getPlayer());//originator colour
                
                //Announce pary switch
                if(tutorial)
                {
                     std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                     if (it != this->players_.end())
                     {    
                          if (numberOf[killer]>0)
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID());
                          else
                              this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID());
                     }
                     std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                     if (it2 != this->players_.end())
                     {
                          this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it2->first->getClientID());
                     }
                }
                //Give new pig boost
                SpaceShip* spaceship = dynamic_cast<SpaceShip*>(victim);
                if (spaceship && spaceship->getEngine())
                {
                    spaceship->getEngine()->setSpeedFactor(5);
                    WeakPtr<Engine>* ptr = new WeakPtr<Engine>(spaceship->getEngine());
                    new Timer(10, false, &createExecutor(createFunctor(&Dynamicmatch::resetSpeedFactor, this))->setDefaultValue(0, ptr), true);
                }

            }
            // killer vs piggy
            else if (source==killer &&target==piggy)		//party and colour switch	
            {
            playerParty_[victim->getPlayer()]=killer;
            playerParty_[originator->getPlayer()]=piggy;

            setPlayerColour(victim->getPlayer()); 		//victim colour
            setPlayerColour(originator->getPlayer());		//originator colour
             
            if(tutorial) //Announce pary switch
            {
                 std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                 if (it != this->players_.end())
                 {
                      this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID());
                 }
                 std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(victim->getPlayer());
                 if (it2 != this->players_.end())
                 {
                      this->gtinfo_->sendStaticMessage("Take the chasers down.",it2->first->getClientID());
                 }
                }
            }
            //Case: friendly fire
            else if (friendlyfire && (source == target))
            {
                std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                if (it != this->players_.end())
                    {
                        it->second.frags_--;
                    }
            }
        }// from far far away not to be removed!
        return false; //default: no damage
    }



    bool Dynamicmatch::allowPawnDeath(Pawn* victim, Pawn* originator)
    {	
        //killers can kill chasers and killers can be killed by chasers
        if ((playerParty_[originator->getPlayer()] == killer && playerParty_[victim->getPlayer()] == chaser)||(playerParty_[victim->getPlayer()] == killer &&
        playerParty_[originator->getPlayer()] == chaser ))
        {
            if (playerParty_[originator->getPlayer()] == killer)	//reward the killer
            {
                std::map<PlayerInfo*, Player>::iterator it = this->players_.find(originator->getPlayer());
                if (it != this->players_.end())
                {
                    it->second.frags_+=20;	//value must be tested
                }
            }
        return true;
        }
        else return false;
    }
    
    void Dynamicmatch::playerStartsControllingPawn(PlayerInfo* player, Pawn* pawn) //set party + colouring
    {
        if (!player)
            return;
	
        Dynamicmatch::setPlayerColour(player);	//Set playercolour
        evaluatePlayerParties();
    }

    void Dynamicmatch::playerEntered(PlayerInfo* player) //standardfunction
    {
        if (!player)// only for safety
            return;
        playerParty_[player]=chaser;		//Set playerparty
        numberOf[chaser]++;
        Gametype::playerEntered(player);
        const std::string& message6 = player->getName() + " entered the game";
        COUT(0) << message6 << std::endl;
        Host::Broadcast(message6);
    }

    bool Dynamicmatch::playerLeft(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerLeft(player);
        if (valid_player)
        {
            switch (playerParty_[player])
            {
            case 0: numberOf[chaser]--; break;
            case 1: numberOf[piggy]--; break;
            case 2: numberOf[killer]--; break;
            }
            const std::string& message = player->getName() + " left the game";
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
		//remove player from map
            playerParty_.erase (player);
		//adjust player parties
            evaluatePlayerParties();
        }

        return valid_player;
    }


    void Dynamicmatch::tick(float dt)
    {
        SUPER(Dynamicmatch, tick, dt);

        if (this->hasStarted() && !gameEnded_)
        {   pointsPerTime =pointsPerTime + dt;
            gameTime_ = gameTime_ - dt;
            if (pointsPerTime > 3.0f)//hard coded!! should be changed
            {
                pointsPerTime=0.0f;
                rewardPig();
            }
            if (gameTime_<= 0)
            {
                this->gameEnded_ = true;
                this->end();        
            }
	    if ( gameTime_ <= timesequence_ && gameTime_ > 0)
            {
                const std::string& message = multi_cast<std::string>(timesequence_) + " seconds left!";

                this->gtinfo_->sendAnnounceMessage(message);

                if (timesequence_ >= 30 && timesequence_ <= 60)
                {
                    timesequence_ = timesequence_ - 10;
                }
                else if (timesequence_ <= 30)
                {
                    timesequence_ = timesequence_ - 5;
                }
                else
                {
                    timesequence_ = timesequence_ - 30;
                }
            }
	}
    }

    void Dynamicmatch::rewardPig()
    {
        for (std::map< PlayerInfo*, int >::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it) //durch alle Spieler iterieren und alle piggys finden
        {
            if (it->second==piggy)
            {
                 //Spieler mit der Pig-party frags++
                 std::map<PlayerInfo*, Player>::iterator it2 = this->players_.find(it->first);// still not sure if right syntax
                 if (it2 != this->players_.end())
                 {
                     it2->second.frags_++;
                 }
            }
        }
    }
    void Dynamicmatch::setPlayerColour(PlayerInfo* player) // sets a players colour
    {
        std::map<PlayerInfo*, int>::const_iterator it_player = this->playerParty_.find(player);
        Pawn* pawn = dynamic_cast<Pawn*>(player->getControllableEntity());
            if (pawn)
            {
                pawn->setRadarObjectColour(this->partyColours_[it_player->second]);

                std::set<WorldEntity*> pawnAttachments = pawn->getAttachedObjects();
                for (std::set<WorldEntity*>::iterator it = pawnAttachments.begin(); it != pawnAttachments.end(); ++it)
                {
                    if ((*it)->isA(Class(TeamColourable)))
                    {
                        TeamColourable* tc = orxonox_cast<TeamColourable*>(*it);
                        tc->setTeamColour(this->partyColours_[it_player->second]);
                    }
                }
            }
	}

    void Dynamicmatch::evaluatePlayerParties() //manages the notEnough booleans (here the percentage of special players is implemented)
    {
        //pigs: 1 + every 6th player is a pig
        if ( (1+this->getNumberOfPlayers()/6) > numberOf[piggy])
        {
            notEnoughPigs=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == CLIENTID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID());
                }
            }
        }
        else
        {
             notEnoughPigs=false;
             if(tutorial&&(!notEnoughKillers)&&(!notEnoughChasers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)//in order to catch nullpointer
                           continue;
                       if (it->first->getClientID() == CLIENTID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID());
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID());
                       else if (it->second==killer)
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID());
                  }
                  
             }
        }
        //killers: every 4th player is a killer
        if (getNumberOfPlayers()/4 > numberOf[killer])
        {
            notEnoughKillers=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == CLIENTID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID());
                }
            }
        }
        else
        {
            notEnoughKillers=false;
            if(tutorial&&(!notEnoughPigs)&&(!notEnoughChasers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)
                           continue;
                       if (it->first->getClientID() == CLIENTID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID());
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID());
                       else if (it->second==killer)
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID());
                  }
                  
             }
            
        }
        //chasers: there are more chasers than killers + pigs
        if (numberOf[piggy]+numberOf[killer] > numberOf[chaser])
        {
            notEnoughChasers=true;
            if (tutorial) // Announce selectionphase
            {
             for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (!it->first)//in order to catch nullpointer
                        continue;
                    if (it->first->getClientID() == CLIENTID_UNKNOWN)
                        continue;
                    this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID());
                }
            }
        }
        else
        {
             notEnoughChasers=false;
             if(tutorial&&(!notEnoughPigs)&&(!notEnoughKillers)) //Selection phase over
             {
                  for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                  {
                       if (!it->first)
                           continue;
                       if (it->first->getClientID() == CLIENTID_UNKNOWN)
                           continue;
                       else if (it->second==chaser)
                       {
                           if (numberOf[killer]>0)
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible. Defend yourself against the killers.",it->first->getClientID());
                           else
                               this->gtinfo_->sendStaticMessage("Shoot at the victim as often as possible.",it->first->getClientID());
                       }
                       else if (it->second==piggy)
                           this->gtinfo_->sendStaticMessage("Either hide or shoot a chaser.",it->first->getClientID());
                       else if (it->second==killer)
                           this->gtinfo_->sendStaticMessage("Take the chasers down.",it->first->getClientID());
                  }
                  
             }
        }	
    }

    int Dynamicmatch::getParty(PlayerInfo* player) // helper function for ArtificialController
    {
        return this->playerParty_[player];
    }

    void Dynamicmatch::resetSpeedFactor(WeakPtr<Engine>* ptr)// helper function
    {
        if (*ptr)
        {
            (*ptr)->setSpeedFactor(1.0f);
        }
        delete ptr;
    }

    bool Dynamicmatch::playerChangedName(PlayerInfo* player) //standardfunction
    {
        bool valid_player = Gametype::playerChangedName(player);
        if (valid_player)
        {
            const std::string& message = player->getOldName() + " changed name to " + player->getName();
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }

        return valid_player;
    }

    void Dynamicmatch::start() 
    {	
        Gametype::start();
        if(!tutorial)
        {
            std::string message("Dynamicmatch started!");
            COUT(0) << message << std::endl;
            Host::Broadcast(message);
        }
        else if(tutorial) // Announce selectionphase
        {
            for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
            {
                if (it->first->getClientID() == CLIENTID_UNKNOWN)
                    continue;
                this->gtinfo_->sendStaticMessage("Selection phase: Shoot at everything that moves.",it->first->getClientID());
            }
        }
    }	

    /*void Dynamicmatch::instructions()
    {
        std::string message("Earn points:\n\n\n\tIf you're red: Chase the blue player!\n\n\tIf you're blue shoot at a red player or hide.\n\n\tIf you're green: You've got the licence to kill red players!");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
        callInstructions_.setTimer(10, false, createExecutor(createFunctor(&Dynamicmatch::furtherInstructions, this)));
    }

    void Dynamicmatch::furtherInstructions()
    {
        std::string message("After 3 Minutes the game is over.");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
    }*/
    void Dynamicmatch::end()
    {
        Gametype::end();

        std::string message("Time out. Press F2 to see the points you scored.");
        COUT(0) << message << std::endl;
        Host::Broadcast(message);
	/*for (std::map<PlayerInfo*, int>::iterator it = this->playerParty_.begin(); it != this->playerParty_.end(); ++it)
                {
                    if (it->first->getClientID() == CLIENTID_UNKNOWN)
                        continue;

                    if (it->second == 1)
                        this->gtinfo_->sendAnnounceMessage("You have won the match!", it->first->getClientID());
                    else
                        this->gtinfo_->sendAnnounceMessage("You have lost the match!", it->first->getClientID());
                }*/
    }
    SpawnPoint* Dynamicmatch::getBestSpawnPoint(PlayerInfo* player) const
    {
        int desiredTeamNr = -1;
        std::map<PlayerInfo*, int>::const_iterator it_player = this->playerParty_.find(player);
        if (it_player != this->playerParty_.end())
            desiredTeamNr = it_player->second;

        // Only use spawnpoints of the own team (or non-team-spawnpoints)
        std::set<SpawnPoint*> teamSpawnPoints = this->spawnpoints_;
        for (std::set<SpawnPoint*>::iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); )
        {
            if ((*it)->isA(Class(TeamSpawnPoint)))
            {
                TeamSpawnPoint* tsp = orxonox_cast<TeamSpawnPoint*>(*it);
                if (tsp && static_cast<int>(tsp->getTeamNumber()) != desiredTeamNr)//getTeamNumber!!
                {
                    teamSpawnPoints.erase(it++);
                    continue;
                }
            }

            ++it;
        }

        if (teamSpawnPoints.size() > 0)
        {
            unsigned int randomspawn = static_cast<unsigned int>(rnd(static_cast<float>(teamSpawnPoints.size())));
            unsigned int index = 0;
            for (std::set<SpawnPoint*>::const_iterator it = teamSpawnPoints.begin(); it != teamSpawnPoints.end(); ++it)
            {
                if (index == randomspawn)
                    return (*it);

                ++index;
            }
        }

        return 0;
    }

}
