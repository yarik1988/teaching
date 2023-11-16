#include <iostream>
#include <cstring>
#define LEN 100
using namespace std;

int find_first_occ(char str[LEN],char wtf[LEN])
{
    for (int i=0;i<strlen(str)-strlen(wtf)+1;i++)
        {
        bool is_ok=true;
        for (int j=0;j<strlen(wtf);j++)
            if (str[i+j]!=wtf[j]) {
                is_ok=false;
                break;
            }
        if (is_ok) return i;
        }
    return -1;
}

char* replace_part(char str[LEN],char wtr[LEN],int start,int end)
{
    int res_len=strlen(str)-(end-start)+strlen(wtr);
    char* res=new char[res_len+1];
    res[res_len]='\0';
    for (int i=0;i<start;i++)
        res[i]=str[i];
    for (int i=start;i<start+strlen(wtr);i++)
        res[i]=wtr[i-start];

    for (int i=start+strlen(wtr);i<res_len;i++)
        res[i]=str[i-start-strlen(wtr)+end];
    return res;
}

int main()
{
    char str[LEN];
    char wtf[LEN];
    char wtr[LEN];
    cin>>str>>wtf>>wtr;
    int start_pos=find_first_occ(str,wtf);
    if (start_pos<0) return -1;
    auto res=replace_part(str,wtr,start_pos,start_pos+strlen(wtf));
    cout<<res;
}
