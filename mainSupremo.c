#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.c"
#include  <stdbool.h>
#include "list_answer.c"

typedef struct{
    char *nombre;
    char *tipo;
    char *marca;
    size_t stock;
    size_t precio;
}Producto;

typedef struct{
    Producto * producto;
    char * nombreCarro;
    int cantidad;
}ProductoCarrito;

const char *get_csv_field (char * tmp, int k) {
    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

int is_equal_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2)==0) return 1;
    return 0;
}

int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

void chomp(char *s) {
    while(*s && *s != '\n' && *s != '\r') s++;
    *s = 0;
}

Producto * crearProducto(){
    Producto * new = (Producto*)malloc(sizeof(Producto));
    new->nombre = (char*)calloc(100,sizeof(char));
    new->tipo = (char*)calloc(30,sizeof(char));
    new->marca = (char*)calloc(30,sizeof(char));
    new->stock = 0;
    new->precio = 0;
    return new;
}

void agregarProducto(Map* mapa){
    Producto * new = crearProducto();
    
    fflush(stdin);
    printf("Ingrese nombre del producto: \n > ");
    fgets(new->nombre, 100, stdin);
    chomp(new->nombre);
    printf("\n");
    if(searchMap(mapa, new->nombre) != NULL){
        Producto * copiaProd = searchMap(mapa, new->nombre);

        printf("\nProducto existente\n");
        printf("No agregar (1)\nAgregar de todas formas (2)\n > ");
        int resultado;
        scanf("%d", &resultado);
        printf("\n");
        if(resultado == 2){ 
            int cantidadAgregada;
            eraseMap(mapa, new->nombre);
            printf("Ingrese la cantidad: \n > ");
            scanf("%d", &cantidadAgregada);
            printf("\n");
            copiaProd->stock += cantidadAgregada;
            insertMap(mapa,new->nombre,copiaProd);
            printf("\n\n");
            return;
        } else{ free(new); return; }
    }
    fflush(stdin);
    printf("Ingrese tipo del producto: \n > ");
    fgets(new->tipo, 30, stdin);
    chomp(new->tipo);
    printf("\n");

    printf("Ingrese marca del producto: \n > ");
    fgets(new->marca, 30, stdin);
    chomp(new->marca);
    printf("\n");

    printf("Ingrese stock del producto: \n > ");
    scanf("%ld",&new->stock);
    printf("\n");

    printf("Ingrese precio del producto: \n > ");
    scanf("%ld", &new->precio);
    printf("\n\n");

    insertMap(mapa, new->nombre, new);
}

void importarCSV(Map * mapaNombre){
    char nombreArchivo [100];

    printf("Ingrese Nombre de Archivo a Importar\n > ");

    fflush(stdin);
    fgets(nombreArchivo,100,stdin);
    chomp(nombreArchivo);
    printf("\n");

    //if(strcmp("Archivo_100productos",nombreArchivo) != 0) return;
    FILE *fp = fopen(nombreArchivo,"r");

    if (fp == NULL){
        printf("Archivo no existe\n");
        return;
    }

    char linea[1024];

    while (fgets (linea, 1023, fp) != NULL) {
        Producto * new = crearProducto();
    
        strcpy(new->nombre,get_csv_field(linea, 0));
        chomp(new->nombre);
        strcpy(new->marca,get_csv_field(linea, 1));
        chomp(new->marca);
        strcpy(new->tipo,get_csv_field(linea, 2));
        chomp(new->tipo);
        new->stock = atoi(get_csv_field(linea, 3));
        new->precio = atoi(get_csv_field(linea, 4));
        
        insertMap(mapaNombre,(new->nombre),new);
    }

    printf("Archivo importado con exito\n");
    
    fclose(fp);
}

void mostrarProducto(Producto* prod){
    printf("Nombre: %s\nMarca: %s\nTipo: %s\n", prod->nombre, prod->marca, prod->tipo);
    printf("Stock: %ld\n$%ld\n\n", prod->stock, prod->precio);
}

void mostrarMapa(Map * mapa){
    Producto* aux = firstMap(mapa);
    int k = 0;
    while (aux) {
        mostrarProducto(aux);
        aux = nextMap(mapa);
        k++;
    }
    if (!k) printf("No hay productos\n");
}

