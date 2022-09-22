# MIT License
# 
# Copyright (c) 2022 CSCS, ETH Zurich
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 
# @author jgphpc
import os
import sys
import reframe as rfm
import reframe.utility.sanity as sn
import reframe.utility.udeps as udeps


# RFM_TRAP_JOB_ERRORS=1 reframe -c
# /scratch/snx3000/piccinal/DEL/jenkins/sph-exa-cscs.git/.jenkins/reframe_ci.py
# --system daint:gpu -n ci_unittests.*peers_perf -r -J p=debug -J account=usup
# --keep-stage-files

# RFM_TRAP_JOB_ERRORS=1 ~/R -c NEW/reframe_ci.py -n ci --system daint:gpu \
# -S image=jfrog.svc.cscs.ch/contbuild/testing/anfink/44692846847247/sph-exa_build \
# -S build_type=cuda -J p=cscsci -r
_unittests = [
    # RFM_TRAP_JOB_ERRORS
    # "lsx",
    # "ls",
    "/usr/local/sbin/coord_samples/coordinate_test",
]

# {{{ unittests
unittests = [
    "/usr/local/sbin/coord_samples/coordinate_test",
    "/usr/local/sbin/hydro/kernel_tests_std",
    "/usr/local/sbin/hydro/kernel_tests_ve",
    "/usr/local/sbin/integration_mpi/box_mpi",
    "/usr/local/sbin/integration_mpi/domain_2ranks",
    "/usr/local/sbin/integration_mpi/domain_nranks",
    "/usr/local/sbin/integration_mpi/exchange_domain",
    "/usr/local/sbin/integration_mpi/exchange_focus",
    "/usr/local/sbin/integration_mpi/exchange_general",
    "/usr/local/sbin/integration_mpi/exchange_halos",
    "/usr/local/sbin/integration_mpi/exchange_halos_gpu",   # TODO: -p debug
    "/usr/local/sbin/integration_mpi/exchange_keys",
    "/usr/local/sbin/integration_mpi/focus_transfer",
    "/usr/local/sbin/integration_mpi/focus_tree",
    "/usr/local/sbin/integration_mpi/globaloctree",
    "/usr/local/sbin/integration_mpi/treedomain",
    # new:
    "/usr/local/sbin/performance/neighbors_test_gpu",
    "/usr/local/sbin/integration_mpi/assignment_gpu",       # -N2 -n2 -pdebug
    "/usr/local/sbin/integration_mpi/domain_gpu",           # -N2 -n2 -pdebug
    "/usr/local/sbin/integration_mpi/exchange_domain_gpu",  # -N2 -n2 -pdebug
    #
    "/usr/local/sbin/performance/hilbert_perf",
    "/usr/local/sbin/performance/hilbert_perf_gpu",
    "/usr/local/sbin/performance/octree_perf",
    "/usr/local/sbin/performance/octree_perf_gpu",
    "/usr/local/sbin/performance/peers_perf",
    "/usr/local/sbin/performance/scan_perf",
    "/usr/local/sbin/ryoanji/cpu_unit_tests/ryoanji_cpu_unit_tests",
    "/usr/local/sbin/ryoanji/global_upsweep_cpu",
    "/usr/local/sbin/ryoanji/global_upsweep_gpu",
    # "/usr/local/sbin/ryoanji/ryoanji_demo/ryoanji_demo",
    "/usr/local/sbin/ryoanji/unit_tests/ryoanji_unit_tests",
    "/usr/local/sbin/unit/component_units",
    "/usr/local/sbin/unit/component_units_omp",
    "/usr/local/sbin/unit_cuda/component_units_cuda",
]

# local/bin/sedov_solution
# local/bin/compare_noh.py
# local/bin/sphexa-cuda
# local/bin/sphexa
# local/bin/compare_solutions.py

# }}}

