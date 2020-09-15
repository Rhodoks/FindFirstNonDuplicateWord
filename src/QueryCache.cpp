struct LinklistNode
{
	int nxt,lst;
	unordered_map<string,int>::iterator it;
	
};

//QueryCache Using a linklist and hashmap
//Use LRU algorithm to maintain the cache.

class ClassQueryCache
{
	int MaxCacheSize;
	int HeadUsed,HeadUnused;
	int UsedSize;
	LinklistNode *pool;
	unordered_map<string,int> mp;
public:
	ClassQueryCache(int size)
	{
		MaxCacheSize=size;
		pool=new LinklistNode[size+5];
		HeadUsed=0;
		HeadUnused=1;
		UsedSize=0;
		for (int i=2;i<=MaxCacheSize+5;i++)
		{
			pool[i-1].nxt=i;
			pool[i].lst=i-1;
		}
	}
	
	~ClassQueryCache()
	{
		delete pool;
	}
	
	int Query(const string &str)
	{
		auto p=mp.find(str);
		if (p==mp.end())
			return 0;
		else
			return p->second;
	}
	
	void Update(int id)
	{
		if (id==HeadUsed)
			return;
		pool[pool[id].lst].nxt=pool[id].nxt;
		pool[pool[id].nxt].lst=pool[id].lst;
		pool[id].nxt=HeadUsed;
		pool[HeadUnused].lst=id;
		HeadUnused=id;
	}
	
	void Delete(int id)
	{
		mp.erase(pool[id].it);
		if (id==HeadUsed)
			HeadUsed=pool[HeadUsed].nxt;
		else
			pool[pool[id].lst].nxt=pool[id].nxt;
		pool[pool[id].nxt].lst=pool[id].lst;
		pool[id].nxt=HeadUnused;
		HeadUnused=id;
		UsedSize--;
	}
	
	void Insert(const string &str)
	{
		if (UsedSize==MaxCacheSize)
		{
			Delete(pool[0].lst);
		}
		mp[str]=HeadUnused;
		int tmp=HeadUnused;
		pool[tmp].it=mp.find(str);
		HeadUnused=pool[HeadUnused].nxt;
		pool[tmp].nxt=HeadUsed;
		pool[HeadUsed].lst=tmp;
		HeadUsed=tmp;
		UsedSize++;
	}
};
 
