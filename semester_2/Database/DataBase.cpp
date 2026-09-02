#include <stdio.h>
#include <conio.h>
#include <io.h>       // _isatty: is stdin/stdout a real console?
#include <iostream>
#include <stdlib.h>
#include <string.h>

using namespace std;
struct tree_type
{
char type[10];
long int index;
};
struct tree
{
bool show_flag;
char name[10];
int age;
long int index;
} cur_tree;

FILE* open_or_create(const char *fname)
{
FILE *f=fopen(fname,"r+b");       // update mode fails if the file does not exist yet
if (!f)
f=fopen(fname,"w+b");             // first run: create an empty file
return f;
}

// The index is the last field of both structs, so the last bytes of the file
// hold the highest index so far. An empty file has none, so start at 1.
long int next_index(FILE *fp)
{
long int ind=0;
fseek(fp,0,SEEK_END);
if (ftell(fp)>=(long)sizeof(long int))
    {
    fseek(fp,-(long)sizeof(long int),SEEK_END);
    fread(&ind,sizeof(long int),1,fp);
    }
return ind+1;
}

void deltree(FILE *fp, tree &cur_tree)
{
cur_tree.show_flag=false;
fpos_t cur_pos;
fgetpos(fp,&cur_pos);
fseek(fp,-(long)sizeof(tree),SEEK_CUR);
fwrite(&cur_tree,sizeof(tree),1,fp);
printf("%s tree deleted successfully\n",cur_tree.name);
fsetpos(fp,&cur_pos);
}

void tree_type_menu()
{
int c=0;
tree_type tt;
FILE* ftype=open_or_create("types.dat");
if (!ftype)
{
printf("Cannot open types.dat\n");
return;
}
    while (c!=27 && c!=EOF)
    {
     printf("show all types -- 2\n");
     printf("enter type -- 1\n");
     printf("quit -- other key\n");
     do {
        c=getch();
        } while (c==13);
     switch (c)
        {
        case '1':   {
                tt.index=next_index(ftype);
                printf("Enter type\n");
                scanf("%9s",tt.type);
                fseek(ftype,0,SEEK_END);
                fwrite(&tt,sizeof(tree_type),1,ftype);
                } break;
        case '2':  {
                    fseek(ftype,0,SEEK_SET);
                    while (fread(&tt,sizeof(tree_type),1,ftype))
                            {
                            printf("Type=%s\n",tt.type);
                            printf("Index=%ld\n",tt.index);
                            }
                } break;
            default: c=27;
        }
    }
fclose(ftype);
}
int main()
{
int c=0;
long int ind=0;
FILE* fp=open_or_create("park.dat");
if (!fp)
{
printf("Cannot open park.dat\n");
return 1;
}
while (c!=27 && c!=EOF)
{
printf("enter tree -- 1\n");
printf("show all trees -- 2\n");
printf("delete tree by index -- 3\n");
printf("delete tree by name -- 4\n");
printf("enter tree type menu  -- 5\n");
printf("quit -- other key\n");
do {
    c=getch();
} while (c==13);

switch (c)
    {
    case '1':   {
                cur_tree.index=next_index(fp);
                cur_tree.show_flag=true;
                printf("Enter name\n");
                scanf("%9s",cur_tree.name);
                printf("Enter age\n");
                scanf("%d",&cur_tree.age);
                fseek(fp,0,SEEK_END);
                fwrite(&cur_tree,sizeof(tree),1,fp);
                } break;
    case '2':
                {
                fseek(fp,0,SEEK_SET);
                while (fread(&cur_tree,sizeof(tree),1,fp)/*&&cur_tree.show_flag*/)
                        {
                        printf ("________________________\n");
                        printf("Name=%s\n",cur_tree.name);
                        printf("Age=%d\n",cur_tree.age);
                        printf("Index=%ld\n",cur_tree.index);
                        printf("Is_show=%d\n",cur_tree.show_flag);
                        }
                } break;
    case '3':
                {
                printf("enter index to delete\n");
                scanf("%ld",&ind);
                fseek(fp,0,SEEK_SET);
                while (fread(&cur_tree,sizeof(tree),1,fp))
                  if (cur_tree.show_flag&&cur_tree.index==ind)
                  deltree(fp,cur_tree);
                } break;
    case '4':
                {
                printf("enter name to delete\n");
                char tree_name[10];
                scanf("%9s",tree_name);
                fseek(fp,0,SEEK_SET);
                while (fread(&cur_tree,sizeof(tree),1,fp))
                  if (cur_tree.show_flag&&strcmp(cur_tree.name,tree_name)==0)
                  deltree(fp,cur_tree);
                } break;
    case '5': tree_type_menu();break;
    default: c=27;
    }
}
FILE* temp=fopen("temp.dat","wb");
fseek(fp,0,SEEK_SET);
while (fread(&cur_tree,sizeof(tree),1,fp))
if (cur_tree.show_flag==true)
fwrite(&cur_tree,sizeof(tree),1,temp);
fclose(fp);
fclose(temp);
remove("park.dat");
rename("temp.dat","park.dat");
return 0;
}
