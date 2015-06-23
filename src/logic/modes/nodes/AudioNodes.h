/*
 * PlaySongNode.h
 *
 *  Created on: 12/04/2011
 *      Author: jorge
 */

#ifndef PLAYSONGNODE_H_
#define PLAYSONGNODE_H_

#include "BtNode.h"

namespace audio {
	class CAudioManager;
}

namespace logic {

	/**
	 * Clase nodo de un BT de juego,
	 * que permite usar el gestor de audio.
	 */
	class CAudioNode : public CGameNode {
	public:
		CAudioNode():_audio(0){}
		virtual ~CAudioNode(){}

		virtual void init(void* agent);

	protected:
		audio::CAudioManager*	_audio;
	};

	/**
	 * Clase nodo de un BT de juego
	 * que accede a algun elemento mediante
	 * un ID.
	 */
	class CAudioByIdNode : public CAudioNode {
	public:
		CAudioByIdNode(const int id) : CAudioNode(), _id(id) {}
		virtual ~CAudioByIdNode() {}
	protected:
		int _id;	/// Id de cancion o efecto
	};

	/**
	 * A partir del ID comienza la reproduccion
	 * de una cancion.
	 */
	class CPlaySongNode : public CAudioByIdNode {
	public:
		CPlaySongNode(const int songId) : CAudioByIdNode(songId){}
		BEHAVIOR_STATUS execute(void* agent);
	};

	/**
	 * A partir de un ID comienza la reproduccion
	 * de una cancion en modo bucle infinito.
	 */
	class CPlayLoopSongNode : public CAudioByIdNode {
	public:
		CPlayLoopSongNode(const int songId) : CAudioByIdNode(songId){}
		BEHAVIOR_STATUS execute(void* agent);
	};

	/**
	 * A partir del ID comienza la reproduccion
	 * de un efecto de sonido.
	 */
	class CPlayEffect : public CAudioByIdNode {
	public:
		CPlayEffect(const int effectId) : CAudioByIdNode(effectId){}
		BEHAVIOR_STATUS execute(void* agent);
	};

	/**
	 * A partir del ID comienza la reproduccion
	 * de un dialogo.
	 */
	class CPlayDialogue : public CAudioByIdNode {
	public:
		CPlayDialogue(const int dialogueId) : CAudioByIdNode(dialogueId){}
		BEHAVIOR_STATUS execute(void* agent);
	};

	/**
	 * Detiene cualquier audio que se este
	 * reproduciendo.
	 */
	class CStopSongs : public CAudioNode {
	public:
		CStopSongs() : CAudioNode() {}
		BEHAVIOR_STATUS execute(void* agent);
	};

	/**
	 * Hace un fade del audio que se este reproduciendo
	 * durante el tiempo pasado como parametro y en el
	 * canal especificado.
	 */
	class CFadeOutChannel : public CAudioNode {
	public:
		CFadeOutChannel(int channel, int ms):
			CAudioNode(),
			_channel(channel),
			_millisecs(ms){}
		BEHAVIOR_STATUS execute(void* agent);
	private:
		int _channel, _millisecs;
	};

}

#endif /* PLAYSONGNODE_H_ */
