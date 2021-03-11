#include <iostream>
using namespace std;
int input[10000009];
int result[10000009];
int main() {
    int n;
    cin >> n;
    int num = 0;
    for (int i = 0; i < n; ++i) {
        int m;
        int v;
        cin >> m >> v;
        input[i] = v;

    }
    result[0]=input[n-1];
    for (int i = n-1; i >=1 ; --i) {
        if (result[num]>=input[i-1]) { num++;
        result[num]=input[i-1];}
    }
    cout << num+1;
    return 0;
}
