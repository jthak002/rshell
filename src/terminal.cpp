#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
bool CONTROL=true;
//****************************************************
bool execute(char* a, int &t)
{
    string scom,sarg;
    int status;
    unsigned size=sizeof(a)/sizeof(char);
    if(size==0)
        return false;
        
    bool flag=false;
    for(unsigned i=0;i<size || a[i]!='\0';i++)
    {
        if(a[i]==' ' && !flag)
        {
            flag=true; continue;
        }
        if(flag)
        {
            sarg=sarg+a[i];
            continue;
        }
        scom=scom+a[i];
    }
    unsigned scom_length=scom.length()+1;
    char * command = new char [scom_length];
    command[scom.length()]='\0';
    unsigned sarg_length=sarg.length()+1;
    char * arguments = new char [sarg_length];
    arguments[sarg.length()]= '\0';
    for(unsigned i =0;i<scom.length();i++)
    {
        command[i]=scom.at(i);
    }
    for(unsigned i =0;i<sarg.length();i++)
    {
        arguments[i]=sarg.at(i);
    }
    
    char* final_command [3];
    if(sarg.length()==0)
    {
        final_command[0]=command;
        final_command[1]=NULL;
        final_command[2]=NULL;
    }
    else
    {
        final_command[0]=command;
        final_command[1]=arguments;
        final_command[2]=NULL;
    }
    //TEST HARNESS:::::cout<<final_command[0]<<" "<<final_command[1]<<" "<<final_command[2]<<endl;
    
    pid_t c_pid, pid;
    c_pid=fork();
    
    if(c_pid==-1)
    {
        cout<<"fork failed: please try again"<<endl;
        perror("FORK:");
        return false;
    }
    else if(c_pid==0)
    {
        // if(final_command[0]=="exit")
        //     exit(0);
        t=execvp(final_command[0],final_command);
        perror("INVALID COMMAND ENTERED: COMMAND NOT FOUND");
        exit(-1);
    }    
    else
    {
        pid = wait(&status);
        if (WIFEXITED(status)) {
            t =  WEXITSTATUS(status);
        }
        if(pid){}
        return true;
    }
}
//****************************************************



void parse_string ( string text)
{
    if(text.length()==0)
        return;
    string noctext; //command without the comments
    char * final_command;
    vector <string> connectors; 
    
    char_separator<char> sepcomment("#");
    tokenizer<char_separator<char> > token_comment(text, sepcomment);
    bool strip_comment=false;
    BOOST_FOREACH(string t, token_comment)
    {
        if(!strip_comment)
        {
            noctext=t;
            strip_comment=true;
        }
    }
    
    // cout<<"noctext"<<endl;
    bool i1=noctext.find("&&") == std::string::npos;
    bool i2=noctext.find("||") == std::string::npos;
    bool i3=noctext.find(";") == std::string::npos;
    // cout<<i1<<i2<<i3<<endl;
    if( i1 && i2 && i3 )
    {
        string nostext;
        char_separator<char> sepspaces(" ");
        tokenizer<char_separator<char> > token_spaces(noctext, sepspaces);
        bool first_token_added=false;
        BOOST_FOREACH(string t, token_spaces)
        {
            if(!first_token_added)
            {
                first_token_added=true;
                nostext=nostext+t;
            }
            else
                nostext=nostext + " " + t;        
        }
        if(nostext=="exit")//********EXPERIMENTAL*****TURN BACK ON IF STOPS WORKING
             exit(0);       //required for extra space exit commands
        if(nostext.length()==0)
            return;
        //nostext=nostext+'\0';
        final_command=new char[nostext.size() + 1];
        memcpy(final_command, nostext.c_str(), nostext.size() + 1);
        int m;
        execute(final_command, m);
        return;
        
    }
    else if ( i1 && i1 && !i3)
    {
        string nosctext;
        char_separator<char> sepsemicolon(";");
        tokenizer<char_separator<char> > token_semicolon(noctext, sepsemicolon);
        BOOST_FOREACH(string t, token_semicolon)
        {
            parse_string(t);
        }
        return;
    }
    
    else if( (!i1 || !i2) && i3 )
    {
        
    }
    
}
int main()
{
    string t;
    char* login=getlogin();
    char hostname[1024];
    hostname[1023]='\0';
    int hostname_affirm=gethostname(hostname,30);
    while(CONTROL)
    {
        if(login==NULL || hostname_affirm!=0)
        cout<<"$ ";
        else
        cout<<login<<"@"<<hostname<<"$ ";
        getline(cin,t);
        if(t=="exit")
            exit(0);
        parse_string(t);
    }
    return 0;
}
