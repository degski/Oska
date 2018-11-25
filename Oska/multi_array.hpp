
#pragma once

#include <cassert> // assert...
#include <cstdint> // intptr_t...
#include <cstddef> // std::size_t...

#include <array>
#include <type_traits>


#ifndef NOMINMAX
#define NOMINMAX
#endif


//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//


namespace ma {

template < typename T, intptr_t I, intptr_t BaseI = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
class Vector {

	T m_data [ I ];

public:

	Vector ( ) : m_data { T ( ) } { }
	Vector ( const Vector & v_ ) { memcpy ( m_data, v_.m_data, I * sizeof ( T ) ); }
	Vector ( Vector && v_ ) { memcpy ( m_data, v_.m_data, I * sizeof ( T ) ); }
	template < typename ... Args >
	Vector ( Args ... a_ ) : m_data { a_ ... } { }

	typedef std::array < intptr_t, 1 > extents_type;

	T &at ( const intptr_t i_ ) {

		assert ( i_ >= BaseI && i_ < I + BaseI );

		return ( m_data - BaseI ) [ i_ ];
	}

	T at ( const intptr_t i_ ) const {

		assert ( i_ >= BaseI && i_ < I + BaseI );

		return ( m_data - BaseI ) [ i_ ];
	}

	T &at_r ( const intptr_t i_ ) {

		assert ( i_ >= BaseI && i_ < I + BaseI );

		return ( m_data + I - 1 + BaseI ) [ -i_ ];
	}

	T at_r ( const intptr_t i_ ) const {

		assert ( i_ >= BaseI && i_ < I + BaseI );

		return ( m_data + I - 1 + BaseI ) [ -i_ ];
	}

	T *data ( ) {

		return m_data;
	}

	const T *data ( ) const {

		return m_data;
	}

	std::size_t size ( ) const {

		return I;
	}

	extents_type extents ( ) const {

		return extents_type { I };
	}
};


template<typename T, intptr_t I, intptr_t J, intptr_t BaseI = 0, intptr_t BaseJ = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
class Matrix {

	T m_data [ I * J ];

public:

	Matrix ( ) : m_data { T ( ) } { }
	Matrix ( const Matrix & m_ ) { memcpy ( m_data, m_.m_data, I * J * sizeof ( T ) ); }
	Matrix ( Matrix && m_ ) { memcpy ( m_data, m_.m_data, I * J * sizeof ( T ) ); }
	template < typename ... Args >
	Matrix ( Args ... a_ ) : m_data { a_ ... } { }

	typedef std::array < intptr_t, 2 > extents_type;

	T const &ref ( const intptr_t i_, const intptr_t j_ ) const {

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data - BaseJ - BaseI * J ) [ j_ + i_ * J ];
	}

	T &at ( const intptr_t i_, const intptr_t j_ ) {

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data - BaseJ - BaseI * J ) [ j_ + i_ * J ];
	}

	T at ( const intptr_t i_, const intptr_t j_ ) const {

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data - BaseJ - BaseI * J ) [ j_ + i_ * J ];
	}

	T const &ref_r ( const intptr_t i_, const intptr_t j_ ) {

		// Mirror the matrix coordinates...

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data + I * J - 1 + BaseJ + BaseI * J ) [ -j_ - i_ * J ];
	}

	T &at_r ( const intptr_t i_, const intptr_t j_ ) {

		// Mirror the matrix coordinates...

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data + I * J - 1 + BaseJ + BaseI * J ) [ -j_ - i_ * J ];
	}

	T at_r ( const intptr_t i_, const intptr_t j_ ) const {

		// Mirror the matrix coordinates...

		assert ( i_ >= BaseI );
		assert ( i_ < I + BaseI );
		assert ( j_ >= BaseJ );
		assert ( j_ < J + BaseJ );

		return ( m_data + I * J - 1 + BaseJ + BaseI * J ) [ -j_ - i_ * J ];
	}

	T *data ( ) {

		return m_data;
	}

	const T *data ( ) const {

		return m_data;
	}

	std::size_t size ( ) const {

		return I * J;
	}

	extents_type extents ( ) const {

		return extents_type { I, J };
	}
};

template<typename T, intptr_t I, intptr_t J, intptr_t BaseI = 0, intptr_t BaseJ = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
using MatrixRM = Matrix < T, I, J, BaseI, BaseJ >;

