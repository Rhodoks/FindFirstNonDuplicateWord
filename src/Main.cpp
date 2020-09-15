/*
editor: Rhodoks 
last modify: 17:34 2020/9/15
*/

#include <bits/stdc++.h>
using namespace std;

#include "Trie.cpp" 
#include "QueryCache.cpp"

#define _ 0

const int QueryCacheSize=20*1024*1024;

int main()
{
	cout<<"Please Input File Path:"; 
	cin>>SourceFilePath;
	cout<<"Please Word Folder Path:"; 
	cin>>HashFolderPath;
	fstream FileI(SourceFilePath,ios::in);
	if (!FileI)
	{
		cout<<"Failed in Opening Input File."<<endl;
		return 0;
	}
	FileI.seekg(0,ios::end);
	long long FileLen=FileI.tellg();
	FileI.seekg(0,ios::beg);
	cout<<"Open Input File Successfully"<<endl;
	cout<<"The Size of File is: "<<FileLen<<"B"<<endl;
	
	
	long long WordId=0;
	ClassTrie Trie;
	unordered_map<string,long long> ShortWordMap;
	ClassQueryCache QueryCache1(QueryCacheSize),QueryCache2(QueryCacheSize);
	string Word,HashVal; 
	
	cout<<"Begin to Processing Input Data"<<endl;
	while (!FileI.eof())
	{
		FileI>>Word;
		WordId++;
		if (Word.size()<=PrefixLen)
		{
			//Words with length <= 2 will be preserved in a hashmap in memory.
			//If existing only once, record the id of word.
			//Otherwise the value of map is 0.
			if (ShortWordMap.find(Word)==ShortWordMap.end())
				ShortWordMap[Word]=WordId;
			else
				ShortWordMap[Word]=0;
		}
		else
		{
			//Processing words wirh length >= 3.
			
			//Querycache1 preserves the words having existed only once.
			//Querycache2 preserves the words having existed for two or more times.
			
			//If a word has existed for two or more times, we don't need to record them.
			int CacheId=QueryCache2.Query(Word);
			if (CacheId)
			{
				QueryCache2.Update(CacheId);
				continue;
			}
			
			//Otherwise We check it in Cache1.
			//If we find it, then it can be added to Cache2
			//If we can't, just add it to Cache1
			//no matter the result we find in Cache1, we should record them to hash file.
			CacheId=QueryCache1.Query(Word);
			if (CacheId==0)
				QueryCache1.Insert(Word);
			else
			{
				QueryCache1.Delete(CacheId);
				QueryCache2.Insert(Word);
			}
			
			//Get the hash value of word.
			HashVal=GetHash(Word);
			
			//Insert the record into trie.
			//Record will be written to the buffer of the node.
			//If buffer size has exceed the limit, it will be written to hash file. 
			Trie.insert(HashVal,Word.substr(PrefixLen),WordId);
		}
	}
	FileI.close();
	cout<<"Input File Processing Completed"<<endl;
	cout<<WordId<<" Words in Total"<<endl;
	cout<<"Begin to Write Remaining Buffers in Memory to Storage..."<<endl;
	Trie.hashwrite(0);
	cout<<"Finish."<<endl;
	cout<<"Begin to Calculate Answer"<<endl;
	pair<string,long long> ans={string(""),0};
	
	//Get answer of hash file.
	Trie.getans(ans);
	
	//Get answer of short word.
	for (auto p:ShortWordMap)
		updans(ans,p);
	if (ans.second)
	{
		cout<<"The First Non-Duplicate Word is "<<'\"'<<ans.first<<'\"'<<endl;
		cout<<"Its Word Id is "<<ans.second<<endl;
	}
	else
	{
		cout<<"Can't Find Non-Duplicate Word!"<<endl;
	}
	return ~~(0^_^0);
}


