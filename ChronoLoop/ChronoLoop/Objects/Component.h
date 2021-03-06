#pragma once
#include "..\Common\Math.h"
#include "..\Rendering\Mesh.h"
#include <unordered_map>
#include "..\Physics\Physics.h"
#include <unordered_set>
#include "../Rendering/RenderSet.h"	
#include "../Rendering/Structures.h"
#include "../Particles/ParticleSystem.h"

namespace Epoch
{
	class BaseObject;
	class Transform;
	//class Mesh;

	enum ComponentType
	{
		eCOMPONENT_UNKNOWN = 0,
		eCOMPONENT_CODE,
		eCOMPONENT_AUDIOEMITTER,
		eCOMPONENT_AUDIOLISTENER,
		eCOMPONENT_COLLIDER,
		eCOMPONENT_UI,
		eCOMPONENT_MESH,
		eCOMPONENT_EFFECT,
		eCOMPONENT_LIGHT,
		eCOMPONENT_MAX
	};

	class Component
	{
		friend class Physics;
		friend class BaseObject;

		static unsigned short mComponentCount;
		unsigned int mComponentId;	//unique component id
		unsigned short mComponentNum;  //the nth number component of a base object. This is for knowing the position in the bitset

	protected:
		bool mDestroyed = false;
		bool mIsEnabled = true, mIsValid = true;
		ComponentType mType = eCOMPONENT_MAX;
		BaseObject* mObject = nullptr;
	public:
		Component();
		Component(ComponentType _cType);
		virtual ~Component();
		inline ComponentType GetType() { return mType; };
		inline bool IsEnabled() { return mIsEnabled; };
		inline bool IsValid() { return mIsValid; }
		inline void Disable() { mIsEnabled = false; };
		inline void Enable() { mIsEnabled = true; };
		virtual void Update() = 0;
		virtual void Destroy() = 0;
		unsigned short GetComponentNum() { return mComponentNum; }
		void GetMatrix(matrix4& _m);
		unsigned short GetColliderId() { return mComponentId; };
		Transform& GetTransform();
		Transform& GetTransform() const;

		inline BaseObject* GetBaseObject() {
			return mObject;
		}

		//Dont call this unless you are absolutly sure you know what you are doing
		inline void SetComponentId(unsigned short _id) { mComponentId = _id; }
	};

	class Listener : public Component
	{
	public:
		Listener() : Component(ComponentType::eCOMPONENT_AUDIOLISTENER) {}
		void Update() {}
		void Destroy();
	};

	class Emitter :public Component
	{
	protected:
		void SendEvent(uint64_t _event);
	public:
		enum EventType { eNone = -1, ePlay, ePause, eResume, eStop };

		Emitter() : Component(ComponentType::eCOMPONENT_AUDIOEMITTER)
		{

		}

		virtual void CallEvent(EventType _et = eNone) = 0;

		void Update();
		void Destroy();
	};

	class AudioEmitter : public Emitter
	{
	private:
		std::unordered_map<EventType, uint64_t> mEvents;
		bool mPlaying, mPaused;

	public:
		inline bool isPlaying() const { return mPlaying; }
		inline bool isPaused() const { return mPaused; }

		void AddEvent(EventType _et, uint64_t _id);
		void StopAllSound();
		void CallEvent(EventType _et = eNone);
	};

	class SFXEmitter : public Emitter
	{
	private:
		uint64_t mEvent;
	public:
		void SetEvent(uint64_t _event);
		void CallEvent(EventType _et = eNone);
	};

	class Collider : public Component
	{
	public:
		enum ColliderType
		{
			eCOLLIDER_Mesh,
			eCOLLIDER_Sphere,
			eCOLLIDER_Cube,
			eCOLLIDER_Plane,
			eCOLLIDER_Button,
			eCOLLIDER_Controller,
			eCOLLIDER_OrientedCube
		};

		ColliderType mColliderType;
		bool mShouldMove, mIsTrigger, mPickUpAble = false, mShowCol = false;
		vec3f mVelocity, mAcceleration, mTotalForce, mForces, mImpulsiveForce, mGravity, mWeight, mDragForce;
		float mMass, mElasticity, mKineticFriction, mStaticFriction, mInvMass, mRHO, mDrag, mArea;

