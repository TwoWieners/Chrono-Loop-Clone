#pragma once
#include "CodeComponent.hpp"
#include "..\Common\Logger.h"
#include "..\Core\LevelManager.h"
#include "..\Messager\Messager.h"
#include "..\Core\Pool.h"
#include "..\Core\TimeManager.h"
#include "..\Common\Settings.h"
#include "..\Actions\UIRewind.h"
#include "..\Actions\CCPauseToCancel.h"
#include "..\Actions\CCTeleToPlay.h"
#include "..\Actions\CCDisplayOnPause.h"
#include "..\Actions\UICreateToDeleteClone.h"
#include "..\Actions\UIClonePlusToMinus.h"
#include "..\Actions\UICloneText.h"
#include "..\Actions\BoxSnapToControllerAction.hpp"
#include "..\Actions\TeleportAction.hpp"
#include "..\Rendering\Draw2D.h"
#include "..\Rendering\Renderer.h"
#include "..\Rendering\TextureManager.h"
#include "..\Objects\TransparentMeshComponent.h"
#include <wrl\client.h>


namespace Epoch
{

	struct CCEnterLevel1 : public CodeComponent
	{
		const wchar_t* _basePath = L"../Resources/Soundbanks/";
		const wchar_t* _initSB = L"Init.bnk";
		const wchar_t* _aSB = L"Test_Soundbank.bnk";


		bool once = true;
		void SetOnce(bool _set) { once = _set; };
		bool GetOnce() { return once; };
		virtual void OnTriggerEnter(Collider& _col1, Collider& _col2)
		{
			once = false;
		}

		virtual void Update()
		{
			if (!once)
			{
				Settings::GetInstance().SetBool("LevelIsLoading", true);
				Level* next = new Level;
				next->LoadLevel("../Resources/Level2.xml");
				// Todo: Un-hardcode this
				// use a setting string for next level path?
				//LM::LevelStatus status = LevelManager::GetInstance().LoadLevelAsync("../Resources/Level1_2_6.xml", &next);
				if (/*status == LM::LevelStatus::Success*/ true)
				{
					// Clean up the current level and request the new one be used next time.
					Physics::Instance()->PhysicsLock.lock();
					TimeManager::Instance()->Destroy();
					Physics::Instance()->mObjects.clear();
					LevelManager::GetInstance().RequestLevelChange(next);

					//Sound Initializing---------------------------------------------------
					TimeManager::Instance();

					Listener* ears = new Listener();
					Emitter* ambient = new Emitter();
					ambient->AddSoundEvent(Emitter::sfxTypes::ePlayLoop, AK::EVENTS::PLAY_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::ePauseLoop, AK::EVENTS::PAUSE_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::eResumeLoop, AK::EVENTS::RESUME_TEST2);
					ambient->AddSoundEvent(Emitter::sfxTypes::eStopLoop, AK::EVENTS::STOP_TEST2);
					Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Listener, 0, false, (void*)new m_Listener(ears, "Listener")));
					Messager::Instance().SendInMessage(new Message(msgTypes::mSound, soundMsg::ADD_Emitter, 0, false, (void*)new m_Emitter(ambient, "ambiance")));

					//new stuff
					Transform identity, t;
					t.SetMatrix(matrix4::CreateXRotation(DirectX::XM_PI / 2) * matrix4::CreateTranslation(8.8f, 1.3f, -4.75f));
					BaseObject* RightController = Pool::Instance()->iGetObject()->Reset("Controller1 - 0", t);
					BaseObject* LeftController = Pool::Instance()->iGetObject()->Reset("Controller2 - 0", identity);
					BaseObject* headset = Pool::Instance()->iGetObject()->Reset("Headset - 0", identity);
					MeshComponent *mc = new MeshComponent("../Resources/Controller.obj");

					ControllerCollider* rightConCol = new ControllerCollider(RightController, vec3f(-0.15f, -0.15f, -0.15f), vec3f(0.15f, 0.15f, 0.15f), false);
					BoxSnapToControllerAction* pickup = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup)->mControllerRole = eControllerType_Primary;
					MeshComponent *rightRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					rightRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					mc->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta = new TeleportAction(eControllerType_Primary);
					TimeManipulation* tm = new TimeManipulation(eControllerType_Primary);
					RightController->AddComponent(mc);
					RightController->AddComponent(rightConCol);
					RightController->AddComponent(pickup);
					RightController->AddComponent(rightRaycaster);
					RightController->AddComponent(ta);
					RightController->AddComponent(tm);

