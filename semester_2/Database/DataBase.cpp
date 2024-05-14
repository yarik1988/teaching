#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

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

void deltree(FILE *fp, tree &cur_tree)
{
cur_tree.show_flag=false;
fpos_t cur_pos;
fgetpos(fp,&cur_pos);
fseek(fp,-sizeof(tree),SEEK_CUR);
fwrite(&cur_tree,sizeof(tree),1,fp);
printf("%s tree deleted successfully\n",cur_tree.name);
fseek(fp,cur_pos,SEEK_SET);
}

void tree_type_menu()
{
char c;
tree_type tt;
FILE* ftype=fopen("../types.dat","r+b");
    while (c!=27)
    {
     printf("show all types -- 2\n");
     printf("enter type -- 1\n");
     printf("quit -- Esc\n");
     c=getch();
     system("cls");
     switch (c)
        {
        case '1':   {
                long int ind;
                fseek(ftype,-sizeof(long int),SEEK_END);
                if (fread(&ind,sizeof(long int),1,ftype))
                tt.index=ind+1; else
                tt.index=1;
                printf("Enter type\n");
                scanf("%s",&tt.type);
                fseek(ftype,0,SEEK_END);
                fwrite(&tt,sizeof(tree_type),1,ftype);
                } break;
        case '2':  {
                    fseek(ftype,0,SEEK_SET);
                    while (!feof(ftype))
                        {
                        if (fread(&tt,sizeof(tree_type),1,ftype))
                            {
                            printf("Type=%s\n",tt.type);
                            printf("Index=%d\n",tt.index);
                            }
                        }
                } break;
        }
    }
fclose(ftype);
}
int main()
{
char c;
long int ind=0;
FILE* fp=fopen("../park.dat","r+b");
while (c!=27)
{
printf("enter tree -- 1\n");
printf("show all trees -- 2\n");
printf("delete tree by index -- 3\n");
printf("delete tree by name -- 4\n");
printf("enter tree type menu  -- 5\n");
printf("quit -- Esc\n");
c=getch();
system("cls");
switch (c)
    {
    case '1':   {
                fseek(fp,-sizeof(long int),SEEK_END);
                if (fread(&ind,sizeof(long int),1,fp))
                cur_tree.index=ind+1; else
                cur_tree.index=1;
                cur_tree.show_flag=true;
                printf("Enter name\n");
                scanf("%s",&cur_tree.name);
                printf("Enter age\n");
                scanf("%d",&cur_tree.age);
                fseek(fp,0,SEEK_END);
                fwrite(&cur_tree,sizeof(tree),1,fp);
                } break;
    case '2':
                {
                fseek(fp,0,SEEK_SET);
                while (!feof(fp))
                        {
                        if (fread(&cur_tree,sizeof(tree),1,fp)/*&&cur_tree.show_flag*/)
                            {
                            printf ("________________________\n");
                            printf("Name=%s\n",cur_tree.name);
                            printf("Age=%d\n",cur_tree.age);
                            printf("Index=%d\n",cur_tree.index);
                            printf("Is_show=%d\n",cur_tree.show_flag);
                            }
                        }
                } break;
    case '3':
                {
                printf("enter index to delete");
                scanf("%d",&ind);
                fseek(fp,0,SEEK_SET);
                while (!feof(fp))
                  if (fread(&cur_tree,sizeof(tree),1,fp)&&cur_tree.show_flag&&cur_tree.index==ind)
                  deltree(fp,cur_tree);
                } break;
    case '4':
                {
                printf("enter name to delete");
                char tree_name[10];
                scanf("%s",&tree_name);
                fseek(fp,0,SEEK_SET);
                while (!feof(fp))
                  if (fread(&cur_tree,sizeof(tree),1,fp)&&cur_tree.show_flag&&strcmp(cur_tree.name,tree_name)==0)
                  deltree(fp,cur_tree);
                } break;
    case '5': tree_type_menu();break;
    }

}
FILE* temp=fopen("temp.dat","wb");
fseek(fp,0,SEEK_SET);
while (!feof(fp))
if (fread(&cur_tree,sizeof(tree),1,fp)&&cur_tree.show_flag==true)
fwrite(&cur_tree,sizeof(tree),1,temp);
fclose(fp);
fclose(temp);
remove("park.dat");
rename("temp.dat","park.dat");
return 0;
}