		virtual void Update();
		virtual void Destroy();

		vec3f AddForce(vec3f _force) { mForces = _force; return mForces; };
		virtual vec3f GetPos();
		virtual void SetPos(const vec3f& _newPos);
	};

	//class MeshCollider : public Collider
	//{
	//public:
	//	MeshCollider() {}
	//	MeshCollider(BaseObject* _obj, bool _move, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, char* _path);
	//	Mesh* mMesh;
	//};

	class SphereCollider : public Collider
	{
	public:
		SphereCollider() {}
		SphereCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, float _radius, std::string _ColliderShape = "../Resources/Sphere.obj");
		SphereCollider(vec3f _pos, float _rad) { mCenter = _pos; mRadius = _rad; };
		bool visible = false;
		vec3f mCenter;
		float mRadius;
		RenderShape* mShape;
		GhostList<matrix4>::GhostNode* mNode = nullptr;
		virtual void Update();
		virtual void Destroy();
	};

	class PlaneCollider : public Collider
	{
	public:
		PlaneCollider() {}
		PlaneCollider(BaseObject* _obj, bool _trigger, float _staticFriction, float _kineticFriction, float _offset, vec3f _norm);
		PlaneCollider(vec3f _norm, float _offset) { mNormal = _norm; mOffset = _offset; };
		vec3f mNormal;
		float mOffset;

	};

	class Frustrum : public Collider
	{
		Frustrum() {}
		PlaneCollider mFaces[6];
		vec3f mPoints[8];
	};

	class CubeCollider : public Collider
	{
	public:
		CubeCollider() {}
		CubeCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, vec3f _min, vec3f _max, std::string _ColliderShape = "../Resources/UnitCube.obj");
		vec3f mMin, mMax, mMinOffset, mMaxOffset;
		RenderShape* mShape;
		GhostList<matrix4>::GhostNode* mNode = nullptr;
		bool visible = false;
		virtual void Update();
		virtual void Destroy();

		virtual void SetPos(const vec3f& _newPos);
	};

	class OrientedCubeCollider : public Collider
	{
	public:
		OrientedCubeCollider() {}
		OrientedCubeCollider(BaseObject* _obj, bool _move, bool _trigger, vec3f _gravity, float _mass, float _elasticity, float _staticFriction, float _kineticFriction, float _drag, float _xRadius, float _yRadius, float _zRadius);
		float mWidth, mHeight, mDepth;
		vec3f mAxis[3];
		virtual void SetPos(const vec3f& _newPos);
	};

	class ButtonCollider : public CubeCollider
	{
	public:
		ButtonCollider() {}
		ButtonCollider(BaseObject* _obj, vec3f _min, vec3f _max, float _mass, float normForce, vec3f _pushNormal);
		vec3f mPushNormal;
		PlaneCollider mUpperBound, mLowerBound;
		bool mPress;
	};

	class ControllerCollider : public CubeCollider
	{
	public:
		ControllerCollider() {}
		ControllerCollider(BaseObject* _obj, vec3f _min, vec3f _max, bool _left);
		bool mLeft;
		virtual void Update();
		std::unordered_set<Collider*> mHitting;
	};

	class Effect : public Component
	{
	public:
		Effect() : Component(ComponentType::eCOMPONENT_EFFECT) {}
		void Update() {}
		void Destroy() {}
		ParticleEmitter* mEmitter;

		void FireEffect()
		{
			if (mEmitter)
				mEmitter->FIRE();
		}
	};

	class LightSource : public Component
	{
		Light * mLight;
		LightSource() : Component(ComponentType::eCOMPONENT_LIGHT) {}
		void Update()
		{
			//TODO: FIx this
			//mLight->Position = *GetTransform().GetPosition();
		}
		void Destroy()
		{
			Light* temp = mLight;
			delete mLight;
			mLight = nullptr;
		}
	};

	/*
	business entity- gmail, twitter, facebook, steam account
	art, audio, marketing, designer students ?

	first initial last name, password lower case
	gdserv.fullsail.com:8080
	install doc, follow it
	*/
}