void MostrarPorNombre(Map * mapa){
    char key[100];
    printf("Ingrese Nombre de producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 100, stdin);
    chomp(key);
    printf("\n\n");

    Producto* aux = searchMap(mapa,key);

    if(aux == NULL){
        int res;
        printf("Nombre no existente");
        printf("\nPara agregar (1)\nVolver al Menu Principal (2)\n > ");
        scanf("%d", &res);
        printf("\n\n");
        if(res == 1) agregarProducto(mapa);
        else return;
    } 
    mostrarProducto(aux);
    printf("\n");
}

void MostrarPorTipo(Map * mapa){
    char key[30];
    printf("Ingrese Tipo del producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);
    printf("\n\n");

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    int k = 0;
    while(aux != NULL){
        if(strcmp(aux->tipo,key) == 0){
            mostrarProducto(aux);
            k++;
        }
        aux = nextMap(mapa);
    }
    if (!k) printf("Tipo no existente\n\n");
}

void MostrarPorMarca(Map * mapa){
    char key[30];
    printf("Ingrese Marca del producto a buscar: \n > ");
    fflush(stdin);
    fgets(key, 30, stdin);
    chomp(key);
    printf("\n\n");

    Producto * aux = firstMap(mapa);// esta ordenado asi que deberia de darme el primer dato en relacion a la key
    int k = 0;
    while(aux != NULL){
        if(strcmp(aux->marca,key) == 0){
            mostrarProducto(aux);
            k++;
        }
        aux = nextMap(mapa);
    }
    if (!k) printf("Marca no existente\n\n");
}

ProductoCarrito * searchList(List * lista, Producto * buscado){
    ProductoCarrito * aux = firstList(lista);

    while (aux != NULL){
        if (is_equal_string(aux->producto->nombre, buscado->nombre))
            return aux;
        aux = nextList(lista);
    }
    return NULL;
}

void agregarDataCarrito(Map * mapa, Map* carrito){
    char * nombreProd = (char*)malloc(100*sizeof(char));
    fflush(stdin);
    printf("Ingrese nombre del producto: \n > ");
    fgets(nombreProd, 100, stdin);
    chomp(nombreProd);
    printf("\n");

    Producto * prod = searchMap(mapa, nombreProd);
    if (prod == NULL){
        printf("Producto no existente\n\n");
        free(nombreProd);
        return;
    }

    ProductoCarrito * new = (ProductoCarrito*)malloc(100*sizeof(ProductoCarrito));
    new->producto = prod;
    printf("Ingrese cantidad\n > ");

    int cant;
    scanf("%d", &cant);
    printf("\n");
    

    char * nombreCarrito = (char*)malloc(100*sizeof(char));
    fflush(stdin);
    printf("Ingrese nombre del carrito: \n > ");
    fgets(nombreCarrito, 100, stdin);
    chomp(nombreCarrito);
    printf("\n");
    new->nombreCarro = (char*)malloc(100*sizeof(char));
    strcpy(new->nombreCarro, nombreCarrito); 

    List * auxC = searchMap(carrito, nombreCarrito);

    if (auxC == NULL){
        if (new->producto->stock < cant){
        printf("No hay stock suficiente de este producto\nStock: %d\n", new->producto->stock);
        return;
    }
        List * listaP = createList();
        insertMap(carrito, nombreCarrito, listaP);
        auxC = searchMap(carrito, nombreCarrito);
        printf("Se creo el carrito \"%s\"\n",nombreCarrito);
    }
    else{
        ProductoCarrito * auxP = searchList(auxC, new->producto);
        if (auxP != NULL){
            printf("Producto ya existe en el carrito\n");
            if (new->producto->stock < new->producto->stock + cant){
                printf("No hay suficiente stock\nStock: %d\n", new->producto->stock);
            }
            else auxP->cantidad += cant;
            return;
        }
    }
    new->cantidad = cant;
    pushBack(auxC, new);
    printf("Se agrego correctamente el producto\n");
}

void mostrarListaCarrito(List * lista){
    int k = 1;
    ProductoCarrito * aux = firstList(lista);
    printf("Carrito \"%s\":\n", aux->nombreCarro);
    while(aux!=NULL){
        printf("(%d) %s: %d\n",k, aux->producto->nombre, aux->cantidad);
        aux = nextList(lista);
        k++;
    }
    printf("\n");
}

void mostrarProductosCarrito(Map * carrito){
    List * aux = firstMap(carrito);
    if(aux==NULL){
        printf("No hay carritos\n");
        return;
    }

    while (aux != NULL){
        mostrarListaCarrito(aux);
        aux = nextMap(carrito);
    }
}

void eliminarDataCarrito(Map * carrito){
    char nombreCarrito[100];
    fflush(stdin);
    printf("Ingrese nombre del carrito: \n > ");
    fgets(nombreCarrito, 100, stdin);
    chomp(nombreCarrito);
    printf("\n");

    List * lista = searchMap(carrito, nombreCarrito);
    if (lista == NULL){
        printf("El carrito buscado no existe\n");
        return;
    }
    ProductoCarrito * ultimo = lastList(lista);
    printf("Se eliminara el ultimo producto ingresado en el carrito\n\n");
    printf("[ %s: %d ]\n", ultimo->producto->nombre, ultimo->cantidad);
    printf("\nDesea eliminarlo de todos modos?\nSi(1)\nNo(2)\n > ");
    
    fflush(stdin);
    int res;
    scanf("%d",&res);
    printf("\n");
    
    if (res == 1){
        popBack(lista);
        printf("Eliminado con exito\n");
        if (firstList(lista) == NULL){
            eraseMap(carrito, nombreCarrito);
            printf("Carrito ya no contiene productos\nCarrito eliminado con exito\n");
        }

        return;
    }
    printf("No se elimino el producto\n");
}

size_t calcularPrecioTotal(List * lista){
    ProductoCarrito * aux = firstList(lista);
    size_t total = 0;

    while (aux != NULL){
        total += aux->producto->precio;
        aux = nextList(lista);
    }
    return total;
}

void restarStock(List * lista){
    ProductoCarrito * aux = firstList(lista);

    while (aux != NULL){
        aux->producto->stock -= aux->cantidad;
        aux = nextList(lista);
    }
}

void concretarCompra(Map * carrito){
    char * nombreCarrito = (char*)malloc(100*sizeof(char));
    fflush(stdin);
    printf("Ingrese nombre del carrito: \n > ");
    fgets(nombreCarrito, 100, stdin);
    chomp(nombreCarrito);
    printf("\n");

    List * lista = searchMap(carrito, nombreCarrito);
    if (lista == NULL){
        printf("El carrito buscado no existe\n");
        free(nombreCarrito);
        return;
    }

    int res;
    size_t total = calcularPrecioTotal(lista);
    mostrarListaCarrito(lista);
    printf("Total a pagar: $%ld\n\n", total);

    printf("Desea finalizar la compra?\nSi(1)\nNo(2)\n > ");

    fflush(stdin);
    scanf("%d", &res);
    printf("\n");

    if (res == 1){
        restarStock(lista);
        eraseMap(carrito, nombreCarrito);
        printf("Compra finalizada\n$%ld\n", total);
        return;
    }
    printf("No se realizo la compra\n");
}

bool menu(Map * mapa, Map * carrito){
    int res;
    printf ("________________\nQue desea hacer?\nCatalogo de productos(1)\nCarritos de compra(2)\nImportar Archivo (3)\nEXIT(4)\n > ");
    scanf("%d", &res);
    printf("\n");
    //Catálogo de compras
    if(res == 1){ 
        int respuesta;
        printf("Agregar producto (1)\nMostrar Producto: \n    Por Nombre (2)\n");
        printf("    Por Tipo (3)\n    Por Marca (4)\nMostrar Todos Los Datos (5) \nVolver al Menu(6)\n");
        printf(" > ");

        scanf("%d", &respuesta);
        printf("\n");

        if (respuesta == 1){
        agregarProducto(mapa);
        return true;  
        } 
        if (respuesta == 2) {
            MostrarPorNombre(mapa);
            return true;
        }
        if (respuesta == 3){
            MostrarPorTipo(mapa);
            return true;
        }
        if(respuesta == 4){
            MostrarPorMarca(mapa);
            return true;
        }
        if(respuesta == 5){ 
            printf("\n");
            mostrarMapa(mapa);
            return true;
        }
        if(respuesta == 6) return true;
        return false;
    }
    
    //Carritos de compra
    if (res == 2){
        int respuesta;
        printf("Agregar a Carritos(1)\nEliminar ultimo del Carrito(2)\nMostrar Carritos(3)\nConcretar Compra(4)\nVolver al Menu(5)\n > ");
        scanf("%d", &respuesta);
        printf("\n");
        if (respuesta == 1){
            agregarDataCarrito(mapa, carrito);
            return true;
        }
        if (respuesta == 2){
            eliminarDataCarrito(carrito);
            return true;
        }
        if (respuesta == 3){
            mostrarProductosCarrito(carrito);
            return true;
        }
        if (respuesta == 4){
            concretarCompra(carrito);
            return true;
        }
        if (respuesta == 5) return true;
        return false;
    }

    if (res == 3){
        importarCSV(mapa);
        return true;
    }
    
    return false;
}


int main() {

    
    Map * mapa = createMap(is_equal_string);
    Map * carrito = createMap(is_equal_string);

    setSortFunction(carrito, lower_than_string);
    setSortFunction(mapa,lower_than_string);

    while(menu(mapa, carrito));

    printf("Gracias por usar nuestro programa!\n > ");
    fflush(stdin);
    getchar();
    
    return 0;
}
