#include <iostream>
using namespace std;

struct Matr{
    unsigned rows,columns;
    unsigned maxSize;
    unsigned* rowIndex;
    unsigned* columnIndex;
    int* values;
};

void matrix_allocation(Matr* pMatr, unsigned rows, unsigned columns, unsigned maxSize);
void matrix_free(Matr* pMatr);
int set_element(Matr* pMatr, unsigned row, unsigned column, int value);
int get_element(Matr* pMatr, unsigned row, unsigned column);
void print_matr(Matr* pMatr);
void print_options_Matrix(Matr* pMatr);
int input_init(char* caption);
void init_from_console(Matr* pMatr);
void input_from_file(Matr* pMatr);
void sum_of_matrix(Matr* pMatr1, Matr* pMatr2,Matr* pMatr3);
void trasport_matrix(Matr* pMatr1, Matr* pMatr2);
void multi_matr(Matr* pMatr1, Matr* pMatr2,Matr* pMatr3);
void compression(Matr* pMatr1);


int main() {
    Matr matrix1;
    Matr matrix2;
    Matr matrix3;
    Matr matrix4;

    matrix_allocation(&matrix1, 5, 5, 15);
    matrix_allocation(&matrix2, 5, 5, 15);
    matrix_allocation(&matrix3, 5, 5, 15);
    matrix_allocation(&matrix4, 5, 5, 15);

    //инициализация с клавы
  //  init_from_console(&matrix4);
 
    //инициализация из файла
    input_from_file(&matrix1);
    input_from_file(&matrix2);
    
    
    cout<<"Matrix #1 : "<<endl;
    print_matr(&matrix1);
    
    cout<<"Matrix #2 : "<<endl;
    print_matr(&matrix2);
    
    cout<<"Sum of two matrix : "<<endl;
    sum_of_matrix(&matrix1,&matrix2,&matrix3);
    print_matr(&matrix3);
    
    cout<<"Multiplication of two matrix : "<<endl;
    multi_matr(&matrix1,&matrix2,&matrix4);
    print_matr(&matrix4);
    
    
    cout<<"Matrix compression : "<<endl;
    compression(&matrix1);
    
    matrix_free(&matrix1);
    matrix_free(&matrix2);
    matrix_free(&matrix3);
    
    return 0;
}

void matrix_allocation(Matr* pMatr, unsigned rows, unsigned columns, unsigned maxSize){
    pMatr->rows=rows;
    pMatr->columns=columns;
    pMatr->maxSize= maxSize;
    
    pMatr->rowIndex=new unsigned[rows+1];
    pMatr->columnIndex=new unsigned[maxSize];
    pMatr->values=new int[maxSize];
    for(unsigned i =0; i<(rows+1);i++){
        pMatr->rowIndex[i]=0;
    }
    
}
void matrix_free(Matr* pMatr){
    delete  pMatr->rowIndex;
    delete pMatr->columnIndex;
    delete pMatr->values;
}


int set_element(Matr* pMatr, unsigned row, unsigned column, int value){
    unsigned i,j;
    if((row>pMatr->rows) || (column>pMatr->columns)) return 1;
    for(i = pMatr->rowIndex[row];i<pMatr->rowIndex[row+1];i++){
        if(pMatr->columnIndex[i]==column){
            if(value){
                pMatr->values[i]=value;
                return 0;
            }
            for(j=i;j<(pMatr->rowIndex[pMatr->rows]-1);j++){
                pMatr->values[j]=pMatr->values[j+1];
                pMatr->columnIndex[j]=pMatr->columnIndex[j+1];
            }
            for(j=row+1;j<(pMatr->rows+1);j++){
                pMatr->rowIndex[j]--;
            }
            return 0;
        }
    }
    if(!value){
        return 0;
    }
    if(pMatr->rowIndex[pMatr->rows]==pMatr->maxSize) return 2;
    for(j=pMatr->rowIndex[pMatr->rows];j>i;j--){
        pMatr->values[j]=pMatr->values[j-1];
        pMatr->columnIndex[j]=pMatr->columnIndex[j-1];
    }
    pMatr->values[i]=value;
    pMatr->columnIndex[i]=column;
    for(j=row+1;j<(pMatr->rows+1);j++){
        pMatr->rowIndex[j]++;
    }
    return 0;
}

int get_element(Matr* pMatr, unsigned row, unsigned column){
    for(unsigned i = pMatr->rowIndex[row];i<pMatr->rowIndex[row+1];i++){
        if(pMatr->columnIndex[i]==column){
            return pMatr->values[i];
        }
    }
    return 0;
}

