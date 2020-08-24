#include <iostream>
#include <bits/stdc++.h>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

struct nodo {
    double x;
    double y;
    string ciudad;
    int poblacion;
};

struct PR_quadtree {
    nodo n;
    double x1, x2, y1, y2;
    PR_quadtree* N[4];
    string tipo;
};

int cuadrante(nodo n, PR_quadtree* q) {             //Entrega el cuadrante del PR_quadtree q en el que está el nodo n
    if ((q->x1 + q->x2) / 2 > n.x) {      //Si esta a la izq
        if ((q->y1 + q->y2) / 2 < n.y) return 1;  //Si esta arriba                  
        else return 3;   //Si esta abajo
    }
    else if ((q->y1 + q->y2) / 2 < n.y){  //Si esta a la der y arriba
    return 2;
  }
    else return 4;    //Si esta a la derecha abajo
}


void heredar(PR_quadtree* aux, int old_cuad) {      //Función que hereda un nodo cuando un cuadrante es subdividido
    aux->N[old_cuad-1] = new PR_quadtree;
    aux->N[old_cuad - 1]->n = aux->n;
    for (int i = 0; i < 4; i++) {               //Crea un nuevo cuadrante hijo de aux, con el mismo nodo asociado que aux y de tipo negro
        aux->N[old_cuad-1]->N[i] = NULL;
    }
    aux->N[old_cuad - 1]->tipo = "negro";
    switch (old_cuad) {                                     //Las dimensiones del cuadrante cambian según el subcuadrante en el que está
    case 1:
        aux->N[old_cuad - 1]->x1 = aux->x1;                     //Mantiene los límites izq y superior si está en 1
        aux->N[old_cuad - 1]->x2 = (aux->x1 + aux->x2) / 2;
        aux->N[old_cuad - 1]->y1 = aux->y1;
        aux->N[old_cuad - 1]->y2 = (aux->y1 + aux->y2) / 2;
        break;
    case 2:
        aux->N[old_cuad - 1]->x1 = (aux->x1 + aux->x2) / 2;     //Mantiene los límites der y superior si está en 2
        aux->N[old_cuad - 1]->x2 = aux->x2;
        aux->N[old_cuad - 1]->y1 = aux->y1;
        aux->N[old_cuad - 1]->y2 = (aux->y1 + aux->y2) / 2;
        break;
    case 3:
        aux->N[old_cuad - 1]->x1 = aux->x1;                     //Mantiene los límites izq e inferior si está en 3
        aux->N[old_cuad - 1]->x2 = (aux->x1 + aux->x2) / 2;
        aux->N[old_cuad - 1]->y1 = (aux->y1 + aux->y2) / 2;
        aux->N[old_cuad - 1]->y2 = aux->y2;
        break;
    case 4:
        aux->N[old_cuad - 1]->x1 = (aux->x1 + aux->x2) / 2;     //Mantiene los límites der e inferior si está en 4
        aux->N[old_cuad - 1]->x2 = aux->x2;
        aux->N[old_cuad - 1]->y1 = (aux->y1 + aux->y2) / 2;
        aux->N[old_cuad - 1]->y2 = aux->y2;
        break;
    }
}

class Quadtree {
public:
    PR_quadtree* raiz;          //Tiene como parámetros la raíz y su tamaño
    int tam;
    Quadtree() {
        raiz = NULL;
        tam = 0;
    }

