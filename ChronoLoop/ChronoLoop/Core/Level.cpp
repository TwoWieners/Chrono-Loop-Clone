#include "stdafx.h"
#include "Level.h"
#include "../Actions/TeleportAction.hpp"
#include "../Actions/CCElasticReactionWithPlane.h"
#include "../Actions/BoxSnapToControllerAction.hpp"
#include "../Actions/CCElasticSphereToSphere.h"
#include "../Actions/CCElasticAABBtoAABB.h"
#include "../Actions/CCElasticAABBToSphere.h"
#include "../Actions/TimeManipulation.h"
#include "../Actions/HeadsetFollow.hpp"
#include "../Actions\CodeComponent.hpp"
#include "../Actions/CCButtonPress.h"
#include "../Actions\CCEnterLevel.h"
#include "../Actions/MainMenuBT.h"
#include "../Objects/MeshComponent.h"
#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"

namespace Epoch {
	
	Level::Level() {}

	Level::~Level() {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			delete *it;
		}
		mObjectList.clear();

		//for (auto iter = mObjectMap.begin(); iter != mObjectMap.end(); ++iter) { 
		//	for (int i = 0; i < iter->second.size(); ++i) {
		//		// TODO: Put objects back in the object pool.
		//		// Since this is a singleton and this is only destroyed when closing 
		//		// the program, would it be more efficient to just delete them directly?
		//		delete iter->second[i];
		//	}
		//}
		//mObjectMap.clear();
	}

	void Level::Initialize(BaseObject * _headset, BaseObject * _lController, BaseObject * _rController) {
		mHeadset = _headset;
		mController1 = _lController;
		mController2 = _rController;
		//CommandConsole::Instance().AddCommand(L"/WIREFRAME", ToggleEntireLevelsWireframe);
		std::vector<Component*> codes1 = mController1->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		for (size_t x = 0; x < codes1.size(); ++x) {
			if (dynamic_cast<TimeManipulation*>(codes1[x])) {
				mTMComponent1 = ((TimeManipulation*)codes1[x]);
				break;
			}
		}

		std::vector<Component*> codes2 = mController2->GetComponents(Epoch::ComponentType::eCOMPONENT_CODE);
		for (size_t x = 0; x < codes2.size(); ++x) {
			if (dynamic_cast<TimeManipulation*>(codes2[x])) {
				mTMComponent2 = ((TimeManipulation*)codes2[x]);
				break;
			}
		}
	}

	BaseObject * Level::FindObjectWithName(std::string _name) {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName() == _name) {
				return *it;
			}
		}
		return nullptr;
	}

	std::vector<BaseObject*> Level::FindAllObjectsWithName(std::string _name) {
		std::vector<BaseObject*> objects;
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it)->GetName() == _name) {
				objects.push_back(*it);
			}
		}
		return objects;
	}

	void Level::AddObject(BaseObject * _obj) {
		mObjectList.push_back(_obj);
	}

	bool Level::RemoveObject(BaseObject * _obj) {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			if ((*it) == _obj) {
				mObjectList.erase(it);
				return true;
			}
		}
		return false;
	}

	void Level::SetHeadsetAndControllers(BaseObject *& _headset, BaseObject *& _controller1, BaseObject *& _controller2, ControllerCollider* _c1Collider, ControllerCollider* _c2Collider) {
		//Remove the action componets and 
		//Set the new BaseObjects to the current controller so new objects can follow old controller movement as clones.
		unsigned short headid = _headset->GetUniqueID();
		unsigned short cl1id = _controller1->GetUniqueID();
		unsigned short cl2id = _controller2->GetUniqueID();
		unsigned short c1paramCodeCollid = _c1Collider->GetColliderId();
		unsigned short c2paramCodeCollid = _c2Collider->GetColliderId();

		Component* controller1Collider = mController1->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);
		Component* controller2Collider = mController2->GetComponentIndexed(eCOMPONENT_COLLIDER, 0);

		std::string headname = _headset->GetName();
		std::string Controller1name = _controller1->GetName();
		std::string Controller2name = _controller2->GetName();

		//mObjectMap[_headset->GetName()].push_back(_headset);
		//mObjectMap[_controller1->GetName()].push_back(_controller1);
		//mObjectMap[_controller2->GetName()].push_back(_controller2);

		_headset->SetUniqueID(mHeadset->GetUniqueID());
		_controller1->SetUniqueID(mController1->GetUniqueID());
		_controller2->SetUniqueID(mController2->GetUniqueID());
		_headset->SetName(mHeadset->GetName());
		_controller1->SetName(mController1->GetName());
		_controller2->SetName(mController2->GetName());
		_c1Collider->SetComponentId(controller1Collider->GetColliderId());
		_c2Collider->SetComponentId(controller2Collider->GetColliderId());

		mHeadset->SetUniqueID(headid);
		mController1->SetUniqueID(cl1id);
		mController2->SetUniqueID(cl2id);
		mHeadset->SetName(headname);
		mController1->SetName(Controller1name);
		mController2->SetName(Controller2name);
		controller1Collider->SetComponentId(c1paramCodeCollid);
		controller2Collider->SetComponentId(c2paramCodeCollid);

		mHeadset->SetUniqueID(headid);
		mController1->SetUniqueID(cl1id);
		mController2->SetUniqueID(cl2id);
		mHeadset->SetName(headname);
		mController1->SetName(Controller1name);
		mController2->SetName(Controller2name);
		controller1Collider->SetComponentId(c1paramCodeCollid);
		controller2Collider->SetComponentId(c2paramCodeCollid);

		mObjectList.push_back(_headset);
		mObjectList.push_back(_controller1);
		mObjectList.push_back(_controller2);

		////Add the headset and controllers to the time manager with their new ids
		TimeManager::Instance()->AddObjectToTimeline(mHeadset);
		TimeManager::Instance()->AddObjectToTimeline(mController1);
		TimeManager::Instance()->AddObjectToTimeline(mController2);

	}

	void Level::CallStart() {
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			auto codes = (*it)->GetComponents(eCOMPONENT_CODE);
			for (auto oit = codes.begin(); oit != codes.end(); ++oit) {
				((CodeComponent*)*oit)->Start();
			}
		}
	}

	void Level::LoadLevel(std::string _file) 
	{
		// Load the xml file, I put your XML in a file named test.xml
		TiXmlDocument XMLdoc(_file.c_str());
		bool loadOkay = XMLdoc.LoadFile();
		if (loadOkay)
		{
			SystemLogger::GetLog() << _file.c_str() << " loaded" << std::endl;
			TiXmlElement *pRoot, *pObject, *pData, *pApp, *pLineFormat;
			pRoot = XMLdoc.FirstChildElement("Level");
			if (pRoot)
			{
				// Parse objects
				pObject = pRoot->FirstChildElement("Object");
				while (pObject)
				{
					std::vector<std::string> codeComs;
					std::string elementType, name, meshFile, textureFile, colliderType;
					vec4f position, rotation, scale, colliderPosition, colliderScale, normal, pushNorm, gravity(0, -9.81f, 0, 1);
					float mass, elasticity, staticF, kineticF, normF, drag, radius;
					bool collider = false, trigger = false, canMove, physical = false;
					pData = pObject->FirstChildElement();
					while (pData)
					{
						switch (pData->Type())
						{
						case TiXmlNode::NodeType::TINYXML_ELEMENT:
							SystemLogger::GetLog() << "Element: '" << pData->Value() << "'" << std::endl;
							elementType = std::string(pData->Value());
							if (elementType == "Collider")
								collider = true;
							pData = (TiXmlElement*)pData->FirstChild();
							break;
						case TiXmlNode::NodeType::TINYXML_TEXT:
							SystemLogger::GetLog() << "Value: '" << pData->Value() << "'" << std::endl;
							if (elementType == "Name")
								name = pData->Value();
							else if (elementType == "Mesh")
								meshFile = pData->Value();
							else if (elementType == "Texture")
								textureFile = pData->Value();
							else if (elementType == "Position")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									if (collider)
										colliderPosition.xyzw[i] = std::strtof(token.c_str(), nullptr);
									else
										position.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Rotation")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									rotation.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Scale")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									if (collider)
										colliderScale.xyzw[i] = std::strtof(token.c_str(), nullptr);
									else
										scale.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Move")
							{
								std::string temp(pData->Value());
								canMove = temp.find("True") != std::string::npos;
								int thing = 0;
							}
							else if (elementType == "Type")
								colliderType = pData->Value();
							else if (elementType == "Trigger")
								trigger = pData->Value() == "True";
							else if (elementType == "Radius")
							{
								radius = std::strtof(pData->Value(), nullptr);
								colliderScale = vec4f(radius, radius, radius, 1);
							}
							else if (elementType == "Mass")
								mass = std::strtof(pData->Value(), nullptr);
							else if (elementType == "Elasticity")
								elasticity = std::strtof(pData->Value(), nullptr);
							else if (elementType == "StaticFriction")
								staticF = std::strtof(pData->Value(), nullptr);
							else if (elementType == "KeneticFriction")
								kineticF = std::strtof(pData->Value(), nullptr);
							else if (elementType == "Drag")
							{
								drag = std::strtof(pData->Value(), nullptr);
								//if (pData->Parent()->Parent()->NextSiblingElement())
								//	pData = pData->Parent()->Parent()->NextSiblingElement();
								//else
								//	pData = nullptr;
							}
							else if (elementType == "Normal")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									normal.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "PushNormal")
							{
								size_t pos = 0;
								int i = 0;
								std::string s = std::string(pData->Value()) + ',';
								while ((pos = s.find(",")) != std::string::npos)
								{
									std::string token = s.substr(0, pos);
									pushNorm.xyzw[i] = std::strtof(token.c_str(), nullptr);
									i++;
									s.erase(0, pos + 1);
								}
							}
							else if (elementType == "Gravity")
							{
								//size_t pos = 0;
								//int i = 0;
								//std::string s = std::string(pData->Value()) + ',';
								//while ((pos = s.find(",")) != std::string::npos)
								//{
								//	std::string token = s.substr(0, pos);
								//	gravity.xyzw[i] = std::strtof(token.c_str(), nullptr);
								//	gravity.w = 1;
								//	i++;
								//	s.erase(0, pos + 1);
								//}
							}
							else if(elementType == "NormalForce")
								normF = std::strtof(pData->Value(),nullptr);
							else
								codeComs.push_back(elementType);

							if (pData != nullptr)
								pData = pData->Parent()->NextSiblingElement();
							break;
						default:
							break;
						}
					}
					SystemLogger::GetLog() << "Element: name= '" << pObject->Value() << "'" << std::endl;

					
					Transform transform;
					matrix4 mat = matrix4::CreateScale(scale.x, scale.y, scale.z) * 
								  matrix4::CreateXRotation(rotation.x) * 
								  matrix4::CreateYRotation(rotation.y) * 
								  matrix4::CreateZRotation(rotation.z) * 
								  matrix4::CreateTranslation(position.x, position.y, position.z);
					transform.SetMatrix(mat);
					BaseObject* obj = new BaseObject(name, transform);

					if (!meshFile.empty())
					{
						std::string path = "../Resources/";
						path.append(meshFile);
						MeshComponent* mesh = new MeshComponent(path.c_str());
						path = "../Resources/";
						path.append(textureFile);
						mesh->AddTexture(path.c_str(), eTEX_DIFFUSE);
						obj->AddComponent(mesh);
					}

					if (colliderType == "OBB")
					{
						physical = true;
						vec4f min = colliderPosition - vec4f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z, 1);
						vec4f max = colliderPosition + vec4f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z, 1);
						CubeCollider* col = new CubeCollider(obj, canMove, trigger, gravity, mass, elasticity, staticF, kineticF, drag, min, max);
						obj->AddComponent(col);
					}
					else if (colliderType == "Sphere")
					{
						physical = true;

						SphereCollider* col = new SphereCollider(obj, canMove, trigger, gravity, mass, elasticity, staticF, kineticF, drag, radius);
						obj->AddComponent(col);
					}
					else if (colliderType == "Button")
					{
						physical = true;

						vec4f min = colliderPosition - vec4f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z, 1);
						vec4f max = colliderPosition + vec4f(colliderScale.x * scale.x, colliderScale.y * scale.y, colliderScale.z * scale.z, 1);
						ButtonCollider* col = new ButtonCollider(obj, min, max, mass, normF, pushNorm);
						obj->AddComponent(col);
					}
					else if (colliderType == "Plane")
					{
						physical = true;

						PlaneCollider* col = new PlaneCollider(obj, canMove, trigger, vec4f(0,0,0,0), mass, elasticity, staticF, kineticF, drag, -1, normal);//TODO: Fix offset
						obj->AddComponent(col);
					}

					for (int i = 0; i < codeComs.size(); ++i)
					{
						if (codeComs[i] == "BoxSnapToController")
						{
							BoxSnapToControllerAction* code = new BoxSnapToControllerAction();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "ButtonPress")
						{
							CCButtonPress* code = new CCButtonPress();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "AABBtoAABB")
						{
							CCElasticAABBtoAABB* code = new CCElasticAABBtoAABB();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "AABBtoSphere")
						{
							CCElasticAABBToSphere* code = new CCElasticAABBToSphere();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "ElasticPlane")
						{
							CCElasticReactionWithPlane* code = new CCElasticReactionWithPlane();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "SpheretoSphere")
						{
							CCElasticSphereToSphere* code = new CCElasticSphereToSphere();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "EnterLevel")
						{
							CCEnterLevel* code = new CCEnterLevel();
							obj->AddComponent(code);
						}
						else if (codeComs[i] == "HeadsetFollow")
						{
							HeadsetFollow* code = new HeadsetFollow();
							obj->AddComponent(code);
						}
					}

					if (physical)
						Physics::Instance()->mObjects.push_back(obj);

					AddObject(obj);
					pObject = pObject->NextSiblingElement("Object");
				}

				SystemLogger::GetLog() << std::endl;
			}
			else
			{
				SystemLogger::GetLog() << "Cannot find 'Configuration' node" << std::endl;
			}
		}
	}

	void Level::Update() {
		//*Insert Code Here When we Get to It**//
		for (auto it = mObjectList.begin(); it != mObjectList.end(); ++it) {
			(*it)->Update();
		}
	}


	void Level::ToggleEntireLevelsWireframe(void* _command, std::wstring _ifOn) {
		//CommandConsole* cc = (CommandConsole*)_command;
		//if (_ifOn == L"ON") {
		//	for (auto it = mObjectList.begin(); it != sInstance->mObjectList.end(); ++it) {
		//		for (size_t x = 0; x < (*it)->GetComponents(ComponentType::eCOMPONENT_MESH).size(); ++x) {
		//			MeshComponent* tempMComp = (MeshComponent*)((*it)->GetComponents(ComponentType::eCOMPONENT_MESH)[x]);
		//
		//			tempMComp->SetRasterState(eRS_WIREFRAME);//< - This line
		//		}
		//	}
		//	CommandConsole::Instance().DisplaySet(L"");
		//} else if (_ifOn == L"OFF") {
		//	for (auto it = mObjectList.begin(); it != sInstance->mObjectList.end(); ++it) {
		//		for (size_t x = 0; x < (*it)->GetComponents(ComponentType::eCOMPONENT_MESH).size(); ++x) {
		//			MeshComponent* tempMComp = (MeshComponent*)((*it)->GetComponents(ComponentType::eCOMPONENT_MESH)[x]);
		//			tempMComp->SetRasterState(eRS_FILLED);
		//		}
		//	}
		//	CommandConsole::Instance().DisplaySet(L"");
		//} else {
		//	CommandConsole::Instance().DisplaySet(L"INVALID INPUT: " + _ifOn + L"\nCORRECT INPUT: /WIREFRAME (ON/OFF)");
		//}
	}

} // Epoch Namespace