#pragma once


namespace DX
{
	class Component
	{
	public:
		Component();

		virtual void Apply()const = 0;
	};
}

