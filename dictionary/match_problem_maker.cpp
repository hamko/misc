#include <bits/stdc++.h>
using namespace std;

#ifdef _WIN32
#define scanfll(x) scanf("%I64d", x)
#define printfll(x) printf("%I64d", x)
#else
#define scanfll(x) scanf("%lld", x)
#define printfll(x) printf("%lld", x)
#endif
#define rep(i,n) for(long long i = 0; i < (long long)(n); i++)
#define repi(i,a,b) for(long long i = (long long)(a); i < (long long)(b); i++)
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define fi first
#define se second
#define mt make_tuple
#define mp make_pair
template<class T1, class T2> bool chmin(T1 &a, T2 b) { return b < a && (a = b, true); }
template<class T1, class T2> bool chmax(T1 &a, T2 b) { return a < b && (a = b, true); }

using ll = long long; using vll = vector<ll>; using vvll = vector<vll>;
using ld = long double;  using vld = vector<ld>; 
using vi = vector<int>; using vvi = vector<vi>;
vll conv(vi& v) { vll r(v.size()); rep(i, v.size()) r[i] = v[i]; return r; }
using P = pair<ll, ll>;

template <typename T, typename U> ostream &operator<<(ostream &o, const pair<T, U> &v) {  o << "(" << v.first << ", " << v.second << ")"; return o; }
template<size_t...> struct seq{}; template<size_t N, size_t... Is> struct gen_seq : gen_seq<N-1, N-1, Is...>{}; template<size_t... Is> struct gen_seq<0, Is...> : seq<Is...>{};
template<class Ch, class Tr, class Tuple, size_t... Is>
void print_tuple(basic_ostream<Ch,Tr>& os, Tuple const& t, seq<Is...>){ using s = int[]; (void)s{0, (void(os << (Is == 0? "" : ", ") << get<Is>(t)), 0)...}; }
template<class Ch, class Tr, class... Args> 
auto operator<<(basic_ostream<Ch, Tr>& os, tuple<Args...> const& t) -> basic_ostream<Ch, Tr>& { os << "("; print_tuple(os, t, gen_seq<sizeof...(Args)>()); return os << ")"; }
ostream &operator<<(ostream &o, const vvll &v) { rep(i, v.size()) { rep(j, v[i].size()) o << v[i][j] << " "; cout << endl; } return o; }
template <typename T> ostream &operator<<(ostream &o, const vector<T> &v) { o << '['; rep(i, v.size()) o << v[i] << (i != v.size()-1 ? ", " : ""); o << "]";  return o; }
template <typename T>  ostream &operator<<(ostream &o, const set<T> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it << (next(it) != m.end() ? ", " : ""); o << "]";  return o; }
template <typename T, typename U>  ostream &operator<<(ostream &o, const map<T, U> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it << (next(it) != m.end() ? ", " : ""); o << "]";  return o; }
template <typename T, typename U>  ostream &operator<<(ostream &o, const unordered_map<T, U> &m) { o << '['; for (auto it = m.begin(); it != m.end(); it++) o << *it; o << "]";  return o; }
string bits_to_string(ll mask, ll n) { string s; rep(i, n) s += '0' + !!(mask & (1ll << i)); return s; }
#define ldout fixed << setprecision(40) 

static const double EPS = 1e-14;
static const long long INF = 1e18;
static const long long mo = 1e9+7;

bool ismatch(string s, string p) {
    if (s.length() != p.length()) return false;
    map<char, char> memo;
    map<char, char> memo_rev;
    rep(i, s.length()) {
        if (memo_rev.count(s[i]))  {
            if (memo_rev[s[i]] == p[i])
                continue;
            else 
                return false;
        } 
        if (!memo.count(p[i])) {
            memo[p[i]] = s[i];
            memo_rev[s[i]] = p[i];
        } else {
            if (memo[p[i]] != s[i]) {
                return false;
            }
        }
    }
    rep(i, p.size()) {
        if (!memo.count(p[i])) {
            return false;
        }
    }
    return true;
}

pair<string, ll> get_pattern(string s) {
    map<char, char> memo;
    rep(i, s.length()) {
        ll tmp = memo.size();
        if (!memo.count(s[i]))
            memo[s[i]] = tmp + 'A';
    }
    string ret;
    rep(i, s.length()) {
        ret += memo[s[i]];
    }
    return pair<string, ll>(ret, memo.size());
}

ll count_num(string pattern) {
    ll counter = 0;
    ifstream fin("eiwa.txt");
    string line;
    while (getline(fin, line)) {
        string token;
        istringstream stream(line);
        while (getline(stream, token, '\t')) {
            if (ismatch(token, pattern)) {
                counter++;
            }
            break;
        }
    }
    return counter;
}
vector<string> enumerate(string pattern) {
    ll counter = 0;
    ifstream fin("eiwa.txt");
    string line;
    vector<string> ret;
    while (getline(fin, line)) {
        string token;
        istringstream stream(line);
        while (getline(stream, token, '\t')) {
            if (ismatch(token, pattern)) {
                ret.push_back(token);
                if (ret.size() > 20) return ret;
            }
            break;
        }
    }
    return ret;
}
int main(int argc, char** argv) {
    cin.tie(0); ios::sync_with_stdio(false);
    ifstream fin("eiwa.txt");
//    ifstream fin("test.txt");
    set<string> memo;
    string line;
    while (getline(fin, line)) {
        string token;
        istringstream stream(line);
        ll counter = 0;
        while (getline(stream, token, '\t')) {
            if (token.find(".") != string::npos) break;
            if (token.find(",") != string::npos) break;
            if (token.find("@") != string::npos) break;
            if (token.find("/") != string::npos) break;
            if (token.find(" ") != string::npos) break;
            auto tmp = get_pattern(token);
            string pattern = tmp.fi;
            if (memo.count(pattern)) break;
            memo.insert(pattern);
            ll comp = tmp.se;
            if (enumerate(pattern).size() < 10 && comp < 5) {
                cout << token << " " << pattern << endl;
                cout << comp << " " << enumerate(pattern) << endl;
            }
            break;
        }
    }
    return 0;
}
