include(cstone_add_performance_test)

cstone_add_performance_test(octree.cpp octree_perf 1)
cstone_add_performance_test(peers.cpp peers_perf 1)
cstone_add_performance_test(scan.cpp scan_perf 1)
cstone_add_performance_test(hilbert.cpp hilbert_perf 1)

# only scan.cpp provides some coverage beyond the unit tests
cstone_add_test(scan_perf EXECUTABLE scan_perf RANKS 1)

if(CMAKE_CUDA_COMPILER)
    cstone_add_cuda_performance_test(neighbor_driver.cu $<TARGET_OBJECTS:cuda_find_neighbors_obj> cudaNeighborsTest 1)
    cstone_add_cuda_performance_test(octree.cu "" octree_perf_gpu 1)
    cstone_add_cuda_performance_test(hilbert.cu "" hilbert_perf_gpu 1)
endif()
