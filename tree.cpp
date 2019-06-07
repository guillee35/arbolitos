#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;
struct Node		//estructura de los nodos del arbol
{
    string ans;
    vector<Node*> *v ;
};

template <class Container> // Función para partir strings en vectores de strings
void str_split(const std::string& str, Container& cont,
              char delim = '$')
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}


Node *root;		//puntero a nodo utilizado en las funciones
Node *start;		//puntero al primero nodo del arbol

fstream file;
string filename;       //Elegir el fichero en el que vamos a buscar


void ready()
{

	cout<<endl<<endl<<endl<<"Este programa en C++ permite hacer busquedas en un fichero"<<endl;
	cout<<"Para ello se realizaran varias preguntas que guiaran al programa hasta el elemento buscado "<<endl;
	cout<<"Si no encuentra lo que esta buscando puede incluirlo en el fichero, para lo cual debera definir: "<<endl;
	cout<<"\tEl nombre del nuevo elemento a clasificar"<<endl;
	cout<<"\tLa pregunta que distinga entre los elementos a evaluar "<<endl<<endl<<endl;

}

int choose_file(string &file_name)
{
	int n;
	cout<<"Elige el NUMERO correspondiente a lo que quieres clasificar:"<<endl;
	cout<<"\t1.	Animales"<<endl;
	cout<<"\t2.	Comida"<<endl;
	cout<<"\t3.	Peliculas"<<endl;
	cout<<"\t4.	Musica"<<endl;
	cin>>n;
	switch(n)
	{
		case 1: {file_name="Animales.txt";
		break;}
		case 2: {file_name="Comida.txt";
		break;}
		case 3: {file_name="Peliculas.txt";
		break;}
		case 4: {file_name="Musica.txt";
		break;}
		default:{
		cout<<endl<<"El numero seleccionado no esta entre las opciones propuestas"<<endl<<endl;
		return 1;}
	}
	cout<<endl<<"ABRIENDO EL FICHERO: "<<file_name<<endl;
	return 0;
}


void Serialize (Node*& root, fstream& file)		//Escribe el arbol en el fichero
{
    if(root == NULL)
    {
        file<<"#"<<endl;
    }
    else
    {
	vector<string> choices;		//OPTIMIZAR
	str_split(root->ans,choices);
	int node_size=choices.size()-1;


	//std::copy(choices.begin(), choices.end(), std::ostream_iterator<std::string>(cout, "\n")); //Output choices

        file<< root->ans <<endl;
	for(int i=0;i<node_size;i++)
	{
        	Serialize((*root->v)[i],file);
	}
    }
}

void Deserialize (Node*& root,fstream& file)	//Construye el arbol a partir del fichero
{
	string str;
	if(!file.eof())
	{
		getline(file,str);
		cin.clear();
	}

	else
	{
		return;
        }

	if(strcmp(str.c_str(),"#")) //Comprobar que el nodo no esté vacio

	{
		root = new Node;
                root->ans = str;
		std::vector<string> choices;	//OPTIMIZAR
		str_split(str,choices);
		int node_size=choices.size()-1;
		root->v=new vector<Node*>(node_size);
		for(int i=0;i<node_size;i++)
		{
			Deserialize((*root->v)[i],file);
		}
	}
	else
	{
		root = NULL;
	}
}


