//  Copyright (c) 2015 Daniel Bourgeois
//  Copyright (c) 2021 Karame M.Shokooh
//  Copyright (c) 2021 Srinivas Yadav
//  Copyright (c) 2022 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/local/init.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "adjacentdifference_tests.hpp"

////////////////////////////////////////////////////////////////////////////
void adjacent_difference_test_direct()
{
    using namespace hpx::execution;
    test_adjacent_difference_direct(hpx::launch::sync, seq);
    test_adjacent_difference_direct(hpx::launch::sync, unseq);

    test_adjacent_difference_direct(hpx::launch::async, par);
    test_adjacent_difference_direct(hpx::launch::async, par_unseq);

    test_adjacent_difference_async_direct(hpx::launch::sync, seq(task));
    test_adjacent_difference_async_direct(hpx::launch::async, par(task));
}

void adjacent_difference_test_sender()
{
    using namespace hpx::execution;
    test_adjacent_difference_sender(hpx::launch::sync, seq(task));
    test_adjacent_difference_sender(hpx::launch::sync, unseq(task));

    test_adjacent_difference_sender(hpx::launch::async, par(task));
    test_adjacent_difference_sender(hpx::launch::async, par_unseq(task));
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    adjacent_difference_test_direct();
    adjacent_difference_test_sender();

    return hpx::local::finalize();
}

int main(int argc, char* argv[])
{
    // add command line option which controls the random number generator seed
    using namespace hpx::program_options;
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run");

    // By default this test should run on all available cores
    std::vector<std::string> const cfg = {"hpx.os_threads=all"};

    // Initialize and run HPX
    hpx::local::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    HPX_TEST_EQ_MSG(hpx::local::init(hpx_main, argc, argv, init_args), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
