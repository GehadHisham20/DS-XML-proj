/*///////////////////////////////byte pair technique 
#include <iostream>
#include<string>
#include<string.h>
#include <map>
using namespace std;
pair<int, int> fndmax(map<char, int> sampleMap)
{
    pair<char, int> maxval = make_pair(0, 0);

    // Iterate in the map to find the required entry
    map<char, int>::iterator currentEntry;
    for (currentEntry = sampleMap.begin(); currentEntry != sampleMap.end();++currentEntry)
    {

        if (currentEntry->second> maxval.second) 
        { 
            maxval= make_pair(currentEntry->first,currentEntry->second);
        }
    }

    return maxval;
}


string bytepair(string x)
{
    //string news;

	map<char, int>mp;
	for (int i = 0;i < x.length();i++)
	{	
			mp[x[i] + x[i+1]] = 0;

	}
	for (int i = 0;i < x.length();i++)
	{
		if (mp[x[i] + x[i + 1]])
			mp[x[i] + x[i + 1]]++;
	}
    pair<char, int> a = fndmax(mp);
    bool fnd = 0;
    for (int i = 0;i < x.length();i++)
    {
        for (i = 'a';i <= 'z';i++)
        {
            if ('a' != x[i])
                fnd = 1;

        }
    }

    
}*/
///////////////////////////////////////////////run length//////////////////////////////
#include<iostream>
#include<string.h>
#include<string>
////////run length 
using namespace std;
string runlenencode(string str)
{
    string x;
    int n = str.length();
    for (int i = 0; i < n; i++)
    {

        int cnt = 1;
        while (i < n - 1 && str[i] == str[i + 1])
        {
            cnt++;
            i++;
        }

        x += str[i]+ std::to_string(cnt);
             //+cnt;
        // x += to_string(count) + str[i];
        //cout << str[i] << cnt;
    }
    return x;
}
//compliixtity o(n)
/*string runlendecode(string s) 
{
    int i = 0;
    string ans = "";
    while (i < s.length()) {
        int j = i;
        while (isdigit(s[j]) && (j < s.length())) 
        {
            j++;
        }
        int a = stoi(s.substr(i, j - i + 1));
        int k = j;
        while (isalpha(s[k]) && k < s.length()) {
            k++;
        }
        auto t = s.substr(j, k - j);
        for (int k = 0; k < a; ++k) {
            ans += t;
        }
        i = k;
    }
    return ans;
}*/
string runlendecode(string s)
{
    string ans = "";
    if (s != "")
    {
        for (int i = 0;i < s.length();i += 2)
        {
            char ch = s[i];
            int cnt1 = int(s[i + 1])-48;
           // cout << cnt1 << "lol";
            for (int j = 0;j < cnt1;j++)
            {
                ans += ch;
            }
        }
    }
    return ans;
}
//complixity o(n)
int main()
{

    //string x = "aaabdaaabac";
    string x = "zzzzhhhhkkkombyqivdfghjklyturieowamzxncvbghyrueiks";
   string q= runlenencode(x);
   cout << q << endl;
   string finall = runlendecode(q);
   cout << finall;



	return 0;
}
/////////////////////////////////////////////////////huffman ///////////////////
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

class Node {
public:
	int freq;
	char ch;
	Node* left;
	Node* right;
};

Node* new_node(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();
	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;
	return node;
}
// it is used to know which is smaller to be added to huffmen tree
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

void encode(Node* root, string a,unordered_map<char, string>& huff_exp)
{
	if (root == nullptr)
		return;

	if (!root->left && !root->right) {
		huff_exp[root->ch] =a ;
	}

	encode(root->left, a + "0", huff_exp);
	encode(root->right, a + "1", huff_exp);
}
void decode(Node* root, int& idx, string str)
{
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right)
	{
		cout << root->ch;
		return;
	}

	idx++;

	if (str[idx] == '0')
		decode(root->left, idx, str);
	else
		decode(root->right, idx, str);
}

void Hufftree(string x)
{
	unordered_map<char, int> freq;
	for (char ch : x) 
	{
		freq[ch]++;
	}

	// piroty queue to know what node to add next acc to huffman tree
	priority_queue<Node*, vector<Node*>, comp> pq;

	for (auto nxtnode : freq)
	{
		pq.push(new_node(nxtnode.first, nxtnode.second, nullptr, nullptr));
	}

	while (pq.size() != 1)
	{
		// Remove the two nodes of highest priority = lowest frequency 
		
		Node* left = pq.top(); pq.pop();
		Node* right = pq.top();	pq.pop();

		//new node =sum of two least frequancy node 
		int sum = left->freq + right->freq;
		pq.push(new_node('\0', sum, left, right));
	}
	Node* root = pq.top();
	unordered_map<char, string> huff_exp;
	encode(root, "", huff_exp);
	string str = "";
	for (char ch : x) {
		str += huff_exp[ch];
	}
	cout << "Encoded string :\n" << str << '\n';

	int idx = -1;
	cout << "Decoded string : \n";
	while (idx < (int)str.size() - 2) {
		decode(root, idx, str);
	}
}


int main()
{
	string y = "MISSISSIPPI RIVER";
	string x = "hbuoibocbocbdocpspspppjbdbsbibos";
	Hufftree(x);
	cout << endl;
	Hufftree(y);

	return 0;
}
