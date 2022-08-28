#include "PCHIPInterpolator.h"
#include <cstdlib>
#include <cmath>

// Based on algine::PCHIPInterpolator: https://github.com/congard/algine
// algine::PCHIPInterpolator is based on https://github.com/slabanja/SimplePCHIP
// which is written in Julia and licensed under the Simplified "2-clause" BSD License:
// Copyright (c) 2017: mattias slabanja. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

template <typename T>
inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template<typename T>
inline bool approxZero(T n) {
    constexpr T epsilon = 10E-8;
    return std::abs(n) <= epsilon;
}

std::vector<PointF> PCHIPInterpolator::getInterpolatedPoints(const std::vector<PointF> &src, PointF scale, int count) {
    // step 1: calculate ds

    auto edgeDerivative = [](float h1, float h2, float d1, float d2) {
        float d = ((2 * h1 + h2) * d1 - h2 * d2) / (h1 + h2);

        if (sign(d) != sign(d1))
            return 0.0f;

        if (sign(d1) != sign(d2)  &&  std::abs(d) > std::abs(3 * d1))
            return 3 * d1;

        return d;
    };

    auto xAt = [&](int i) { return src[i].x * scale.x; };
    auto yAt = [&](int i) { return src[i].y * scale.y; };

    auto h = [&, xAt](int i) { return xAt(i + 1) - xAt(i); };
    auto d = [&, yAt, h](int i) { return (yAt(i + 1) - yAt(i)) / h(i); };

    int n = static_cast<int>(src.size());
    std::vector<float> ds(n);

    if (n == 2) {
        std::fill(ds.begin(), ds.end(), d(0));
    } else {
        float dl = d(0);
        float hl = h(0);

        for (int i = 1; i < n - 1; ++i) {
            float dr = d(i);
            float hr = h(i);

            if (sign(dl) != sign(dr) || approxZero(dl) || approxZero(dr)) {
                ds[i] = 0.0f;
            } else {
                float wl = 2 * hl + hr;
                float wr = hl + 2 * hr;
                ds[i] = (wl + wr) / (wl / dl + wr / dr);
            }

            dl = dr;
            hl = hr;
        }

        ds[0] = edgeDerivative(h(0), h(1), d(0), d(1));
        ds[n - 1] = edgeDerivative(h(n - 2), h(n - 3), d(n - 2), d(n - 3));
    }

    // step 2: interpolate

    // optimization: remember last (previous) index,
    // so we will not start search from the beginning
    int i = 0;

    auto interpolateY = [&, xAt, yAt](float x) {
        // linear search; we don't need binary search since
        // it will give worse complexity in our particular case
        while (i < n - 1 && x >= xAt(i + 1)) ++i;

        // Treat right endpoint as part of rightmost interval
        if (i == n - 1)
            --i;

        float x1 = xAt(i);
        float x2 = xAt(i + 1);
        float y1 = yAt(i);
        float y2 = yAt(i + 1);
        float d1 = ds[i];
        float d2 = ds[i + 1];
        float h = x2 - x1;

        auto phi = [](float t) { return 3 * powf(t, 2) - 2 * powf(t, 3); };
        auto psi = [](float t) { return powf(t, 3) - powf(t, 2); };

        return y1 * phi((x2 - x) / h) + y2 * phi((x - x1) / h) - d1*h * psi((x2 - x) / h) + d2 * h * psi((x - x1) / h);
    };

    std::vector<PointF> result;
    result.reserve(count);

    for (int j = 0; j < count; ++j) {
        // calculate x by mixing (linear interpolation)
        float t = (float) j / (float) (count - 1);
        float x = xAt(0) + (xAt(n - 1) - xAt(0)) * t;
        float y = interpolateY(x);
        result.emplace_back(x / scale.x, y / scale.y); // scale back
    }

    return result;
}
