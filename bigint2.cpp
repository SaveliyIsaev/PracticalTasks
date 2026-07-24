// #pragma GCC optimize("Ofast")
// #pragma GCC optimize("inline")
// #pragma GCC target("avx2")

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <iostream>
#include <stdint.h>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <utility>
#include <bitset>
#include <random>
#include <string>
#include <vector>
#include <cmath>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <set>

using namespace std;

#define fasttype ios_base::sync_with_stdio(0); std::cin.tie(0); std::cout.tie(0);
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using sigmed = signed;
using str = std::string;
#define ff first
#define ss second
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()
#define kmod (ll)(1e9 + 7)
#define mod69 696969697ll
#define mod1e99 1000000009ll
#define mod121 1234567891ll
#define mod179 1791791791ll
#define mod99 998244353ll
#define YES std::cout << "YES\n"
#define NO std::cout << "NO\n"
const long double pi = 3.1415926535l;

/*
    #ILoveCodestyle
    #ILoveСкуфизм
                     */


template<typename T, typename K>
istream& operator>>(istream& in, pair<T, K>& a) {
  in >> a.ff >> a.ss;
  return in;
}
template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
  for (auto& x : a) in >> x;
  return in;
}

template<typename T, typename K>
ostream& operator<<(ostream& out, const pair<T, K>& a) {
  return out << a.ff << ' ' << a.ss;
}

template<typename T, typename K>
ostream& operator<<(ostream& out, vector<pair<T, K>>& a) {
  for (auto& x : a) out << x << '\n';
  return out;
}

template<typename T>
ostream& operator<<(ostream& out, vector<T>& a) {
  for (auto& x : a) out << x << ' ';
  return out;
}

template<typename T>
ostream& operator<<(ostream& out, vector<vector<T>>& a) {
  for (auto& x : a) out << x << '\n';
  return out;
}

ostream& operator<<(ostream& out, vector<str>& a) {
  for (auto& x : a) out << x << '\n';
  return out;
}

vector<ll> sieve(ll n) {
  vector<bool> O(n + 1, 1);
  for (ll Z = 2; Z * Z <= n; ++Z) {
    if (O[Z]) {
      for (ll V = Z * Z; V <= n; V += Z) O[V] = 0;
    }
  }
  vector<ll> SVO;
  for (ll i = 2; i <= n; ++i)
    if (O[i])
      SVO.pb(i);
  return SVO;
}

ll fastpow(ll a, ll m, ll p) {
  ll ans = 1;
  for (ll i = 1; i <= m; i <<= 1) {
    if (m & i) ans = ans * a % p;
    a = a * a % p;
  }
  return ans;
}

ll fastpow(ll a, ll m) {
  ll ans = 1;
  for (ll i = 1; i <= m; i <<= 1) {
    if (m & i) ans *= a;
    a *= a;
  }
  return ans;
}

constexpr ll MOD = ~0u;

