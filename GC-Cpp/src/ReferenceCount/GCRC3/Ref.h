#pragma once

#include "../../Utilities/Log.h"
#include "RCObject.h"

#include <string>
#include <utility>
#include <thread>
#include <chrono>

namespace GCRC3
{
	template<typename T>
	class Ref
	{
	public:
		Ref() = default;

		Ref(const Ref& otherRef)
		{
			m_RCObject = otherRef.m_RCObject;
			m_RCObject->IncrRef();
		}

		~Ref()
		{
			if (m_RCObject)
			{
				m_RCObject->DecrRef();
			}
		}

		void Set(RCObject<T>* RCObject)
		{
			m_RCObject = RCObject;
		}

		auto Get()
		{
			return m_RCObject->GetIObject();
		}

		auto operator->()
		{
			return m_RCObject->GetIObject();
		}

		T& operator*()
		{
			return *(m_RCObject->GetIObject());
		}

		void operator=(const Ref<T>& otherRef)
		{
			if (m_RCObject)
				m_RCObject->DecrRef();

			m_RCObject = otherRef.m_RCObject;
			m_RCObject->IncrRef();
		}

		void operator=(T* otherRef)
		{
			m_RCObject->DecrRef();

			auto& library = RCObject<T>::GetRCObjectLib();

			for (auto& obj : library)
			{
				if (obj->GetIObject() == otherRef)
				{
					m_RCObject = obj;
					m_RCObject->IncrRef();

					return;
				}
			}

			m_RCObject->SetIObject(otherRef);
			m_RCObject->IncrRef();
		}

	private:
		RCObject<T>* m_RCObject = nullptr;
	};

	template<typename T, typename... Ts>
	static Ref<T> MakeRef(Ts&&... Args)
	{
		RCObject<T>* rcObject = new RCObject<T>(std::forward<Ts>(Args)...);
		Ref<T> ref;
		ref.Set(rcObject);
		return ref;
	}
}