# {{{ unittests_params
unittests_params = {
    'lsx': "1",
    'ls': "1",
    "/usr/local/sbin/unit_cuda/component_units_cuda": "g",
    "/usr/local/sbin/coord_samples/coordinate_test": "1",
    "/usr/local/sbin/hydro/kernel_tests_ve": "1",
    "/usr/local/sbin/hydro/turbulence_tests": "1",
    "/usr/local/sbin/hydro/kernel_tests_std": "1",
    #
    "/usr/local/sbin/integration_mpi/exchange_keys": "5",
    "/usr/local/sbin/integration_mpi/exchange_focus": "2",
    "/usr/local/sbin/integration_mpi/treedomain": "5",
    "/usr/local/sbin/integration_mpi/exchange_general": "5",
    "/usr/local/sbin/integration_mpi/globaloctree": "2",
    "/usr/local/sbin/integration_mpi/exchange_domain": "5",
    "/usr/local/sbin/integration_mpi/box_mpi": "5",
    "/usr/local/sbin/integration_mpi/domain_2ranks": "2",
    "/usr/local/sbin/integration_mpi/exchange_halos": "2",
    "/usr/local/sbin/integration_mpi/focus_tree": "5",
    "/usr/local/sbin/integration_mpi/domain_nranks": "5",
    "/usr/local/sbin/integration_mpi/exchange_halos_gpu": "2",
    "/usr/local/sbin/integration_mpi/focus_transfer": "2",
    #
    "/usr/local/sbin/unit/component_units_omp": "1",
    "/usr/local/sbin/unit/component_units": "1",
    #
    "/usr/local/sbin/ryoanji/global_upsweep_gpu": "g",
    "/usr/local/sbin/ryoanji/cpu_unit_tests/ryoanji_cpu_unit_tests": "1",
    "/usr/local/sbin/ryoanji/global_upsweep_cpu": "5",
    "/usr/local/sbin/ryoanji/ryoanji_demo/ryoanji_demo": "?",
    "/usr/local/sbin/ryoanji/unit_tests/ryoanji_unit_tests": "g",
    #
    "/usr/local/sbin/performance/hilbert_perf_gpu": "g",
    "/usr/local/sbin/performance/octree_perf_gpu": "g",
    "/usr/local/sbin/performance/peers_perf": "1",
    "/usr/local/sbin/performance/octree_perf": "1",
    # "/usr/local/sbin/performance/neighbors_test_gpu": "g",
    # --> /usr/local/sbin/performance/cudaNeighborsTest
    # "/usr/local/sbin/performance/hilbert_perf": "1",
    # "/usr/local/sbin/performance/scan_perf": "1",
    #
    "/usr/local/sbin/integration_mpi/assignment_gpu": "2",
    "/usr/local/sbin/integration_mpi/domain_gpu": "2",
    "/usr/local/sbin/integration_mpi/exchange_domain_gpu": "2", # -pdebug
}
# TODO: https://sarus.readthedocs.io/en/stable/cookbook/gpu/gpudirect.html
# }}}

#{{{ 2022/08: ci unittests
@rfm.simple_test
class ci_unittests(rfm.RunOnlyRegressionTest):
    valid_systems = ['dom:gpu', 'daint:gpu', 'hohgant:mc']
    valid_prog_environs = ['builtin']
    image = variable(str, value='/usr/local')
    unittest = parameter(unittests)
    sourcesdir = None
    num_tasks = 1

#     @run_before('run')
#     def set_pe(self):
#         if self.current_environ.name not in ['hohgant']:
#             modules = ['sarus']

    @run_before('run')
    def set_executable(self):
        self.executable = self.unittest.replace("/usr/local", self.image)

    @run_before('run')
    def set_ntasks(self):
        if unittests_params[self.unittest] in ["2", "5"]:
            self.num_tasks = int(unittests_params[self.unittest])

        # TODO: -pdebug ?
        if self.unittest.split('/')[-1] in [
            'exchange_halos_gpu', 'assignment_gpu', 'domain_gpu',
            'exchange_domain_gpu'
        ]:
            self.num_tasks_per_node = 1

#         self.job.launcher.options = [
#             'sarus', 'run', mpiflag,
#             f'{self.image}:{self.build_type}',
#         ]
#        self.executable = self.unittest

    #{{{ sanity
    @sanity_function
    def assert_sanity(self):
        skip = [
            '/usr/local/sbin/performance/peers_perf',
            '/usr/local/sbin/performance/octree_perf_gpu',
            '/usr/local/sbin/performance/octree_perf',
            '/usr/local/sbin/performance/hilbert_perf_gpu',
            '/usr/local/sbin/performance/hilbert_perf',
            '/usr/local/sbin/performance/neighbors_test_gpu',
        ]
        if self.unittest in skip:
            return sn.all([sn.assert_not_found(r'error', self.stdout)])
        else:
            return sn.all([
                sn.assert_found(r'PASS', self.stdout),
                ])
    #}}}