class BigNum {
  bool sign;
  vector<ll> a;
public:
  BigNum() : a{0} {}
  BigNum(BigNum& x) : sign(x.sign), a(x.a) {}
  BigNum(BigNum&& x) : sign(x.sign), a(std::exchange(x.a, vector<ll>())) {}
  BigNum(str s) {
    a.push_back(0);
    if (s.empty()) return;
    sign = (s[0] == '-');
    if (s[0] == '-' || s[0] == '+') s.erase(s.begin());
    for (char &x : s) {
      ll z = 0;
      for (ll i = a.size() - 1; i >= 0; --i) {
        ll p = a[i] * 10ll + z;
        z = p >> 32;
        p &= MOD;
        a[i] = p;
      }
      if (z) a.insert(a.begin(), z);
      z = 0;
      a.back() += x - '0';
      for (ll i = a.size() - 1; i >= 0; --i) {
        if (z + a[i] > MOD) {
          z = 1;
          a[i] &= MOD;
        } else {
          z = 0;
          a[i] += z;
        }
      }
      if (z) a.insert(a.begin(), z);
    }
  }
  BigNum(ll x) {
    if (x < 0) {
      sign = 1;
      x * -1;
    }
    if (x > MOD) a = {(ll)(x >> 32), (ll)x};
    else a = {(ll)x};
  }
  BigNum(bool sign, vector<ll> &x) : sign(sign), a(x) {}
  BigNum(bool sign, vector<ll>&& x) : sign(sign), a(std::exchange(x, vector<ll>())) {}
  BigNum& operator=(BigNum& x) {
    a = x.a;
    sign = x.sign;
    return *this;
  }
  BigNum& operator=(BigNum&& x) {
    sign = x.sign;
    a = std::exchange(x.a, vector<ll>());
    return *this;
  }
  BigNum operator+(BigNum y) {
    if (sign == y.sign) return BigNum(sign, posSum(a, y.a));
    else return *this - y;
  }
  BigNum operator-(BigNum y) {
    if (a.size() < y.a.size() || a.size() == y.a.size() && a < y.a) {
      BigNum z = y - *this;
      return BigNum(!z.sign, std::move(z.a));
    }
    if (sign != y.sign) return BigNum(sign, posSum(a, y.a));
    else return BigNum(sign, posSubs(a, y.a));
  }
  BigNum operator*(BigNum y) {
    BigNum k = Karazuba(*this, y);
    while (k.a.size() > 1 && !k.a[0]) k.a.erase(k.a.begin());
    return k;
  }
  BigNum operator*(ll y) {
    BigNum x = *this;
    return x *= y;
  }
  BigNum& operator*=(ll y) {
    if (y < 0) {
      sign = !sign;
      y *= -1;
    }
    if (!y) {
      a = {0};
      return *this;
    }
    ll z = 0;
    for (ll i = a.size() - 1; i >= 0; --i) {
      ll p = a[i] * y + z;
      z = p >> 32;
      p &= MOD;
      a[i] = p;
    }
    if (z) a.insert(a.begin(), z);
    return *this;
  }
  BigNum operator+=(BigNum b) {
    return *this = *this + b;
  }
  ll operator%(ll x) {
    ll z = 0;
    ll p = 1;
    for (ll i = a.size() - 1; i >= 0; --i) {
      z = (a[i] * p + z) % x;
      p <<= 32;
      p %= x;
    }
    return z;
  }
  BigNum operator/(ll x) {
    vector<ll> res = a;
    ll z = 0;
    for (auto &y : res) {
      ll p = (y + z) % x;
      y = (y + z) / x;
      z = p << 32;
    }
    while (res[0] == 0 && res.size() > 1) res.erase(res.begin());
    return BigNum(sign, res);
  }
  friend istream& operator>>(istream& in, BigNum& a) {
    str s;
    in >> s;
    a = BigNum(s);
    return in;
  }
  
