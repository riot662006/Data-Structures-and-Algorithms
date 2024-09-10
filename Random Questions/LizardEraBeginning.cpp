/*

In the game Lizard Era: Beginning the protagonist will travel with three companions: Lynn, Meliana and Worrigan. Overall the game has n mandatory quests. To perform each of them, you need to take exactly two companions.

The attitude of each of the companions to the hero is an integer. Initially, the attitude of each of them to the hero of neutral and equal to 0. As the hero completes quests, he makes actions that change the attitude of the companions, whom he took to perform this task, in positive or negative direction.

Tell us what companions the hero needs to choose to make their attitude equal after completing all the quests. If this can be done in several ways, choose the one in which the value of resulting attitude is greatest possible.

Input
The first line contains positive integer n (1 ≤ n ≤ 25) — the number of important tasks.

Next n lines contain the descriptions of the tasks — the i-th line contains three integers li, mi, wi — the values by which the attitude of Lynn, Meliana and Worrigan respectively will change towards the hero if the hero takes them on the i-th task. All the numbers in the input are integers and do not exceed 107 in absolute value.

Output
If there is no solution, print in the first line "Impossible".

Otherwise, print n lines, two characters is each line — in the i-th line print the first letters of the companions' names that hero should take to complete the i-th task ('L' for Lynn, 'M' for Meliana, 'W' for Worrigan). Print the letters in any order, if there are multiple solutions, print any of them.

Examples
inputCopy
3
1 0 0
0 1 0
0 0 1
outputCopy
LM
MW
MW
inputCopy
7
0 8 9
5 9 -2
6 -8 -7
9 4 5
-4 -9 9
-4 5 2
-6 8 -7
outputCopy
LM
MW
LM
LW
MW
LM
LW
inputCopy
2
1 0 0
1 1 0
outputCopy
Impossible

*/

#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const ll INF = (25ll * 10000000ll) + 1ll;

// FIRST IMPLEMENTATION - BACKTRACKING O(3^n)

bool compare_recurse(const pair<ll, string>& a, const pair<ll, string>& b) {
    return a.first < b.first;
}
 
pair<ll, string> recurse(vector<vector<ll>> &quests, ll  lynn, ll meliana, ll worrigan, int i) {
    if (i == quests.size()) {
        if (lynn == worrigan && lynn == meliana) {
            return make_pair(lynn, "");
        }

        return make_pair(-INF, "Impossible\n");
    }

    pair<ll, string> LM = recurse(quests, lynn + quests[i][0], meliana + quests[i][1], worrigan, i + 1);
    pair<ll, string> LW = recurse(quests, lynn + quests[i][0], meliana, worrigan + quests[i][2], i + 1);
    pair<ll, string> MW = recurse(quests, lynn, meliana + quests[i][1], worrigan + quests[i][2], i + 1);

    LM.second = "LM\n" + LM.second;
    LW.second = "LW\n" + LW.second;
    MW.second = "MW\n" + MW.second;

    pair<ll, string> max_recurse = max({LM, LW, MW}, compare_recurse);
    if (max_recurse.first == -INF) {
        max_recurse.second = "Impossible\n";
    }

    return max_recurse;
}

// ---------------

// For (a, b) and (c, d), if you want to check if a + c == b + d, you can check if a - b == d - c
// therefore for (lynn_left, meliana_left) (lynn_right, meliana_right), to check if lynn == melania, check if lynn_left - meliana_left == -(lynn_right - meliana_right)

vector<vector<ll>> get_section_subsets(const vector<vector<ll>>::iterator &start_index, const vector<vector<ll>>::iterator &end_index) {
    vector<vector<ll>> subsets;

    int size = end_index - start_index;
    ll base_three_mask = 1;

    ll lynn_max = 0;
    ll meliana_max = 0;
    ll worrigan_max = 0;

    for (int i = 0; i < size; ++i) {
        lynn_max += (*(start_index + i))[0];
        meliana_max += (*(start_index + i))[1];
        worrigan_max += (*(start_index + i))[2];

        base_three_mask *= 3;
    }

    for (ll cur_mask = 0; cur_mask < base_three_mask; ++cur_mask) {
        vector<ll> subset({lynn_max, meliana_max, worrigan_max, cur_mask});

        ll temp_mask = cur_mask;

        for (int i = 0; i < size; ++i) {
            ll digit = temp_mask % 3;

            subset[digit] -= (*(start_index + i))[digit]; // instead of selecting two to ad, select one to remove
            temp_mask /= 3;
        }

        subsets.push_back(subset);
    }

    vector<vector<ll>> complementable_subsets;

    for (auto subset : subsets) {    
        // {mel - lynn, woyr - mel, lynn , mask}
        complementable_subsets.push_back(vector<ll>({subset[1] - subset[0], subset[2] - subset[1], subset[0], subset[3]}));
    }

    return complementable_subsets;
}

int main() {
    int n;
    cin >> n;

    vector<vector<ll>> quests(n, vector<ll> (3, 0));
    for (int i = 0; i < n; ++i) {
        cin >> quests[i][0] >> quests[i][1] >> quests[i][2];
    }

    /*
    pair<ll, string> total = recurse(quests, 0, 0, 0, 0);
    cout << total.second << endl;
    */

    vector<vector<ll>> quests_left = get_section_subsets(quests.begin(), quests.begin() + (quests.size() + 1) / 2);
    vector<vector<ll>> quests_right = get_section_subsets(quests.begin() + (quests.size() + 1) / 2, quests.end());

    sort(quests_right.begin(), quests_right.end());

    ll res = -INF;
    ll res_left_mask = 0;
    ll res_right_mask = 0;


    for (auto left : quests_left) {
        auto after_complement_it = lower_bound(quests_right.begin(), quests_right.end(), vector<ll>({-left[0], -left[1], INF, 0})); // in case of multiple to make sure the last one is picked

        if (after_complement_it == quests_right.begin()) // case comes before first one. Not available
            continue;

        auto pos_complement = *(after_complement_it - 1);

        if (left[0] != -pos_complement[0] || left[1] != -pos_complement[1]) continue;
        
        if (res < (left[2] + pos_complement[2])) {
            res = left[2] + pos_complement[2];

            res_left_mask = left[3];
            res_right_mask = pos_complement[3];
        }
    }

    if (res == -INF) {
        cout << "Impossible" << endl;
        return 0;
    }

    for (int i = 0; i < (quests.size() + 1) / 2; ++i) {
        ll digit = res_left_mask % 3;

        if (digit == 0) {
            cout << "MW";
        } else if (digit == 1) {
            cout << "LW";
        } else {
            cout << "LM";
        }

        cout << endl;
        res_left_mask /= 3;
    }

    for (int i = (quests.size() + 1) / 2; i < quests.size(); ++i) {
        ll digit = res_right_mask % 3;

        if (digit == 0) {
            cout << "MW";
        } else if (digit == 1) {
            cout << "LW";
        } else {
            cout << "LM";
        }

        cout << endl;
        res_right_mask /= 3;
    }

    return 0;
}