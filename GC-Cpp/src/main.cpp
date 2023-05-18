#include "ReferenceCount/GCRC1/RCObject.h"
#include "ReferenceCount/GCRC1/Ref.h"

#include "ReferenceCount/GCRC2/RCObject.h"
#include "ReferenceCount/GCRC2/Ref.h"

#include "ReferenceCount/GCRC3/RCObject.h"
#include "ReferenceCount/GCRC3/Ref.h"

#include <memory>
#include <assert.h>

struct Specification
{
	int ID;
	std::string Name;
	bool Enabled;
};

class TestApp
{
public:
	TestApp(Specification& specification) 
		: m_Specification(specification)
	{

	}

	void Start()
	{

	}
	Specification m_Specification;
private:
};

#define ITERATIONS 30
#define POINTERS 1000

void BenchmarkVanilla()
{
	{
		PROFILE("BenchmarkVanilla");

		for (int i = 0; i < ITERATIONS; i++)
		{
			GCRC1::Ref<int> pointersOuter[2 * POINTERS];
			for (int i = 0; i < 2 * POINTERS; i++)
				pointersOuter[i] = GCRC1::MakeRef<int>(i + 100);

			{
				GCRC1::Ref<int> pointersInner[POINTERS];
				for (int i = 0; i < POINTERS; i++)
					pointersInner[i] = GCRC1::MakeRef<int>(i - 100);

				for (int i = 0; i < POINTERS; i++)
					pointersOuter[i] = pointersInner[i];

				int sum = 0;
				for (int i = 0; i < POINTERS; i++)
				{
					sum += *pointersOuter[i];
				}
			}
		}
	}

	std::cout << "	Garbage size: " << ITERATIONS * 3 * POINTERS << " Garbage collected: " << GCRC1::RCObject<int>::GarbageCollected << std::endl;
}

void BenchmarkInterval()
{
	GCRC2::RCObject<int>::Initialize(300);

	{
		PROFILE("BenchmarkInterval");

		for (int i = 0; i < ITERATIONS; i++)
		{
			GCRC2::Ref<int> pointersOuter[2 * POINTERS];
			for (int i = 0; i < 2 * POINTERS; i++)
				pointersOuter[i] = GCRC2::MakeRef<int>(i + 100);

			{
				GCRC2::Ref<int> pointersInner[POINTERS];
				for (int i = 0; i < POINTERS; i++)
					pointersInner[i] = GCRC2::MakeRef<int>(i - 100);

				for (int i = 0; i < POINTERS; i++)
					pointersOuter[i] = pointersInner[i];

				int sum = 0;
				for (int i = 0; i < POINTERS; i++)
				{
					sum += *pointersOuter[i];
				}
			}
		}
	}

	std::cout << "	Garbage size: " << ITERATIONS * 3 * POINTERS << " Garbage collected: " << GCRC2::RCObject<int>::GarbageCollected << std::endl;
}

void BenchmarkGarbageSize()
{
	GCRC3::RCObject<int>::Initialize(300, 5000000);

	{
		PROFILE("BenchmarkGarbageSize");

		for (int i = 0; i < ITERATIONS; i++)
		{
			GCRC3::Ref<int> pointersOuter[2 * POINTERS];
			for (int i = 0; i < 2 * POINTERS; i++)
				pointersOuter[i] = GCRC3::MakeRef<int>(i + 100);

			{
				GCRC3::Ref<int> pointersInner[POINTERS];
				for (int i = 0; i < POINTERS; i++)
					pointersInner[i] = GCRC3::MakeRef<int>(i - 100);

				for (int i = 0; i < POINTERS; i++)
					pointersOuter[i] = pointersInner[i];

				int sum = 0;
				for (int i = 0; i < POINTERS; i++)
				{
					sum += *pointersOuter[i];
				}
			}
		}
	}

	std::cout << "	Garbage size: " << ITERATIONS * 3 * POINTERS << " Garbage collected: " << GCRC3::RCObject<int>::GarbageCollected << std::endl;
}

void BenchmarkSharedPtr()
{
	{
		PROFILE("BenchmarkSharedPtr");

		for (int i = 0; i < ITERATIONS; i++)
		{
			std::shared_ptr<int> pointersOuter[2 * POINTERS];
			for (int i = 0; i < 2 * POINTERS; i++)
				pointersOuter[i] = std::make_shared<int>(i + 100);

			{
				std::shared_ptr<int> pointersInner[POINTERS];
				for (int i = 0; i < POINTERS; i++)
					pointersInner[i] = std::make_shared<int>(i - 100);

				for (int i = 0; i < POINTERS; i++)
					pointersOuter[i] = pointersInner[i];

				int sum = 0;
				for (int i = 0; i < POINTERS; i++)
				{
					sum += *pointersOuter[i];
				}
			}
		}
	}
}

void BenchmarkRawPtr()
{
	{
		PROFILE("BenchmarkRawPtr");

		for (int i = 0; i < ITERATIONS; i++)
		{
			int* pointersOuter[2 * POINTERS];
			for (int i = 0; i < 2 * POINTERS; i++)
				pointersOuter[i] = new int(i + 100);

			{
				int* pointersInner[POINTERS];
				for (int i = 0; i < POINTERS; i++)
					pointersInner[i] = new int(i - 100);

				for (int i = 0; i < POINTERS; i++)
				{
					delete pointersOuter[i];
					pointersOuter[i] = pointersInner[i];
				}

				int sum = 0;
				for (int i = 0; i < POINTERS; i++)
				{
					sum += *pointersOuter[i];
				}
			}

			for (int i = 0; i < 2 * POINTERS; i++)
				delete pointersOuter[i];
		}
	}
}

int main()
{
	// Performance Benchmarks.

	BenchmarkVanilla();
	BenchmarkInterval();
	BenchmarkGarbageSize();
	BenchmarkRawPtr();
	BenchmarkSharedPtr();

	Specification specification;
	specification.ID = 1;
	specification.Name = "Sample App";
	specification.Enabled = true;

	// Test for correctness compared to std::shared_ptr.

	{
		PROFILE("Ref");

		GCRC1::Ref<TestApp> app1 = GCRC1::MakeRef<TestApp>(specification);
		app1->m_Specification.ID = 1;
		GCRC1::Ref<TestApp> app2 = GCRC1::MakeRef<TestApp>(specification);
		app2->m_Specification.ID = 2;

		app1 = app2;

		GCRC1::Ref<TestApp> app3 = GCRC1::MakeRef<TestApp>(specification);
		app3->m_Specification.ID = 3;

		app2 = app3;

		GCRC1::Ref<TestApp> app4 = GCRC1::MakeRef<TestApp>(specification);
		app4->m_Specification.ID = 4;

		app1 = app4;
	}

	{
		PROFILE("Shared Ptr");

		std::shared_ptr<TestApp> app1 = std::make_shared<TestApp>(specification);
		app1->m_Specification.ID = 1;
		std::shared_ptr<TestApp> app2 = std::make_shared<TestApp>(specification);
		app2->m_Specification.ID = 2;

		app1 = app2;

		std::shared_ptr<TestApp> app3 = std::make_shared<TestApp>(specification);
		app3->m_Specification.ID = 3;

		app2 = app3;

		std::shared_ptr<TestApp> app4 = std::make_shared<TestApp>(specification);
		app4->m_Specification.ID = 4;

		app1 = app4;
	}

	return 0;
}