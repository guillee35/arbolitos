#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;
struct Node		//estructura de los nodos del arbol
{
    string ans;
    struct Node* left;
    struct Node* right;
};
Node *root;		//puntero a nodo utilizado en las funciones
Node *start;		//puntero al primero nodo del arbol

fstream file;
string filename;       //Elegir el fichero en el que vamos a buscar


void question(Node* root, Node* prev);
void Serialize (Node*& root, fstream& file);
void Deserialize(Node*& root, fstream& file);
void updatetree(Node* fin, Node*& prefin);
void finalquestion(Node* root, Node* prev);
void ready();

void ready()
{

	cout<<endl<<endl<<endl<<"Este programa en C++ permite hacer busquedas en un fichero"<<endl;
	cout<<"Para ello se realizaran varias preguntas que guiaran al programa hasta el elemento buscado "<<endl;
	cout<<"Si no encuentra lo que esta buscando puede incluirlo en el fichero, para lo cual debera definir: "<<endl;
	cout<<"\tEl nombre del nuevo elemento a clasificar"<<endl;
	cout<<"\tLa pregunta que distinga entre el nuevo el elemento y el que propuso el programa "<<endl<<endl<<endl;
	cout<<"NOTA: solo se puede responder a las preguntas con:"<<endl;
	cout<<"\t[y] para si"<<endl;
	cout<<"\t[n] para no"<<endl<<endl<<endl;

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
        file<<"#"<<endl;
    else
    {
        file<< root->ans <<endl;
        Serialize(root->left,file);
        Serialize(root->right,file);
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
		cout<< "Parse ended";
                return;
            }
	    if(strcmp(str.c_str(),"#"))
            {
                cout<<str <<endl;
		root = new Node;
                root->ans = str;
                Deserialize(root->left,file);
                Deserialize(root->right,file);
            }
            else
            {
                root = NULL;
            }
}


void updatetree(Node* fin, Node*& prefin)
{
    string ans,qn,reply;
    cout<<endl<<"Que estabas buscando entonces?"<<endl;			//Nuevo nodo_respuesta que quiero clasificar
    cin.ignore();
    getline(cin,ans);
    cout<<"Que pregunta (con respuesta si o no) distigue " << ans << " de " << fin->ans << " ?" <<endl;		//Nuevo nodo_pregunta para distinguir entre la nueva respuesta y la anterior
    getline(cin,qn);
    Node *ansnode, *qnnode;
    ansnode = new Node;			//Crea nodo_respuesta
    ansnode->ans = ans;
    ansnode->left = NULL;
    ansnode->right = NULL;

    qnnode = new Node;			//Crea nodo_pregunta
    qnnode->ans = qn;

    cout<<"Cual seria la respuesta a esta pregunta para "<<ans<<"?  [y/n]"<<endl;			//Como sera la nueva estructura del arbol dependiendo de la respuesta a la pregunta
    cin>>reply;
    while(reply!="y" && reply!="n"){
    	cout<<"Por favor, responda solo con [y/n]"<<endl;
    	cin>>reply;
    }

    if (reply=="y")
    {
	qnnode->left = ansnode;
	qnnode->right = fin;
    }
    else
    {
    	qnnode->left = fin;
    	qnnode->right = ansnode;
    }

    if(prefin->left == fin)				//Inclusion de los nuevos nodos en el arbol
    {
        prefin->left = qnnode;
    }

    else if(prefin->right == fin)
    {
        prefin->right = qnnode;
    }

    cout<<endl<<"Actualizacion del fichero"<<endl<<endl;
    file.open(filename,ios::out);  //Abre el fichero para escribir
        if(file.is_open())
        {
                Serialize(start,file);
		cout << endl << "Fichero actualizado correctamente";
        }
        else
        {
                cout << endl << "NO SE PUEDO ACTUALIZAR EL FICHERO" << endl;
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
    string reply;
    if(root->left!=NULL && root->right!=NULL)		//Son los nodo_preguntas
    {
        cout<< endl << root->ans << "\t[y/n]" <<endl;
        cin>>reply;
        if(reply=="y")
        {
	    question(root->left,root);
	}
        else if(reply=="n")
	{
	    question(root->right,root);
	}
        else
        {
            cout<<"Por favor, responda solo con [y/n]"<<endl;
            question(root,prev);
        }
    }
    else		//Son los nodo_respuesta, las raices del arbol
    {
        finalquestion(root,prev);
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
