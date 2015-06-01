#pragma once

#include <type_traits>

namespace interplot
{

// shorthand underlying type acquisition and conversions

template<typename T>
using enum_type = typename std::underlying_type<T>::type;

template<typename T>
constexpr enum_type<T> enum_cast( T value )
{
	return static_cast<enum_type<T>>( value );
}

template<typename T>
constexpr T enum_cast( enum_type<T> value )
{
	return static_cast<T>( value );
}

template<typename T>
struct is_enum_class
{
	static constexpr bool value =
		std::is_enum<T>::value && !std::is_convertible<T, enum_type<T>>::value;
};


// bitflag functionality

template<typename T>
struct EnumFlags
{
public:
	constexpr EnumFlags( const T& value )
		: m_Value( enum_cast( value ) )
	{
	}

	constexpr EnumFlags<T> operator&( const T& rhs ) const
	{
		return EnumFlags<T>( enum_cast<T>( m_Value & enum_cast( rhs ) ) );
	}
	constexpr EnumFlags<T> operator|( const T& rhs ) const
	{
		return EnumFlags<T>( enum_cast<T>( m_Value | enum_cast( rhs ) ) );
	}
	constexpr EnumFlags<T> operator^( const T& rhs ) const
	{
		return EnumFlags<T>( enum_cast<T>( m_Value ^ enum_cast( rhs ) ) );
	}
	
	inline EnumFlags<T>& operator&=( const T& rhs )
	{
		m_Value &= rhs;
		return *this;
	}
	inline EnumFlags<T>& operator|=( const T& rhs )
	{
		m_Value |= rhs;
		return *this;
	}
	inline EnumFlags<T>& operator^=( const T& rhs )
	{
		m_Value ^= rhs;
		return *this;
	}

	inline EnumFlags<T>& operator=( const T& rhs )
	{
		m_Value = enum_cast( rhs );
		return *this;
	}

	constexpr EnumFlags<T> operator~() const
	{
		return EnumFlags<T>( enum_cast<T>( ~m_Value ) );
	}

	inline bool operator==( const EnumFlags<T>& rhs )
	{
		return ( m_Value == rhs.m_Value );
	}

	constexpr bool is_set( T flag ) const
	{
		return ( ( m_Value & enum_cast( flag ) ) != 0 );
	}

	constexpr enum_type<T> raw() const
	{
		return m_Value;
	}

private:
	enum_type<T> m_Value;
};

template<typename T,
	typename = typename std::enable_if<is_enum_class<T>::value>::type>
constexpr EnumFlags<T> operator&( const T& lhs, const T& rhs )
{
	return ( EnumFlags<T>( lhs ) & rhs );
}
template<typename T,
	typename = typename std::enable_if<is_enum_class<T>::value>::type>
constexpr EnumFlags<T> operator|( const T& lhs, const T& rhs )
{
	return ( EnumFlags<T>( lhs ) | rhs );
}
template<typename T,
	typename = typename std::enable_if<is_enum_class<T>::value>::type>
constexpr EnumFlags<T> operator^( const T& lhs, const T& rhs )
{
	return ( EnumFlags<T>( lhs ) ^ rhs );
}

}