void updatetree(Node* fin, Node*& prefin)
{
    string ans,qn,temp;
    int resp;
    fin->v=new vector<Node*>;

    cout<<endl<<"Que estabas buscando entonces?"<<endl;			//Nuevo nodo_respuesta que quiero clasificar
    cin.ignore();
    getline(cin,ans);

    cout<<"Que pregunta  distigue " << ans << " de " << fin->ans << " ?" <<endl;		//Nuevo nodo_pregunta para distinguir entre la nueva respuesta y la anterior
    getline(cin,qn);
    cout<<"Cuantas respuestas existen para esa pregunta?"<<endl;
    cin>>resp;
    fin->v=new vector<Node*>(resp);
    (*fin->v)[0]=new Node;
    (*fin->v)[1]=new Node;
    cout<<"Cual es la respuesta para " << ans << " ?" <<endl;
    cin>>temp;
    qn=qn+"$"+temp;
    (*fin->v)[0]->ans=ans;
    cout<<"Cual es la respuesta para " << fin->ans << " ?" <<endl;
    cin>>temp;
    qn=qn+"$"+temp;
    (*fin->v)[1]->ans=fin->ans;

    for(int i=2;i<resp;i++)
    {
	(*fin->v)[i]=new Node;
	cout<<"Dame otro resultado posible" <<endl;
	cin.ignore();
	getline(cin,ans);
	(*fin->v)[i]->ans=ans;
	cout<<"Cual es la respuesta para \" " << ans << " \" ?" <<endl;
	cin>>temp;
	qn=qn+"$"+temp;
    }
    fin->ans=qn;

    cout<<endl<<"Actualizando del fichero..."<<endl;
    file.open(filename,ios::out);  //Abre el fichero para escribir
        if(file.is_open())
        {
                Serialize(start,file);
		cout << endl << "Fichero actualizado correctamente"<<endl;
        }
        else
        {
                cout << endl << "ERROR: NO SE PUEDO ABRIR EL FICHERO" << endl;
        }




}

void finalquestion(Node* root, Node* prev)
{
    string reply;
    cout << endl << "Es " << root->ans <<" lo que estabas buscando? \t[y/n]"<<endl;
    cin>>reply;
    if(reply=="y")			//Elemento encontrado, busqueda finalizada
    {
	cout<<endl<<"Lo he encontrado"<<endl;
    }
    else if(reply=="n")		//Hay que modificar el arbol
    {
	updatetree(root, prev);
    }
    else
    {
        cout<<"Por favor, responda solo con [y/n]"<<endl;
        finalquestion(root,prev);
    }
}

void question(Node* root, Node* prev)
{
    int reply;
    std::vector<string> choices;
    str_split(root->ans,choices);

    if(choices.size()==1)  //Pregunta o respuesta?
    {
        finalquestion(root,prev);
    }
    else
    {
	cout<<choices[0]<<endl<<endl;
	for(int i=1;i<choices.size();i++)
	{
	cout<<"\t"<<i<<".- "<<choices[i]<<endl;
	}
	cin>>reply;
	cin.clear();
	cin.ignore();
	if(cin.fail()||reply<1||reply >= choices.size())
	{
		cout<<"Respuesta no válida, pruebe otra vez"<<endl;
		question(root,prev);

	}
	else
        {
	    question((*root->v)[reply-1],root);
	}
    }
}


int main()
{
	ready();	//Instrucciones iniciales
	int x;
	string reply;
	while(x=choose_file(filename));
	file.open(filename,ios::in); 	//Abre el fichero para leerlo
	if(file.is_open()){				//Comprueba si se ha podido abrir el fichero

		Deserialize(start,file);		//Construir el arbol
		file.close();
		do{
			question(start,NULL);
			cout<<"Quieres hacer otra busqueda en "<<filename<<"?	[y/n]"<<endl;
			cin>>reply;
			while(reply!="y" && reply!="n")
			{
				cout<<"Por favor, responda solo con [y/n]"<<endl;
				cin>>reply;
			}
		}while(reply=="y");
	}
	else
	{
		cout<<endl<<"FICHERO NO ENCONTRADO, por favor, comprueba que el fichero necesario se encuentra en el directorio"<<endl;
	}
	file.open(filename,ios::out);	//Abre el fichero para escribir
	if(file.is_open())
	{
		Serialize(start,file);
	}
	else
	{
		cout<<endl<<"NO SE PUEDO ACTUALIZAR EL FICHERO"<<endl;
	}
    return 0;
}
