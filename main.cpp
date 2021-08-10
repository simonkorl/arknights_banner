#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

int num;
double a[101][2], b[101], c[101];
const double NOT6 = 0.98;
const double SINGLE_POOL_UP = 0.01;
const double SINGLE_POOL_WAI = 0.01;
const double LIMIT_POOL_UP = 0.007; // 限定池单六星单抽出率
const int LIM = 50; // 软保底界限
const double INC = 0.02; // 软保底单抽增加概率
double not_six_when_x(int x, int lim) {
  if(x < lim) {
    return NOT6;
  } else {
    return (NOT6 - INC * (x - lim + 1));
  }
}
double get_six_in_single_pool(int x, int lim) {
  if(x < lim) {
    return SINGLE_POOL_UP;
  } else {
    return SINGLE_POOL_UP + (x - lim + 1) * SINGLE_POOL_UP;
  }
}
double wai_six_in_single_pool(int x, int lim) {
  if(x < lim) {
    return SINGLE_POOL_WAI;
  } else {
    return (SINGLE_POOL_UP + (x - lim + 1) * SINGLE_POOL_WAI);
  }
}
double wai_six_in_double_pool(int x, int lim) {
  if(x < lim) {
    return 0.006;
  } else {
    return (0.006 + (x - lim + 1) * 0.006);
  }
}
double wai_six_in_double_pool_2(int x,int lim) {
  if(x < lim) {
    return 0.013;
  } else {
    return 0.013 + (x - lim + 1) * 0.013;
  }
}
double get_six_in_double_pool(int x, int lim) {
  if(x < lim){
    return 0.014;
  } else {
    return 0.014 + (x - lim + 1) * 0.014;
  }
}
double sum_single[100005], sum_get_double_in_double[100005], sum_get_single_in_double[100005];
double sum_get_double_in_double_pool[100005];
int main() {
  cout << "Input maximum roll number: ";
  cin >> num;
  a[0][0] = 1;
  b[0] = 1;
  c[0] = 1;
  sum_single[0] = 1;
  FILE *fp = fopen("data.csv", "w");
  for(int i = 0;i < num; i++) {
    double x = 0, y = 0, z = 0, w = 0;
    for(int j = 99; j >= 0; j--) {
      a[j + 1][0] = a[j][0] * not_six_when_x(j, 50);
      a[j + 1][1] = a[j][1] * not_six_when_x(j, 50);
      b[j + 1] = b[j] * not_six_when_x(j, 50);
      c[j + 1] = c[j] * not_six_when_x(j, 50);

      x += b[j] * wai_six_in_single_pool(j, 50);
      w += c[j] * wai_six_in_double_pool_2(j, 50);

      y += a[j][0] * wai_six_in_double_pool(j, 50);
      z += a[j][1] * wai_six_in_double_pool_2(j, 50);
      z += a[j][0] * get_six_in_double_pool(j, 50);
    }
    b[0] = x;
    a[0][0] = y;
    a[0][1] = z;
    c[0] = w;
    double sum = 0, sum1 = 0, sum2 = 0;
    for(int i = 0; i < 100; i++) {
      sum += b[i];
    }
    for(int j = 0;j < 100; j++) {
      sum_single[i + 1] += b[j];
      sum_get_double_in_double[i + 1] += a[j][1] + a[j][0];
      sum_get_single_in_double[i + 1] += c[j];
    }
    for(int j = 1; j <= i; ++j) {
      sum_get_double_in_double_pool[i + 1] += (sum_single[j - 1] - sum_single[j]) * (1 - sum_single[i + 1 - j]);
    }
    fprintf(fp, "%d,%.4lf,%.4lf,%.4lf,%.4lf\n",
            i + 1, // 抽数
            1 - sum_single[i + 1], // 单 UP 池
            1 - sum_get_double_in_double[i + 1], // 限定池双六星
            1 - sum_get_single_in_double[i + 1], // 限定池目标六星
            sum_get_double_in_double_pool[i + 1] // 两个单 UP 池获得两个 UP
            );
  }
  cout << "Results can be found in data.csv" << endl;
  fclose(fp);
}
