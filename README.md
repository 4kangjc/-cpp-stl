# -cpp-stl
容器基本实现
#pragma once
namespace kjc {
    template<typename _Tp, size_t _Sz>
    class array {
    public:
        using value_type = _Tp;
        using size_type = size_t;
        using pointer = _Tp*;
        using reference = _Tp&;

        using iterator = pointer;
    protected:
        value_type _Arr[_Sz ? _Sz : 1];

    public:
        iterator begin() {
            return iterator(&_Arr[0]);
        }
        iterator end() {
            return iterator(_Arr + _Sz);
        }
    public:
        reference operator[](size_type index) {
            return _Arr[index];
        }
        const reference operator[](size_type index) const {
            return _Arr[index];
        }
        constexpr size_t size() {
            return _Sz;
        }
    };
}