    void insertar(nodo n) {
        int count = 0;
        if (raiz == NULL) {                 //Si es el primer nodo a insertar establece los parámetros de la raíz
            raiz = new PR_quadtree;
            raiz->n = n;
            for (int i = 0; i < 4; i++) {
                raiz->N[i] = NULL;
            }
            raiz->tipo = "negro";
            //Se setean las dimensiones del cuadrante como las
            //totales del mapa
            raiz->x1 = -180;
            raiz->x2 = 180;
            raiz->y1 = 90;
            raiz->y2 = -90;
            //cout << "1\n";
            return;
        }
        else {                      //Caso contrario busca donde insertar el nodo
            PR_quadtree* aux;
            aux = raiz;
            while (aux != NULL) {
                //cout << aux->x1 << " " << aux->x2 << " " << aux->y1 << " " << aux->y2 << "\n";
                int cuad = cuadrante(n, aux);     //Cuadrante del nodo nuevo
                int old_cuad = cuadrante(aux->n, aux);  //Cuadrante del nodo en aux
                //Herencia del nodo aux->n
                //verifica si ya se había heredado o no el PR_quadtree actual
                if (aux->N[old_cuad-1] == NULL) { heredar(aux, old_cuad); }
                //Insertar nuevo nodo
                if (cuad != old_cuad) {   //Si está en un cuadrante distinto a aux->n
                    if (aux->N[cuad - 1] == NULL) {
                        aux->tipo = "gris";
                        aux->N[cuad - 1] = new PR_quadtree;     //Crea el nuevo cuadrante y establece sus parámetros
                        aux->N[cuad - 1]->n = n;
                        aux->N[cuad - 1]->tipo = "negro";
                        for (int i = 0; i < 4; i++) {
                            aux->N[cuad-1]->N[i] = NULL;
                        }
                        switch (cuad) {
                        case 1: {
                            aux->N[cuad - 1]->x1 = aux->x1;
                            aux->N[cuad - 1]->x2 = (aux->x1 + aux->x2) / 2;
                            aux->N[cuad - 1]->y1 = aux->y1;
                            aux->N[cuad - 1]->y2 = (aux->y1 + aux->y2) / 2;
                            break;
                        }
                        case 2: {
                            aux->N[cuad - 1]->x1 = (aux->x1 + aux->x2) / 2;
                            aux->N[cuad - 1]->x2 = aux->x2;
                            aux->N[cuad - 1]->y1 = aux->y1;
                            aux->N[cuad - 1]->y2 = (aux->y1 + aux->y2) / 2;
                            break;
                        }
                        case 3: {
                            aux->N[cuad - 1]->x1 = aux->x1;
                            aux->N[cuad - 1]->x2 = (aux->x1 + aux->x2) / 2;
                            aux->N[cuad - 1]->y1 = (aux->y1 + aux->y2) / 2;
                            aux->N[cuad - 1]->y2 = aux->y2;
                            break;
                        }
                        case 4: {
                            aux->N[cuad - 1]->x1 = (aux->x1 + aux->x2) / 2;
                            aux->N[cuad - 1]->x2 = aux->x2;
                            aux->N[cuad - 1]->y1 = (aux->y1 + aux->y2) / 2;
                            aux->N[cuad - 1]->y2 = aux->y2;
                            break;
                        }
                        }
                        //cout << count+1 <<" \n";
                        //cout << aux->N[cuad - 1]->x1 << " " << aux->N[cuad - 1]->x2 << " " << aux->N[cuad - 1]->y1 << " " << aux->N[cuad - 1]->y2 << "\n";
                        return;
                    }
                    else {                      //Si el nodo está en un cuadrante distinto a aux->n pero que está ocupado
                        aux->tipo = "gris";
                        aux = aux->N[cuad - 1];     //Se asegura de que el tipo de aux sea gris y avanza al siguiente cuadrante aumentando su profundidad
                        count++;
                        //cout << "cuadrante " << cuad << " ";
                        //cout << "conflicto con " << aux->n.ciudad << " ";
                    }
                    
                }
                else {                      // Si está en el mismo cuadrante que aux->n
                    aux->tipo = "gris";             //Se asegura de que el tipo de aux sea gris y avanza al siguiente subcuadrante
                    aux = aux->N[cuad - 1];
                    if (n.x == aux->n.x && n.y == aux->n.y) return;     //Si el nodo ya existe, sale de la función
                    count++;
                    //cout << count << " " << cuad << " ";
                    //cout << "conflicto con " << aux->n.ciudad << "\n";
                }
            }
        }
    }

