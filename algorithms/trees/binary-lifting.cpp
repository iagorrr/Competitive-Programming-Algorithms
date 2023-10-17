/*
 * far[h][i] = the node that 2^h far from node i
 * sometimes is useful invert the order of loops
 * time : O(nlogn)
 * */
const int maxlog = 20;
int far[maxlog + 1][n + 1];
int n;
for (int h = 1; h <= maxlog; h++) {
  for (int i = 1; i <= n; i++) {
    far[h][i] = far[h - 1][far[h - 1][i]];
  }
}
