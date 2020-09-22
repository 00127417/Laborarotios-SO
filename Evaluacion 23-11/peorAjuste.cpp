#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
#define MEMORY_LENGHT 64

using namespace std;

//Estructura que contiene toda la informacion de los procesos
typedef struct {
	int status;
    int pid;
    int size;
    int direction;
}Slot;

//Se define el tipo 'Vector' para poder usar un vector de tipo de la estructura
//Se declara el vector que que guarda todos los procesos y se declara el mapa de bits todo de forma global
typedef vector<Slot> Vector;
Vector RAM;
int bitMap[MEMORY_LENGHT];

//La funcion llena de ceros el mapa de bits para que simule que esta vacio, es decir, no hay procesos
//Lo mismo ocurre con el vector donde se le inserta elemento el cual contiene el estado(cero si esta libre y uno si esta ocupado)
//el pid que es el nombre del proceso, el tamaño del hueco en este caso es 64 por que toda la memoria esta vacia
//y la direccion donde inicia el hueco
void init(){
	int i;
	for(i = 0; i<MEMORY_LENGHT;i++){
		bitMap[i] = 0;
	}
	Slot newSlot = {0, 0, MEMORY_LENGHT, 0};
	RAM.push_back(newSlot);
}

//La funcion imprime los datos del mapa de bits y realiza un salto de linea cada 8 bits
void showBitMap(){
	int i;
	for(i = 1; i<= MEMORY_LENGHT ;i++){
		cout<<bitMap[i-1];
		if(i%8 == 0)cout<<'\n';
	}
}

//Funcion que muestra los datos del vector que simula la ram
void showRAM(){

    cout<<"Status\t PID\t Direction\t Size\n";

    for(int i = 0; i< RAM.size();i++){
        cout<<RAM.at(i).status<<"\t";
        cout<<RAM.at(i).pid<<"\t";
        cout<<RAM.at(i).direction<<"\t";
        cout<<RAM.at(i).size<<"\n";
    }
    
}

//se recibe el vector y el tamaño del nuevo proceso
int peorAjuste(Vector ram, int size){
    int i = 0;
    int majorBlock = -1;
    int majorBlockSize = -1;
    //se realiza un for each para evaluar cada proceso y cada hueco que hay en la 'ram'
    for(auto slot: ram){
        //se debe cumplir que el estado este en cero es decir libre, que el tamaño del hueco sea mayor que el
        // del proceso y que no se halla encontrado hueco o que el hueco actual sea mayor
        if(slot.status == 0 && slot.size >= size && (majorBlock == -1 || slot.size > majorBlockSize)){
            
            //selecciona el hueco actual como el mayor
            majorBlock = i;
            //declara el tamaño del hueco para compararlo con los demas
            majorBlockSize = slot.size;
        }
        i++;
    }
    //retorna el hueco con mayor tamaño
    return majorBlock;
}

//funcion que llena el mapa de bits, esta recibe la direccion inicial, el tamaño del hueco o proceso y que valor colocar(1,0)
void writeBitMap(int direction, int size, int data){
    //el for recore de cero hasta el tamaño, pero al colocar la data al iterador se le suma la direccion para
    // que esta se encuente en la direccion de memoria exacta
    for(int i = 0; i < size; i++)bitMap[direction+i] = data;
}

//Funcion recibe el pid(nombre) y el tamaño del proceso
bool createProcess(int pid, int size){

    //guardamos que hueco tiene el mayor tamaño
    int slot = peorAjuste(RAM,size);
    
    //variable que nos asegura que se encontró un hueco lo suficientemente grande para el proceso
    bool answer = (slot != -1);
    
    //si hay espacio
    if(answer){
        //obtenemos la direccion del hueco mas grande
        int direction = RAM.at(slot).direction;

        //creamos el nuevo proceso con la direcion inicial del hueco mas grande
        Slot newProcess = {1,pid,size, direction};

        //calculamos el espacio restante del hueco al agregar el nuevo proceso
        int remaining = RAM.at(slot).size - size;

        //en estas dos lineas emplazamos el hueco por el nuevo proceso
        RAM.erase(RAM.begin()+slot);
        RAM.insert(RAM.begin()+slot,newProcess);

        //si nos queda espacio en la 'ram' despues de agregar el proceso
        if(remaining > 0){

            //se crea y se inserta el hueco del tamaño del espacio que sobro y la direccion inicial la colocamos al final del proceso
            Slot newBlock = {0, 0, remaining, direction + size};
            RAM.insert(RAM.begin()+slot+1,{0, 0, remaining, direction + size});
        }
        //se escribe el mapa de bit con la adicion del nuevo proceso
        writeBitMap(direction,size,1);
    }else{
        //si no hay espacio suficiente se termina el programa
        cout<<"No se tiene espacio para el proceso "<<pid<<"\n";
        cout<<"Sugerencia: libere memoria.\n";
        exit(-1);
    }
    return answer;
}

//se recibe la direccion del hueco a compactar
bool compact(int index){
    bool compactable = false;

    //se asegura que se encuentre dentro de los limites de la memoria
    if(index >= 0 && (index + 1) < RAM.size()){

        //se asegura que el hueco actual este libre y que el siguiente tambien
        compactable = RAM.at(index).status == 0 && RAM.at(index + 1).status == 0;
    }

    //si es posible compactar
    if(compactable){
        //obtenemos el tamaño del siguiente hueco y acontinuacion se elimina
        int size = RAM.at(index + 1).size;

        RAM.erase(RAM.begin() + index + 1);

        //al actual hueco se le suma el tamaño del hueco siguiente
        RAM.at(index).size += size;
    }

    return compactable;
}

//se recibe el pid(nombre) del proceso
bool destroyProcess(int pid){

    //inicializamos el contador y lo vamos aumentando hasta encontrar el proceso que desea eliminar
    int index = 0;
    for(auto slot : RAM){
        index++;
        if(slot.pid = pid) break;
    }
    
    //se resta uno al indice para asegurar que se encontro y si el valor es negativo significa que no encontró el proceso
    index--;
    bool finded = (index != -1);

    //si encontró el proceso
    if(finded){
        //se guardan los datos del proceso para utilizarlo mas adelante
        Slot deleteBlock = RAM.at(index);

        //se declara el proceso como hueco y se quita el pid(nombre)
        RAM.at(index).status = 0;
        RAM.at(index).pid = 0;

        //se escriben los cambios en el mapa de bits para ello se usan los valores del proceso que se guardó antes
        //se obtiene la direccion inicial y el tamaño para cambiar los unos por ceros
        writeBitMap(deleteBlock.direction, deleteBlock.size,0);

        //si es posible se compacta el hueco actual con el siguiente hueco(si existe) 
        compact(index);
        //se compacta el hueco anterior con el actual de ser posible, asumiendo que hay un hueco antes.
        compact(index - 1);
    }

    return finded;
}


int main(int argc, char const *argv[])
{
	init();

	showBitMap();
	showRAM();

    createProcess(1,3);
    createProcess(2,2);

	showBitMap();
	showRAM();

    destroyProcess(1);

    showBitMap();
	showRAM();

    createProcess(3,10);

    showBitMap();
	showRAM();

    createProcess(4,48);

    showBitMap();
	showRAM();

    createProcess(5,1);

    showBitMap();
	showRAM();

    /*createProcess(6,3);

    showBitMap();
	showRAM();
    */

	return 0;
}
