#include<iostream>
#include<string>
#include<vector>

std::vector<int> alphabet(26, 0);

void ResetAlphabet()
{
    for (int i = 0; i < 26; ++i)
    {
        alphabet[i] = 0;
    }
}

bool CheckString(const std::string& a, const std::string& b)
{
    ResetAlphabet();

    for (int i = 0; i < 5; ++i)
    {
        ++alphabet[int(b[i] - 'a')];
    }

    for (int i = 1; i < 5; ++i)
    {
        if (alphabet[int(a[i] - 'a')] > 0)
        {
            --alphabet[int(a[i] - 'a')];
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
    std::cout << std::boolalpha << CheckString("words", "dross");

    return 0;
}