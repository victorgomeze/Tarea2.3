#include<stdio.h>
#include<mpi.h>
#include<fstream>
#include <iostream>
#include <vector>
using namespace std;

void contar_Carreras(char *direct,int proce)
{
	int cont=-1,numero[2],match=0,pos,largo;
	string linea, total="";
	vector <string> vec;
	ifstream fixero(direct);
	MPI_Recv(numero, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);   //cuenta cada procesador
    fixero.seekg(numero[0]);
    getline(fixero,linea);
	while(numero[1]>fixero.tellg())
		{
			getline(fixero,linea);
			pos=linea.find(",")+1;
			string cad4 = linea.substr(pos);
			pos=cad4.find(",");
			string cad3 =cad4.substr(0,pos);
			for(int i=0;i<vec.size();i++)
			{
				if(vec[i]==cad3)
				{
					i=vec.size();
					match=1;
				}
			}

			if(match==0)
			{
				vec.push_back(cad3);
			}
			match=0;
		}
	cout<<endl;
	cout<<"//////////////PROCESADOR "<<proce<<"//////////////"<<endl;
	cout<<endl;
	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
		total=total+"-"+vec[i];
	}
	cout<<endl;
	cout<<"CARRERAS ENCONTRADAS PROCESADOR "<<proce<<": "<<vec.size()<<endl;
	total=total+"/";
	largo=total.length();
	char todo[largo];
	total.copy(todo,largo); //el vector se guarda en un char para poder enviarlo mediante el send
	MPI_Send(&largo, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	MPI_Send(&todo, largo, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	fixero.close();
}



void filtrar_Copias(int in, vector <string> &vec) //funcion para eliminar copias provenientes de los otros procesadores
{
	int cantodo,pos,match=0,hello=0;
	string datos,cad3;
	MPI_Recv(&cantodo, 1, MPI_INT, in, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recibe tamaño del char que traera la informacion
	char todo[cantodo];
	MPI_Recv(&todo, cantodo, MPI_CHAR, in, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recibe char con toda la informacion recolectada por el procesador
	datos=todo;
	while(hello==0)
	{	
		pos=datos.find("-")+1;
		datos = datos.substr(pos);
		pos=datos.find("-");
		cad3 =datos.substr(0,pos);
		for(int i=0;i<cad3.length();i++)
		{
			if(cad3[i]=='/') //se verifica si se llego al final de la cadena
			{
				pos=cad3.find("/");
				cad3=cad3.substr(0,pos);
				hello=1;
			}
		}
		for(int i=0;i<vec.size();i++)
			{
				if(vec[i]==cad3)
				{
					i=vec.size();
					match=1;
				}
			}

			if(match==0)
			{

				vec.push_back(cad3);
			}
			match=0;
	}
}


void repartiendo(char *direct,int tamano) // trabaja solo el proce 0
{
	int numero[2], cadauno,final,conta,match=0,pos;
	vector <string> vec;
	string linea;
	ifstream fixero(direct);
	fixero.seekg(0, fixero.end); 
	final = fixero.tellg();
	cadauno=final/tamano;
	    

	fixero.seekg(0);
	for(int i=1;i<tamano;i++) //se reparten las cargas , envia inicio y final a cada procesador
	{
	    numero[0]=cadauno*i;
	    numero[1]=cadauno*(i+1);
	    MPI_Send(numero, 2, MPI_INT, i, 0, MPI_COMM_WORLD);

	}


	getline(fixero,linea); //se salta la primera linea
    while(cadauno>fixero.tellg())   // comienza a trabajar en los rangos asignados, el procesador 0 siempre comienza desde el inicio del archivo
		{
			getline(fixero,linea);
			pos=linea.find(",")+1;
			string cad4 = linea.substr(pos);
			pos=cad4.find(",");
			string cad3 =cad4.substr(0,pos);
			for(int i=0;i<vec.size();i++)
			{

				if(vec[i]==cad3)  //revisa las carreras guardadas para no ingresar una copia de la misma carrera
				{
					i=vec.size();
					match=1;
				}
			}

			if(match==0) //si no se encuentra una copia se añade la carrera al vector
			{
				vec.push_back(cad3);
			}
			match=0;
		}
	cout<<endl;
	cout<<"//////////////PROCESADOR 0//////////////"<<endl;
	cout<<endl;
	for(int i=0;i<vec.size();i++) //carreras encontradas por el procesador 0
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
	cout<<"CARRERAS ENCONTRADAS PROCESADOR 0: "<<vec.size()<<endl;
	for(int i=1;i<tamano;i++)
	{
		filtrar_Copias(i,vec); 
	}
    cout<<endl;
	cout<<"//////////////TOTAL CARRERAS//////////////"<<endl;
	cout<<endl;
   	for(int i=0;i<vec.size();i++)
	{
		cout<<vec[i]<<endl;
	}
	cout<<endl;
	cout<<"TOTAL CARRERAS: "<<vec.size()<<endl;
	fixero.close();
}



int main(int argc, char* argv[]){

  	int tamano, procesador;
  	MPI_Init(&argc,&argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &tamano); 
  	MPI_Comm_rank(MPI_COMM_WORLD, &procesador);
  	if(argc<2)
  	{
  		cout<<"archivo csv no ingresado"<<endl;
  	}
  	else
  	{
		if(procesador==0)
		{
			repartiendo(argv[1],tamano);
		}
		else
		{
			contar_Carreras(argv[1],procesador);
		}
	}


  MPI_Finalize();
}