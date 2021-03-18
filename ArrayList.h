#pragma once
#include<utility>
namespace kjc {
	template<typename T>
	class ArrayList {
	public:
		using value_type = T;
		using pointer = T*;
		using iterator = T*;
		using reference = T&;
		using const_reference = const T&;
		using const_pointer = const T*;
	protected:
		iterator start;
		iterator finish;
		iterator endOfStorange;
	public:
		ArrayList() : start(nullptr), finish(nullptr), endOfStorange(nullptr) {
			ReAlloc(2);
		}
		ArrayList(size_t n) : start(new value_type[n]), finish(start + n), endOfStorange(start + n) { }
		ArrayList(size_t n, value_type value) : start(new value_type[n]) {
			for (size_t i = 0; i < n; ++i) {
				start[i] = value;
			}
			finish = endOfStorange = start + n;
		}
		ArrayList(const ArrayList<T>& arr) {
			int n = arr.size();
			if (n < size()) {
				for (size_t i = n; i < size(); ++i) {
					start[i].~T();
				}
			}
			reserve(n);
			for (size_t = 0; i < n; ++i) {
				start[i] = arr[i];
			}
			endOfStorange = finish = start + n;
		}
		ArrayList<T>& operator=(const ArrayList<T>& arr) {
			if (this == &arr) {
				return *this;
			}
			size_t n = arr.size();
			if (this->size() < n) {
				delete[] start;
				start = new T[n];
			}
			for (int i = 0; i < n; ++i) {
				start[i] = arr[i];
			}
			endOfStorange = finish = start + n;
		}
		~ArrayList() {
			clear();
			::operator delete(start, cap() * sizeof(value_type));
			//delete[] start;
			//start = finish = end_of_storange = nullptr;
		}
	public:
		void reserve(size_t newCapacity);
		reference operator[] (size_t index) {
			return start[index];
		}
		const reference operator[] (size_t index) const {
			return start[index];
		}
		size_t size() const { return size_t(finish - start); }
		size_t cap() const { return size_t(endOfStorange - start); }
		iterator begin() { return start; }
		iterator end() { return finish; }
		const_pointer cbegin() const { return start; }
		const_pointer cend() const { return finish; }
		bool empty() const { return size() == 0; }
		reference front() { return *begin(); }
		const_reference front() const { return (const_reference)*cbegin(); }
		reference back() { return *(end() - 1); }
		const_reference back() const { return *(cend() - 1); }
	public:
		void push_back(const T& x);
		void push_back(T&& x);
		void pop_back();
		template<typename... Args>
		reference emplace_back(Args&&... args);
		void clear();
		pointer erase(pointer pos);
	private:
		void ReAlloc(size_t newCapacity);
	};
};

namespace kjc {
	template<typename T>
	void ArrayList<T>::reserve(size_t newCapacity) {
		size_t n = cap();
		if (newCapacity < n) {
			return;
		}
		pointer newBlock = (pointer) ::operator new(newCapacity * sizeof(value_type));
		clear();
		::operator delete(start, n * sizeof(value_type));
		start = newBlock;
		finish = endOfStorange = start + newCapacity;
	}
	template<typename T>
	void ArrayList<T>::ReAlloc(size_t newCapacity) {
		size_t n = size();
		if (newCapacity < n) {
			return;
		}
		pointer newBlock = (pointer) ::operator new(sizeof(value_type) * newCapacity);
		//pointer newBlock = new value_type[newCapacity];
		for (size_t i = 0; i < n; ++i) {
			new (&newBlock[i]) T(std::move(start[i]));
			//newBlock[i] = std::move(start[i]);
		}
		clear();
		::operator delete(start, cap() * sizeof(value_type));
		//delete[] start;
		start = newBlock;
		finish = start + n;
		endOfStorange = start + newCapacity;
	}
	template<typename T>
	void ArrayList<T>::push_back(const T& x) {
		if (finish == endOfStorange) {
			ReAlloc(2 * size());
		}
		*finish = x;
		finish++;
	}
	template<typename T>
	void ArrayList<T>::push_back(T&& x) {
		if (finish == endOfStorange) {
			ReAlloc(2 * size());
		}
		*finish = std::move(x);
		finish++;
	}
	template<typename T>
	void ArrayList<T>::pop_back() {
		if (size() > 0) {
			finish--;
			finish->~T();
		}
	}
	template<typename T>
	template<typename... Args>
	T& ArrayList<T>::emplace_back(Args&&... args) {
		if (finish == endOfStorange) {
			ReAlloc(2 * size());
		}
		new(finish) T(std::forward<Args>(args)...);
		//*finish = T(std::forward<Args>(args)...);
		return *finish++;
	}
	template<typename T>
	T* ArrayList<T>::erase(pointer pos) {
		if (pos == finish - 1) {
			pop_back();
			return pos;
		}
		for (auto it = pos + 1; it != finish; ++it) {
			*(it - 1) = std::move(*it);
		}
		--finish;
		return pos;
	}
	template<typename T>
	void ArrayList<T>::clear() {
		while (start != finish) {
			start->~T();
			++start;
		}
		start = finish = endOfStorange = nullptr;
	}
}
