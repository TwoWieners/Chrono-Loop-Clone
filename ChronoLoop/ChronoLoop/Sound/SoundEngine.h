#pragma once
//Credit to John Murphy
#include <vector>
#include <map>

#include "../Objects/Component.h"
#include "AkSoundEngineDLL.h"

#define DLL __declspec(dllexport)

//Needed to work with Wwise soundbanks and the .h they generate
#if defined(_WIN64)
typedef unsigned __int64 AudioEvent;				///< Integer (unsigned) type for pointers
#else
typedef __w64 unsigned int AudioEvent;			///< Integer (unsigned) type for pointers
#endif
extern "C"
{
	class AudioWrapper
	{
		static AudioWrapper* audioSystem;

	private:
		std::vector<Emitter> mEmitters;
		std::vector<Listener> mListeners;
		std::map<std::wstring, AkBankID> mRegisteredSoundBanks;

		bool mIsInitialize = false;
		float mWorldScale;
	public:
		AudioWrapper* GetInstance() { return audioSystem; }

		AudioWrapper();
		~AudioWrapper();

		bool Initialize();
		void Shutdown();
		void Update();

		void SetWorldScale(float _scale);
		bool IsInitialized() { return mIsInitialize; }

		bool AddListener(const Listener * _listener, const char* _name);
		bool RemoveListener(const Listener * _listener);
		bool AddEmitter(const Emitter * _emitter, const char* _name);
		bool RemoveEmitter(const Emitter * _emitter);

		//Posts an event at a pos, emitter location, or near a listener.
		bool MakeEvent(AudioEvent _id, float* _pos);
		bool MakeEvent(AudioEvent _id, const Emitter * _emitter);
		bool MakeEvent(AudioEvent _id, unsigned int _listenerID = 0);

		//Set soundbank path(s)
		void SetBasePath(const wchar_t* _strPath);
		bool LoadSoundBank(const wchar_t* _BankName);
		bool UnloadSoundBank(const wchar_t* _BankName);

	};
}