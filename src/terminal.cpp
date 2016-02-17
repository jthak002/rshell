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
static bool prev=true;
//****************************************************
//EXECUTE FUNCTION NEEDS WORK--CAN BE MADE A LOT SHORTER--WORKS FOR MILESTONE#1
//****************************************************
bool execute(char* a, int &t)   //This function executes the command by accetping a c-string
{                               //t is a testing variable -- will be utilized later
    string scom,sarg;           //string for accepting 'COMMAND' and 'ARGUMENTS'
    int status;                 //TESTING VARIABLE--TO BE USED LATER
    unsigned size=sizeof(a)/sizeof(char);   //FINDING THE SIZE OF char* a
    if(size==0)                             //if size is zero exit program right here
        return false;
    
    //flag to make sure only the 'COMMAND' part of the program goes into scom    
    bool flag=false;                        
    for(unsigned i=0;i<size || a[i]!='\0';i++)//converting a to string type variables scom and sarg
    {
        //this snipped makes sure that only the COMMAND goes into the scom part of the program
        if(a[i]==' ' && !flag)  
        {
            flag=true; continue; //once it encounters a space, the rest
        }
        if(flag)
        {
            sarg=sarg+a[i];     // of the string goues in to sarg
            continue;
        }
        scom=scom+a[i]; //as you can see scom only has the command part
    }
    unsigned scom_length=scom.length()+1;   //this extra line has to be put in because C++ does not allow 
    char * command = new char [scom_length];    //variables for declaring an array's size.
    command[scom.length()]='\0';        //the last character is NULL because the cstring needs a null character to signal end..This was a bugfix for execvp not executing 
    unsigned sarg_length=sarg.length()+1;   //the cstring properly
    char * arguments = new char [sarg_length];
    arguments[sarg.length()]= '\0';
    for(unsigned i =0;i<scom.length();i++)  //NOW we convert the scom string to a cstring by iterating through the string and  
    {                                       //assigning each character to its repective place. this complex method has been implemented
        command[i]=scom.at(i);              //because compilation was giving a "string deprecated when converting to char*" error
    }
    for(unsigned i =0;i<sarg.length();i++)
    {
        arguments[i]=sarg.at(i);
    }
    
    char* final_command [3];  //the final_command pointer array goes into the execvp function, which is decribed inthe man
    if(sarg.length()==0)                //pages as follows: execvp(char* final_command[0], final_command )
    {
        final_command[0]=command;       //single word commands do not have arguments
        final_command[1]=NULL;          //hence, the 2nd and thrid positions are filled with null
        final_command[2]=NULL;
    }
    else
    {
        final_command[0]=command;       // multiword commands
        final_command[1]=arguments;
        final_command[2]=NULL;
    }
    //TEST HARNESS:::::cout<<final_command[0]<<" "<<final_command[1]<<" "<<final_command[2]<<endl;
    
    pid_t c_pid, pid;
    c_pid=fork();
    
    if(c_pid==-1)                       //failed fork
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
        printf("-rshell: %s: command not found\n", command);
        exit(-1);
        return false;
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
//EXECVP taken from man pages and examples
//****************************************************
// BOOL HELPER
bool parses_string ( string text)
{
        string nostext;
        char_separator<char> sepspaces(" ");                //removing spaces using boost libraries
        tokenizer<char_separator<char> > token_spaces(text, sepspaces);  //
        bool first_token_added=false;
        BOOST_FOREACH(string t, token_spaces)               //recompiling the string with exactly one space between
        {                                                   //the words in the string::replicates terminal behaviour
            if(!first_token_added)
            {
                first_token_added=true;                      
                nostext=nostext+t;                          //The first token should not have spaces-->since the string is empty the next statement will
            }                                               //add a space before the COMMAND
            else
                nostext=nostext + " " + t;        
        }
        if(nostext=="exit")//********EXPERIMENTAL*****TURN BACK ON IF STOPS WORKING
             exit(0);       //required for extra space exit commands
        if(nostext.length()==0)                             //ANOTHER CASE FOR EMPTY string 
            return false;
        //nostext=nostext+'\0';
        char* final_command=new char[nostext.size() + 1];         //CREATING char* for passing to execute(char* , int&)
        memcpy(final_command, nostext.c_str(), nostext.size() + 1);
        int m;
        
        return execute(final_command, m);
}


//bool helper


void parse_string ( string text)
{
    if(text.length()==0)                                    //empty string length , returns control back to call
        return;                                             //This covers the caseof the empty prompt.
    string noctext; //command without the comments
    char * final_command;                                   
    
    char_separator<char> sepcomment("#");                   //SEPARATING THE COMMENTS FROM THE INPUT---strip it off
    tokenizer<char_separator<char> > token_comment(text, sepcomment);   //TO BE REVIEWED--WHEN QUOTATION MARKS COME INTO PLAY
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
    bool i1=noctext.find("&&") == std::string::npos;        //checking for presence of &&
    bool i2=noctext.find("||") == std::string::npos;        //                         ||
    bool i3=noctext.find(";") == std::string::npos;         //                          ;
    // cout<<i1<<i2<<i3<<endl;
    if( i1 && i2 && i3 )                                    //CASE 1: single commands
    {
        string nostext;
        char_separator<char> sepspaces(" ");                //removing spaces using boost libraries
        tokenizer<char_separator<char> > token_spaces(noctext, sepspaces);  //
        bool first_token_added=false;
        BOOST_FOREACH(string t, token_spaces)               //recompiling the string with exactly one space between
        {                                                   //the words in the string::replicates terminal behaviour
            if(!first_token_added)
            {
                first_token_added=true;                      
                nostext=nostext+t;                          //The first token should not have spaces-->since the string is empty the next statement will
            }                                               //add a space before the COMMAND
            else
                nostext=nostext + " " + t;        
        }
        if(nostext=="exit")//********EXPERIMENTAL*****TURN BACK ON IF STOPS WORKING
             exit(0);       //required for extra space exit commands
        if(nostext.length()==0)                             //ANOTHER CASE FOR EMPTY string 
            return;
        //nostext=nostext+'\0';
        final_command=new char[nostext.size() + 1];         //CREATING char* for passing to execute(char* , int&)
        memcpy(final_command, nostext.c_str(), nostext.size() + 1);
        int m;
        execute(final_command, m);
        return;
        
    }
    else if ( i1 && i1 && !i3)                              //CASE 2: multiple commands chained only with ';'
    {
        string nosctext;                                    //using boost to split the string with semi colons
        char_separator<char> sepsemicolon(";");
        tokenizer<char_separator<char> > token_semicolon(noctext, sepsemicolon);
        BOOST_FOREACH(string t, token_semicolon)
        {
            parse_string(t);                                //Each segmented string without semi colons is passed into
        }                                                   //parse_string() again----RECURSIVE
        return;
    }
    
    else if( (!i1 || !i2) && i3 )
    {
        vector<char> connector;
        
        for(unsigned i=0;i<noctext.length()-1;)
        {
            if(noctext.at(i)=='&' && noctext.at(i+1)=='&')
            {
                connector.push_back('&');
                i=i+2;
                continue;
            }
            if(noctext.at(i)=='|' && noctext.at(i+1)=='|')
            {
                connector.push_back('|');
                i=i+2;
                continue;
            }
            else{
                i++;
            }
        }
        connector.push_back('\0');
        
        bool yflag=true;
        char_separator<char> sepnotamp("&|");
        tokenizer<char_separator<char> > token_notamp(noctext,sepnotamp);
        unsigned i=0;
        BOOST_FOREACH(string t, token_notamp)
        {
            if(i>connector.size()-1)
                break;
            if(yflag)
            {
                prev=parses_string(t);
                yflag=false;
                continue;
            }
            if(connector.at(i)=='&' && prev==true)
            {
                prev=parses_string(t);
                i++;
            }
            else if(connector.at(i)=='|' && prev==false)
            {
                prev=parses_string(t);
                i++;
            }
            else
            {
                prev=false;
                i++;
                continue;   
            }
            
        }
    }
    else if( (!i1 || !i2) && !i3 )
    {
        string nosctextNOTAMP;                                    //using boost to split the string with semi colons
        char_separator<char> sepsemicolonNOTAMP(";");
        tokenizer<char_separator<char> > token_semicolonNOTAMP(noctext, sepsemicolonNOTAMP);
        BOOST_FOREACH(string t, token_semicolonNOTAMP)
        {
            parse_string(t);                                //Each segmented string without semi colons is passed into
        }                                                   //parse_string() again----RECURSIVE
        return;
    }
}

int main()
{
    string t;               //accepting command line input 
    char* login=getlogin(); //getting the login
    char hostname[1024];    //hostname
    hostname[1023]='\0';    //CONVENTIONS FOR OBTAINING HOSTNAME
    int hostname_affirm=gethostname(hostname,30);
    while(CONTROL)
    {
        if(login==NULL || hostname_affirm!=0)   //checking if hostname is available
        cout<<"$ ";         //DISPLAYING THE DOLLAR PROMPT IF hostname is not available (like in cloud9)
        else
        cout<<login<<"@"<<hostname<<"$ ";   //display prompt if prompt is available
        getline(cin,t);
        if(t=="exit")
            exit(0);
        parse_string(t);
    }
    return 0;
}
