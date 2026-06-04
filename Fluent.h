/***************************************************************************
*** Copyright (c) 2023 Simpson Strong-Tie Company Inc. All Rights Reserved.
***
*** This software is distributed under the terms of the license agreement between the IntelliCAD
*** Technology Consortium and its members, as well as the additional terms set forth below. By
*** downloading or using this software, you are agreeing to be bound by such terms. If you do not or
*** cannot agree to such terms, please do not download or use the software.
*****************************************************************************/

#pragma once

#include <optional>
#include <type_traits>

// True when T is std::optional<X>, false otherwise. Used by Fluent::then to dispatch.
template<typename T> struct is_optional                   : std::false_type {};
template<typename T> struct is_optional<std::optional<T>> : std::true_type  {};

// ========================================================================
// Fluent<T>: Wraps a value and allows chaining transforms and side effects.
// When T is std::optional<X>, then() short-circuits on nullopt — subsequent
// steps are skipped and nullopt propagates to the end of the chain.
template <typename T>
class Fluent {
	T value;
public:
	explicit Fluent( T v ) : value( std::move( v ) ) {}

	// Entry point: construct a Fluent<T> from a value.
	static Fluent from( T v )
	{
		return Fluent( std::move( v ) );
	}

	// Transform the value: returns Fluent<OutT> where OutT = decltype(func(value)).
	// Always calls func regardless of optional state — prefer then() for optional chains.
	template<typename F>
	auto map( F&& func )
	{
		using otype = decltype(func( value ));
		return Fluent<otype>::from( func( value ) );
	}

	// Unwrap and delegate: returns whatever func(value) returns directly (not wrapped in Fluent).
	template<typename F>
	auto flatMap( F&& func )
	{
		return func( value );
	}

	// Like flatMap but asserts at compile time that func returns std::optional.
	// Use when func already returns optional and you don't want double-wrapping.
	template<typename F>
	auto flatMapOpt( F&& func )
	{
		using otype = decltype(func( value ));
		static_assert(is_optional<otype>::value, "flatMapOpt requires a function that returns std::optional");
		return Fluent<otype>::from( func( value ) );
	}

	// Side effect: calls func(value) for logging or printing, then returns *this unchanged.
	template<typename F>
	Fluent& tap( F&& func )
	{
		func( value );
		return *this;
	}

	// Alias for tap() — Kotlin-style name for side-effect chaining.
	template <typename F>
	Fluent& also( F&& func )
	{
		return tap( std::forward<F>( func ) );
	}

	// Universal transform step.
	// - Plain T:            calls map(func) unconditionally.
	// - std::optional<X>:  calls mapOpt(func), skipping func if value is nullopt.
	template <typename F>
	auto then( F&& func )
	{
		if constexpr ( is_optional<T>::value )
			return mapOpt( std::forward<F>( func ) );
		else
			return map( std::forward<F>( func ) );
	}

	// Unwrap the stored value at the end of the chain.
	T get() const
	{
		return value;
	}

	// Transform for optional chains. T must be std::optional<X>.
	// If value is nullopt, returns Fluent<optional<OutT>>(nullopt) without calling func.
	// If func itself returns optional<Y>, OutT is Y (no double-wrapping).
	template<typename F>
	auto mapOpt( F&& func )
	{
		static_assert(is_optional<T>::value, "mapOpt requires Fluent<std::optional<T>>");
		using InnerT  = typename T::value_type;
		using RawOutT = decltype(func( std::declval<InnerT>() ));
		using OutT    = std::conditional_t<is_optional<RawOutT>::value,
		                                   typename RawOutT::value_type,
		                                   RawOutT>;
		if ( !value.has_value() )
			return Fluent<std::optional<OutT>>::from( std::nullopt );
		return Fluent<std::optional<OutT>>::from( func( *value ) );
	}
};
// ========================================================================
