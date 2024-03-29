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
 * @brief  A few C++ wrappers for MPI C functions
 *
 * @author Sebastian Keller <sebastian.f.keller@gmail.com>
 */

#pragma once

#include <vector>
#include <cuda_runtime.h>

#include "cstone/primitives/mpi_wrappers.hpp"
#include "cstone/util/noinit_alloc.hpp"
#include "cstone/cuda/errorcheck.cuh"

#ifdef USE_GPU_DIRECT
constexpr inline bool useGpuDirect = true;
#else
constexpr inline bool useGpuDirect = false;
#endif

template<class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
auto mpiSendGpuDirect(T* data,
                      int count,
                      int rank,
                      int tag,
                      std::vector<MPI_Request>& requests,
                      [[maybe_unused]] std::vector<std::vector<T, util::DefaultInitAdaptor<T>>>& buffers)
{
    if constexpr (!useGpuDirect)
    {
        std::vector<T, util::DefaultInitAdaptor<T>> hostBuffer(count);
        checkGpuErrors(cudaMemcpy(hostBuffer.data(), data, count * sizeof(T), cudaMemcpyDeviceToHost));
        auto errCode = mpiSendAsync(hostBuffer.data(), count, rank, tag, requests);
        buffers.push_back(std::move(hostBuffer));

        return errCode;
    }
    else { return mpiSendAsync(data, count, rank, tag, requests); }
}

template<class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
auto mpiRecvGpuDirect(T* data, int count, int rank, int tag, MPI_Status* status)
{
    if constexpr (!useGpuDirect)
    {
        std::vector<T, util::DefaultInitAdaptor<T>> hostBuffer(count);
        auto errCode = mpiRecvSync(hostBuffer.data(), count, rank, tag, status);
        checkGpuErrors(cudaMemcpy(data, hostBuffer.data(), count * sizeof(T), cudaMemcpyHostToDevice));

        return errCode;
    }
    else { return mpiRecvSync(data, count, rank, tag, status); }
}
