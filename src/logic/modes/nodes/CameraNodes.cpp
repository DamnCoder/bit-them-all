/*
 * RandomPathCamera.cpp
 *
 *  Created on: 09/04/2011
 *      Author: jorge
 */

#include "CameraNodes.h"

#include "logic/Game.h"
#include "logic/entitys/Scenario.h"
#include "gui/cam/AbstractCamera.h"

namespace logic {

	BEHAVIOR_STATUS CCameraNode::execute(void* agent)
	{
		_data->game->addCamera(_camId);
		//std::cout<<"Añadida camara "<<_camId<<"\n";
		return BT_SUCCESS;
	}

	void CCameraNode::init(void* agent)
	{
		//std::cout<<"Iniciando nodo CameraNode\n";
		_data = (tGameData*)agent;
	}

//----------------------------------------------------------------------------------------------

	BEHAVIOR_STATUS CClearCameraQueue::execute(void* agent){
		_data->game->clearCameras();
		//std::cout<<"Limpiada la cola de camaras\n";
		return BT_SUCCESS;
	}

	void CClearCameraQueue::init(void* agent)
	{
		//std::cout<<"Iniciando nodo Clear Cameras Node\n";
		_data = (tGameData*)agent;
	}

//----------------------------------------------------------------------------------------------

	CRandomPathCameras::CRandomPathCameras(int numCameras):
			_numCameras(numCameras)
	{
	}

	CRandomPathCameras::~CRandomPathCameras() {
	}

	BEHAVIOR_STATUS CRandomPathCameras::execute(void* agent){

		if (!_executed){
			std::vector<int> randomNum = getNonRepeatedRandomNumbers(_numCameras, (int)_cameras.size());
			//std::cout<<"[CRandomPathCamera::execute] Camaras ";
			for(uint i=0; i<randomNum.size(); ++i){
				//std::cout<<_cameras[randomNum[i]]->id()<<" ";
				_data->game->addCamera(_cameras[randomNum[i]]->id());
			}

			//std::cout<<"\n";
			_executed = true;
			_data->game->setCameraActive(true);
			return BT_RUNNING;
		}

		if(_data->game->cameraActive()){
			return BT_RUNNING;
		}

		//std::cout<<"[CRandomPathCamera::execute] Terminadas las camaras\n";

		return BT_SUCCESS;
	}

	void CRandomPathCameras::init(void* agent){
		//std::cout<<"Iniciando nodo RandomPathCamera\n";

		_data = (tGameData*)agent;

		// Recupero camaras y selecciono el numero de camaras definido al azar
		_cameras = _data->game->scenario()->getCameras();
	}



}
