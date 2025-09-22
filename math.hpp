#pragma once

using ll = long long;
using ld = long double;

ld fastpow(ld x, ll m) {
    ld ans = 1;
    for (ll i = 1; i <= m; i <<= 1) {
        if (m & i) ans *= x;
        x *= x;
    }
    return ans;
}