					BaseObject *clonePanel = Pool::Instance()->iGetObject()->Reset("ClonePanel", identity);
					MeshComponent* disp = new MeshComponent("../Resources/ClonePanel.obj");
					disp->AddTexture("../Resources/ClonePanel.png", eTEX_DIFFUSE);
					clonePanel->AddComponent(disp);
					clonePanel->SetParent(RightController);
					RightController->AddChild(clonePanel);

					t.SetMatrix(matrix4::CreateScale(.1f, .1f, .1f));
					BaseObject *timeDisplay = Pool::Instance()->iGetObject()->Reset("TimeIndicatorLine", t);
					MeshComponent* tdisp = new MeshComponent("../Resources/TimeIndicatorLine.obj");
					tdisp->AddTexture("../Resources/TimeIndicatorLine.png", eTEX_DIFFUSE);
					timeDisplay->AddComponent(tdisp);
					timeDisplay->SetParent(RightController);
					RightController->AddChild(timeDisplay);

					BaseObject *timeDisplayNeedle = Pool::Instance()->iGetObject()->Reset("TimeIndicatorNeedle", t);
					MeshComponent* tdispn = new MeshComponent("../Resources/TimeIndicator.obj");
					tdispn->AddTexture("../Resources/TimeIndicator.png", eTEX_DIFFUSE);
					timeDisplayNeedle->AddComponent(tdispn);
					timeDisplayNeedle->SetParent(RightController);
					RightController->AddChild(timeDisplayNeedle);

					t.SetMatrix(matrix4::CreateTranslation(0, .015f, .054f));
					BaseObject *rewindDisplay = Pool::Instance()->iGetObject()->Reset("RewindDisplay", t);
					MeshComponent* rewind = new MeshComponent("../Resources/UIRewind.obj");
					rewind->AddTexture("../Resources/rewind.png", eTEX_DIFFUSE);
					UIRewind* spin = new UIRewind();
					CCDisplayOnPause* rdop = new CCDisplayOnPause();
					rewindDisplay->AddComponent(rdop);
					rewindDisplay->AddComponent(spin);
					rewindDisplay->AddComponent(rewind);
					rewindDisplay->SetParent(RightController);
					RightController->AddChild(rewindDisplay);

					t.SetMatrix(matrix4::CreateScale(.75f, 1, 1) * matrix4::CreateTranslation(0.073f, -0.018f, -0.043f));
					BaseObject *cloneDisplayBack = Pool::Instance()->iGetObject()->Reset("cloneDisplayBack", t);
					TransparentMeshComponent* cdispb = new TransparentMeshComponent("../Resources/UIClone.obj");
					cdispb->AddTexture("../Resources/clearBlue.png", eTEX_DIFFUSE);
					cloneDisplayBack->AddComponent(cdispb);
					cloneDisplayBack->SetParent(RightController);
					RightController->AddChild(cloneDisplayBack);

					t.SetMatrix(matrix4::CreateTranslation(0.073f, -0.016f, -0.043f));
					BaseObject *cloneDisplay = Pool::Instance()->iGetObject()->Reset("cloneDisplay", t);
					MeshComponent* cdisp = new MeshComponent("../Resources/UIClone.obj");
					//cdisp->AddTexture("../Resources/clear.png", eTEX_DIFFUSE);

					HRESULT hr;
					Microsoft::WRL::ComPtr<ID3D11Texture2D> screenTex;
					D3D11_TEXTURE2D_DESC bitmapDesc;
					bitmapDesc.Width = 256;
					bitmapDesc.Height = 256;
					bitmapDesc.MipLevels = 1;
					bitmapDesc.ArraySize = 1;
					bitmapDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
					bitmapDesc.SampleDesc.Count = 1;
					bitmapDesc.SampleDesc.Quality = 0;
					bitmapDesc.Usage = D3D11_USAGE_DEFAULT;
					bitmapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					bitmapDesc.CPUAccessFlags = 0;
					bitmapDesc.MiscFlags = 0;


