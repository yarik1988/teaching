#include <iostream>
#include <cstring>
using namespace std;

is_palindrome(char* str)
{
 int n=strlen(str);
 for (int i=0;i<n/2;i++)
            if (str[i]!=str[n-i-1]) return false;
 return true;
}


int main()
{
    char str[200]="12345321";
    //cin>>str;
    int n=strlen(str);
    for (int length=1;length<=n-1;length++)
         for (int start=0;start<n-length;start++)
        {
             char f1[200];
             char f2[200];
             strncpy(f1,str,start);
             f1[start] = '\0';
             strncpy(f2,str+start+length,n);
             strcat(f1,f2);
             if (is_palindrome(f1))
             {
                 cout<<f1<<endl;
                 cout<<start<<endl;
                 cout<<length<<endl;
                 return 0;
             }
        }

    return 0;
}