void print_matr(Matr* pMatr){
    unsigned i,j;
    for(i=0;i<pMatr->rows;i++){
        for(j=0;j<pMatr->columns;j++){
            cout<<get_element(pMatr,i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl<<endl;
}

void print_options_Matrix(Matr* pMatr){
    unsigned i;
    cout<<"Row indexes array : ";
    for(i=0;i<(pMatr->rows+1);i++){
        cout<<pMatr->rowIndex[i]<<" ";
        
    }
    cout<<endl;
    cout<<"Column indexes array : ";
    for(i=0;i<(pMatr->rowIndex[pMatr->rows]);i++){
        cout<<pMatr->columnIndex[i]<<" ";
    }
    cout<<endl;
    cout<<"Values array : ";
    for(i=0;i<pMatr->rowIndex[pMatr->rows];i++){
        cout<<pMatr->values[i]<<" ";
    }
    cout<<endl<<endl;
}

int input_init(char* caption){
    int input=0;
    cout<<"Input "<<caption<<endl;
    cin>>input;
    while(getchar()!='\n');
    return input;
}

void init_from_console(Matr* pMatr){
    int row,column;
    int value;
    char c;
    do{
        row=input_init("row");
        column=input_init("column");
        value=input_init("value");
        
        switch (set_element(pMatr, row, column, value)) {
            case 1:
                cout<<"Index out of range!"<<endl;
                break;
            case 2:
                cout<<"Buffer overload!"<<endl;
                break;
                
            default:print_matr(pMatr);
                break;
        }
        do{
            cout<<"Continue? (y/n)"<<endl;
            c= getchar();
            if(c=='y'||c=='Y') break;
            if(c=='n'||c=='N') return;
            cout<<"Invalid input "<<endl;
            while(getchar()!='\n');
        }while(1);
    }while(1);
}

void input_from_file(Matr* pMatr){
    char* filename = "data.txt";
    FILE* fp;
    if(!(fp=fopen(filename,"r"))){
        cout<<"Can not open"<<endl;
        return;
    }
    unsigned row, column;
    unsigned value;
    do{
        if(fscanf(fp,"%u %u %d", &row,&column,&value)==3){
            set_element(pMatr, row, column, value);
        }
        else break;
    }while(!feof(fp));
    fclose(fp);
}


void sum_of_matrix(Matr* pMatr1, Matr* pMatr2,Matr* pMatr3){
    unsigned i,j;
    for(i=0;i<pMatr1->rows;i++){
        for(j=0;j<pMatr1->columns;j++){
            if(get_element(pMatr1,i,j)+get_element(pMatr2,i,j)!=0){
                set_element(pMatr3, i, j, get_element(pMatr1,i,j)+get_element(pMatr2,i,j));
            }
            
        }
    }
}

void trasport_matrix(Matr* pMatr1, Matr* pMatr2){
    unsigned i,j;
    for(i=0;i<pMatr1->rows;i++){
        for(j=0;j<pMatr1->columns;j++){
            if(get_element(pMatr1,i,j)){
            set_element(pMatr2, j, i,get_element(pMatr1,i,j));
            }
        }
    }
}

void multi_matr(Matr* pMatr1, Matr* pMatr2,Matr* pMatr3){

        int i, j;
    int elemen;

        for (i = 0; i < pMatr1->rows; i++)
        {
            for (j = 0; j < pMatr2->columns; j++)
            {
                
                set_element(pMatr3, i, j, 0);
                elemen=0;
                for (int k = 0; k < pMatr2->rows; k++){
                    elemen+= get_element(pMatr1,i,k) * get_element(pMatr2,k,j);
                set_element(pMatr3, i, j, elemen);
                }
            }
        }

 
}

void compression(Matr* pMatr1){
    const int n = 100;
    int* mass = new int[n];
    int* mass2 = new int[n];
    unsigned i,j;
    int l=0;
    int y=0;
    int count;
    for(i=0;i<pMatr1->rows;i++){
        for(j=0;j<pMatr1->columns;j++){
            mass[l]=get_element(pMatr1, i, j);
            l++;
        }
    }
    for(int i =0;i<l;i++){
        cout<<mass[i];
    }
    cout<<endl;
    for(int i =0;i<l;i++){
        if(mass[i]==0){
            count=1;
            while(mass[i+1]==0){
                count++;
                i++;
            }
            mass2[y]=count;
            y++;
            mass2[y]=0;
        }
        else{
            mass2[y]=mass[i];
        }
        y++;
    }
    cout<<endl;
    for(int i =0;i<y;i++){
        cout<<mass2[i];
    }
    delete []  mass;
    delete [] mass2;
    cout<<endl<<endl;
}
