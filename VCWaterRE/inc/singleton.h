#pragma once

template<typename T>
class ISingleton
{
protected:
	ISingleton() {};
	~ISingleton() {};
private:
	ISingleton(ISingleton const&) {};
	ISingleton& operator=(ISingleton const&){};
	
public:
	static T &Get()
	{
		static T s;
		return s;
	}
};