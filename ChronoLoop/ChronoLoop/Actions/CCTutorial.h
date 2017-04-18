#pragma once
#include "CodeComponent.hpp"
#include "../Common/Settings.h"
#include <vector>
#include "../Objects/BaseObject.h"
#include "../Objects/MeshComponent.h"
#include "../Objects/Component.h"
#include "../Actions/CCProgressBar.h"
#include "../Input/VRInputManager.h"

namespace Epoch
{
	
	struct CCTutorial : public CodeComponent
	{
		std::vector<BaseObject*> boards;
		BaseObject* button, *stand, *exit;
		float scaleX, scaleY;
		float tempScaleX, tempScaleY;
		bool scalingDone;
		CCProgessBar* pb;

		PSAnimatedMultiscan_Data mScanlineData;
		
		virtual void Start()
		{
			mScanlineData.DiffuseAlpha = 0.9f;
			mScanlineData.MultiscanAlpha = 0.9f;
			mScanlineData.ScanlineAlpha = 0.9f;

			scalingDone = false;
			button = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("Button");
			((ButtonCollider*)button->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Disable();
			((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->Disable();
			((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);

			exit = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("Exit");
			((MeshComponent*)exit->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);

			stand = LevelManager::GetInstance().GetCurrentLevel()->FindObjectWithName("Buttonstand");
			((CubeCollider*)stand->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Disable();
			((MeshComponent*)stand->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);

			Settings::GetInstance().SetInt("tutStep", 1);
			Transform identity;
			//Teleport Initialize
			BaseObject* mTeleportBoard = new BaseObject("mTeleportBoard", identity);
			MeshComponent* tm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			tm->AddTexture("../Resources/tutTeleport.png", eTEX_DIFFUSE);
			tm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			tm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			tm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			tm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			tm->SetVisible(false);
			mTeleportBoard->AddComponent(tm);
			boards.push_back(mTeleportBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mTeleportBoard);

			//Pick Up Initialize
			BaseObject* mPickUpBoard = new BaseObject("mPickUpBoard", identity);
			MeshComponent* pum = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			pum->AddTexture("../Resources/tutPickUp.png", eTEX_DIFFUSE);
			pum->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			pum->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			pum->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			pum->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			pum->SetVisible(false);
			mPickUpBoard->AddComponent(pum);
			boards.push_back(mPickUpBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPickUpBoard);

			//Pause Time Initialize
			BaseObject* mPauseTimeBoard = new BaseObject("mPauseTimeBoard", identity);
			MeshComponent* ptm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			ptm->AddTexture("../Resources/tutPauseTime.png", eTEX_DIFFUSE);
			ptm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ptm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ptm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ptm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ptm->SetVisible(false);
			mPauseTimeBoard->AddComponent(ptm);
			boards.push_back(mPauseTimeBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mPauseTimeBoard);

			//Rewind Initialize
			BaseObject* mRewindBoard = new BaseObject("mRewindBoard", identity);
			MeshComponent* rm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			rm->AddTexture("../Resources/tutRewind.png", eTEX_DIFFUSE);
			rm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			rm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			rm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			rm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			rm->SetVisible(false);
			mRewindBoard->AddComponent(rm);
			boards.push_back(mRewindBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mRewindBoard);
			pb = new CCProgessBar();
			Settings::GetInstance().SetFloat("TutorialRewind - CurProgress", 0);
			Settings::GetInstance().SetFloat("TutorialRewind - FinalProgress", 50);
			pb->SetFinalProgress(50);
			pb->SetCurProgress(0);
			mRewindBoard->AddComponent(pb);
			

			//Create Clone Initialize
			BaseObject* mCreateCloneBoard = new BaseObject("mCreateCloneBoard", identity);
			MeshComponent* ccm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			ccm->AddTexture("../Resources/tutClone.png", eTEX_DIFFUSE);
			ccm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			ccm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			ccm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			ccm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			ccm->SetVisible(false);
			mCreateCloneBoard->AddComponent(ccm);
			boards.push_back(mCreateCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCreateCloneBoard);

			//Accept Time Initialize
			BaseObject* mAcceptBoard = new BaseObject("mAcceptBoard", identity);
			MeshComponent* am = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			am->AddTexture("../Resources/tutAcceptTime.png", eTEX_DIFFUSE);
			am->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			am->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			am->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			am->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			am->SetVisible(false);
			mAcceptBoard->AddComponent(am);
			boards.push_back(mAcceptBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mAcceptBoard);

			//Cancel Time Initialize
			BaseObject* mCancelBoard = new BaseObject("mCancelBoard", identity);
			MeshComponent* cm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			cm->AddTexture("../Resources/tutCancelTime.png", eTEX_DIFFUSE);
			cm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			cm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			cm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			cm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			cm->SetVisible(false);
			mCancelBoard->AddComponent(cm);
			boards.push_back(mCancelBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mCancelBoard);

			//Delete Clone Initialize
			BaseObject* mDeleteCloneBoard = new BaseObject("mDeleteCloneBoard", identity);
			MeshComponent* dcm = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			dcm->AddTexture("../Resources/tutDeleteClone.png", eTEX_DIFFUSE);
			dcm->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			dcm->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			dcm->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			dcm->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			dcm->SetVisible(false);
			mDeleteCloneBoard->AddComponent(dcm);
			boards.push_back(mDeleteCloneBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mDeleteCloneBoard);

			//UI Initialize
			BaseObject* mUIBoard = new BaseObject("mUIBoard", identity);
			MeshComponent* uim = new MeshComponent("../Resources/PlaneCorrection.obj", .75f);
			uim->AddTexture("../Resources/tutUI.png", eTEX_DIFFUSE);
			uim->AddTexture("../Resources/MultiscanUneven.png", eTEX_REGISTER4);
			uim->AddTexture("../Resources/Scanline.png", eTEX_REGISTER5);
			uim->SetData(eCB_PIXEL, eBufferDataType_Scanline, ePB_REGISTER1, &mScanlineData);
			uim->SetPixelShader(ePS_TRANSPARENT_SCANLINE);
			uim->SetVisible(false);
			mUIBoard->AddComponent(uim);
			boards.push_back(mUIBoard);
			LevelManager::GetInstance().GetCurrentLevel()->AddObject(mUIBoard);

			//Pause Menu Initialize
			//BaseObject* mPauseMenuBoard = new BaseObject("mPauseMenuBoard", identity);
			//MeshComponent* pmm = new MeshComponent("../Resources/tutPauseMenu.png");
			//pmm->SetVisible(false);
			//mPauseMenuBoard->AddComponent(tm);

			//Settings Initialize
			//Settings::GetInstance().SetBool("tutTeleport", false);
			//Settings::GetInstance().SetBool("tutPickUp", false);
			//Settings::GetInstance().SetBool("tutPause", false);
			//Settings::GetInstance().SetBool("tutRewind", false);
			//Settings::GetInstance().SetBool("tutAcceptCancelTime", false);
			//Settings::GetInstance().SetBool("tutUI", false);
			//Settings::GetInstance().SetBool("tutCreateClone", false);
			//Settings::GetInstance().SetBool("tutDeleteClone", false);
			//Settings::GetInstance().SetBool("tutPauseMenu", false);

		}

		virtual void Update()
		{
			bool thing = ((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible();
			int t = Settings::GetInstance().GetInt("tutStep");
			switch (Settings::GetInstance().GetInt("tutStep"))
			{
			case 1: //Teleport
				if(!((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible())
				{
					((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);

					//boards[0]->GetTransform().SetMatrix(matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateTranslation(2, 1.5f, 0));
					scaleX = 0.05f;
					scaleY = 0;
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (scaleY < 1.0f)
					scaleY += 0.1f;
				else if(scaleX < 1.0f)
					scaleX += 0.05f;

				if(tempScaleX != scaleX || tempScaleY != scaleY)
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX,1, scaleY) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateTranslation(2, 1.5f, 0));

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 2://Pick Up
				if (!((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[0]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateTranslation(2, 1.5f, 0));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[0]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				}
				else
				{
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY)
						boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 3://Pause Time
				if (!((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					//((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(0, 1, 0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					//boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));

					boards[1]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateZRotation(-1.5708f) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(0.785398f) * matrix4::CreateTranslation(-3.55f, 1.7f, 3.71f));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[1]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY)
						boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 4://Rewind
				if (!((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					//((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[3]->GetTransform().SetMatrix(matrix4::CreateScale(0, 1, 0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					pb->GetProgressBar()->GetTransform().SetMatrix(pb->GetProgressBar()->GetTransform().GetMatrix() * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 0.6f, 0));
					pb->OnEnable();
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[2]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[2]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						//pb->GetProgressBar()->GetTransform().SetMatrix(pb->GetProgressBar()->GetTransform().GetMatrix() * matrix4::CreateScale(scaleX, 1, scaleY)); 
						boards[3]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				pb->SetCurProgress(Settings::GetInstance().GetFloat("TutorialRewind - CurProgress"));
				break;
			case 5://Creating Clone
				if (!((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					//((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					boards[8]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
					pb->OnDisable();
				}
				tempScaleX = scaleX;
				tempScaleY = scaleY;
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[3]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[3]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
						boards[8]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 6://Accepting Time / Canceling Time
				if (!((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					//((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					//((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					boards[6]->GetTransform().SetMatrix(matrix4::CreateScale(0,1,0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
				}
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[4]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[4]->GetTransform().GetMatrix());
					boards[8]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[8]->GetTransform().GetMatrix());
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[4]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						((MeshComponent*)boards[8]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
						boards[6]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 1.1f));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);
				((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 7://Delete Clone
				if (!((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible()) {
					//((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					//((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
					scaleX = 1.0f;
					scaleY = 1.0f;
					boards[7]->GetTransform().SetMatrix(matrix4::CreateScale(0, 1, 0) * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
				}
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[5]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[5]->GetTransform().GetMatrix());
					boards[6]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[6]->GetTransform().GetMatrix());
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[5]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						((MeshComponent*)boards[6]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				} else {
					if (scaleY < 1.0f)
						scaleY += 0.05f;
					else if (scaleX < 1.0f)
						scaleX += 0.05f;
					if (tempScaleX != scaleX || tempScaleY != scaleY) {
						boards[7]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY)  * matrix4::CreateXRotation(1.5708f) * matrix4::CreateYRotation(-1.5708f) * matrix4::CreateTranslation(4.5f, 1.7f, 0));
					}
				}

				mScanlineData.MultiscanVOffset += TimeManager::Instance()->GetDeltaTime() / 25.0f;
				mScanlineData.ScanlineVOffset += TimeManager::Instance()->GetDeltaTime();
				if (mScanlineData.ScanlineVOffset > 2.5f) {
					mScanlineData.ScanlineVOffset = -0.5f;
				}
				((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->UpdateData(eCB_PIXEL, ePB_REGISTER1, &mScanlineData);

				break;
			case 8://End
				if (((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->IsVisible() && !LevelManager::GetInstance().GetCurrentLevel()->GetTimeManipulator()->isTimePaused()) {
					//((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
					((ButtonCollider*)button->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Enable();
					((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->Enable();
					((MeshComponent*)button->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((CubeCollider*)stand->GetComponentIndexed(eCOMPONENT_COLLIDER, 0))->Enable();
					((MeshComponent*)stand->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					((MeshComponent*)exit->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(true);
					scalingDone = false;
				}
				if (!scalingDone) {
					if (scaleX >= 0.05f)
						scaleX -= 0.05f;
					else if (scaleY >= 0.0f)
						scaleY -= 0.1f;
					boards[7]->GetTransform().SetMatrix(matrix4::CreateScale(scaleX, 1, scaleY) * boards[7]->GetTransform().GetMatrix());
					if (scaleY <= 0.0f) {
						((MeshComponent*)boards[7]->GetComponentIndexed(eCOMPONENT_MESH, 0))->SetVisible(false);
						scalingDone = true;
					}
				}
				break;
			}
		}

		virtual void OnDestroy()
		{
			Settings::GetInstance().SetInt("tutStep", 0);
		}
	};

}