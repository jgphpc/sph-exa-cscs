/*
 * A miniapp that demonstrates how to make a ring model
 */

#include <fstream>
#include <iomanip>
#include <iostream>

/*
#include <nlohmann/json.hpp>
#include <arbor/assert_macro.hpp>
*/
#include <arbor/common_types.hpp>
#include <arbor/context.hpp>
/*
#include <arbor/load_balance.hpp>
#include <arbor/mc_cell.hpp>
*/
#include <arbor/profile/meter_manager.hpp>
#include <arbor/profile/profiler.hpp>
#include "profile/profiler_macro.hpp"
/*
#include <arbor/simple_sampler.hpp>
#include <arbor/simulation.hpp>
#include <arbor/recipe.hpp>
#include <arbor/version.hpp>

#include <aux/ioutil.hpp>
#include <aux/json_meter.hpp>
#include "parameters.hpp"

#ifdef ARB_MPI_ENABLED
#include <mpi.h>
#include <aux/with_mpi.hpp>
#endif
*/

/*
using arb::cell_gid_type;
using arb::cell_lid_type;
using arb::cell_size_type;
using arb::cell_member_type;
using arb::cell_kind;
using arb::cell_probe_address;
*/
using arb::time_type;

#include "algorithms.hpp"
#include "vector.hpp"
//{{{!
/*
// Writes voltage trace as a json file.
void write_trace_json(const arb::trace_data<double>& trace);

// Generate a cell.
arb::mc_cell branch_cell(arb::cell_gid_type gid, const cell_parameters& params);

class ring_recipe: public arb::recipe {
public:
    ring_recipe(unsigned num_cells, cell_parameters params, unsigned min_delay):
        num_cells_(num_cells),
        cell_params_(params),
        min_delay_(min_delay)
    {}

    cell_size_type num_cells() const override {
        return num_cells_;
    }

    arb::util::unique_any get_cell_description(cell_gid_type gid) const override {
        return branch_cell(gid, cell_params_);
    }

    cell_kind get_cell_kind(cell_gid_type gid) const override {
        return cell_kind::cable1d_neuron;
    }

    // Each cell has one spike detector (at the soma).
    cell_size_type num_sources(cell_gid_type gid) const override {
        return 1;
    }

    // The cell has one target synapse, which will be connected to cell gid-1.
    cell_size_type num_targets(cell_gid_type gid) const override {
        return 1;
    }

    // Each cell has one incoming connection, from cell with gid-1.
    std::vector<arb::cell_connection> connections_on(cell_gid_type gid) const override {
        cell_gid_type src = gid? gid-1: num_cells_-1;
        return {arb::cell_connection({src, 0}, {gid, 0}, event_weight_, min_delay_)};
    }

    // Return one event generator on gid 0. This generates a single event that will
    // kick start the spiking.
    std::vector<arb::event_generator> event_generators(cell_gid_type gid) const override {
        std::vector<arb::event_generator> gens;
        if (!gid) {
            gens.push_back(arb::explicit_generator(arb::pse_vector{{{0, 0}, 0.1, 1.0}}));
        }
        return gens;
    }

    // There is one probe (for measuring voltage at the soma) on the cell.
    cell_size_type num_probes(cell_gid_type gid)  const override {
        return 1;
    }

    arb::probe_info get_probe(cell_member_type id) const override {
        // Get the appropriate kind for measuring voltage.
        cell_probe_address::probe_kind kind = cell_probe_address::membrane_voltage;
        // Measure at the soma.
        arb::segment_location loc(0, 0.0);

        return arb::probe_info{id, kind, cell_probe_address{loc, kind}};
    }

private:
    cell_size_type num_cells_;
    cell_parameters cell_params_;
    double min_delay_;
    float event_weight_ = 0.01;
};

struct cell_stats {
    using size_type = unsigned;
    size_type ncells = 0;
    size_type nsegs = 0;
    size_type ncomp = 0;

    cell_stats(arb::recipe& r) {
#ifdef ARB_MPI_ENABLED
        int nranks, rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &nranks);
        ncells = r.num_cells();
        size_type cells_per_rank = ncells/nranks;
        size_type b = rank*cells_per_rank;
        size_type e = (rank==nranks-1)? ncells: (rank+1)*cells_per_rank;
        size_type nsegs_tmp = 0;
        size_type ncomp_tmp = 0;
        for (size_type i=b; i<e; ++i) {
            auto c = arb::util::any_cast<arb::mc_cell>(r.get_cell_description(i));
            nsegs_tmp += c.num_segments();
            ncomp_tmp += c.num_compartments();
        }
        MPI_Allreduce(&nsegs_tmp, &nsegs, 1, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&ncomp_tmp, &ncomp, 1, MPI_UNSIGNED, MPI_SUM, MPI_COMM_WORLD);
#else
        ncells = r.num_cells();
        for (size_type i=0; i<ncells; ++i) {
            auto c = arb::util::any_cast<arb::mc_cell>(r.get_cell_description(i));
            nsegs += c.num_segments();
            ncomp += c.num_compartments();
        }
#endif
    }

    friend std::ostream& operator<<(std::ostream& o, const cell_stats& s) {
        return o << "cell stats: "
                 << s.ncells << " cells; "
                 << s.nsegs << " segments; "
                 << s.ncomp << " compartments.";
    }
};
*/
//!}}}


