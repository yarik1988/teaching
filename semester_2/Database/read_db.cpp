#include <iostream>

struct stud_info
{
    unsigned int id;
    char name[20];
    int age;
    bool gender;
};

void read_seq()
{
    stud_info part;
    FILE *fp=fopen("../db.dat","rb");
    while (!feof(fp)) {
        fread(&part, sizeof(stud_info), 1, fp);
        if (!feof(fp)) std::cout<<"Name: "<<part.name<<" Age: "<<part.age<<" Gender: "<<part.gender<<std::endl;
    }
    fclose(fp);
}

void read_all()
{
    FILE *fp=fopen("../db.dat","rb");
    fseek(fp,0,SEEK_END);
    long pos=ftell(fp);
    fseek(fp,0,SEEK_SET);
    int count=pos/sizeof(stud_info);
    stud_info* part=new stud_info[count];
    fread(part, sizeof(stud_info), count, fp);
    for (int i=0;i<count;i++)
        std::cout<<"Name: "<<part[i].name<<" Age: "<<part[i].age<<" Gender: "<<part[i].gender<<std::endl;
    delete[] part;
    fclose(fp);
}


int main() {
read_all();
}


