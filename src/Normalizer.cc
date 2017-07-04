// This file is part of LatticeTester.
//
// LatticeTester
// Copyright (C) 2012-2016  Pierre L'Ecuyer and Universite de Montreal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef WITH_NTL
#include "NTL/ZZ.h"
#include "NTL/tools.h"
#endif

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "latticetester/Types.h"
#include "latticetester/Normalizer.h"


namespace LatticeTester
{

const int Normalizer::MAX_DIM;

/*-------------------------------------------------------------------------*/

Normalizer::Normalizer (const RScal & logDensity0, int maxDim, std::string name,
                        NormType norm, double beta0) :
      m_name(name), m_norm(norm), m_logDensity(logDensity0), m_maxDim(maxDim),
      m_beta(beta0)
{
   m_bounds = new double[maxDim + 1];
}

/*-------------------------------------------------------------------------*/


void Normalizer::init (const RScal &logDensity0, double beta0)
/*
 * Computes the vector m_bounds that corresponds to the upper bound for a lattice of
 * density \f$Density_0\f$.
 */
{
   double x, y;
   double logBeta;
   m_logDensity = logDensity0;
   m_beta = beta0;

   y = 1.0;
   logBeta = log (m_beta);

   //PW_TODO: check 1/2 gamma
   for (int j = 1; j <= m_maxDim; j++) {
      y =  1. / j;
      x = 0.5 * log (getGamma(j)) + j * logBeta - y * logDensity0;
      //log calculation to handle large values of n

      if (m_norm == L2NORM)
         x *= 2; // L2norm is always used squared

      m_bounds[j] = exp (x);
   }
}


/*-------------------------------------------------------------------------*/

std::string Normalizer::ToString () const
{
   std::ostringstream os;
   os << "-----------------------------\n"
   << "Content of Normalizer object:\n\n Normalizer = " << m_name;
   os << "\n n = " << exp(m_logDensity);
   os << "(log(n) = " << m_logDensity << ")";
   os << "\n beta = " << std::setprecision (4) << m_beta << "\n\n";

   //   os.setf(std::ios::left);
   os << std::setprecision (13);
   for (int t = 1; t <= m_maxDim; t++) {
      os << " Bound[" << std::setw(2) << std::right << t << "] = "
      << std::setw(14) << std::left << m_bounds[t] << "\n";
   }
   os << "\n";
   return os.str ();
}


/*-------------------------------------------------------------------------*/

double Normalizer::getGamma (int) const
{
   return 1.0;
}


/*-------------------------------------------------------------------------*/

double & Normalizer::getPreComputedBound (int j)
{
   assert (j >= 1 && j <= m_maxDim);
   return m_bounds[j];
}

/*-------------------------------------------------------------------------*/

double Normalizer::getBound (int j) const
{
   assert (j >= 1 && j <= m_maxDim);

   double x,y;
   double logBeta;
   y = 1./j;
   logBeta = log(m_beta);

   x = 0.5 * log(getGamma(j)) + j * logBeta - y * m_logDensity;
   //log calculation to handle large values of n

   if (m_norm == L2NORM)
      x *= 2; // L2norm is always used squared

   return exp(x);

}

} // end namespace LatticeTester
