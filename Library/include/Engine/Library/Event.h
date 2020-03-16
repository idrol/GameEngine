#pragma once

#include <functional>
#include <vector>

/// Optional template args
template<typename T, typename Y = T, typename W = Y, typename Z = W>
class Event<T, Y, W, Z> {
public:

	template<typename T, typename Y = T, typename W = Y, typename Z = W>
	void Invoke(T& param1, Y& param2 = default(Y), W& param3 = default(W), Z& param4 = default(Z)) {
		for (auto it : cbs) {
			it(param1, param2, param3, param4);
		}
	}


	template<typename T, typename Y = T, typename W = Y, typename Z = W>
	void operator+(std::function<T, Y, W, Z> cb) {
		cbs.emplace_back(cb);
	}

	template<typename T, typename Y = T, typename W = Y, typename Z = W>
	void operator-(std::function<T, Y, W, Z> cb) {
		auto it = std::find(cbs.front(), cbs.back(), cb);
		if (it != cbs.end()) cbs.erase(it);
	}

protected:

	std::vector<std::function<T, Y, W, Z>> cbs;

};
