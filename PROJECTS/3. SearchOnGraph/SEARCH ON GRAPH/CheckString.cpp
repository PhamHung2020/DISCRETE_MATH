#include<iostream>
#include<vector>
#include<string>

std::vector<int> alphabet(26, 0);

bool CheckString(const std::string& a, const std::string& b)
{
    // Reset alphabet
    for (int i = 0; i < 26; ++i)
    {
        alphabet[i] = 0;
    }

    // frequency of each letter in string b
    for (int i = 0; i < 5; ++i)
    {
        ++alphabet[b[i] - 'a'];
    }

    for (int i = 1; i < 5; ++i)
    {
        // if a[i] appeared in string b
        if (alphabet[a[i] - 'a'] > 0)
        {
            --alphabet[a[i] - 'a'];
        }
        else
        {
            return false;
        }
    }

    return true;
}

int main()
{
    return 0;
}