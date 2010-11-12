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

/**
@file
@brief
    Declaration of Game Singleton.
 */

#ifndef _Game_H__
#define _Game_H__

#include "CorePrereqs.h"

#include <cassert>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/cat.hpp>

#include "util/Debug.h"
#include "util/StringUtils.h"

/**
@def
    Adds a new GameState to the Game. The second parameter is the name as string
    and every following paramter is a constructor argument (which is usually non existent)
*/
#define DeclareGameState(className, stateName, bIgnoreTickTime, bGraphicsMode) \
    static bool BOOST_PP_CAT(bGameStateDummy_##className, __LINE__) = orxonox::Game::declareGameState<className>(#className, stateName, bIgnoreTickTime, bGraphicsMode)

namespace orxonox
{
    class GameConfiguration;

    /**
    @brief
        Main class responsible for running the game.
    */
    class _CoreExport Game
    {
    public:
        Game(int argc, char** argv);
        ~Game();

        void setStateHierarchy(const std::string& str);
        GameState* getState(const std::string& name);

        void run();
        void stop();

        void requestState(const std::string& name);
        void requestStates(const std::string& names);
        void popState();

        const Clock& getGameClock() { return *this->gameClock_; }

        float getAvgTickTime() { return this->avgTickTime_; }
        float getAvgFPS()      { return this->avgFPS_; }

        void addTickTime(uint32_t length);

        template <class T>
        static bool declareGameState(const std::string& className, const std::string& stateName, bool bIgnoreTickTime, bool bConsoleMode);
        static Game& getInstance() { assert(singletonRef_s); return *singletonRef_s; }

    private:
        class _CoreExport GameStateFactory
        {
        public:
            virtual ~GameStateFactory() { }
            static GameState* fabricate(const std::string& className, const GameStateConstrParams& params);
            template <class T>
            static void createFactory(const std::string& className)
                { factories_s[className] = new TemplateGameStateFactory<T>(); }
            static void destroyFactories();
        private:
            virtual GameState* fabricate(const GameStateConstrParams& params) = 0;
            static std::map<std::string, GameStateFactory*> factories_s;
        };
        template <class T>
        class TemplateGameStateFactory : public GameStateFactory
        {
        public:
            GameState* fabricate(const GameStateConstrParams& params)
                { return new T(params); }
        };

        struct GameStateInfo
        {
            std::string stateName;
            std::string className;
            bool bIgnoreTickTime;
            bool bGraphicsMode;
        };

        struct StatisticsTickInfo
        {
            uint64_t    tickTime;
            uint32_t    tickLength;
        };

        Game(Game&); // don't mess with singletons

        void loadState(GameState* state);
        void unloadState(GameState* state);

        std::map<std::string, GameState*>    gameStates_;
        std::vector<GameState*>              activeStates_;
        boost::shared_ptr<GameStateTreeNode> rootStateNode_;
        boost::shared_ptr<GameStateTreeNode> activeStateNode_;
        std::vector<boost::shared_ptr<GameStateTreeNode> > requestedStateNodes_;

        Core*                           core_;
        Clock*                          gameClock_;
        GameConfiguration*              configuration_;

        bool                            bChangingState_;
        bool                            bAbort_;

        // variables for time statistics
        uint64_t                        statisticsStartTime_;
        std::list<StatisticsTickInfo>   statisticsTickTimes_;
        uint32_t                        periodTime_;
        uint32_t                        periodTickTime_;
        float                           avgFPS_;
        float                           avgTickTime_;

        static std::map<std::string, GameStateInfo> gameStateDeclarations_s;
        static Game* singletonRef_s;        //!< Pointer to the Singleton
    };

    template <class T>
    /*static*/ bool Game::declareGameState(const std::string& className, const std::string& stateName, bool bIgnoreTickTime, bool bGraphicsMode)
    {
        std::map<std::string, GameStateInfo>::const_iterator it = gameStateDeclarations_s.find(getLowercase(stateName));
        if (it == gameStateDeclarations_s.end())
        {
            GameStateInfo& info = gameStateDeclarations_s[getLowercase(stateName)];
            info.stateName = stateName;
            info.className = className;
            info.bIgnoreTickTime = bIgnoreTickTime;
            info.bGraphicsMode = bGraphicsMode;
        }
        else
        {
            COUT(0) << "Error: Cannot declare two GameStates with the same name." << std::endl;
            COUT(0) << "       Ignoring second one ('" << stateName << "')." << std::endl;
        }

        // Create a factory to delay GameState creation
        GameStateFactory::createFactory<T>(className);

        // just a required dummy return value
        return true;
    }
}

#endif /* _Game_H__ */