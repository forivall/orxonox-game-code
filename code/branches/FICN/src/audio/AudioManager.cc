/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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
 *      ...
 *   Co-authors:
 *      ...
 *
 */

#include "AudioManager.h"



namespace audio
{
	AudioManager::AudioManager()
	{
    ambientPath = "audio/ambient";

    alutInit(NULL, 0);


	}

	AudioManager::~AudioManager()
	{
		for (unsigned int i=0;i<=bgSounds.size();i++)
		{
			bgSounds[i].release();
		}
		alutExit();
	}

	void AudioManager::ambientStart()
	{
		currentBgSound = 0;
		if (bgSounds.size() > 0)
		{
			if(!bgSounds[currentBgSound].playback())
			{
    		orxonox::Error("Ogg refused to play.");
			}
			else
			{
				std::cout << "Started playing background sound"<<std::endl;
			}
		}

	}

	void AudioManager::ambientStop()
	{
		std::cout << "Stopped playing background sound"<<std::endl;
	}

	void AudioManager::ambientAdd(std::string file)
	{
    std::string path = ambientPath + "/" + file + ".ogg";
		AudioStream tmp(path);
		tmp.open();
		if (tmp.isLoaded())
		{
			bgSounds.push_back(tmp);
			std::cout << "Added background sound "<<file<<std::endl;
		}
	}

	void AudioManager::update()
	{
		if (bgSounds.size() > 0)
		{
			if (bgSounds[currentBgSound].isLoaded())
			{
				bool playing = bgSounds[currentBgSound].update();
		    if(!bgSounds[currentBgSound].playing() && playing)
		    {
		        if(!bgSounds[currentBgSound].playback())
		            orxonox::Error("Ogg abruptly stopped.");
		        else
		            orxonox::Error("Ogg stream was interrupted.");

		    }
				if (!playing)
				{
// 					if (currentBgSound < bgSounds.size()-1)
// 					{
// 						currentBgSound++;
// 					}
// 					else
// 					{
// 						currentBgSound=0;
// 					}
          // switch to next sound in list/array
          currentBgSound = ++currentBgSound % bgSounds.size();

					if (!bgSounds[currentBgSound].isLoaded())
					{
						bgSounds[currentBgSound].release();
						bgSounds[currentBgSound].open();
					}
					bgSounds[currentBgSound].playback();
					std::cout << "Playing next background sound "<<std::endl;
				}
			}
		}
	}

	void AudioManager::setPos(std::vector<float> newPosition)
	{

	}

	void AudioManager::setSpeed(std::vector<float> newSpeed)
	{

	}

	void AudioManager::setOri(std::vector<float> at, std::vector<float> up)
	{

	}
}