    void eliminar(nodo n) {
        if (raiz == NULL) {
            return;
        }
        else {
            PR_quadtree* aux;
            aux = raiz;
            while (aux != NULL) {
                //cout << aux->x1 << " " << aux->x2 << " " << aux->y1 << " " << aux->y2 << "\n";
                int cuad = cuadrante(n, aux);       //Encuentra el cuadrante de aux en que está el nodo n
                if (aux->N[cuad - 1] == NULL) {         //Si el cuadrante es blanco, el nodo no está en el quadtree
                    return;
                }
                else if (aux->N[cuad - 1]->tipo == "gris") {             //Si el cuadrante es de tipo gris                                            
                    if (aux->n.x == n.x && aux->n.y == n.y) {       //Si el nodo asociado a aux es n, busca un nuevo nodo para aux
                        for (int i = 0; i < 4; i++) {
                            if (i + 1 != cuad && aux->N[i] != NULL) {
                                aux->n = aux->N[i]->n;
                                break;
                            }
                            else if (i == 3) {                  //Si el nodo asociado a aux no tiene para asociar, busca al siguiente nivel
                                for (int it = 0; it < 4; it++) {
                                    if (aux->N[cuad - 1]->N[it] != NULL && aux->N[cuad - 1]->N[it]->n.x != n.x && aux->N[cuad - 1]->N[it]->n.y != n.y) {
                                        aux->n = aux->N[cuad - 1]->N[it]->n;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    aux = aux->N[cuad - 1];           //Avanza hacia el cuadrante en que estÃ¡ el nodo buscado
                }
                else {              //En caso de que sea negro
                    if (aux->n.x == n.x && aux->n.y == n.y) {       //Si el nodo asociado a aux es n, busca un nuevo nodo para aux
                        for (int i = 0; i < 4; i++) {
                            if (i + 1 != cuad && aux->N[i] != NULL) {
                                aux->n = aux->N[i]->n;
                                break;
                            }
                            else if (i == 3) {                  //Si el nodo asociado a aux no tiene para asociar, busca al siguiente nivel
                                for (int it = 0; it < 4; it++) {
                                    if (aux->N[cuad - 1]->N[it] != NULL && aux->N[cuad - 1]->N[it]->n.x != n.x && aux->N[cuad - 1]->N[it]->n.y != n.y) {
                                        aux->n = aux->N[cuad - 1]->N[it]->n;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if (aux->N[cuad - 1]->n.ciudad == n.ciudad || (aux->N[cuad - 1]->n.x == n.x && aux->N[cuad - 1]->n.y == n.y)) {
                        //cout << aux->N[cuad - 1]->n.ciudad << " eliminado\n";
                        aux->N[cuad - 1] = NULL;
                        return;
                    }
                    else {
                        cout << "No existe el nodo " << n.ciudad << " " << n.x<<", "<<n.y<< endl;
                        return;
                    }
                }
            }
        }
    }

    int cont_pts(float x, float y, int d, PR_quadtree* Q) {
        int count = 0;
        int cuad;

        if (y + d >= 90)  d = 90 - y;
        else if (y - d < -90) d = 90 + y;
        if (x + d > 180) d = 180 - x;
        else if (x - d < -180) d = 180 + x;

        //cout << Q->x1 << " " << Q->x2 << " " << Q->y1 << " " << Q->y2 << "\n";

        //Define los límites de la región
        float x1 = x - d, x2 = x + d, y1 = y + d, y2 = y - d;

        for (int i = 0; i < 4; i++) {
            if (Q->N[i] != NULL) {
                switch (i+1) {
                case 1: {
                    if (Q->N[i]->x2 >= x1 && Q->N[i]->y2 <= y1) {     //Si exista área dentro del cuadrante 1 (límite der a la der de x1 y límite inf bajo y1)
                        //cout << "Area en cuadrante " << i + 1 << " de tipo " << Q->N[i]->tipo << endl;
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo está dentro del área aumenta el contador
                                //cout << "Se encontro un punto dentro del area en el cuadrante " << i + 1 << endl;
                                count++;
                            }
                        }
                        else {      //Si es de  tipo gris
                            //cout << "{" << endl;
                            count += cont_pts(x, y, d, Q->N[i]);
                            //cout << "}" << endl;
                        }
                    }
                    else //cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 2: {
                    if (Q->N[i]->x1 <= x2 && Q->N[i]->y2 <= y1) {     //Si exista área dentro del cuadrante 2 (límite izq a la izq de x2 y límite inf bajo y1)
                        //cout << "Area en cuadrante " << i + 1 << " de tipo " << Q->N[i]->tipo << endl;
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo está dentro del área aumenta el contador
                                //cout << "Se encontro un punto dentro del area en el cuadrante " << i + 1 << endl;
                                count++;
                            }
                        }
                        else {      //Si es de  tipo gris
                           // cout << "{" << endl;
                            count += cont_pts(x, y, d, Q->N[i]);
                            //cout << "}" << endl;
                        }
                    }
                    //else cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 3: {
                    if (Q->N[i]->x2 >= x1 && Q->N[i]->y1 >= y2) {     //Si exista área dentro del cuadrante 3 (límite der a la der de x2 y límite sup sobre y2)
                        //cout << "Area en cuadrante " << i + 1 << " de tipo " << Q->N[i]->tipo << endl;
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo está dentro del área aumenta el contador
                                //cout << "Se encontro un punto dentro del area en el cuadrante " << i + 1 << endl;
                                count++;
                            }
                        }
                        else {      //Si es de  tipo gris
                            //cout << "{" << endl;
                            count += cont_pts(x, y, d, Q->N[i]);
                           // cout << "}" << endl;
                        }
                    }
                    //else cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 4: {
                    if (Q->N[i]->x1 <= x2 && Q->N[i]->y1 >= y2) {     //Si exista área dentro del cuadrante 4 (límite izq a la izq de x2 y límite sup sobre y2)
                        //cout << "Area en cuadrante " << i + 1 << " de tipo " << Q->N[i]->tipo << endl;
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo está dentro del área aumenta el contador
                             //   cout << "Se encontro un punto dentro del area en el cuadrante " << i + 1 << endl;
                                count++;
                            }
                        }
                        else {      //Si es de  tipo gris
                           // cout << "{" << endl;
                            count += cont_pts(x, y, d, Q->N[i]);
                          //  cout << "}" << endl;
                        }
                    }
                    //else cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                }
            }
            //else cout << "Cuadrante " << i + 1 << " blanco\n";
        }
        return count;
    }

    int pob_en_pt(int x, int y, PR_quadtree* Q) {
        int d = 1, pob, pts;
        pob = pob_en_reg(x, y, d, Q);
        pts = cont_pts(x, y, d, Q);
        return pob / pts;
    }

    int pob_en_reg(float x, float y, int d, PR_quadtree* Q) {
        int count = 0;
        int cuad;
        float x1 = x - d, x2 = x + d, y1 = y + d, y2 = y - d;

        //Define los lÃ­mites de la regiÃ³n
        if (y + d > 90)  y1 = 90;
        else y1 = y + d;
        if (y - d < -90) y2 = -90;
        else y2 = y - d;
        if (x - d < -180) x1 = -180;
        else x1 = x - d;
        if (x + d > 180) x2 = 180;
        else x2 = x + d;

        for (int i = 0; i < 4; i++) {
            if (Q->N[i] != NULL) {
                switch (i + 1) {
                case 1: {
                    if (Q->N[i]->x2 >= x1 && Q->N[i]->y2 <= y1) {     //Si exista Ã¡rea dentro del cuadrante 1 (lÃ­mite der a la der de x1 y lÃ­mite inf bajo y1)
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo estÃ¡ dentro del Ã¡rea aumenta el contador
                                count += Q->N[i]->n.poblacion;
                            }
                        }
                        else {      //Si es de  tipo gris
                            count += pob_en_reg(x, y, d, Q->N[i]);
                        }
                    }
                    //cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 2: {
                    if (Q->N[i]->x1 <= x2 && Q->N[i]->y2 <= y1) {     //Si exista Ã¡rea dentro del cuadrante 2 (lÃ­mite izq a la izq de x2 y lÃ­mite inf bajo y1)
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo estÃ¡ dentro del Ã¡rea aumenta el contador
                                count += Q->N[i]->n.poblacion;
                            }
                        }
                        else {      //Si es de  tipo gris
                            count += pob_en_reg(x, y, d, Q->N[i]);
                        }
                    }
                    //cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 3: {
                    if (Q->N[i]->x2 >= x1 && Q->N[i]->y1 >= y2) {     //Si exista Ã¡rea dentro del cuadrante 3 (lÃ­mite der a la der de x2 y lÃ­mite sup sobre y2)
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo estÃ¡ dentro del Ã¡rea aumenta el contador
                                count += Q->N[i]->n.poblacion;
                            }
                        }
                        else {      //Si es de  tipo gris
                            count += pob_en_reg(x, y, d, Q->N[i]);
                        }
                    }
                    //cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                case 4: {
                    if (Q->N[i]->x1 <= x2 && Q->N[i]->y1 >= y2) {     //Si exista Ã¡rea dentro del cuadrante 4 (lÃ­mite izq a la izq de x2 y lÃ­mite sup sobre y2)
                        if (Q->N[i]->tipo == "negro") {     //Si es de tipo negro
                            if (Q->N[i]->n.x >= x1 && Q->N[i]->n.x <= x2 && Q->N[i]->n.y <= y1 && Q->N[i]->n.y >= y2) {  //Si el nodo estÃ¡ dentro del Ã¡rea aumenta el contador
                                count += Q->N[i]->n.poblacion;
                            }
                        }
                        else {      //Si es de  tipo gris
                            count += pob_en_reg(x, y, d, Q->N[i]);
                        }
                    }
                    //cout << "No hay area en el cuadrante " << i + 1 << endl;
                    break;
                }
                }
            }
        }
        return count;
    }
};