  friend ostream& operator<<(ostream& out, BigNum a) {
    return out << a.a;
    // if (a.sign && a.a[0] != 0) out << '-';
    // if (a.a[0] == 0) return out << 0;
    // str s;
    // while (a.a[0]) {
    //   s += '0' + a % 10;
    //   a = a / 10;
    // }
    // reverse(all(s));
    // return out << s;
  }
private:
  vector<ll> posSum(vector<ll> &x, vector<ll> &y) {
    vector<ll> res;
    reverse(all(x));
    reverse(all(y));
    bool z = 0;
    for (ll i = 0; i < max(x.size(), y.size()); ++i) {
      ll xx = (i < x.size() ? x[i] : 0);
      ll yy = (i < y.size() ? y[i] : 0);
      res.push_back(xx + yy + z & MOD);
      z = xx + yy + z >> 32;
    }
    if (z) res.push_back(1);
    reverse(all(res));
    reverse(all(x));
    reverse(all(y));
    return res;
  }
  vector<ll> posSubs(vector<ll>& x, vector<ll>& y) {
    reverse(all(x));
    reverse(all(y));
    ll z = 0;
    vector<ll> res;
    for (ll i = 0; i < x.size(); ++i) {
      ll xx = x[i];
      ll yy = (i < y.size() ? y[i] : 0);
      if (yy + z <= xx) {
        res.push_back(xx - yy - z);
        z = 0;
      } else {
        res.push_back(xx + MOD + 1 - yy - z);
        z = 1;
      }
    }
    reverse(all(y));
    reverse(all(x));
    while (res.size() > 1 && !res.back()) res.pop_back();
    reverse(all(res));
    return res;
  }
  friend BigNum Karazuba(BigNum x, BigNum y) {
    if (x.sign != y.sign) {
      x.sign = y.sign = 0;
      x = Karazuba(x, y);
      x.sign = 1;
      return x;
    }
    if (x.a.empty()) {
      x.a = {0};
      return y * 0;
    }
    if (y.a.empty()) {
      y.a = {0};
      return x * 0;
    }
    if (x.a.size() == 1) return y * x.a[0];
    if (y.a.size() == 1) return x * y.a[0];
    if (x.a.size() * y.a.size() <= 50) {
      cout << x << '\n' << y << endl;
      BigNum b;
      for (ll z : y.a) {
        if (z > MOD) cout << -1 << '\n';
        b.a.push_back(0);
        if (b.a.size() > 1 && b.a[0] == 0) b.a.erase(b.a.begin());
        b += x * z;
      }
      return b;
    }
    if (x.a.size() < y.a.size()) {
      vector<ll> z(y.a.size() - x.a.size());
      for (ll x : x.a) z.push_back(x);
      std::swap(x.a, z);
    } else if (x.a.size() > y.a.size()) {
      vector<ll> z(x.a.size() - y.a.size());
      for (ll x : y.a) z.push_back(x);
      std::swap(x.a, z);
    }
    while (x.a.size() < y.a.size()) x.a.insert(x.a.begin(), 0);
    while (x.a.size() > y.a.size()) y.a.insert(y.a.begin(), 0);
    ll p = x.a.size() / 2 + 1;
    BigNum c, d, e, f;
    BigNum ce, df, cdef;
    for (ll i = 0; i < x.a.size(); ++i) {
      if (i + p >= x.a.size()) {
        d.a.push_back(x.a[i]);
        f.a.push_back(y.a[i]);
      } else {
        c.a.push_back(x.a[i]);
        e.a.push_back(y.a[i]);
      }
    }
    ce = Karazuba(c, e);
    df = Karazuba(d, f);
    c += d;
    e += f;
    cdef = Karazuba(c, e) - ce - df;
    for (ll i = 0; i < p * p; ++i) ce.a.push_back(0);
    for (ll i = 0; i < p; ++i) cdef.a.push_back(0);
    return ce + cdef + df;
  }
};

//266656141279762961950164914903894730864735048122698134912345336735154009104547025391241255007148928
//268792128316181195268969723252246625174158001226686759030577912383431741302892184649221974046923648
void Solve() {
  BigNum a;
  cin >> a;
  std::cout << a << '\n';
  
}

void ManyTests() {
  ll t = 1;
  cin >> t;
  while (t--) {
    Solve();  
  }
}

sigmed main() {
  // fasttype;
  // ManyTests();
  for (;;) Solve();
}

/*
Through the gates of hell
As we make our way to heaven
Throug the Nazi lines
Primo Victoria

We've been training for years
Now we're ready to strike
As the great operation begins
With the first waves on shore
We're the first ones to fall
Yet soldiers have fallen before
*/

/*
Maybe I, maybe you
Can make a change to the world
Reaching out for a soul
That's kind of lost in the dark
Maybe I maybe you
Can find the key to the stars
To catch the spirit of hope
To save one hopeless heart

You look up to the sky
With all those questions in mind
All you need is to hear
The voice of your heart
In the world full of pain
Someone's calling your name
Why don't wo make it true
Maybe I, maybe you

Maybe I maybe you
Are just dreaming sometimes
But the world would be cold
Witout dreamers like you
*/

/*
Замученный дорогой, я выбился из сил
И в доме лесника я ночлега попросил
С улыбкой добродушной старик меня впустил
И жестом дружелюбным на ужин пригласил

Будь как дома, путник, я ни в чем не откажу
Я ни в чем не откажу
Множество историй, коль желаешь, расскажу
Коль желаешь, расскажу
*/

/*
Humanity
Auf Wiedersehen, it's time to say goodbye
The party's over, as the laughter dies
An angel cries
Humanity
It's au revoir to your insanity
Your fantasies, and lies

You're a drop in the rain
Just a number, not a name 
And you don't see it
You don't believe it
in the end of the day
You're a needle in the 
You signed and sealed it
And now you gotta deal with it
Humanity
*/

/*
Time
It needs time
To win back your love again
I will be there
i will be there...
Love 
Only love
Can bring down the wall someday
I will be there
I will be there

If we'd go again
All the way from the stars
I would try to change
Things that killed our love 
Your pride has built a wall
So strong, that i can't get through
Is there really no change 
To start once again?
I'm loving you...
*/

