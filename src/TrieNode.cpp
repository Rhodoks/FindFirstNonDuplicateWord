#include "Function.cpp"

string SourceFilePath="file.txt";
string HashFolderPath="hash/";
const string HashFileNameSuffix="HASH";

struct TrieNode
{
	int nxt[26],depth;
	long long lstid;
	string buffer;
	string val;
	string prefix;
	fstream file;
	bool isleaf;
public:
	
	TrieNode()
	{
		memset(nxt,0,sizeof(nxt));
		depth=0;
	}
	
	string getfilename()
	{
		return HashFolderPath+val+HashFileNameSuffix;
	}
	
	void filewrite()
	{
		//Write Buffer to disk
		file.open(getfilename(),ios::out|ios::app);
		if (!file)
		{
			cout<<"Open HashFile "+HashFolderPath+val+" Failed !"<<endl;
			exit(0);
		}
		file.write(buffer.data(),sizeof(char)*buffer.size());
		
		//Clear the memory
		string tmp; 
		buffer.swap(tmp);
		file.close();
	}
	
	
	void fileclear()
	{
		file.open(getfilename(),ios::out);
		file.close();
		file.clear();
	} 
	
	void getans(pair<string,long long> &ans)
	{
		fstream FileI(getfilename(),ios::in);
		unordered_map<string,long long> wordmap;
		long long wordid=0;
		while (!FileI.eof())
		{
			string record;
			FileI>>record;
			if (record.size()==0)
				break;
			
			//split the word and id.	
			int digitpos=0;
			for (;!isdigit(record[digitpos]);digitpos++);
			string word=record.substr(0,digitpos);
			wordid+=str2ll(record.substr(digitpos));
			
			if (wordmap.find(word)==wordmap.end())
				wordmap[word]=wordid;
			else
				wordmap[word]=0;
		}
		for (auto p:wordmap)
			updans(ans,{val.substr(0,PrefixLen)+p.first,p.second});
		FileI.close();
	}
};
