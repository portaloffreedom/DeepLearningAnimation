//
// Created by matteo on 17/06/18.
//

#include "TestTrajectoryIterator.h"
#include <cmath>
#include "catch.hpp"

TestTrajectoryIterator::TestTrajectoryIterator(unsigned int dim, unsigned int n_styles)
        : Trajectory(dim, n_styles)
{}

void test_iterator(TestTrajectoryIterator &trajectory, unsigned const sample_n)
{
    auto samples = trajectory.samples(sample_n);
    int i=0;
    for (auto sample_it = samples.cbegin(); sample_it != samples.cend(); ++sample_it)
    {
        REQUIRE( sample_it->get_index() == std::min(i*samples.sample_jump,
                                                    static_cast<unsigned int>(samples.unfiltered_size()-1)) );
        i++;
    }

    REQUIRE(i == sample_n);
}


TEST_CASE( "Iterator works", "[TrajectoryIterator]" ) {
    TestTrajectoryIterator trajectory(111, 4);

    SECTION("TEST SIZE CALCULATOR") {
        auto samples1 = trajectory.samples(10);
        REQUIRE(samples1.size() == 10);

        auto samples2 = trajectory.samples(1);
        REQUIRE(samples2.size() == 1);

        auto samples3 = trajectory.samples(12);
        REQUIRE(samples3.size() == 12);
    }

    SECTION("TEST ITERATOR") {
        test_iterator(trajectory, 12);
        test_iterator(trajectory, 1);
        test_iterator(trajectory, 2);
        test_iterator(trajectory, 11);
        test_iterator(trajectory, 12);
        test_iterator(trajectory, 13);
        test_iterator(trajectory, 14);
        test_iterator(trajectory, 77);
        test_iterator(trajectory, 111);
    }
}

