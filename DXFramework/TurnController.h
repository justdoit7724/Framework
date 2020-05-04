#pragma once
#include <stack>

template <class T>
class TurnController
{
public:
	void FinishNewTurn(T move)
	{
		prevTurns.push(move);
		while (nextTurns.size() > 0)
			nextTurns.pop();
	}
	bool JumpBack(T* prev) {

		if (prevTurns.empty())
			return false;

		*prev = prevTurns.top(); prevTurns.pop();

		nextTurns.push(*prev);

		return true;
	}
	bool JumpFront(T* next) {
		if (nextTurns.empty())
			return false;

		*next = nextTurns.top(); nextTurns.pop();

		prevTurns.push(*next);

		return true;
	}

private:
	std::stack<T> prevTurns;
	std::stack<T> nextTurns;
};

