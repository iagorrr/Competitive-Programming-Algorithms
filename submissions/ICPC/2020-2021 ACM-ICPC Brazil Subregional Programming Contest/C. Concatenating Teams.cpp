/*
	To a string not beculiar a few things must happen:
	Looking to a string Ai from A:
		- let P be some prefix of A
		- and S be the rest of string
		- so Ai = P + S
		- then P must be a string in A
		- S must be a prefix of some string in B
		in a way that if there is a Bj in B such 
		that Bj = S + S', and S' also belongs to B
			- because than we will have two concatenated strings 
			generated by different strings, these are:
				Ai + S' = P + Bj
			- so Ai, P, S', and Bj are non peculiar
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector<ll>;
using pll = pair<ll, ll>;

const int MAXN(1'000'000);

const ll MOD = 1000027957;
const ll MOD2 = 1000015187;
const ll P = 31;

ll p[MAXN + 1], p2[MAXN + 1];
void precompute() {
	p[0] = p2[0] = 1;
	for (int i = 1; i <= MAXN; i++) p[i] = (P * p[i - 1]) % MOD, p2[i] = (P * p2[i - 1]) % MOD2;
}

struct Hash {
	int n;
	vll h, h2, hi, hi2;
	Hash() {}
	Hash(const string& s) : n(s.size()), h(n), h2(n), hi(n), hi2(n) {
		h[0] = h2[0] = s[0];
		for (int i = 1; i < n; i++)
			h[i] = (s[i] + h[i - 1] * P) % MOD, h2[i] = (s[i] + h2[i - 1] * P) % MOD2;

		hi[n - 1] = hi2[n - 1] = s[n - 1];
		for (int i = n - 2; i >= 0; i--)
			hi[i] = (s[i] + hi[i + 1] * P) % MOD, hi2[i] = (s[i] + hi2[i + 1] * P) % MOD2;
	}
	pll query(int l, int r) {
		ll hash = (h[r] - (l ? h[l - 1] * p[r - l + 1] % MOD : 0));
		ll hash2 = (h2[r] - (l ? h2[l - 1] * p2[r - l + 1] % MOD2 : 0));

		return { (hash < 0 ? hash + MOD : hash), (hash2 < 0 ? hash2 + MOD2 : hash2) };
	}
	pll query_inv(int l, int r) {
		ll hash = (hi[l] - (r + 1 < n ? hi[r + 1] * p[r - l + 1] % MOD : 0));
		ll hash2 = (hi2[l] - (r + 1 < n ? hi2[r + 1] * p2[r - l + 1] % MOD2 : 0));
		return { (hash < 0 ? hash + MOD : hash), (hash2 < 0 ? hash2 + MOD2 : hash2) };
	}
};

pair<int,int> solve(vector<string>& as, vector<string>& bs) {
	// string it self, it's complement, and if belong to bs
	using t = tuple<pll, pll, bool>;
	map<pll, vector<t>> by_S;

	vector<Hash> hash_as;
	set<pll> hashes_as;
	for (auto &ai : as) {
		Hash hash_ai = Hash(ai);
		hash_as.push_back(hash_ai);
		hashes_as.emplace(hash_ai.query(0, hash_ai.n-1));
	}
	
	for (auto &hash_ai : hash_as) {
		int n = hash_ai.n;
		auto full = hash_ai.query(0,n-1);
		for (int i = 0; i < n - 1; i++) {
			auto pref = hash_ai.query(0, i);
			auto suff = hash_ai.query(i+1, n-1);
			if (hashes_as.count(pref)) {
				by_S[suff].push_back({full,pref, 0});
			}
		}
	}

	// save every hash from Bs
	set<pll> bs_full_hashes;
	vector<Hash> bs_hashes;
	for (auto &bi : bs) {
		Hash bi_hash = Hash(bi);
		bs_hashes.emplace_back(bi_hash);
		bs_full_hashes.emplace(bi_hash.query(0, (int)bi.size()-1));
	}

	// See which S are valid
	set<pll> allowed_S;
	for (auto &bi_hash : bs_hashes) {
		int n = bi_hash.n;
		for (int i = 0; i < n -1; i++) {
			auto pref = bi_hash.query(0, i);
			auto suff = bi_hash.query(i+1, n-1);
			if (bs_full_hashes.count(suff)) {
				if (by_S[pref].size() >= 1) {
					allowed_S.emplace(pref);
					by_S[pref].push_back({bi_hash.query(0, n-1), suff, 1});
				}
			}

		}
	}

	// just mark every one as unpeculiar
	set<pll> bad_a, bad_b;
	for (auto &[key, triplets] : by_S) {
		if (allowed_S.count(key) == 0) continue;

		for (auto &[a, b, c ] : triplets) {
			if (c == 0) {
				bad_a.emplace(a);
				bad_a.emplace(b);
			}
			else {
				bad_b.emplace(a);
				bad_b.emplace(b);
			}
		}
	}
	return {as.size() - bad_a.size(), bs.size() - bad_b.size()};
}

int32_t main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	precompute();

	int m, n;
	cin >> m >> n;
	vector<string> as(m), bs(n);
	for (auto& ai : as) cin >> ai;
	for (auto& bi : bs) cin >> bi;

	auto [ansa, ansb] = solve(as, bs);

	cout << ansa << ' ' << ansb << '\n';
}

// AC, hashing