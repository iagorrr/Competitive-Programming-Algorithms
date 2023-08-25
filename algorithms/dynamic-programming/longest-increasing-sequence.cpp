int LIS(int N, const vector<int> &as) {
  vector<int> lis(N + 1, oo);
  lis[0] = -oo;

  auto ans = 0;

  for (int i = 0; i < N; ++i) {
    auto it = lower_bound(lis.begin(), lis.end(), as[i]);
    auto pos = (int)(it - lis.begin());

    ans = max(ans, pos);
    lis[pos] = as[i];
  }

  return ans;
}
