#include<vector>
#include<functional>
#pragma once
namespace kjc {
	template<typename T, typename Container = std::vector<T>, class Pr = std::less<T>>
	class priorityQueue {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = typename Container::const_reference;
		//using size_type = typename Container::size_type;
		using container_type = Container;
		using value_compare = Pr;

		priorityQueue() = default;
		priorityQueue(container_type& c) : c(c) {
			int N = c.size();
			for (int i = (N - 1) / 2; i >= 0; --i) {
				sink(i);
			}
		}
	protected:
		Container c{};
		Pr comp{};
	private:
		void swim(int k) {
			while (k > 0 && comp(c[(k - 1) / 2], c[k])) {
				std::swap(c[(k - 1) / 2], c[k]);
				k = (k - 1) / 2;
			}
		}
		void sink(int k) {
			int N = size();
			while (2 * k + 1 < N) {
				int j = 2 * k + 1;
				if (j + 1 < N && comp(c[j], c[j + 1])) {
					j++;
				}
				if (!comp(c[k], c[j])) {
					break;
				}
				std::swap(c[k], c[j]);
				k = j;
			}
		}
	public:
		bool isempty() const { return c.size() == 0; }
		size_t size() const { return c.size(); }
		void push(value_type& v) {
			int N = size();
			c.push_back(v);
			swim(N);
		}
		void push(value_type&& v) {
			int N = size();
			c.push_back(v);
			swim(N);
		}
		template<typename... Valty>
		void emplace(Valty&&... Val) {
			int N = size();
			c.emplace_back(std::forward<Valty>(Val)...);
			swim(N);
		}
		const_reference top() const noexcept(noexcept(c.front())) {
			return c.front();
		}
		void pop() {
			std::swap(c[0], c[size() - 1]);
			c.pop_back();
			sink(0);
		}
	};
}

/* 支持删除操作的优先队列 */
#pragma once
#include<vector>
#include<functional>
#include <queue>
#pragma once
namespace std {
	template<typename T, typename Container = std::vector<T>, class Pr = std::less<T>>
	class priorityQueue {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = typename Container::const_reference;
		//using size_type = typename Container::size_type;
		using container_type = Container;
		using value_compare = Pr;

		priorityQueue() = default;
		priorityQueue(container_type& c) : q1(c.begin(), c.end()) {}
	private:
		priority_queue<T, Container, Pr> q1, q2;

	public:
		inline bool empty() const { return size() == 0; }
		inline size_t size() const { return q1.size() - q2.size(); }
		void push(const value_type& v) {
			q1.push(v);
		}
		void push(value_type&& v) {
			q1.push(std::move(v));
		}
		template<typename... Valty>
		void emplace(Valty&&... Val) {
			q1.emplace(std::forward<Valty>(Val)...);
		}
		const_reference top() {
			while (q1.size() && q2.size() && q1.top() == q2.top())	q1.pop(), q2.pop();
			if (q1.empty()) {
				return -1;
			}
			return q1.top();
		}
		decltype(auto) pop() {
			const auto value = top();
			q1.pop();
			return value;
		}
		void erase(const T& value) noexcept {
			q2.push(value);
		}
		void erase(T&& value) noexcept {
			q2.push(std::move(value));
		}
	};
}