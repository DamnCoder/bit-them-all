/*
 * CameraXmlDao.cpp
 *
 *  Created on: 22/02/2011
 *      Author: jorge
 */

#include "CameraXmlDao.h"

#include "math/Mathlib.h"
#include "gui/cam/PathCamera.h"

namespace persist {

	CCameraXmlDao::CCameraXmlDao() : CXmlDao<gui::CAbstractCamera*>() {}
	CCameraXmlDao::~CCameraXmlDao() {}

	std::vector<gui::CAbstractCamera*> CCameraXmlDao::read(){
		std::vector<gui::CAbstractCamera*> cameras;

		// Recorremos las camaras
		TiXmlElement* pCameraElement = _hRoot.FirstChild("camera").Element();

		while(pCameraElement){
			std::string camID = pCameraElement->Attribute("id");

			//std::cout<<"[CCameraXmlDao::read] Camera ID: "<<camID<<"\n";
			TiXmlHandle hCamera(pCameraElement);

			std::vector<gui::TCamKeyFrame> keyFrames;

			// Recorremos los keyframes de movimiento de la camara
			TiXmlElement* pKeyFrameElement = hCamera.FirstChild("keyframe").Element();
			while(pKeyFrameElement){
				int frame = 0;
				float speed = 0.0f;
				pKeyFrameElement->QueryIntAttribute("f",&frame);
				pKeyFrameElement->QueryFloatAttribute("speed",&speed);
				//std::cout<<"Frame: "<<frame<<"\n";

				TiXmlHandle hKeyFrame(pKeyFrameElement);
				TiXmlElement* pPositionElement = hKeyFrame.FirstChild("pos").Element();
				TiXmlElement* pLookAtElement = hKeyFrame.FirstChild("lookat").Element();
				TiXmlElement* pQuaternionElement = hKeyFrame.FirstChild("quat").Element();

				float x, y, z, w;
				pPositionElement->QueryFloatAttribute("x", &x);
				pPositionElement->QueryFloatAttribute("y", &y);
				pPositionElement->QueryFloatAttribute("z", &z);
				math::CVector3f pos(x, y, z);

				pLookAtElement->QueryFloatAttribute("x", &x);
				pLookAtElement->QueryFloatAttribute("y", &y);
				pLookAtElement->QueryFloatAttribute("z", &z);
				math::CVector3f lookAt(x, y, z);

				pQuaternionElement->QueryFloatAttribute("x", &x);
				pQuaternionElement->QueryFloatAttribute("y", &y);
				pQuaternionElement->QueryFloatAttribute("z", &z);
				pQuaternionElement->QueryFloatAttribute("w", &w);
				math::CQuaternionf quaternion(w, x, y, z);

				gui::TCamKeyFrame camKeyFrame;
				camKeyFrame.speed = 0.1f;//speed;
				camKeyFrame.position = pos;
				camKeyFrame.lookAt = lookAt;
				camKeyFrame.quaternion = quaternion;
				keyFrames.push_back(camKeyFrame);

				pKeyFrameElement = pKeyFrameElement->NextSiblingElement("keyframe");
			}

			cameras.push_back(new gui::CPathCamera(camID, keyFrames));

			pCameraElement = pCameraElement->NextSiblingElement("camera");
		}

		return cameras;
	}


}