					hr = Renderer::Instance()->GetDevice()->CreateTexture2D(&bitmapDesc, nullptr, screenTex.GetAddressOf());
					Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
					std::string str("Clone Display");
					TextureManager::Instance()->iAddTexture2D(str, screenTex, &srv);
					cdisp->AddTexture(str.c_str(), eTEX_DIFFUSE);

					Font* font = new Font();
					cdisp->GetContext().mTextures[eTEX_DIFFUSE] = srv;
					UICloneText* ct = new UICloneText();
					cloneDisplay->AddComponent(ct);
					cloneDisplay->AddComponent(cdisp);
					cloneDisplay->SetParent(RightController);
					RightController->AddChild(cloneDisplay);

					t.SetMatrix(matrix4::CreateTranslation(-0.039f, 0.015f, 0.054f));
					BaseObject *rewindHelp = Pool::Instance()->iGetObject()->Reset("RewindHelp", t);
					MeshComponent* rhdisp = new MeshComponent("../Resources/help.obj");
					rhdisp->SetVisible(false);
					rhdisp->AddTexture("../Resources/rewindHelp.png", eTEX_DIFFUSE);
					CCDisplayOnPause* dop = new CCDisplayOnPause();
					rewindHelp->AddComponent(dop);
					rewindHelp->AddComponent(rhdisp);
					rewindHelp->SetParent(RightController);
					RightController->AddChild(rewindHelp);

					t.SetMatrix(matrix4::CreateTranslation(0, 0.015f, 0.054f));
					BaseObject *teleportHelp = Pool::Instance()->iGetObject()->Reset("teleportHelp", t);
					MeshComponent* thdisp = new MeshComponent("../Resources/help.obj");
					thdisp->AddTexture("../Resources/teleport.png", eTEX_DIFFUSE);
					CCTeleToPlay* ttp = new CCTeleToPlay();
					teleportHelp->AddComponent(ttp);
					teleportHelp->AddComponent(thdisp);
					teleportHelp->SetParent(RightController);
					RightController->AddChild(teleportHelp);

					t.SetMatrix(matrix4::CreateTranslation(-0.03f, -0.01f, 0.089f));
					BaseObject *pauseHelp = Pool::Instance()->iGetObject()->Reset("pauseHelp", t);
					MeshComponent* phdisp = new MeshComponent("../Resources/help.obj");
					phdisp->AddTexture("../Resources/pause.png", eTEX_DIFFUSE);
					CCPauseToCancel* ptc = new CCPauseToCancel();
					pauseHelp->AddComponent(ptc);
					pauseHelp->AddComponent(phdisp);
					pauseHelp->SetParent(RightController);
					RightController->AddChild(pauseHelp);

					t.SetMatrix(matrix4::CreateTranslation(0.032f, -0.03f, 0.047f));
					BaseObject *cloneHelp = Pool::Instance()->iGetObject()->Reset("cloneHelp", t);
					MeshComponent* chdisp = new MeshComponent("../Resources/help.obj");
					chdisp->AddTexture("../Resources/createClone.png", eTEX_DIFFUSE);
					CCDisplayOnPause* cdop = new CCDisplayOnPause();
					UICreateToDeleteClone* cd = new UICreateToDeleteClone();
					cloneHelp->AddComponent(cd);
					cloneHelp->AddComponent(cdop);
					cloneHelp->AddComponent(chdisp);
					cloneHelp->SetParent(RightController);
					RightController->AddChild(cloneHelp);

					t.SetMatrix(matrix4::CreateScale(.5f, .5f, .5f) * matrix4::CreateTranslation(0.042f, -0.03f, 0.047f));
					BaseObject *clonePlus = Pool::Instance()->iGetObject()->Reset("clonePlus", t);
					MeshComponent* cphdisp = new MeshComponent("../Resources/help.obj");
					cphdisp->AddTexture("../Resources/createClone.png", eTEX_DIFFUSE);
					CCDisplayOnPause* cpdop = new CCDisplayOnPause();
					UIClonePlusToMinus* pm = new UIClonePlusToMinus();
					clonePlus->AddComponent(pm);
					clonePlus->AddComponent(cpdop);
					clonePlus->AddComponent(cphdisp);
					clonePlus->SetParent(RightController);
					RightController->AddChild(clonePlus);

