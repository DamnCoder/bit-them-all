/*
 * PlaySongNode.cpp
 *
 *  Created on: 12/04/2011
 *      Author: jorge
 */

#include "AudioNodes.h"

#include "audio/SdlAudio.h"

namespace logic {

	void CAudioNode::init(void* agent) {
		_data = (tGameData*)agent;
		_audio = audio::CAudioManager::pointer();
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CPlaySongNode::execute(void* agent)
	{
		_audio->playSong(_id, false);
		//std::cout<<"[CPlaySongNode::execute] Tocamos cancion "<<_songId<<"\n";
		return BT_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CPlayLoopSongNode::execute(void* agent)
	{
		_audio->playSong(_id, true);
		//std::cout<<"[CPlayLoopSongNode::execute] Tocamos cancion "<<_songId<<"\n";
		return BT_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CStopSongs::execute(void* agent)
	{
		_audio->stop();
		//std::cout<<"[CStopSongs::execute] Paramos audio\n";
		return BT_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CPlayEffect::execute(void* agent)
	{
		_audio->playEffect(_id, 1);
		//std::cout<<"[CPlayEffect::execute] Usamos efecto "<<_effectId<<"\n";
		return BT_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CPlayDialogue::execute(void* agent)
	{
		_audio->playDialogue(_id);
		//std::cout<<"[CPlayDialogue::execute] Dialogo"<<_id<<"\n";
		return BT_SUCCESS;
	}

	//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CFadeOutChannel::execute(void* agent)
	{
		_audio->fadeOutChannel(_channel, _millisecs);
		//std::cout<<"[CFadeOutChannel::execute] Fade out en canal"<<_channel<<"\n";
		return BT_SUCCESS;
	}

}