template<typename T, intptr_t J, intptr_t I, intptr_t BaseJ = 0, intptr_t BaseI = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
using MatrixCM = Matrix < T, J, I, BaseJ, BaseI >;


template < typename T, intptr_t I, intptr_t J, intptr_t K, intptr_t BaseI = 0, intptr_t BaseJ = 0, intptr_t BaseK = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
class Cube {

	T m_data [ I * J * K ];

public:

	Cube ( ) : m_data { T ( ) } { }
	Cube ( const Cube & c_ ) { memcpy ( m_data, c_.m_data, I * J * K * sizeof ( T ) ); }
	Cube ( Cube && c_ ) { memcpy ( m_data, c_.m_data, I * J * K * sizeof ( T ) ); }
	template < typename ... Args >
	Cube ( Args ... a_ ) : m_data { a_ ... } { }

	typedef std::array < intptr_t, 3 > extents_type;

	T &at ( const intptr_t i_, const intptr_t j_, const intptr_t k_ ) {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );

		return ( m_data + K * ( -BaseJ - BaseI * J ) - BaseK ) [ K * ( j_ + i_ * J ) + k_ ];
	}

	T at ( const intptr_t i_, const intptr_t j_, const intptr_t k_ ) const {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );

		return ( m_data + K * ( -BaseJ - BaseI * J ) - BaseK ) [ K * ( j_ + i_ * J ) + k_ ];
	}

	T &at_r ( const intptr_t i_, const intptr_t j_, const intptr_t k_ ) {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );

		return ( m_data + I * J * K - 1 + BaseJ * K + BaseI * J * K + BaseK ) [ K * ( -j_ - i_ * J ) - k_ ];
	}

	T at_r ( const intptr_t i_, const intptr_t j_, const intptr_t k_ ) const {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );

		return ( m_data + I * J * K - 1 + BaseJ * K + BaseI * J * K + BaseK ) [ K * ( -j_ - i_ * J ) - k_ ];
	}

	T *data ( ) {

		return m_data;
	}

	const T *data ( ) const {

		return m_data;
	}

	std::size_t size ( ) const {

		return I * J * K;
	}

	extents_type extents ( ) const {

		return extents_type { I, J, K };
	}
};


template<typename T, intptr_t I, intptr_t J, intptr_t K, intptr_t L, intptr_t BaseI = 0, intptr_t BaseJ = 0, intptr_t BaseK = 0, intptr_t BaseL = 0, typename = std::enable_if_t < std::is_default_constructible<T>::value, T > >
class HyperCube {

	T m_data [ I * J * K * L ];

public:

	HyperCube ( ) : m_data { T ( ) } { }
	HyperCube ( const HyperCube & h_ ) { memcpy ( m_data, h_.m_data, I * J * K * L * sizeof ( T ) ); }
	HyperCube ( HyperCube && h_ ) { memcpy ( m_data, h_.m_data, I * J * K * L * sizeof ( T ) ); }
	template < typename ... Args >
	HyperCube ( Args ... a_ ) : m_data { a_ ... } { }

	typedef std::array < intptr_t, 4 > extents_type;

	T &at ( const intptr_t i_, const intptr_t j_, const intptr_t k_, const intptr_t l_ ) {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );
		assert ( l_ >= BaseL && l_ < L + BaseL );

		return ( m_data + L * ( K * ( -BaseJ - BaseI * J ) - BaseK ) - BaseL ) [ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
	}

	T at ( const intptr_t i_, const intptr_t j_, const intptr_t k_, const intptr_t l_ ) const {

		assert ( i_ >= BaseI && i_ < I + BaseI );
		assert ( j_ >= BaseJ && j_ < J + BaseJ );
		assert ( k_ >= BaseK && k_ < K + BaseK );
		assert ( l_ >= BaseL && l_ < L + BaseL );

		return ( m_data + L * ( K * ( -BaseJ - BaseI * J ) - BaseK ) - BaseL ) [ L * ( K * ( j_ + i_ * J ) + k_ ) + l_ ];
	}

	T *data ( ) {

		return m_data;
	}

	const T *data ( ) const {

		return m_data;
	}

	std::size_t size ( ) const {

		return I * J * K * L;
	}

	extents_type extents ( ) const {

		return extents_type { I, J, K, L };
	}
};

};
