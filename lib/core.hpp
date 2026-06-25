#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <variant>
#include <queue>
#include <memory>
#include <chrono>
#include <filesystem>

namespace bbl
{

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using size = size_t;

using f32 = float;
using f64 = double;

using chr = char;
using uchr = unsigned char;
using str = std::string_view;
using string = std::string;

template<size N>
struct FixedStr {
	chr value[N];

	constexpr FixedStr(const chr(&str)[N]) {
		for (std::size_t i = 0; i < N; ++i)
			value[i] = str[i];
	}

	constexpr operator str() const { return str(value); }
	constexpr operator string() const { return string(value); }
};

struct StrHash {
	using is_transparent = void;

	size operator()(str sv) const noexcept {
		return std::hash<std::string_view>{}(sv);
	}

	size operator()(const string& s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}

	size operator()(const chr* s) const noexcept {
		return std::hash<std::string_view>{}(s);
	}
};

struct StrEq {
	using is_transparent = void;

	bool operator()(str a, str b) const noexcept {
		return a == b;
	}

	bool operator()(const string& a, const string& b) const noexcept {
		return a == b;
	}

	bool operator()(const char* a, str b) const noexcept {
		return b == a;
	}

	bool operator()(str a, const char* b) const noexcept {
		return a == b;
	}
};

template<typename T>
using ref = std::shared_ptr<T>;
template<typename T>
using weak_ref = std::weak_ptr<T>;
template<typename T>
constexpr ref<T> newRef(auto&&... args) {
	return std::make_shared<T>(std::forward<decltype(args)>(args)...);
}

template<class T, class U> ref<T> refScast(const ref<U>& other) noexcept {
	auto ptr = static_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{other, ptr};
}
template<class T, class U> ref<T> refScast(ref<U>&& other) noexcept {
	auto ptr = static_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{std::move(other), ptr};
}
template<class T, class U> ref<T> refDcast(const ref<U>& other) noexcept {
	if (auto ptr = dynamic_cast<typename ref<T>::element_type*>(other.get()))
		return ref<T>{other, ptr};
	return {};
}
template<class T, class U> ref<T> refDcast(ref<U>&& other) noexcept {
	if (auto ptr = dynamic_cast<typename ref<T>::element_type*>(other.get()))
		return ref<T>(std::move(other), ptr);
	return {};
}
template<class T, class U> ref<T> refCcast(const ref<U>& other) noexcept {
	auto ptr = const_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{other, ptr};
}
template<class T, class U> ref<T> refCcast(ref<U>&& other) noexcept {
	auto ptr = const_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{std::move(other), ptr};
}
template<class T, class U> ref<T> refRcast(const ref<U>& other) noexcept {
	auto ptr = reinterpret_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{other, ptr};
}
template<class T, class U> ref<T> refRcast(ref<U>&& other) noexcept {
	auto ptr = reinterpret_cast<typename ref<T>::element_type*>(other.get());
	return ref<T>{std::move(other), ptr};
}

template<typename T>
using box = std::unique_ptr<T>;
template<typename T>
constexpr box<T> newBox(auto&&... args) {
	return std::make_unique<T>(std::forward<decltype(args)>(args)...);
}

template<class T>
using vec = std::vector<T>;

template<class T, size S>
using arr = std::array<T, S>;

template<class K, class V, class H = std::hash<K>, class E = std::equal_to<K>>
using dict = std::unordered_map<K, V, H, E>;

template<class K, class V>
using map = std::map<K, V>;

template <class... _Types>
using var = std::variant<_Types...>;

template <class T>
using opt = std::optional<T>;

template<class K>
using fifo = std::queue<K>;

template<class T1, class T2>
using pair = std::pair<T1, T2>;

template< class T1, class T2 >
pair<T1, T2> newPair(T1 x, T2 y) {
	return std::make_pair(x, y);
}

template< class T1, class T2 >
constexpr pair<std::unwrap_ref_decay_t<T1>,
	std::unwrap_ref_decay_t<T2>>
	newPair(T1&& x, T2&& y) {
	return std::make_pair(std::forward<T1>(x), std::forward<T2>(y));
}

template<typename bits, std::underlying_type<bits>::type max>
class Flags {
public:
	using bitsType = bits;
	using maskType = std::underlying_type<bits>::type;

	inline constexpr Flags(): m_mask(0) {}
	inline constexpr Flags(bitsType b) : m_mask(static_cast<maskType>(b)) {}
	inline constexpr Flags(const Flags<bits, max>& other) = default;
	inline constexpr Flags(maskType m):m_mask(m) {}

	auto operator<=> (const Flags<bits, max>&) const = default;

	inline constexpr bool operator!() const {
		return !m_mask;
	}

	inline constexpr Flags<bits, max> operator&(const Flags<bits, max>& other) const {
		return Flags<bits, max>(m_mask & other.m_mask);
	}
	inline constexpr Flags<bits, max> operator|(const Flags<bits, max>& other) const {
		return Flags<bits, max>(m_mask | other.m_mask);
	}
	inline constexpr Flags<bits, max> operator^(const Flags<bits, max>& other) const {
		return Flags<bits, max>(m_mask ^ other.m_mask);
	}
	inline constexpr Flags<bits, max> operator~() const {
		return Flags<bits, max>(m_mask ^ max);
	}
	inline constexpr Flags<bits, max>& operator=(const Flags<bits, max>& other) = default;
	inline constexpr Flags<bits, max>& operator|=(const Flags<bits, max>& other) {
		m_mask |= other.m_mask;
		return *this;
	}
	inline constexpr Flags<bits, max>& operator&(const Flags<bits, max>& other) {
		m_mask &= other.m_mask;
		return *this;
	}
	inline constexpr Flags<bits, max>& operator^=(const Flags<bits, max>& other) {
		m_mask ^= other.m_mask;
		return *this;
	}

