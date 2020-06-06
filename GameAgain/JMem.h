#pragma once

#include<vector>
namespace jstl {

	template <typename T>
	class Ticketer {
	private:
		std::vector<int> freemem;
		std::vector<int> used;
		std::vector<T> objects;
	public:
		Ticketer();
		int add(T object);
		void remove(int index);
		T get(int);
		int getIt(int);
	};

	template<typename T>
	Ticketer<T>::Ticketer() : freemem(), used(), objects() {}

	template<typename T>
	int Ticketer<T>::add(T object)
	{
		int newLocation;
		if (freemem.size() > 0) {
			int freeIndex = freemem[freemem.size() - 1];
			freemem.pop_back();
			used.push_back(freeIndex);
			objects[freeIndex] = object;
			newLocation = freeIndex;
		}
		else {
			used.push_back(objects.size());
			newLocation = objects.size();
			objects.push_back(object);
		}

		return newLocation;
	}

	template<typename T>
	void Ticketer<T>::remove(int object)
	{
		for (int i = 0; i < used.size(); i++) {
			if (used[i] == object) {
				freemem.push_back(used[i]);
				used.erase(used.begin() + i);
				break;
			}
		}
	}

	template<typename T>
	T Ticketer<T>::get(int object)
	{
		return objects.at(object);
	}

	template<typename T>
	int Ticketer<T>::getIt(int offset) {
		if (offset < used.size()) {
			return used[offset];
		}
		else {
			return -1;
		}
	}
}