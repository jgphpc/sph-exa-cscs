/*
 * MIT License
 *
 * Copyright (c) 2021 CSCS, ETH Zurich
 *               2021 University of Basel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*! @file
 * @brief Evaluate choice of propagator
 *
 * @author Sebastian Keller <sebastian.f.keller@gmail.com>
 * @author Jose A. Escartin <ja.escartin@gmail.com>
 */

#pragma once

#include <variant>

#include "ipropagator.hpp"
#include "std_hydro.hpp"
#include "ve_hydro.hpp"
#ifdef SPH_EXA_HAVE_H5PART
#include "turb_ve.hpp"
#endif

namespace sphexa
{

template<class DomainType, class ParticleDataType>
std::unique_ptr<Propagator<DomainType, ParticleDataType>>
propagatorFactory(const std::string& choice, size_t ngmax, size_t ng0, std::ostream& output, size_t rank)
{
    if (choice == "ve")
    {
        return std::make_unique<HydroVeProp<DomainType, ParticleDataType>>(ngmax, ng0, output, rank);
    }
    if (choice == "std") { return std::make_unique<HydroProp<DomainType, ParticleDataType>>(ngmax, ng0, output, rank); }
    if (choice == "turbulence")
    {
#ifdef SPH_EXA_HAVE_H5PART
        return std::make_unique<TurbVeProp<DomainType, ParticleDataType>>(ngmax, ng0, output, rank);
#endif
    }

    throw std::runtime_error("Unknown propagator choice: " + choice);
}

} // namespace sphexa
