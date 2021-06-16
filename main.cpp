#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<string> address_v;
vector<int> cache_def;
int hit_miss_array[INT16_MAX];
int lsb_index[65];

bool is_number(const string &s);
int BinaryToDecimal(string s);
int LSB();

int main(int argc, char *argv[])
{

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " reference.list cache.org index.rpt" << std::endl;
        return 1;
    }

    string str, testcase;

    freopen(argv[1], "r", stdin);

    // The cache vector will be (Address_bits, BLock_size, Cache_sets, Associativity)
    while (cin >> str)
    {
        if (is_number(str))
        {
            //cout << "[DEBUG] cache_def "<< str << endl;
            cache_def.push_back(atoi(str.c_str()));
        }
    }

    fclose(stdin);
    cin.clear();
    freopen(argv[2], "r", stdin);

    // The address_v vector will be (first_address, second_address ....)
    while (cin >> str)
    {
        if (is_number(str))
        {
            address_v.push_back(str);
        }
        else if (str != ".benchmark" && str != ".end")
        {
            testcase = str;
        }
    }

    fclose(stdin);
    cin.clear();
    freopen(argv[3], "w", stdout);

    cout << "Address bits: " << cache_def[0] << "\n";
    cout << "Block size: " << cache_def[1] << "\n";
    cout << "Cache sets: " << cache_def[2] << "\n";
    cout << "Associativity: " << cache_def[3] << "\n";
    cout << "\n"
         << "\n";

    int miss_count = LSB();

    cout << ".benchmark ";
    cout << testcase << "\n";
    for (int i = 0; i < address_v.size(); i++)
    {
        cout << address_v[i] << " ";
        if (hit_miss_array[i])
        {
            cout << "hit"
                 << "\n";
        }
        else
        {
            cout << "miss"
                 << "\n";
        }
    }
    cout << ".end"
         << "\n";
    cout << "\n";
    cout << "Total cache miss count: " << miss_count << "\n";

    fclose(stdout);
    return 0;
}

bool is_number(const string &s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

int BinaryToDecimal(string s)
{
    return stoi(s, nullptr, 2);
    ;
}

int LSB()
{
    int block_offset = log(cache_def[1]) / log(2); // Find the block offset bit count
    int index = log(cache_def[2]) / log(2);        // Find the index bit count
    int address_bit = cache_def[0];                // Find data tag bit count
    int associativity = cache_def[3];
    int data_tag = address_bit - block_offset - index;
    int miss_count = 0;
    //cout << "associativity= " << associativity << "\n";

    // Log data
    cout << "Offset bit count: " << block_offset << "\n";
    cout << "Indexing bit count: " << index << "\n";
    cout << "Indexing bits: ";
    for (int i = block_offset + index - 1; i >= block_offset; i--)
    {
        cout << i << " ";
    }
    cout << "\n"
         << "\n";
    int cache_size = cache_def[2];
    vector<pair<string, int>> cache[cache_size]; // Cache simulator, for example we have two-way associativity
                                                 // ----------------------------------------------
                                                 //     ||     way-0         ||      way-1       ||
                                                 // ----------------------------------------------
                                                 // | 0 || address | nru-bit || address | nru-bit||
                                                 // | 1 || address | nru-bit || address | nru-bit||
    vector<pair<string, int>>::iterator it;

    //Format NRU-bit to 1
    for (int i = 0; i < cache_size; i++)
    {
        for (int j = 0; j < associativity; j++)
        {
            pair<string, int> p;
            p.first = "";
            p.second = 1;
            cache[i].push_back(p);
        }
    }

    for (int i = 0; i < address_v.size(); i++)
    {
        //cout << "[DEBUG] " << i << "th Address [" << address_v[i] << "]";
        string indexing = address_v[i].substr(data_tag, index);
        string tag = address_v[i].substr(0, data_tag);

        //cout << tag << endl;
        //cout << " pick = [" << indexing << "]";

        int position = BinaryToDecimal(indexing);
        //cout << " cache_position=[" << position << "]" << endl;
        bool to_replace = false;

        for (it = cache[position].begin(); it != cache[position].end(); it++)
        {
            //cout << "[DEBUG] position "<< position << "data=" << it->first << endl;
            if (it->first == tag)
            {
                hit_miss_array[i] = 1;
                it->second = 0;
                break;
            }
            else if (it->second == 1)
            {
                hit_miss_array[i] = 0;
                it->first = tag;
                //cout << "[DEBUG] position "<< position << "data after move=" << it->first << endl;
                it->second = 0;
                miss_count++;
                break;
            }
            else if (it == (cache[position].end() - 1) && it->second == 0)
            {
                hit_miss_array[i] = 0;
                miss_count++;
                to_replace = true;
            }
        }

        // Only execute if every current way is 0
        if (to_replace)
        {
            // replace all nru-bit to 1
            for (it = cache[position].begin(); it != cache[position].end(); it++)
            {
                it->second = 1;
            }
            //put the new item to head
            it = cache[position].begin();
            it->first = tag;
            it->second = 0;
        }
    }

    return miss_count;
}
