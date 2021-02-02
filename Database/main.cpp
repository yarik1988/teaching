#include <iostream>

struct stud_info
{
    unsigned int id;
    char name[20];
    int age;
    bool gender;
};

int main() {
    unsigned int id=0;
    stud_info part;
    FILE *fp=fopen("../db.dat","rb");
    fseek(fp,0,SEEK_END);
    long pos=ftell(fp);
    if (pos>0)
            {
            fseek(fp,pos-sizeof(stud_info),SEEK_SET);
            fread(&part,sizeof(part),1,fp);
            id=part.id+1;
            }
    fclose(fp);

    fp=fopen("../db.dat","ab");
    fseek(fp,0,SEEK_END);
    while (true)
    {
        part.id=id;
        std::cout<<"enter name";
        std::cin>>part.name;
        std::cout<<"enter age";
        std::cin>>part.age;
        std::cout<<"enter gender";
        std::cin>>part.gender;
        if (part.age<0) break;
        else fwrite(&part,sizeof(part),1,fp);
        id++;
    }
    fclose(fp);

    return 0;
}