#}}}

#{{{ 2022/08: cpu tests
@rfm.simple_test
class ci_cputests(rfm.RunOnlyRegressionTest):
    # ms sarus/1.4.2 # do not use 1.5 to pull images
    # sarus pull --login art.cscs.ch/contbuild/testing/jg/ \
    #                    sph-exa_install:cuda_debug-gpud
    # descr = 'run unittests'
    valid_systems = ['dom:gpu', 'daint:gpu', 'hohgant:mc']
    valid_prog_environs = ['builtin']
    image = variable(str,
                     value='jfrog.svc.cscs.ch/contbuild/testing/anfink/9590261141699040/pasc/sphexa/sph-exa_install')
                     # value='art.cscs.ch/contbuild/testing/jg/sph-exa_install')
    build_type = variable(str, value='pr287')
    # build_type = parameter(['cuda'])
    # build_type = parameter(['cuda_debug-gpud', 'cuda_release-gpud'])
    unittest = parameter(['sedov', 'sedov --ve', 'noh'])
    sourcesdir = None
    num_tasks = 1
    # num_tasks_per_node = 1
    modules = ['sarus']

    @run_before('run')
    def set_sarus(self):
        mpiflag = '--mpi'
        hdf5path = '/usr/local/HDF_Group/HDF5/1.13.2/lib'
        self.job.launcher.options = [
            'sarus', 'run', mpiflag,
            f'{self.image}:{self.build_type}',
            'bash', '-c',
            f'"LD_LIBRARY_PATH={hdf5path}:$LD_LIBRARY_PATH ',  # note the space
            # 'MPICH_RDMA_ENABLED_CUDA=1 ',
            # 'LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libcuda.so ',
            '/usr/local/bin/sphexa', '--init',
        ]
        self.executable = self.unittest
        self.executable_opts = ['-s', '1', '-n', '50', '"']
        #self.variables = {
        #    'LD_LIBRARY_PATH': ''
        #}

    #{{{ sanity
    @sanity_function
    def assert_sanity(self):
        regex1 = r'Total execution time of \d+ iterations of \S+ up to t ='
        return sn.all([
            sn.assert_found(regex1, self.stdout),
        ])
    #}}}
#}}}