					//pat added
					MeshComponent *mc2 = new MeshComponent("../Resources/Controller.obj");
					ControllerCollider* leftConCol = new ControllerCollider(LeftController, vec3f(-0.15f, -0.15f, -0.15f), vec3f(0.15f, 0.15f, 0.15f), true);
					BoxSnapToControllerAction* pickup2 = new BoxSnapToControllerAction();
					((BoxSnapToControllerAction*)pickup2)->mControllerRole = eControllerType_Secondary;
					MeshComponent *leftRaycaster = new MeshComponent("../Resources/RaycastCylinder.obj");
					leftRaycaster->AddTexture("../Resources/Teal.png", eTEX_DIFFUSE);
					mc2->AddTexture("../Resources/vr_controller_lowpoly_texture.png", eTEX_DIFFUSE);
					TeleportAction *ta2 = new TeleportAction(eControllerType_Secondary);
					TimeManipulation* tm2 = new TimeManipulation(eControllerType_Secondary);
					LeftController->AddComponent(mc2);
					LeftController->AddComponent(leftConCol);
					LeftController->AddComponent(pickup2);
					LeftController->AddComponent(leftRaycaster);
					LeftController->AddComponent(ta2);
					LeftController->AddComponent(tm2);

					MeshComponent *visibleMesh2 = new MeshComponent("../Resources/TinyCube.obj");
					visibleMesh2->AddTexture("../Resources/cube_texture.png", eTEX_DIFFUSE);
					visibleMesh2->SetVisible(false);
					headset->AddComponent(ambient);
					headset->AddComponent(visibleMesh2);

					AudioWrapper::GetInstance().STOP();

					ambient->Play();

					HeadsetFollow* hfollow = new HeadsetFollow();
					headset->AddComponent(hfollow);
					headset->AddComponent(ears);


					LevelManager::GetInstance().RequestLevelChange(next);


					//Enter effect
					Particle* p = &Particle::Init();
					p->SetPos(vec3f(8.88f, 0, -4.1f));
					p->SetColors(vec3f(.2f, .2f, 1), vec3f(0, 1, .2f));
					p->SetLife(200);
					p->SetSize(1.25f / 2.0f, .15f / .2f);
					ParticleEmitter* emit = new TeleportEffect(500, 250, 2, vec3f(8.88f, 0, -4.1f));
					emit->SetParticle(p);
					emit->SetTexture("../Resources/BasicRectP.png");
					((TeleportEffect*)emit)->y1 = 8;
					((TeleportEffect*)emit)->y2 = 12;
					ParticleSystem::Instance()->AddEmitter(emit);
					emit->FIRE();

					p = &Particle::Init();
					p->SetPos(vec3f(8.88f, 0, -4.1f));
					p->SetColors(vec3f(.5f, 0, .25f), vec3f(.2f, .8f, .5f));
					p->SetLife(1000);
					p->SetSize(.25f, .05f);
					emit = new TeleportEffect(500, 150, 1, vec3f(8.88f, 0, -4.1f));
					emit->SetTexture("../Resources/BasicCircleP.png");
					emit->SetParticle(p);
					((TeleportEffect*)emit)->y1 = 1;
					((TeleportEffect*)emit)->y2 = 5;
					ParticleSystem::Instance()->AddEmitter(emit);
					emit->FIRE();

					next->AssignPlayerControls(headset, LeftController, RightController);
					next->AddObject(headset);
					next->AddObject(LeftController);
					next->AddObject(RightController);
					next->AddObject(cloneDisplay);
					next->AddObject(cloneDisplayBack);
					next->AddObject(clonePanel);
					next->AddObject(timeDisplay);
					next->AddObject(timeDisplayNeedle);
					next->AddObject(rewindDisplay);
					next->AddObject(rewindHelp);
					next->AddObject(pauseHelp);
					next->AddObject(cloneHelp);
					next->AddObject(teleportHelp);
					next->AddObject(clonePlus);

					TimeManager::Instance()->AddObjectToTimeline(RightController);
					TimeManager::Instance()->AddObjectToTimeline(LeftController);
					TimeManager::Instance()->AddObjectToTimeline(headset);




					SystemLogger::Debug() << "Loading complete" << std::endl;
					Physics::Instance()->PhysicsLock.unlock();
					Settings::GetInstance().SetBool("LevelIsLoading", false);
				}
			}
		}
	};

}