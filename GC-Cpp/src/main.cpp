#include "RFObject.h"
#include "Ref.h"

#include <memory>

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

void TestCopy(Ref<TestApp> application)
{
	application->m_Specification.ID = 2;
	Ref<TestApp> application1 = application;
	application1->m_Specification.ID = 3;
}

void TestRef(Ref<TestApp>& application)
{
	Ref<TestApp> application1 = application;
}

void BenchmarkSharedPtr()
{
	FUNC_PROFILE();

	const int num_ptrs = 50000;
	const int num_iterations = 100;

	for (int i = 0; i < num_iterations; i++) {
		std::shared_ptr<int> ptrs[num_ptrs];
		for (int j = 0; j < num_ptrs; j++) {
			ptrs[j] = std::make_shared<int>(j);
		}
		int sum = 0;
		for (int j = 0; j < num_ptrs; j++) {
			sum += *ptrs[j];
		}
	}

	return;
}

void BenchmarkRef()
{
	FUNC_PROFILE();

	const int num_ptrs = 50000;
	const int num_iterations = 100;

	for (int i = 0; i < num_iterations; i++) {
		Ref<int> ptrs[num_ptrs];
		for (int j = 0; j < num_ptrs; j++) {
			ptrs[j] = MakeRef<int>(j);
		}
		int sum = 0;
		for (int j = 0; j < num_ptrs; j++) {
			sum += *ptrs[j];
		}
	}

	return;
}

int main()
{
	Specification specification;
	specification.ID = 1;
	specification.Name = "Sample App";
	specification.Enabled = true;

	BenchmarkRef();
	BenchmarkSharedPtr();

	{
		PROFILE("Ref");

		Ref<TestApp> app1 = MakeRef<TestApp>(specification);
		app1->m_Specification.ID = 1;
		Ref<TestApp> app2 = MakeRef<TestApp>(specification);
		app2->m_Specification.ID = 2;

		app1 = app2;

		Ref<TestApp> app3 = MakeRef<TestApp>(specification);
		app3->m_Specification.ID = 3;

		app2 = app3;

		Ref<TestApp> app4 = MakeRef<TestApp>(specification);
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

	{
		Ref<TestApp> application = MakeRef<TestApp>(specification);
		application->Start();

		TestCopy(application);

		TestRef(application);

		Ref<TestApp> application1 = application;
		application1->m_Specification.ID = 2;
		Ref<TestApp>& application2 = application;
		application2->m_Specification.ID = 3;

		application = application1;
	}

	{
		std::shared_ptr<TestApp> application = std::make_shared<TestApp>(specification);

		std::shared_ptr<TestApp> application1 = application;
		application1->m_Specification.ID = 2;
		std::shared_ptr<TestApp> application2 = application;
		application2->m_Specification.ID = 3;
	}

	return 0;
}