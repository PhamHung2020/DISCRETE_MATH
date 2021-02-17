#include<iostream>
#include<queue>
#include<map>
#include<unordered_map>
int main()
{
    std::unordered_map<int, int> mp;
    mp[2] = 5;
    mp[0] = 6;
    mp[1] = 7;
    mp[3] = 1;
    std::cout << mp.begin()->second << '\n';
    mp[3] = 0;
    std::cout << mp.begin()->second << '\n';
    return 0;
}