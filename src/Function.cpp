const int PrefixLen=2;
const int HashLen=3;
const int HashBase=97;
const int HashMod=26;

//formula: (s_0 s_1 s_2 ... s_k)_HashBase mod HashMod
char Hash(string str)
{
	int res=0;
	for (int i=0;i<str.size();i++)
		res=(res*HashBase+str[i])%HashMod;
	return res+'a';
}


//The hash value is a string of length 3
//the first and second char is the prefix of word.
//So these two char don't need to be written to disk.
//the third char is hash value.
string GetHash(string str)
{
	string tmp=str.substr(0,PrefixLen);
	tmp.push_back(Hash(str));
	return tmp;
}

//Function to comvert long long int to string
string ll2str(long long x)
{
	string s;
	if (x==0)
		s.push_back('0');
	while (x)
	{
		s.push_back(x%10+'0');
		x/=10;
	}
	reverse(s.begin(),s.end());
	return s;
}


//Function to comvert string to long long int
long long str2ll(string str)
{
	long long ans=0;
	for (auto p:str)
		ans=ans*10+p-'0';
	return ans;
}

//Use record p to update current answer
void updans(pair<string,long long> &ans,pair<string,long long> p)
{
	if (p.second && (ans.second==0 || p.second<ans.second))
		ans=p;
}