int main(int argc, char** argv) {
    try {
        bool root = true;

/*
#ifdef ARB_MPI_ENABLED
        aux::with_mpi guard(argc, argv, false);
        auto context = arb::make_context(arb::proc_allocation(), MPI_COMM_WORLD);
        {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            root = rank==0;
        }
#else
        auto context = arb::make_context();
#endif

*/
        auto context = arb::make_context();
#ifdef ARB_PROFILE_ENABLED
        arb::profile::profiler_initialize(context);
#endif
        std::cout << "starting... " << "\n";

        arb::profile::meter_manager meters;
        meters.start(context);
        // sum of 10 times 2 is 20 
        std::vector<int> v1(10, 2);
        arb::algorithms::sum(v1);
        meters.checkpoint("sum1", context); // profiling

        // make an array 1:20 and sum it up
        auto n = 20000;
        std::vector<int> v2(n);
        for(auto i=0; i<n; ++i) { v2[i] = i+1; }
        arb::algorithms::sum(v2);
        meters.checkpoint("sum2", context); // profiling

        size_t const N_IT = 100;
        size_t const SIZE = 10000;
        //cout << "Vector: ";                 
        Vector sph_v1{1.,2.,3.}, sph_v2{4.,5.,6.};
        VectorField sph_f1(SIZE), sph_f2(SIZE);
        std::fill(sph_f1.begin(), sph_f1.end(), sph_v2);
        std::fill(sph_f2.begin(), sph_f2.end(), sph_v2);

        // https://arbor.readthedocs.io/en/latest/profiler.html#marking-regions
        PE(transform);
        for (auto i=0; i!=N_IT; ++i) {
            std::transform(sph_f1.begin(), sph_f1.end(), sph_f2.cbegin(), sph_f1.begin(),
                 [](auto const &thisValue, auto const &fieldValue)
                 {return thisValue - fieldValue;});
        }    
        PL();
        meters.checkpoint("sum3", context); // profiling        

        auto profile = arb::profile::profiler_summary();
        std::cout << profile << "\n";

        auto report = arb::profile::make_meter_report(meters, context);
        std::cout << report;

//{{{!
/*
        std::cout << aux::mask_stream(root);

        // Print a banner with information about hardware configuration
        std::cout << "gpu:      " << (has_gpu(context)? "yes": "no") << "\n";
        std::cout << "threads:  " << num_threads(context) << "\n";
        std::cout << "mpi:      " << (has_mpi(context)? "yes": "no") << "\n";
        std::cout << "ranks:    " << num_ranks(context) << "\n" << std::endl;
*/ 

//!}}}
    }
    catch (std::exception& e) {
        std::cerr << "exception caught in ring miniapp:\n" << e.what() << "\n";
        return 1;
    }

    return 0;
}

//{{{!
/*
void write_trace_json(const arb::trace_data<double>& trace) {
    std::string path = "./voltages.json";

    nlohmann::json json;
    json["name"] = "ring demo";
    json["units"] = "mV";
    json["cell"] = "0.0";
    json["probe"] = "0";

    auto& jt = json["data"]["time"];
    auto& jy = json["data"]["voltage"];

    for (const auto& sample: trace) {
        jt.push_back(sample.t);
        jy.push_back(sample.v);
    }

    std::ofstream file(path);
    file << std::setw(1) << json << "\n";
}

// Helper used to interpolate in branch_cell.
template <typename T>
double interp(const std::array<T,2>& r, unsigned i, unsigned n) {
    double p = i * 1./(n-1);
    double r0 = r[0];
    double r1 = r[1];
    return r[0] + p*(r1-r0);
}

arb::mc_cell branch_cell(arb::cell_gid_type gid, const cell_parameters& params) {
    arb::mc_cell cell;

    // Add soma.
    auto soma = cell.add_soma(12.6157/2.0); // For area of 500 μm².
    soma->rL = 100;
    soma->add_mechanism("hh");

    std::vector<std::vector<unsigned>> levels;
    levels.push_back({0});

    // Standard mersenne_twister_engine seeded with gid.
    std::mt19937 gen(gid);
    std::uniform_real_distribution<double> dis(0, 1);

    double dend_radius = 0.5; // Diameter of 1 μm for each cable.

    unsigned nsec = 1;
    for (unsigned i=0; i<params.max_depth; ++i) {
        // Branch prob at this level.
        double bp = interp(params.branch_probs, i, params.max_depth);
        // Length at this level.
        double l = interp(params.lengths, i, params.max_depth);
        // Number of compartments at this level.
        unsigned nc = std::round(interp(params.compartments, i, params.max_depth));

        std::vector<unsigned> sec_ids;
        for (unsigned sec: levels[i]) {
            for (unsigned j=0; j<2; ++j) {
                if (dis(gen)<bp) {
                    sec_ids.push_back(nsec++);
                    auto dend = cell.add_cable(sec, arb::section_kind::dendrite, dend_radius, dend_radius, l);
                    dend->set_compartments(nc);
                    dend->add_mechanism("pas");
                    dend->rL = 100;
                }
            }
        }
        if (sec_ids.empty()) {
            break;
        }
        levels.push_back(sec_ids);
    }

    // Add spike threshold detector at the soma.
    cell.add_detector({0,0}, 10);

    // Add a synapse to the mid point of the first dendrite.
    cell.add_synapse({1, 0.5}, "expsyn");

    return cell;
}
*/
//!}}}
