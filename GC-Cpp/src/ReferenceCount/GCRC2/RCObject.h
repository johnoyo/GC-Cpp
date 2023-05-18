#pragma once

#include "../../Utilities/Log.h"

#include <string>
#include <vector>
#include <utility>
#include <mutex>
#include <syncstream>

namespace GCRC2
{
	template<typename T>
	class RCObject
	{
	public:
		template <typename... Ts>
		RCObject(Ts&&... Args)
		{
			m_IObject = new T(std::forward<Ts>(Args)...);
			m_RefCount++;

			s_Lock.lock();

			s_RCObjectInitialized.push_back(true);
			s_RCObjectLib.push_back(this);

			s_Lock.unlock();
		}

		~RCObject()
		{
			if (m_IObject != nullptr)
			{
				delete m_IObject;
			}
		}

		void IncrRef()
		{
			m_RefCount++;
		}

		void DecrRef()
		{
			if (m_RefCount)
				m_RefCount--;
		}

		uint32_t GetRef() const
		{
			return m_RefCount;
		}

		T* GetIObject() const
		{
			return m_IObject;
		}

		void SetIObject(T* object)
		{
			m_IObject = object;
		}

		static std::vector<RCObject*>& GetRCObjectLib()
		{
			return s_RCObjectLib;
		}

		static void Initialize(int sleepDuration)
		{
			s_Worker = std::thread([sleepDuration]
			{
				while (true)
				{
					Collect();

					std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
				}
			});

			s_Worker.detach();
		}

		static void Collect()
		{
			s_Lock.lock();

			for (int i = 0; i < s_RCObjectLib.size(); i++)
			{
				if (s_RCObjectLib[i] != nullptr && i < s_RCObjectInitialized.size())
				{
					if (s_RCObjectInitialized[i])
					{
						if (s_RCObjectLib[i]->GetRef() == 0)
						{
							GarbageCollected++;

							delete s_RCObjectLib[i];

							s_RCObjectInitialized[i] = false;
						}
					}
				}
			}

			s_Lock.unlock();
		}

		inline static uint32_t GarbageCollected;

	private:
		uint32_t m_RefCount = 0;
		T* m_IObject = nullptr;

		inline static std::thread s_Worker;
		inline static std::mutex s_Lock;
		inline static std::vector<RCObject*> s_RCObjectLib;
		inline static std::vector<bool> s_RCObjectInitialized;
	};
}