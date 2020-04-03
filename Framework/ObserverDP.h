#pragma once

#define NOTIFY_NONE -1

class Observer
{
private:
	friend class Subject;
	virtual void Notify(int id, const void* data) = 0;
};


class Subject
{
private:
	std::unordered_set<Observer*> observers;

public:
	void AddObserver(Observer* observer) { observers.insert(observer); }
	void RemoveObserver(Observer* observer) { observers.erase(observer); }
	void Notify(int id, const void* data) {
		for (auto ob : observers)
			ob->Notify(id, data);
	}
};