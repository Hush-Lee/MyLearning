#pragma once
#include "common.hpp"

namespace muduo {
	class TimerId{
	public:
		TimerId();
		TimerId(Timer* timer);
	};
}
