#pragma once
namespace kjc {
	template<typename... Values>
	class tuple;

	template<>
	class tuple<> {};

	template<typename Head, typename... Tail>
	class tuple<Head, Tail...> : private tuple<Tail...> {
		using inherited = tuple<Tail...>;
	protected:
		Head m_head;
	public:
		tuple() = default;
		tuple(Head v, Tail... vtail) : m_head(v), inherited(vtail...) {}

		Head head() const {
			return m_head;
		}
		inherited& tail() {
			return *this;
		}
	};

}