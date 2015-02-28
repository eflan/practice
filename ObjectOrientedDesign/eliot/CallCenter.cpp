#include <stdio.h>
#include <queue>

struct Call {};

class CallCenterPhones;

class Employee
{
public:
	void dispatchCall(CallCenterPhones *phones, Call *call)
	{
		if(_call == nullptr)
		{
			_call = call;
		}
		else
		{
			escalate(phones, call);
		}
	}
	
	virtual void escalate(CallCenterPhones *phones, Call *call) = 0;
	
private:
	Call *_call;
};

class Respondent : public Employee
{
public:
	virtual void escalate(CallCenterPhones *phones, Call *call);
};

class Manager : public Employee
{
public:
	virtual void escalate(CallCenterPhones *phones, Call *call);
};

class Director : public Employee
{
public:
	virtual void escalate(CallCenterPhones *phones, Call *call);
};


class CallCenterPhones
{
public:
	CallCenterPhones()
		: _respondents(),
		  _managers(),
		  _directors()
	{
	}
	
	void addRespondent(Respondent *r)
	{
		_respondents.push(r);
	}

	void addManager(Manager *m)
	{
		_managers.push(m);
	}

	void addDirector(Director *d)
	{
		_directors.push(d);
	}
	
	void dispatchCall(Call *call)
	{
		dispatchCall(_respondents, call);
	}
		
	void escalateToManager(Call *call)
	{
		dispatchCall(_managers, call);
	}
	
	void escalateToDirector(Call *call)
	{
		dispatchCall(_directors, call);
	}
	
private:
	void dispatchCall(std::queue<Employee *> &tier, Call *call)
	{
		Employee *youreUp = tier.front();
		tier.pop();
		youreUp->dispatchCall(this, call);
		tier.push(youreUp);
	}

private:
	std::queue<Employee *> _respondents;
	std::queue<Employee *> _managers;
	std::queue<Employee *> _directors;
};

void Respondent::escalate(CallCenterPhones *phones, Call *call)
{
	phones->escalateToManager(call);
}

void Manager::escalate(CallCenterPhones *phones, Call *call)
{
	phones->escalateToDirector(call);
}

void Director::escalate(CallCenterPhones *phones, Call *call)
{
	throw "All directors are busy!";
}

void CallCenter()
{
	CallCenterPhones phones;
	
	for(size_t i = 0; i < 1000; i++)
	{
		phones.addRespondent(new Respondent());
	}

	for(size_t i = 0; i < 100; i++)
	{
		phones.addManager(new Manager());
	}

	for(size_t i = 0; i < 10; i++)
	{
		phones.addDirector(new Director());
	}
	
	size_t countCalls = 0;
	bool exceptionCaught = false;
	
	try
	{
		Call call;
		for(countCalls = 0; countCalls < 1111; countCalls++)
		{
			phones.dispatchCall(&call);
		}
	}
	catch(const char *const exceptionMessage)
	{
		exceptionCaught = true;
	}

	if(!exceptionCaught)
	{
		printf("Director was not assigned a call while busy. Something went wrong!\n");
	}
	else if(countCalls != 1110)
	{
		printf("Director escalated to prematurely!\n");
	}
	else
	{
		printf("Call center simulation succesful.\n");
	}
}
