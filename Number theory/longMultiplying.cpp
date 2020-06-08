#include <iostream>
using namespace std;
int main()
{
    string a,b;//ініціалізація змінних a i b рядкового типу
    cout << "Enter a:" << endl; //запит першого множника
    bool validA=false;
    bool aNeg=false;
    while (!validA) { //цикл перевірки введених даних на правильність
        cin >> a;
        validA =true;
        if (a[0]=='-') {aNeg=true;a[0]='0';} // щоб дозволити користувачу множити від'ємні числа
        for (int i=0; i<a.length();i++){
            if (!(a[i]>='0' && a[i]<='9')){validA=false;}
        }
        if (!validA) {cout << "Please, enter an integer!\n";}
    }
    cout << "Enter b:" << endl; // отримання другого "довгого" числа у вигляді рядка
    bool validB=false;
    bool bNeg=false;
    while (!validB) { //той же цикл для числа б
        cin >> b;
        validB =true;
        if (b[0]=='-') {bNeg=true;b[0]='0';}
        for (int i=0; i<b.length();i++){
            if (!(b[i]>='0' && b[i]<='9')){validB=false;}
        }
        if (!validB) {cout << "Please, enter an integer!\n";}
    }
    int aLen = a.length();
    int bLen = b.length();
    int* aArr =new int[aLen]; int* bArr=new int[bLen]; // ініціалізація масивів, які будуть містити цифри числа
    int* abRes = new int[aLen+bLen]; // ініціалізація масиву-результату довжиною, яка рівна сумі довжин двох попередніх масивів
    for (int i=0;i<aLen;i++){aArr[i]=0;} for (int i=0;i<bLen;i++){bArr[i]=0;} // присвоєння елементам масивів початкових значень 0
    for (int i=0;i<aLen+bLen;i++){abRes[i]=0;}
    for (int i=aLen-1;i>=0;i--) {aArr[i]=a[i]-'0';} // перетворення символів у цифри та присвоєння відповідних значень елементам масивів
    for (int i=bLen-1;i>=0;i--) {bArr[i]=b[i]-'0';}
    int ost=0; //створення змінної, яка відповідатиме за "верщок" при множенні
    for (int i=aLen-1;i>=0;i--) { //запуск циклу для кожного елементу масиву а
        for (int j=bLen-1;j>=0;j--) { //запуск циклу для кожного елемента б
            abRes[i+j]=(abRes[i+j]+ost+aArr[i]*bArr[j]); //запис результату множення + "вершок" + поперереднє значення на позиції
            ost=abRes[i+j]/10; //підрахунок нового "вершка"
            abRes[i+j]=abRes[i+j]%10; //запис новго значення на позиції
        }
        if (i!=0) {abRes[i-1]=abRes[i-1]+ost;ost=0;}//запис остачі після завершення циклу
    }
    if (aNeg&&!bNeg||bNeg&&!aNeg){cout<<'-';}//вивід знаку мінус на позначення від'ємного результату
    if (ost) {cout << ost;} // перевірка наявності "вершка" після завершення множення та вивід на екран при його наявності
    int zeroCounter = 0;//перевірка нулів на початку числа
    bool Zeros = true;
    while (Zeros) {if(abRes[zeroCounter]==0){zeroCounter++;}else {Zeros=false;}}
    if (zeroCounter==aLen+bLen-1){cout<<0;}
    for (int i=0+zeroCounter;i<aLen+bLen-1;i++) {cout << abRes[i];} // виведення результату з елементів масиву
    cout << endl;
    delete[] aArr;
    delete[] bArr;
    delete[] abRes;
    return 0;
}