	inline constexpr operator bool() const {
		return !!m_mask;
	}

	inline constexpr operator maskType() const {
		return m_mask;
	}
private:
	maskType m_mask;
};


template<class vec_view, bool reverse>
class VecViewIterator {
public:
	using value_type = typename vec_view::data_type;
	using pointer = value_type*;
	using reference = value_type&;
	using const_reference = const reference;

public:
	constexpr VecViewIterator(pointer val):m_ptr(val) {}
	constexpr VecViewIterator(const VecViewIterator&) = default;
	constexpr VecViewIterator& operator=(const VecViewIterator&) = default;

	constexpr VecViewIterator& operator++() {
		m_ptr++;
		return *this;
	}

	constexpr VecViewIterator operator++(int) {
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	constexpr VecViewIterator& operator--() {
		m_ptr--;
		return *this;
	}

	constexpr VecViewIterator operator--(int) {
		auto tmp = *this;
		--(*this);
		return tmp;
	}

	constexpr bool operator==(const VecViewIterator& rhs) const noexcept {
		return m_ptr == rhs.m_ptr;
	}

	constexpr bool operator!=(const VecViewIterator& rhs) const noexcept {
		return m_ptr != rhs.m_ptr;
	}

	constexpr reference operator[](const size n) noexcept {
		return m_ptr[n];
	}

	constexpr pointer operator->() noexcept {
		return m_ptr;
	}

	constexpr reference operator*() noexcept {
		return *m_ptr;
	}
private:
	pointer m_ptr;
};

template<class DataT>
class VecView {
public:
	using data_type = DataT;
	using pointer = data_type*;
	using reference = data_type&;
	using const_reference = const data_type&;
	using size_type = std::size_t;

	using iterator = VecViewIterator<VecView<DataT>, false>;
	using reverse_iterator = VecViewIterator<VecView<DataT>, true>;

public:
	constexpr VecView() noexcept = default;
	constexpr VecView(const VecView& rhs) noexcept = default;
	constexpr VecView(VecView&& rhs) noexcept = default;
	constexpr VecView(pointer data, size_type size):m_data(data), m_size(size) {}
	constexpr VecView(vec<data_type>& v) : m_data(v.data()), m_size(v.size()) {}

	template<class R> constexpr explicit VecView(R&& r) = delete;
	VecView(std::nullptr_t) = delete;

	constexpr VecView& operator=(const VecView& view) noexcept = default;
	constexpr VecView& operator=(VecView&& view) noexcept = default;
	constexpr VecView& operator=(vec<data_type>& v) noexcept { m_data = v.data(); m_size = v.size(); }

	constexpr bool operator==(const VecView& rhs) const { return m_data == rhs.m_data && m_size == rhs.m_size; }
	constexpr bool operator==(const vec<data_type>& rhs) const { return m_data == rhs.data() && m_size == rhs.size(); }

	constexpr operator vec<data_type>() const {
		vec<data_type> v;
		v.resize(m_size);
		for (size_t i = 0; i < m_size; i++) {
			v[i] = m_data[i];
		}
		return v;
	}
	constexpr operator pointer() const {
		return m_data;
	}

	constexpr iterator begin() const noexcept {
		return iterator(m_data);
	}
	constexpr iterator end() const noexcept {
		return iterator(m_data + m_size);
	}

	constexpr reverse_iterator rbegin() const noexcept {
		return reverse_iterator(m_data + m_size - 1);
	}
	constexpr reverse_iterator rend() const noexcept {
		return reverse_iterator(m_data - 1);
	}

	constexpr const_reference at(size_type pos) const {
		if (m_data == nullptr || pos >= m_size || pos < 0)
			throw std::out_of_range("Index out of range");
		else
			return m_data[pos];
	}
	constexpr reference operator[](size_type pos) const {
		return m_data[pos];
	}

	constexpr reference front() const {
		return m_data[0];
	}
	constexpr reference back() const {
		return m_data[m_size - 1];
	}

	constexpr pointer data() const noexcept {
		return m_data;
	}

	constexpr size_type size() const noexcept {
		return m_size;
	}
	constexpr size_type length() const noexcept {
		return m_size;
	}

	constexpr bool empty() const noexcept {
		return !m_data || m_size == 0;
	}

private:
	pointer m_data = nullptr;
	size_type m_size = 0;
};

using Blob = vec<u8>;
using BlobView = VecView<u8>;

namespace utils
{

constexpr size_t valConst{ 0xcbf29ce484222325 };
constexpr size_t primeConst{ 0x100000001b3 };

constexpr size_t fnv1a(const char* const str, const size_t value = valConst) noexcept {
	return (str[0] == '\0') ? value : fnv1a(&str[1], (value ^ size_t(str[0])) * primeConst);
}

constexpr size_t fnv1a(const str str) noexcept {
	return fnv1a(str.data());
}

}
}