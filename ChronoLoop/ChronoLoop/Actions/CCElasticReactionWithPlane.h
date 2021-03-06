#pragma once
#include "../Objects/BaseObject.h"
#include "CodeComponent.hpp"

namespace Epoch
{

	struct CCElasticReactionWithPlane : public CodeComponent
	{
		bool colliding = false;
		virtual void OnCollision(Collider& _col, Collider& _other, float _time)
		{
			if (!colliding && _other.mColliderType == Collider::eCOLLIDER_Plane)
			{
				colliding = true;
				//SystemLogger::GetLog() << "PLANE COLLISION" << std::endl;

				vec4f normalVel = (((PlaneCollider*)&_other)->mNormal * -(_col.mVelocity * ((PlaneCollider*)&_other)->mNormal));
				_col.mVelocity += normalVel * (1 + _col.mElasticity);

				if (_col.mVelocity.Magnitude() > .5f && _col.mVelocity.y > .1f)
				{
					if (mObject->GetComponentCount(eCOMPONENT_AUDIOEMITTER) > 0)
					{
						if (dynamic_cast<SFXEmitter*>(mObject->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0)))
							((SFXEmitter*)mObject->GetComponentIndexed(eCOMPONENT_AUDIOEMITTER, 0))->CallEvent();
					}
				}
			}
			else
				colliding = false;
		}
	};

}