#pragma once

#include "../../Utilities/Log.h"

#include <string>
#include <vector>
#include <utility>

namespace GCRC1
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
			s_RCObjectLib.push_back(this);
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

			if (m_RefCount == 0)
			{
				s_GarbageSize += sizeof(m_IObject);
				GarbageCollected++;

				delete this;
			}
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

		inline static uint32_t GarbageCollected;

	private:
		uint32_t m_RefCount = 0;
		T* m_IObject = nullptr;
		inline static uint32_t s_GarbageSize;
		inline static std::vector<RCObject*> s_RCObjectLib;
	};
}