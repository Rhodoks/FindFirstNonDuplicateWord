#include "TrieNode.cpp"

const int MinWriteBufferLen=200*1024;

class ClassTrie
{
public:
	TrieNode pool[(int)pow(27,HashLen)];
	int root,cnt;
	
	int getnode()
	{
		return ++cnt;
	}
	
	string cat(string s,char a)
	{
		return s+a;
	}
	
	void build(int curid,string curstr,int curdep)
	{
		pool[curid].depth=curdep;
		if (curdep==HashLen)
		{
			pool[curid].isleaf=true;
			pool[curid].val=curstr;
			pool[curid].fileclear();
			return;
		}
		for (int i=0;i<26;i++)
		{
			pool[curid].nxt[i]=getnode();
			build(pool[curid].nxt[i],cat(curstr,'a'+i),curdep+1);
		}
	}
	
	void destroy(int curid)
	{
		if (pool[curid].isleaf)
		{
			pool[curid].file.close();
			return;
		}
		for (int i=0;i<26;i++)
			destroy(pool[curid].nxt[i]);
	}
	
	void insert(string str,string val,long long wordid)
	{
		int curid=root;
		for (auto p:str)
			curid=pool[curid].nxt[p-'a'];
			
		//Record: Word_without_prefix+Id_difference
		//e.g. 
		//Insert "abcde" wordid=114514
		//last inserted word id is 100000
		//So the record is "cde14514"
		pool[curid].buffer+=val+ll2str(wordid-pool[curid].lstid)+"\n";
		pool[curid].lstid=wordid;
		
		//if bufferlen>limit write buffer into disk.
		if (pool[curid].buffer.size()>MinWriteBufferLen)
			pool[curid].filewrite();
	}
	
	void hashwrite(long long WriteLimit=MinWriteBufferLen,int curid=1)
	{
		//When WriteLimit==0 write all buffer into storage
		if (pool[curid].isleaf)
		{
			if (pool[curid].buffer.size()<WriteLimit)
				return;
			pool[curid].filewrite();
			return;
		}
		for (int i=0;i<26;i++)
			hashwrite(WriteLimit,pool[curid].nxt[i]);
	}
	
	void getans(pair<string,long long> &ans,int curid=1)
	{
		if (pool[curid].isleaf)
		{
			pool[curid].getans(ans);
			return;
		}
		for (int i=0;i<26;i++)
			getans(ans,pool[curid].nxt[i]);
	}
	
	ClassTrie()
	{
		root=getnode();
		cout<<"Begin Initializing Hash File..."<<endl;
		build(root,string(""),0);
		cout<<"Finish."<<endl;
	}
	
	~ClassTrie()
	{
		destroy(root);
	}
};