int main() {
    //leer el csv y guarda cada separación en el vector datos_csv
    ifstream infile("worldcitiespop_fixed.csv");
    string line = "";
    vector<string> datos_csv;
    vector<double> geopoint;
    vector<nodo> nodos2del, nodos2find;
    int count=0;
    while (getline(infile, line) && count < 1000000*8) {
        stringstream buffer(line);
        string word = "";
        while (getline(buffer, word, ';')) {
            datos_csv.push_back(word);
            count++;
        }
    }
    infile.close();
    Quadtree Qtree = Quadtree();
    double x, y;
    int pob;
    clock_t start = clock();
    for (int i = 1; i < datos_csv.size()/8; i++) {
        int del = datos_csv[8 * i + 7].find(',');
        y = stod(datos_csv[8 * i + 7].substr(0,del));
        x = stod(datos_csv[8 * i + 7].substr(del + 1, string::npos));

        //geopoint.push_back(x);
        //geopoint.push_back(y);
        nodo n;
        n.x = x;
        n.y = y;
        n.ciudad = datos_csv[8 * i + 1];
        n.poblacion = stoi(datos_csv[8 * i + 4]);
        //cout << n.ciudad << " " << n.poblacion <<" " << n.x << "," << n.y << "\n";
        Qtree.insertar(n);
        if (i % 5 == 0) {
            nodos2del.push_back(n);
        }
        if (i < 100) {
            nodos2find.push_back(n);
        }
    }
    cout << "Tiempo para insertar " << datos_csv.size() / 8 << " nodos " << (double)(clock()-start)/CLOCKS_PER_SEC <<"\n";
    start = clock();
    for (int i = 0; i < nodos2find.size(); i++) {
       // cout << "Area "<< i+1 << " : " << nodos2find.at(i).x - (i % 50)-1 << ", " << nodos2find.at(i).x +1+ (i % 50) << ", " << nodos2find.at(i).y +1+(i % 50) << ", " << nodos2find.at(i).y-1 - (i % 50) << endl;
        //cout << "Puntos encontrados: " << Qtree.cont_pts(nodos2find.at(i).x, nodos2find.at(i).y, (i%50)+1, Qtree.raiz) << endl;
        pob = Qtree.pob_en_pt(nodos2find.at(i).x, nodos2find.at(i).y, Qtree.raiz);
        cout << nodos2find.at(i).poblacion << " " << pob << endl;
    }
    cout << "Tiempo para encontrar puntos de 100 areas " << (double)(clock() - start) / CLOCKS_PER_SEC << "\n";
    start = clock();
    for (int i = 0; i < nodos2del.size(); i++) {
        Qtree.eliminar(nodos2del.at(i));
    }
    cout << "Tiempo para eliminar " << nodos2del.size() << " nodos " << (double)(clock() - start) / CLOCKS_PER_SEC << "\n";
}