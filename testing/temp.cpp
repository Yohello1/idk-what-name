

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
using namespace std;
using namespace __gnu_pbds;
typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;
typedef pair<int, int> PII;
template <class T>
using Tree = tree<T, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;
struct custom_hash{static uint64_t splitmix64(uint64_t x){x+=0x9e3779b97f4a7c15;x=(x^(x>>30))*0xbf58476d1ce4e5b9;x=(x^(x>>27))*0x94d049bb133111eb;return x^(x>>31);}size_t operator()(uint64_t x)const{static const uint64_t FIXED_RANDOM=chrono::steady_clock::now().time_since_epoch().count();return splitmix64(x+FIXED_RANDOM);}};
const int INF = 0x3f3f3f3f, MOD = 1e9 + 7;
const LL LLINF = 0x3f3f3f3f3f3f3f3f;
const LD PI = acos(-1), EP = 1e-8;
int startTime = clock();
#define IOS ios::sync_with_stdio(0),cin.tie(0);
#define lowbit(x) ((x) & (-x))
#define F first
#define S second
#define deb(...) logger(#__VA_ARGS__, __VA_ARGS__)
template <typename... Args>
void logger(string vals,Args &&...values){cout<<vals<<" = ";string delim;(...,(cout<<delim<<values,delim=", "));cout<<endl;}
double Time(){return(double)(clock()-startTime)/CLOCKS_PER_SEC;}
int rnd(int a,int b) {random_device rd;mt19937 gen(rd());uniform_int_distribution<>distr(a, b);return distr(gen);}
bool is_prime(int n){if(n==2||n==3){return true;}if(n==1||n%2==0||n%3==0){return false;}for(int i=5;i<=n/i;i+=6){if(n%i==0||n%(i+2)==0){return false;}}return true;}
LL qmi(LL a,LL b,LL p){LL res=1;while(b){if(b&1){res=res*a%p;}a=a*a%p;b>>=1;}return res;}
vector<int> base(int num, int base) {vector<int> res;while(num){res.emplace_back(num % base);num/=base;}reverse(res.begin(),res.end());return res;}
template<typename T>inline void scan(T&x){x=0;bool neg=0;T c=getchar();if(c=='-')neg=1,c=getchar();while((c<48)||(c>57))c=getchar();for(;c<48||c>57;c=getchar());for(;c>47&&c<58;c=getchar())x=(x<<3)+(x<<1)+(c&15);if(neg)x*=-1;}
inline void scan(){}template<typename _Ft,typename..._Rint> inline void scan(_Ft&arg,_Rint&...rest){scan(arg);scan(rest...);}
template<typename T>inline void putnl(T n){bool neg=0;if(n<0)n*=-1,neg=1;char snum[65];int i=0;do{snum[i++]=n%10+'0';n/=10;}while(n);--i;if(neg)putchar('-');while(i>=0)putchar(snum[i--]);putchar(10);}
template<typename T>inline void putsp(T n){bool neg=0;if(n<0)n*=-1,neg=1;char snum[65];int i=0;do{snum[i++]=n%10+'0';n/=10;}while(n);--i;if(neg)putchar('-');while(i>=0)putchar(snum[i--]);putchar(32);}
void setIO(string ss = ""){cin.tie(0)->sync_with_stdio(0);if(ss.size()){freopen((ss+".in").c_str(),"r",stdin);freopen((ss+".out").c_str(),"w",stdout);}}
//-----------------------------------------------------------------------------------------------------------------------------------

const int N = 100100;

int n, m;
struct Dude {
    string name; int a, b;
    bool operator< (const Dude &k)const {
        if (a != k.a) {
            return a > k.a;
        } if (b != k.b) {
            return b < k.b;
        }
        return name < k.name;
    }
}p[N];

int32_t main() {
    setIO();
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        string s; int a, b;
        cin >> s >> a >> b;
        p[i] = {s, a, b};
    }
    sort(p, p + n);

    vector<string> s;
    for (int i = 0; i < m; i++) {
        s.emplace_back(p[i].name);
    }
    sort(s.begin(), s.end());
    for (string i : s) {
        cout << i << "\n";
    }
    return 0;
}



/* IMPORTANT STUFF (Things to look out for)
     * INT OVERFLOW (Use long long if uncertain) Check the maximum case
     * ARRAY BOUNDS (Is the index negative?)
     * SPECIAL CASES (n = 0, 1?)
     * DON'T GET STUCK ON ONE APPROACH
     * DON'T WASTE TIME
     * WRITE STUFF DOWN
     * memset(h, -1, sizeof h);
 */
