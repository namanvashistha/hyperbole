string search(map<string,string>&keywords, string key)
{
	map<string,string>::iterator it=keyword.begin();
	int p=0;
	for(it=keyword.begin();it!=keyword.end();it++)
		if(it->first==key)
		{
			p=1;
			break;
		}
	if(p==1)
	{
		if(keyword.at(key)!="/0")
        	return keyword.at(key);
    	else
        	return "null";
	}
	return "null"
    
}
