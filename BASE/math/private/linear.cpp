//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module linear.cpp
///
/// @file   linear.cpp
///
/// This program is free software; you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or (at
/// your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful, but
/// WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software
/// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
///
/// @author    modified by Detlef Hafer
///
//------------------------------------------------------------------------------------
#include <cassert>
#include <cmath>
#include "BASE/math/public/linear.h"

namespace GNU
{
//------------------------------------------------------------------------------------
///  @brief   gsl_fit_linear
///
///
///			 Fit the data (x_i, y_i) to the linear relationship 
///
///   Y = c0 + c1 x
///
///   returning, 
///
///   c0, c1  --  coefficients
///
///   This fit can be used in the case where the errors for the data are
///   uknown, but assumed equal for all points. The resulting
///   variance-covariance matrix estimates the error in the coefficients
///   from the observed variance of the points around the best fit line.
///
///  @author  Detlef Hafer
///
///
///  @param[in]  c1 , double *
///  @param[in]  c0 , double *
///  @param[in] n , const int32_t
///  @param[in]  y , const double *
///  @param[in]  x , const double *
///  @return int32_t
///
//------------------------------------------------------------------------------------
#if 1
int32_t gsl_fit_linear(const double* x, const double* y, const int32_t n, double* intercept, double* slope)
{
	double mxy = 0.0;
	double mx = 0.0;
	double my = 0.0;
	double mxx = 0.0;

	assert(n > 0);
	for (int32_t i = 0; i < n; i++)
	{
		const double dx = x[i];
		const double dy = y[i];
		my  += dy;
		mx  += dx;
		mxy += (dx * dy);
		mxx += (dx * dx);
	}
	*slope = (n * mxy - mx * my) / (n * mxx - mx * mx);
	*intercept = ( my - (*slope) * mx) / n;
	return 0;
}
#else
int32_t gsl_fit_linear(const double* x, const double* y, const int32_t n, double* c0, double* c1)
{
	double m_x = 0, m_y = 0, m_dx2 = 0, m_dxdy = 0;
	int32_t i;

	for (i = 0; i < n; i++)
	{
		m_x += (x[i] - m_x) / (i + 1);
		m_y += (y[i] - m_y) / (i + 1);
	}

	for (i = 0; i < n; i++)
	{
		const double dx = x[i] - m_x;
		const double dy = y[i] - m_y;

		m_dx2 += (dx * dx - m_dx2) / (i + 1);
		m_dxdy += (dx * dy - m_dxdy) / (i + 1);
	}

	/* In terms of y = a + b x */
	double s2 = 0, d2 = 0;
	double b = m_dxdy / m_dx2;
	double a = m_y - m_x * b;

	*c0 = a;
	*c1 = b;
	return 0;
}
#endif

};