#{{{ 2022/08: gpu tests
@rfm.simple_test
class ci_gputests(rfm.RunOnlyRegressionTest):
    # ms sarus/1.4.2 # do not use 1.5 to pull images
    # sarus pull --login art.cscs.ch/contbuild/testing/jg/ \
    #                    sph-exa_install:cuda_debug-gpud
    # descr = 'run unittests'
    valid_systems = ['dom:gpu', 'daint:gpu', 'hohgant:mc']
    valid_prog_environs = ['builtin']
    # jfrog = 'art.cscs.ch/contbuild/testing/jg'
    jfrog = 'jfrog.svc.cscs.ch/contbuild/testing/anfink'
    image = variable(str, value=f'{jfrog}/9590261141699040/pasc/sphexa/sph-exa_install')
    build_type = variable(str, value='pr287')
    # build_type = parameter(['cuda'])
    # build_type = parameter(['cuda_debug-gpud', 'cuda_release-gpud',
    #                         'cuda_debug_plus_gpud',
    #                         'cuda_release_plus_gpud'])
    unittest = parameter(['sedov', 'noh', 'evrard'])
    sourcesdir = None
    num_tasks = 1
    # num_tasks_per_node = 1
    modules = ['sarus']

    @run_before('run')
    def set_sarus(self):
        mpiflag = '--mpi'
        hdf5path = '/usr/local/HDF_Group/HDF5/1.13.2/lib'   # <------------
        mountflag = ('--mount=type=bind,source="$PWD",'
                     'destination="/scratch"')
        if self.unittest in ['evrard']:
            glass_cp = 'cp /scratch/glass.h5 . ;'
        else:
            glass_cp = ''

        self.job.launcher.options = [
            'sarus', 'run', mpiflag, mountflag,
            f'{self.image}:{self.build_type}',
            'bash', '-c',
            f'"{glass_cp} LD_LIBRARY_PATH={hdf5path}:$LD_LIBRARY_PATH ',
            # note the space
            'MPICH_RDMA_ENABLED_CUDA=1 ',
            'LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libcuda.so ',
            '/usr/local/bin/sphexa-cuda', '--init',  # NOTE: -cuda
        ]
        self.executable = self.unittest
        in_path = 'ftp://ftp.cscs.ch/out/jgp/hpc/containers/in'
        in_file = 'glass.h5'
        if self.unittest in ['evrard']:
            self.prerun_cmds = [f'wget --quiet {in_path}/{in_file}']

        fields = '-f rho,p,u,x,y,z,vx,vy,vz'
        opts = {
            # 'sedov': '-s 200 -n 50 -w 200 --quiet;',
            'sedov': f'{fields} -s 200 -n 50 -w 200 --outDir /scratch/ ',
            'noh': f'{fields} -s 200 -n 50 -w 200 --outDir /scratch/ ',
            'evrard': (f'--glass {in_file} -s 10 -n 50 '
                       '-w 10 --outDir /scratch/ '),  # NOTE: no "
        }
        compare_executable = ';ln -fs /usr/local/bin/sedov_solution .;'
        compare = {
            'sedov': ('python3 /usr/local/bin/compare_solutions.py -s 200 '
                      '/scratch/dump_sedov.h5 > /scratch/sedov.rpt "'),
            'noh': ('python3 /usr/local/bin/compare_noh.py -s 200 '
                    '/scratch/dump_noh.h5 > /scratch/noh.rpt "'),
            'evrard': ('echo -e \\"Density L1 error 0.0\\nPressure L1 '
                       'error 0.0\\nVelocity L1 error 0.0\\n\\" > '
                       '/scratch/evrard.rpt "')
        }
        self.executable_opts = [
            opts[self.unittest],
            compare_executable,
            compare[self.unittest]
        ]
        self.postrun_cmds = [
            'cat *.rpt',
            '# "https://reframe-hpc.readthedocs.io/en/stable/manpage.html?'
            'highlight=RFM_TRAP_JOB_ERRORS',
        ]

    @performance_function('')
    def extract_L1(self, metric='Density'):
        if metric not in ('Density', 'Pressure', 'Velocity', 'Energy'):
            raise ValueError(f'illegal value (L1 metric={metric!r})')

        return sn.extractsingle(rf'{metric} L1 error (\S+)$',
                                f'{self.unittest}.rpt', 1, float)

    @run_before('performance')
    def set_perf_variables(self):
        self.perf_variables = {
            'Density': self.extract_L1('Density'),
            'Pressure': self.extract_L1('Pressure'),
            'Velocity': self.extract_L1('Velocity'),
            # 'Energy': self.extract_L1('Energy'),
        }

    @run_before('performance')
    def set_reference(self):
        reference_d = {
            'sedov': {
                'Density':  (0.138, -0.01, 0.01, ''),
                'Pressure':  (0.902, -0.01, 0.01, ''),
                'Velocity':  (0.915, -0.01, 0.01, ''),
                # 'Energy':  (0., -0.05, 0.05, ''),
            },
            'noh': {
                'Density':  (0.955, -0.01, 0.01, ''),
                'Pressure':  (0.388, -0.01, 0.01, ''),
                'Velocity':  (0.0384, -0.05, 0.05, ''),
                # 'Energy':  (0.029, -0.05, 0.05, ''),
            },
            'evrard': {
                'Density':  (0.0, -0.01, 0.01, ''),
                'Pressure':  (0.0, -0.01, 0.01, ''),
                'Velocity':  (0.0, -0.05, 0.05, ''),
                # 'Energy':  (0.029, -0.05, 0.05, ''),
            },
        }
        self.reference = {'*': reference_d[self.unittest]}

    #{{{ sanity
    @sanity_function
    def assert_sanity(self):
        regex1 = r'Total execution time of \d+ iterations of \S+ up to t ='
        return sn.all([
            sn.assert_found(regex1, self.stdout),
        ])
    #}}}
#}}}
