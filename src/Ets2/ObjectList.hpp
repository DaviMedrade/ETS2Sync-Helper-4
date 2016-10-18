#pragma once

namespace Ets2 {
	template <class T> class ObjectList : public std::vector<T*> {
	public:
		~ObjectList<T>() {
			for (auto item : *this) {
				delete item;
			}
		}

		void ObjectList<T>::sort() {
			std::sort(begin(), end(), &(T::compareForSort));
		}
	};
}