/*
I follow the Moskva
Down to Gorky Park
Listening to the wind of change
August summer night
Soldiers passing by
Listening to the wind of change'

Take me
To the magic of the moment
On a glory night
When the children of tomorrow share their dream
(Share their dream) With you and me
With you and me...
*/

/*
Древний град Иерусалим
Довлеет над тобой,
Понтий Пилат!
Ты готов сорваться в Рим, махнуть на все рукой
Забрав с собой солдат
Ты устал от этих лиц
От чужой, неискренней земли
Боль тупая бьет в висок
Дню мучений выпал срок

Кровь за кровь!
В том воля не людей, а богов
Смерть за смерть!
Ты должен не роптать, а терпеть
Здесь твой ад!
Не знаешь ты дороги назад
Пей свой яд!
Ты прокуратор, Понтий Пилат

Над Голгофой - траур мглы, ты чувствуешь беду, 
Понтий Пилат!
Средь солдат, от зноя злых,
Твоих решений ждут
Два вора и святой
Он безумен, видит Бог
Виноват лишь в том, что одинок

*/

/*
Сорок пятый год
Третий Белорусский фронт
Расцвел у Преголя истоков
Вошедший во вкус
Советский союз
Несся красной волнойю с востока
Пруссия вторит: "пришел мой черед",
В ДОТах стрекочут турели
Красная армия рвется вперед,
К центру ее цитадели 

В полымя, в объятья оверлорда
[...] дымятся форты
Словно Бог ангелов в бездну низверг
Восемь дней и горло Отто с треском
Все сильней сжимает Василевский
Пред нами пал Кёнигсберг

*/

/*
Crack of the lightning splitting the ground
Thunder is sounding, artillery pounding
Wrath of the Nazis cast on Bastogne
Facing their forces alone!
ALone!
Alone!
Sent from the skies, dumped into the unknown
Their march to Berlin has begun
Spearhead the charge
Surrounded by foes
Eagles are leading the way!
[...]
*/

/*
Turmoil at the front
Wilhelm's forces on the hunt
There's a thunder in the east
It's an attack of the deseaced

They've been facing poisoned gas
7000 charge en masse
Turned the tide with their attack
Made the enemie's force turn back


And that's when the dead men are marching again
Osowiec, then and again
Hundred men, charge again, die aganin
[...]

Two cobatants spar, Hindenburg against the Tsar
Moving twelve batalions large 
Into an enemy countercharge
They'll be fighting for their lifes
As their enemy revives
Russians won't surrender, no
Striking fear into their foe!

And that's when the dead marched again...
Osowiec then and again 
Hundred men, charge again, die again
[...]


[Solo]


And that's when the dead marched again...
Osowiec then and again 
Hundred men, charge again, die again
[...]
*/

/*
As the Wehrmacht overrun
Russia 1941
They don't belong, we stand our ground, a million strong
*/

/*
Silence...
We remember the silence
On that cold winter day
After many months on the battlefield
And we were used to the violence
And then the cannons went silent
On a cold winter day
And the snow fell, 
Voices tang to me from no man's land
"We are all, we are all, we are all, we are all friends!"
"And today we are brothers, tonight we're all friends"
A moment of peace in a war that never ends
And today we're all brothers, we drink and unite
Now Cristmas has arrived and the snow turns the ground white 
*/

/*From the mist a shape, a ship takes form*/

/*
Этот парень был из тех, кто просто любит жизнь...
Любит праздники и громкий смех,
Пыль дорого и ветра свист...
Он был везде и всегда своим влюблял в себя целый свет
Он гнал свой байк, а не лимузин
Таких друзей больше нет!

И в гостинной при свечах он танцевал как Бог
Но зато менялся на глазах, только вспомнит шум дорог
[...]

Ты летящий вдаль, вдаль ангел
Ты летящий вдаль, вдаль ангел
Ты один только друг, друг на все времена
Немного таких среди нас
Ты летящий вдаль, беспечный ангел

Под гитарный жесткий рок, который так любил
На Харлее он домчать нас мог
До небес и звезд любых
Но он исчез и никто не знал,
Куда теперь мчит его байк
Один бродяга нам сказал, что он отправился в рай
*/