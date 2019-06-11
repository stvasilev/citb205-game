#ifndef GENERATOR_H
#define GENERATOR_H

#include <random>
#include <limits>

auto int_generator(int lower_bound_inclusive, int upper_bound_inclusive)
{

    int const lbi = lower_bound_inclusive;
    int const ubi = upper_bound_inclusive;

    static std::random_device          rd;
    std::mt19937                       mt(rd());
    std::uniform_int_distribution<int> dist(lbi, ubi);

    return [dist, mt]() mutable { return dist(mt); };
}

#endif