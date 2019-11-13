#pragma once
#include <unordered_set>

class Observer
{
public:
	virtual void Notify(void* data) = 0;
};
class Subject
{
private:
	std::unordered_set<Observer*> observers;

public:
	void AddObserver(Observer* observer) { observers.insert(observer); }
	void RemoveObserver(Observer* observer) { observers.erase(observer); }
	void Notify(void* data) {
		for (auto ob : observers)
			ob->Notify(data);
	}
};