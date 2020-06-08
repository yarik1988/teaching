//works not stable. Need to review

#include <iostream>
#include <conio.h>

using namespace std;

string st;
string mask;

bool reversecheck(int si, int mi);

int main()
{
    cout <<"please enter string  ";
    cin>>st;
    cout <<"please enter mask  ";
    cin>>mask;
    bool result=reversecheck(0,0);
    result?cout<<"True":cout<<"False";
    getch();
    return 0;
}

bool reversecheck(int si, int mi){
    int mi1=mi;
    int si1=si;
    while (si1<st.length()&&mi1<mask.length()&&mask[mi1]!='*'){
        if (mask[mi1]!='?')
            if (mask[mi1]!=st[si1]) return false;
        mi1++;
        si1++;
    }
    if (si1==st.length()&&mi1==mask.length()) return true;
    //cout<<"remove prefix symbols"<<mi1<<' '<<si1<<endl;
    //getch();
    bool has_stars=false;
    while (mi1!=mask.length()&&(mask[mi1]=='*'||mask[mi1]=='?')){
        if (mask[mi1]=='?') si1++; else has_stars=true;
        mi1++;
    }
    //cout << mi1 <<"  " <<si1<< "  "<<has_stars<<endl;

    if (si1>st.length()) return false;
    if (mi1==mask.length()&&si1<st.length()&&(!has_stars)) return false;
    if (mi1==mask.length()&&(si1==st.length()||has_stars)) return true;

    int mi2=mi1;
    while (mask[mi2]!='*'&&mask[mi2]!='?') mi2++;
    string st1=mask.substr(mi1,mi2-mi1);
    int si2=si1;
    while (true){
        si2=st.find(st1,si2);
        if (si2==-1) break;
        //cout <<"recur "<<st1<<' '<<mi1+st1.length()<<' '<<si2+st1.length()<<endl;
        //getch();
        if (si2!=-1&&reversecheck(si2+st1.length(),mi1+st1.length())) return true;
        si2++;
    }
    return false;
}
