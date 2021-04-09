#pragma once
namespace kjc {
	template <typename T>
	class unique_ptr {
	public:
		unique_ptr(unique_ptr&& other) noexcept {
			//std::cout << "Move Construct!" << std::endl;
			m_ptr = other.release();
		}
		template <typename U>
		unique_ptr(unique_ptr<U>&& other) noexcept {
			m_ptr = other.release();
		}
		/*unique_ptr& operator=(unique_ptr rhs) noexcept {
			std::cout << "Move Sign!" << std::endl;
			rhs.swap(*this);
			return *this;
		}*/
		unique_ptr& operator=(unique_ptr&& rhs) noexcept {
			//std::cout << "Move Sign!" << std::endl;
			m_ptr = rhs.release();
			return *this;
		}
		~unique_ptr() {
			delete m_ptr;
		}
		explicit unique_ptr(T* ptr = nullptr) : m_ptr(ptr) { }  // 不能隐式转换

	public:
		T* get() const noexcept { return m_ptr; }
		void swap(unique_ptr& rhs) noexcept {
			std::swap(m_ptr, rhs.m_ptr);
		}
		T& operator*() const noexcept { return *m_ptr; }
		T* operator->() const noexcept { return m_ptr; }
		operator bool() const noexcept { return m_ptr; }
		T* release() {
			T* temp = m_ptr;
			m_ptr = nullptr;
			return temp;
		}
		
	private:
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;
	protected:
		T* m_ptr;
	};
}

namespace kjc {
	class referenceCounting {
	public:
		referenceCounting() noexcept : count(1LL) {}
		void add() noexcept { ++count; }
		decltype(auto) minus() noexcept { return --count; }
		decltype(auto) getCount() const { return count; }
	private:
		using ll = long long;
		ll count;
	};
}

namespace kjc {
	template <typename T>
	class shared_ptr {
	public:
		template <class U>
		friend class shared_ptr;

		constexpr shared_ptr() noexcept = default;

		constexpr shared_ptr(nullptr_t) noexcept {} // construct empty shared_ptr

		explicit shared_ptr(T* ptr) : m_ptr(ptr), count(new referenceCounting()) { }

		~shared_ptr() {
			//std::cout << "Destroy!" << std::endl;
			if (m_ptr && !count->minus()) {
				delete m_ptr;
				delete count;
			}
		} 

		shared_ptr(const shared_ptr& ptr) : m_ptr(ptr.m_ptr), count(ptr.count) {
			if (ptr)
				count->add();
		}

		template <typename U>
		shared_ptr(const shared_ptr<U>& ptr) : m_ptr(ptr.m_ptr), count(ptr.count) {
			if (ptr)
				count->add();
		}

		template <typename U>
		shared_ptr(shared_ptr<U>&& ptr) : m_ptr(ptr.m_ptr), count(ptr.count) {
			ptr.m_ptr = nullptr;
			ptr.count = nullptr;
		}

		template <typename U>
		shared_ptr& operator=(const shared_ptr<U>& ptr) {
			//std::cout << "Copy!" << std::endl;
			if (m_ptr && !count->minus()) {
				delete m_ptr;
				delete count;
			}
			m_ptr = ptr.m_ptr;
			count = ptr.count;
			count->add();
			return *this;
		}

		shared_ptr& operator=(const shared_ptr& ptr) {
			//std::cout << "Copy!" << std::endl;
			if (this != &ptr) {
				if (m_ptr && !count->minus()) {
					delete m_ptr;
					delete count;
				}
				m_ptr = ptr.m_ptr;
				count = ptr.count;
				count->add();
			}
			return *this;
		}

		template <typename U>
		shared_ptr& operator=(shared_ptr<U>&& ptr) {
			//std::cout << "Move!" << std::endl;
			if (m_ptr && !count->minus()) {
				delete m_ptr;
				delete count;
			}
			m_ptr = ptr.m_ptr;
			count = ptr.count;
			count->add();
			ptr.m_ptr = nullptr;
			ptr.count = nullptr;
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& ptr) {
			//std::cout << "Move!" << std::endl;
			if (this != &ptr) {
				if (m_ptr && !count->minus()) {
					delete m_ptr;
					delete count;
				}
				m_ptr = ptr.m_ptr;
				count = ptr.count;
				count->add();
				ptr.m_ptr = nullptr;
				ptr.count = nullptr;
			}
			return *this;
		}

	public:
		T* get() const noexcept { return m_ptr; }
		T& operator*() const noexcept { return *m_ptr; }
		T* operator->() const noexcept { return m_ptr; }
		operator bool() const noexcept { return m_ptr; }
		decltype(auto) useCount() const { return count->getCount(); }
		bool unique() const { return useCount() == 1; }
		void swap(shared_ptr& rhs) noexcept {
			std::swap(m_ptr, rhs.m_ptr);
			std::swap(count, rhs.count);
		}
	protected:
		T* m_ptr;
		referenceCounting* count;
	};
}
