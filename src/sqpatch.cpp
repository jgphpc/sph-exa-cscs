#include <iostream>
#include <fstream>
#include <string>

#include "sphexa.hpp"
#include "SqPatch.hpp"

using namespace std;
using namespace sphexa;

#define REPORT_TIME(rank, expr, name) \
    if (rank == 0) sphexa::timer::report_time([&](){ expr; }, name); \
    else { expr; }

int main()
{
    typedef double Real;
    typedef Octree<Real> Tree;
    typedef SqPatch<Real> Dataset;

    // compiler version:
    #ifdef _CRAYC
    //#define CURRENT_PE_ENV "CRAY"
    cout << "compiler: CCE/" << _RELEASE << "." << _RELEASE_MINOR << endl;
    #endif

    //cout << "compiler: GNU/" << <<  << endl;

    #ifdef __GNUC__
    //#define CURRENT_PE_ENV "GNU"
    cout << "compiler: GNU/" << __GNUC__ << "." << __GNUC_MINOR__
        << "." << __GNUC_PATCHLEVEL__
        << endl;
    #endif

    #ifdef __INTEL_COMPILER
    //#define CURRENT_PE_ENV "INTEL"
    cout << "compiler: INTEL/" << __INTEL_COMPILER << endl;
    #endif

    #ifdef __PGI
    //#define CURRENT_PE_ENV "PGI"
    cout << "compiler: PGI/" << __PGIC__
         << "." << __PGIC_MINOR__
         << "." << __PGIC_PATCHLEVEL__
         << endl;
    #endif

    #ifdef USE_MPI
        MPI_Init(NULL, NULL);
        Dataset d(1e6, "bigfiles/squarepatch3D_1M.bin", MPI_COMM_WORLD);
        DistributedDomain<Real> mpi(MPI_COMM_WORLD);
    #else
        Dataset d(1e6, "bigfiles/squarepatch3D_1M.bin");
    #endif

    // {
    //     FILE *checkpoint = fopen("output_sqpatch_2/output5750.txt", "r");
    //     for(int i=0; i<1000000; i++)
    //     {
    //         double dmy1, dmy2;
    //         fscanf(checkpoint, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", 
    //             &d.x[i], &d.y[i], &d.z[i],
    //             &d.vx[i], &d.vy[i], &d.vz[i],
    //             &d.h[i], &d.ro[i], &d.u[i], &d.p[i], &d.c[i],
    //             &d.grad_P_x[i], &d.grad_P_y[i], &d.grad_P_z[i],
    //             &dmy1, &dmy2);
    //         d.neighbors[i].resize(d.ng0);
    //     }
    //     d.stabilizationTimesteps = 1;
    //     for(unsigned int i=0; i<1000000; i++)
    //     {
    //         d.x_m1[i] = d.x[i] - d.vx[i] * d.dt[0];
    //         d.y_m1[i] = d.y[i] - d.vy[i] * d.dt[0];
    //         d.z_m1[i] = d.z[i] - d.vz[i] * d.dt[0];
    //     }
    // }

    Domain<Real, Tree> domain(d.ngmin, d.ng0, d.ngmax);
    Density<Real> density(d.sincIndex, d.K);
    EquationOfStateSqPatch<Real> equationOfState(d.stabilizationTimesteps);
    MomentumEnergySqPatch<Real> momentumEnergy(d.stabilizationTimesteps, d.sincIndex, d.K);
    Timestep<Real> timestep(d.Kcour, d.maxDtIncrease);
    UpdateQuantities<Real> updateQuantities(d.stabilizationTimesteps);
    EnergyConservation<Real> energyConservation;

    vector<int> clist(d.count);
    for(unsigned int i=0; i<d.count; i++)
        clist[i] = i;

#ifndef _JENKINS
    for(int iteration = 0; iteration <= 10000; iteration++)
#else
    for(int iteration = 0; iteration < 1; iteration++)
#endif
    {
        timer::TimePoint start = timer::Clock::now();

        if(d.rank == 0) cout << "Iteration: " << iteration << endl;
        
        #ifdef USE_MPI
            REPORT_TIME(d.rank, mpi.build(d.workload, d.bbox, d.x, d.y, d.z, d.h, clist, d.data, false), "mpi::build");
            REPORT_TIME(d.rank, mpi.synchronizeHalos(&d.x, &d.y, &d.z, &d.h, &d.m), "mpi::synchronizeHalos");
            d.count = clist.size();
            if(d.rank == 0) cout << "# mpi::clist.size: " << clist.size() << " halos: " << mpi.haloCount << endl;
        #endif

        REPORT_TIME(d.rank, domain.buildTree(d.x, d.y, d.z, d.h, d.bbox), "BuildTree");
        // REPORT_TIME(d.rank, mpi.reorder(d.data), "ReorderParticles");
        REPORT_TIME(d.rank, domain.findNeighbors(clist, d.bbox, d.x, d.y, d.z, d.h, d.neighbors), "FindNeighbors");
        REPORT_TIME(d.rank, density.compute(clist, d.bbox, d.neighbors, d.x, d.y, d.z, d.h, d.m, d.ro), "Density");
        REPORT_TIME(d.rank, equationOfState.compute(clist, iteration, d.ro_0, d.p_0, d.ro, d.p, d.u, d.c), "EquationOfState");
        
        #ifdef USE_MPI
            d.resize(d.count); // Discard old neighbors
            REPORT_TIME(d.rank, mpi.synchronizeHalos(&d.vx, &d.vy, &d.vz, &d.ro, &d.p, &d.c), "mpi::synchronizeHalos");
        #endif

        REPORT_TIME(d.rank, momentumEnergy.compute(clist, d.bbox, iteration, d.neighbors, d.x, d.y, d.z, d.h, d.vx, d.vy, d.vz, d.ro, d.p, d.c, d.m, d.grad_P_x, d.grad_P_y, d.grad_P_z, d.du), "MomentumEnergy");
        REPORT_TIME(d.rank, timestep.compute(clist, d.h, d.c, d.dt_m1, d.dt, d.ttot), "Timestep");
        REPORT_TIME(d.rank, updateQuantities.compute(clist, iteration, d.grad_P_x, d.grad_P_y, d.grad_P_z, d.dt, d.du, d.bbox, d.x, d.y, d.z, d.vx, d.vy, d.vz, d.x_m1, d.y_m1, d.z_m1, d.u, d.du_m1, d.dt_m1), "UpdateQuantities");
        REPORT_TIME(d.rank, energyConservation.compute(clist, d.u, d.vx, d.vy, d.vz, d.m, d.etot, d.ecin, d.eint), "EnergyConservation");
        REPORT_TIME(d.rank, domain.updateSmoothingLength(clist, d.neighbors, d.h), "SmoothingLength");

        int totalNeighbors = domain.neighborsSum(clist, d.neighbors);

        if(d.rank == 0)
        {
            cout << "### Check ### Computational domain: ";
            cout << d.bbox.xmin << " " << d.bbox.xmax << " ";
            cout << d.bbox.ymin << " " << d.bbox.ymax << " ";
            cout << d.bbox.zmin << " " << d.bbox.zmax << endl;
            cout << "### Check ### Avg. number of neighbours: " << totalNeighbors << endl;
            cout << "### Check ### Total time: " << d.ttot << ", current time-step: " << d.dt[0] << endl;
            cout << "### Check ### Total energy: " << d.etot << ", (internal: " << d.eint << ", cinetic: " << d.ecin << ")" << endl;
        }

#ifndef _JENKINS
        if(iteration % 100 == 0)
        {
            std::ofstream outputFile("output" + to_string(iteration) + ".txt");
            REPORT_TIME(d.rank, d.writeFile(clist, outputFile), "writeFile");
            outputFile.close();
        }
#endif

        timer::TimePoint stop = timer::Clock::now();
        
        if(d.rank == 0) cout << "=== Total time for iteration " << timer::duration(start, stop) << "s" << endl << endl;
    }

    #ifdef USE_MPI
        MPI_Finalize();
    #endif

    return 0;
}

