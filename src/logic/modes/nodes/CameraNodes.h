/*
 * RandomPathCamera.h
 *
 *  Created on: 09/04/2011
 *      Author: jorge
 */

#ifndef RANDOMPATHCAMERA_H_
#define RANDOMPATHCAMERA_H_

#include "BtNode.h"

namespace gui{
	class CAbstractCamera;
}

namespace logic {

	/**
	 * Añade al gestor de camaras una camara a partir
	 * de su ID.
	 */
	class CCameraNode : public CGameNode {
	public:
		CCameraNode(const std::string& camId):_camId(camId){}
		~CCameraNode(){}

		BEHAVIOR_STATUS execute(void*);
		void 			init(void*);
	private:
		std::string _camId;
	};

	/**
	 * Limpia la lista de camaras que el gestor de
	 * camaras tenga en la lista de camaras a usar.
	 */
	class CClearCameraQueue : public CGameNode {
	public:
		CClearCameraQueue(){}
		~CClearCameraQueue(){}

		BEHAVIOR_STATUS execute(void*);
		void 			init(void*);
	};

	/**
	 * Manda al gestor de camaras un numero aleatorio
	 * de camaras, especificado en el constructor, y
	 * las pone en marcha.
	 */
	class CRandomPathCameras : public CGameNode {
	public:
		CRandomPathCameras(int numCameras);
		~CRandomPathCameras();

		BEHAVIOR_STATUS execute(void*);
		void 			init(void*);

	private:

		bool	_executed;
		int 	_numCameras;
		std::vector<gui::CAbstractCamera*> _cameras;
	};

	/**
	 * Elige una camara al azar para el presentador
	 */
	class CRandomHostCamera : public CGameNode {
	public:
		CRandomHostCamera() {}
		~CRandomHostCamera() {}

	};

}

#endif /* RANDOMPATHCAMERA_H_ */
