#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;
class Node		//estructura de los nodos del arbol
{
 private:
	string ans;
	vector<Node*> *v ;
 public:

	void setAns(string str)//Guarda nueva pregunta
	{
		ans=str;
	}
	string getAns()//Saca pregunta actual
	{
		return ans;
	}

	vector<Node*>* getVector()//Apunta al vector con los nodos que salen de este
	{
		return v;
	}

	void allocateVector(unsigned int size) //crea un nuevo vector de nodos
	{
		v=new vector<Node*> (size);
	}
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
	cout<<"\t3.	Musica"<<endl;
	cout<<"\t4.	Definido por el usuario"<<endl;
	cin>>n;
	switch(n)
	{
		case 1: {file_name="Animales.txt";
		break;}
		case 2: {file_name="Comida.txt";
		break;}
		case 3: {file_name="Musica.txt";
		break;}
		case 4: {file_name="Custom.txt";
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
	vector<string> choices;		//OPTIMIZAR
	str_split(root->getAns(),choices);
	int node_size=choices.size()-1;


	//std::copy(choices.begin(), choices.end(), std::ostream_iterator<std::string>(cout, "\n")); //Output choices

        file<< root->getAns() <<endl;
	for(int i=0;i<node_size;i++)
	{
        	Serialize((*root->getVector())[i],file);
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

	root = new Node;
	root->setAns(str);
	std::vector<string> choices;
	str_split(str,choices);
	int node_size=choices.size()-1;
	root->allocateVector(node_size);
	for(int i=0;i<node_size;i++)
	{
		Deserialize((*root->getVector())[i],file);
	}
}


void updatetree(Node* fin, Node*& prefin)
{
    string ans,qn,temp;
    int resp;

	//Esta sección del código guia al usuario para guardar nuevas soluciones

    cin.ignore();
    cout<<endl<<"Que estabas buscando entonces?"<<endl;
    getline(cin,ans);
    cout<<"Que pregunta  distigue " << ans << " de " << fin->getAns() << " ?" <<endl;
    getline(cin,qn);
    cout<<"Cuantas respuestas existen para esa pregunta?"<<endl;
    cin>>resp;
    cin.ignore();
    fin->allocateVector(resp);
    (*fin->getVector())[0]=new Node;
    (*fin->getVector())[1]=new Node;
    cout<<"Cual es la respuesta para " << ans << " ?" <<endl;
    getline(cin, temp);
    qn=qn+"$"+temp;
    (*fin->getVector())[0]->setAns(ans);
    cout<<"Cual es la respuesta para " << fin->getAns() << " ?" <<endl;
    getline(cin, temp);
    qn=qn+"$"+temp;
    (*fin->getVector())[1]->setAns(fin->getAns());

    for(int i=2;i<resp;i++)
    {
	(*fin->getVector())[i]=new Node;
	cout<<"Dame otro resultado posible" <<endl;
	getline(cin,ans);
	(*fin->getVector())[i]->setAns(ans);
	cout<<"Cual es la respuesta para \" " << ans << " \" ?" <<endl;
	getline(cin, temp);
	qn=qn+"$"+temp;
    }
    fin->setAns(qn);

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
    cout << endl << "Es " << root->getAns() <<" lo que estabas buscando? \t[y/n]"<<endl;
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
    str_split(root->getAns(),choices);

    if(choices.size()==1)  //Pregunta o respuesta?
    {
        finalquestion(root,prev);
    }
    else
    {
	cout<<choices[0]<<endl<<endl;
	for(int i=1;i<choices.size();i++)
	{
	cout<<"\t"<<i<<".- "<<choices[i]<<endl; //Mostrar Pregunta por pantalla
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
	    question((*root->getVector())[reply-1],root);
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
