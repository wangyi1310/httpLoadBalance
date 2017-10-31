#ifndef _Json_
#define _Json_
#include <map>
#include <string>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
enum 
{    Number,
	 String,	
};
struct Node
{
   int type;
   void *val;
   void operator=(const int&src);
   void operator=(const char * src);
   int asInt();
   string asString();
  };
void Node::operator=(const int&src)
{
	type = Number;
	int* tmp = (int*)malloc(sizeof(int));
	*tmp = src;
	val = (void*)tmp;
}
void Node::operator=(const char* src)
{
	type = String;
	string *str = new string(src);
	val = (void*)str;
}
int  Node::asInt()
{
	int tmp = 0;
	string tmp1=string(*static_cast<string*>(val));
    char buff[128]="";
	strcpy(buff,tmp1.c_str());
	tmp=atoi(buff);
	return tmp;
}
string Node::asString()
{
	return string(*static_cast<string*>(val));
}

class MyJson
{
public:
	class Value
	{
	private:
		typedef map<string, Node>::iterator Iterator;
		map<string, Node> val;
		string  name;
	public:
		Value(string mname)
		{
			name = mname;
		}
		Node& operator[](string Key)
		{
			Iterator it = val.find(Key);
			if (it == val.end())
			{
				pair<Iterator, bool> ret;
				ret = val.insert(make_pair(Key, Node()));
				return ret.first->second;
			}
			return it->second;
		}
		void operator=(Value &src)
		{
			name = src.name;
			val = src.val;
		}
		string toStyedString()
		{
			char buff[128] = "";
			string str = name;
			str += ":{\n";
			Iterator it = val.begin();
			for (; it != val.end(); it++)
			{
				str += "	";
				memset(buff, 0, 128);
				sprintf(buff, "\"%s\":", it->first.c_str());
				switch (it->second.type)
				{
				case 0:sprintf(buff + strlen(buff), "%d\n", *(int*)(it->second.val)); break;
				case 1:sprintf(buff + strlen(buff), "%s\n", static_cast<string *>(it->second.val)->c_str()); break;
				}
				str += buff;
			}
			str += "}\n";
			return str;
		}
	};
	class Reader
	{
	public:
		void parse(char *buff, Value &val)
		{
			char *p = buff;
			char *q = p + 1;
			char Key[128] = "";
			char Val[128] = "";
			while (1)
			{
				p = strchr(q + 1, '"');
				if (p == NULL)break;
				p++;
				q = strchr(p + 1, '"');
				strncpy(Key, p, q - p);
				p = q + 2;
				q = strchr(q, '\n');
				strncpy(Val, p, q - p);
				val[Key] = Val;
				memset(Key, 0, 128);
				memset(Val, 0, 128);
			}
		}
	};
};
#endif
