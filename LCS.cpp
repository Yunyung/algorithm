//
//  main.cpp
//  LCS
//
//  Created by Yunyung on 2019/1/19.
//  Copyright © 2019 Yunyung. All rights reserved.
//

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main(int argc, const char * argv[]) {
    string s1, s2;
    cin >> s1 >> s2;

    unsigned long s1_len = s1.length();
    unsigned long s2_len = s2.length();


    int dp[s1_len + 1][s2_len + 1];  // dynamic programming table

    // initial first col and row to 0
    for (int i = 0;i <= s2_len;i++)
        dp[0][i] = 0; // row
    for (int i = 1;i <= s1_len;i++)
        dp[i][0] = 0; // col

    cout << "\t\t";
    for (int i = 0;i < s2_len;i++)
        cout << s2[i] << "\t";
    cout << endl;


    // construct table
    for (int i = 1;i <= s1_len;i++) // The number of rows
    {

        for (int j = 1;j <= s2_len;j++) // The number of cols
        {
             if (s1[i - 1] == s2[j - 1])
                 dp[i][j] = dp[i - 1][j - 1] + 1;
             else
                 dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }

    }
    for (int i = 0;i <= s1_len;i++)
    {
        if (i == 0)
            cout << "\t";
        else
            cout << s1[i - 1] << "\t";
        for (int j = 0;j <= s2_len;j++)
        {
            cout << dp[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "LCS length: " << dp[s1_len][s2_len] << endl;

    // find LCS string (( 依DP table回頭找
    int LCS_len = dp[s1_len][s2_len];
    unsigned long i = s1_len;
    unsigned long j = s2_len;
    string LCS;
    while (LCS_len > 0)
    {
        if (s1[i - 1] == s2[j - 1])
        {
            LCS = s1[i - 1] + LCS;
            LCS_len--;
            i--;
            j--;

        }
        else if (dp[i][j - 1] == dp[i][j])
        {
            j--;
        }
        else if (dp[i - 1][j] == dp[i][j])
        {
            i--;
        }
        else
            cout << "Error occur!" << endl;
    }
    cout << "LCS : " << LCS;
}
