#pragma once
#include "../Objects/BaseObject.h"
#include "../Actions/Action.hpp"

struct BoxSnapToControllerAction : public Action {
	bool mHeld = false;
	bool mHeldLeft = false;

	BoxSnapToControllerAction(BaseObject* _parent) : Action(_parent) {

	}

	virtual void Update() override {
		if (VRInputManager::Instance().iIsInitialized()) {
			if (VRInputManager::Instance().GetController(true).GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(true);
				mHeldLeft = true;
			} else if (VRInputManager::Instance().GetController(false).GetPress(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) {
				SnapToController(false);
				mHeldLeft = false;
			} else if (mHeld) {
				ReleaseCube();
			}
		}
	}

	virtual void SnapToController(bool left) {
		// TODO Fix this because it's pretty jank.
		mHeld = true;
		mHeldLeft = left;
		matrix4 m = Math::FromMatrix(VRInputManager::Instance().GetController(left).GetPose().mDeviceToAbsoluteTracking);
		mObject->GetTransform().SetMatrix(m);
		((CubeCollider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->SetPos(Math::MatrixTranspose(m).tiers[3]);
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = false;
	}

	virtual void ReleaseCube() {
		if (mHeldLeft) {
			vec4f force = VRInputManager::Instance().GetController(true).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		} else {
			vec4f force = VRInputManager::Instance().GetController(false).GetVelocity();
			((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mVelocity = force;
		}
		((Collider*)(mObject->GetComponentIndexed(eCOMPONENT_COLLIDER, 0)))->mShouldMove = true;
		mHeldLeft = mHeld = false;
	}
};