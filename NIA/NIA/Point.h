#pragma once
/*!
\file
\brief declaration and definition of Point struct
*/
namespace viz {
	/// <summary>
	/// Simple struct that stores two values of template type, as x and y.
	/// </summary>
	/// \tparam T Type
	template <typename T>
	struct Point
	{
		T x, y;
		Point(const T& x, const T& y)
			: x(x), y(y)
		{

		}
	};
}

