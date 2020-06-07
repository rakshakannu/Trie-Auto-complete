#include<iostream>
#include<fstream>
#include<cctype>
#include<vector>
#include<string.h>
#include<algorithm>
using namespace std;

class node
{
public:
    node()
    {
        wData = ' ';
        wEnd = false;
    }
    ~node() {}
    char Data()
    {
        return wData;
    }
    void setData(char c)
    {
        wData = c;
    }
    bool WordTerminate()
    {
        return wEnd;
    }
    void setWordTerminate()
    {
        wEnd = true;
    }
    node* findingChild(char c);
    void appendChild(node* child)
    {
        wChildren.push_back(child);
    }
    vector<node*> children()
    {
        return wChildren;
    }
private:
    char wData;
    bool wEnd;
    vector<node*> wChildren;
};

node* node::findingChild(char c)
{
    for ( int ii = 0; ii < wChildren.size(); ii++ )
    {
        node* temp1 = wChildren.at(ii);//returns the character at the specified position
        if ( temp1->Data() == c )
        {
            return temp1;
        }
    }
    return NULL;
}

class Trie
{
public:
    Trie();
    ~Trie();
    void addWord(string s);
    bool Complete(string s,vector<string>&);
    bool searchWord(string);
    void parseTree(node *current,char * s,vector<string>&,bool &loop);

private:
    node* root;
};
Trie::Trie()
{
    root = new node();
}
Trie::~Trie()
{
    // Free memory
}
void Trie::addWord(string s)
{
    node* current = root;
    if ( s.length() == 0 )
    {
        current->setWordTerminate(); // an empty word
        return;
    }
    for ( int ii = 0; ii < s.length(); ii++ )
    {
        node* child = current->findingChild(s[ii]);
        if ( child != NULL )
        {
            current = child;
        }
        else
        {
            node* tmp = new node();
            tmp->setData(s[ii]);
            current->appendChild(tmp);
            current = tmp;
        }
        if ( ii == s.length() - 1 )
            current->setWordTerminate();
    }
}

bool Trie::searchWord(string s)
{
    node* current = root;
    while ( current != NULL )
    {
        for ( int i = 0; i < s.length(); i++ )
        {
            node* temp1 = current->findingChild(s[i]);
            if ( temp1 == NULL )
                return false;
            current = temp1;
        }
        if ( current->WordTerminate() )
            return true;
        else
            return false;
    }
    return false;
}

bool Trie::Complete(string s, vector<string> &res)
{
    node* current=root;
    for ( int ii = 0; ii < s.length(); ii++ )
    {
        node* tmp = current->findingChild(s[ii]);
        if ( tmp == NULL )
            return false;
        current = tmp;
    }
    char c[100];
    strcpy(c,s.c_str());
    bool loop=true;
    parseTree(current,c,res,loop);
    return true;
}

void Trie::parseTree(node *current, char *s,vector<string> &res,bool& loop)
{
    char k[100]= {0};
    char aa[2]= {0};
    if(loop)
    {
        if(current!=NULL)
        {
            if(current->WordTerminate()==true)
            {
                res.push_back(s);
                if(res.size()>20)
                    loop=false;
            }
            vector<node *> child=current->children();
            for(int jj=0; jj<child.size() && loop; jj++)
            {
                strcpy(k,s);
                aa[0]=child[jj]->Data();
                aa[1]='\0';
                strcat(k,aa);
                if(loop)
                    parseTree(child[jj],k,res,loop);
            }
        }
    }
}
bool loadDictionary(Trie* trie,string fn)
{
    ifstream words;
    ifstream input;
    words.open(fn.c_str());
    if(!words.is_open())
    {
        cout<<"Could not open Dictionary file"<<endl;
        return false;
    }
    while(!words.eof())
    {
        char s[100];
        words >> s;
        trie->addWord(s);
    }
    words.close();
    input.close();
    return true;
}

void WriteNewWord(Trie *trie)
{
	cout<<"Enter the word : ";
	string NewWord;
	cin>>NewWord;
	bool OnlyAlpha=true;
	for(int i=0;i<NewWord.length();i++)
	{
		if(!isalpha(NewWord[i]))
			{
				OnlyAlpha=false;
				break;
			}
	}
	if(OnlyAlpha)
	{
		transform(NewWord.begin(), NewWord.end(), NewWord.begin(), ::tolower);
		vector<string> ListOfWords;
    	trie->Complete(NewWord,ListOfWords);
    	if(ListOfWords.size()!=0)
    	{
    		cout<<"The word '"<<NewWord<<"' already exists in the dictionary.\n";
    		return;
		}
		else
		{
			ofstream out;
			out.open("wordlist.txt",ios::app);
			if(!out.is_open())
			{
				cout<<"Sorry!\nCould not open the dictionary!\n";
				out.close();
				return;
			}
			else
			{
				out<<NewWord<<"\n";
				cout<<"Successfully loaded in the dictionary!\n";
				out.close();
				trie->addWord(NewWord);
				return;
			}
		}
	}
	else
	{
		cout<<"\nNot a valid word!\n";
		return;
	}
}

int main()
{
	Trie* trie = new Trie(); //Where new is used to allocate memory for a C++ class object, the object's constructor is called after the memory is allocated
    char mode;
    cout<<"Loading the dictionary file"<<endl;
    loadDictionary(trie,"wordlist.txt");
    while(1)
    {
        cout<<endl<<endl;
        cout<<"Interactive mode,press "<<endl;
        cout<<"1: Auto Complete Feature"<<endl;
        cout<<"2: Enter new words into the dictionary\n";
        cout<<"3: Quit"<<endl<<endl;
        cin>>mode;
        if(isalpha(mode))
        {
        	cout<<"Invalid Input!\n";
        	cout<<"Enter either 1 or 2..";
            continue;
        }
        switch(mode)
        {
        case '1':
        {
            string s;
            cout<<"Enter the partial word : ";
            cin>>s;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            vector<string> ListOfWords;
            trie->Complete(s,ListOfWords);
            if(ListOfWords.size()==0)
            {
                cout<<"Sorry!\nNo suggestions"<<endl;
                cout<<"Do you want to enter this word into the memory?(y/n) : ";
                char pp;
                cin>>pp;
                if(pp=='y' || pp=='Y')
                {
                	WriteNewWord(trie);
				}
            }
            else
            {
                cout<<"Auto complete reply :"<<endl;
                for(int i=0; i<ListOfWords.size(); i++)
                {
                    cout<<" \t     "<<ListOfWords[i]<<endl;
                }
            }
        }
        continue;
        
        case '2':
        	WriteNewWord(trie);
        	continue;
        	
        case '3':
            delete trie;
            return 0;
        default:
        	cout<<"Invalid input!";
        	cout<<"\nEnter either 1 or 2..";
            continue;
        }
    }
}

