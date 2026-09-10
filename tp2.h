#ifndef TP2_H
#define TP2_H

#include <cstddef> // size_t

/*
 * ============================================================
 *  Lista doblemente enlazada -> clase List.
 *  List es un tipo paramétrico: para cada T específico se tiene
 *  un tipo de datos List<T>.
 *
 *  La lista es dueña de sus nodos (RAII): los reserva con
 *  new y los libera en el destructor con delete. Cada nodo contiene
 *  un valor de tipo genérico T. Del contrato del tipo T,
 *  solamente se puede asumir que el tipo T tiene definidos un
 *  constructor por copia, un operador de asignación,
 *  y un destructor adecuados, pero ninguna otra operación.
 * 
 *  Los tipos predefinidos estandar de C++ como int, bool, char,
 *  punteros, string, vector, etc cumplen esto así que se
 *  debe poder crear List<int>, List<bool>, List<string>,
 *  List<vector<int>>, etc.
 *  Como la propia lista tiene definidos en este archivo
 *  esas operaciones, se puede crear listas de listas
 *  como por ejemplo List<List<int>> o List<List<List<string>>>.
 * 
 *  C++ se encarga de llamar automáticamente al destructor de T tras
 *  ejecutar el destructor de un Node<T> desencadenado por
 *  un delete de un nodo (para poder liberar el value del nodo).
 * ============================================================
 */
template<typename T>
class List {
private:
    /*
     * Nodo interno de la lista. Es un detalle de implementación,
     * por eso queda privado. El tipo completo es List<T>::Node
     */
    struct Node {
        // Unico constructor de Node: utiliza el constructor por copia de T
        Node(const T& v) : value(v) {}
        T value;
        Node *next;
        Node *prev;
    };

    Node *head;
    Node *tail;
    size_t size;

public:
    /*
     * Crea una lista vacía.
     * P (Pre):  -
     * Q (Post): is_empty() == true && length() == 0.
     */
    List();

    /* Constructor por copia copia.
     * P (Pre):  other es una List válida.
     * Q (Post): *this ahora contiene una copia independiente de other,
     *            con sus propios nodos diferentes de los de other (deep copy)
     */
    List(const List<T> &other);

    /* Operador de asignacion.
     * P (Pre):  other es una List válida.
     * Q (Post): *this pasa a contener una copia independiente de other,
     *            con sus propios nodos diferentes de los de other (deep copy)
     *           No se debe perder memoria!!!

     */
    List &operator=(const List<T> &other);

    /*
     * Destructor: libera todos los nodos que queden con delete.
     * No necesita encargarse de liberar por separado
     * los valores T de los value explicitamente:
     * C++ llama al destructor del tipo T automaticamente al hacer delete de un nodo
     * P (Pre):  '*this' es una List válida.
     * Q (Post): la memoria de todos los nodos quedó liberada.
     */
    ~List();

    /*
     * Devuelve true si la lista está vacía.
     * P (Pre):  '*this' es una List válida.
     * Q (Post): devuelve length() == 0; no modifica la lista (const).
     */
    bool is_empty() const;

    /*
     * Cantidad de elementos de la lista.
     * P (Pre):  '*this' es una List válida.
     * Q (Post): devuelve la cantidad de elementos (>= 0);
     *           no modifica la lista (const).
     */
    size_t length() const;

    /*
     * Inserta un elemento al principio de la lista.
     * P (Pre):  '*this' es una List válida. Sea n0 = length().
     * Q (Post): length() == n0 + 1, peek_head() == value.
     */
    void insert_head(const T& value);

    /*
     * Inserta un elemento al final de la lista.
     * P (Pre):  '*this' es una List válida. Sea n0 = length().
     * Q (Post): length() == n0 + 1, peek_tail() == value.
     */
    void insert_tail(const T& value);

    /*
     * Elimina el elemento del principio y devuelve el valor que guardaba.
     * P (Pre):  '*this' es una List no vacia. Sea n0 = length().
     * Q (Post): saca el primer nodo, length() == n0 - 1 y
     *           devuelve el valor que tenía.
     */
    T pop_head();

    /*
     * Elimina el elemento del final y devuelve el valor que guardaba.
     * P (Pre):  '*this' es una List no vacia. Sea n0 = length().
     * Q (Post): saca el último nodo, length() == n0 - 1 y
     *           devuelve el valor que tenía.
     */
    T pop_tail();

    /*
     * Devuelve el valor del principio de la lista sin sacarlo.
     * P (Pre):  '*this' es una List no vacia.
     * Q (Post): devuelve el primer valor;
     *           no modifica la lista (const).
     */
    const T &peek_head() const;

    /*
     * Devuelve el valor del final de la lista sin sacarlo.
     * P (Pre):  '*this' es una List<T> no vacia.
     * Q (Post): devuelve el último valor;
     *           no modifica la lista (const).
     */
    const T &peek_tail() const;
    
    /*
     * ============================================================
     *  Iterador de lista. El tipo completo es List<T>::ListIter
     *
     *  Puede acceder a los nodos y miembros privados de List,
     *  al ser una clase anidada dentro del scope List<T>
     *  Se crean con los metodos List::create_head / List::create_tail
     * ============================================================
     */
    class ListIter {
    private:
        List *list;
        List::Node *curr;

        /*
         * Constructor privado: se usa desde las fábricas List::create_head /
         * List::create_tail. Deja el iterador parado en 'start'.
         */
        ListIter(List<T> *list, List<T>::Node *start);

        friend List<T>;

    public:
        /*
         * Avanza el iterador una posición hacia el final.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): si hay un elemento siguiente, avanza y devuelve true;
         *           si no, no se mueve y devuelve false.
         */
        bool forward();

        /*
         * Retrocede el iterador una posición hacia el principio.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): si hay un elemento anterior, retrocede y devuelve true;
         *           si no, no se mueve y devuelve false.
         */
        bool backward();

        /*
         * Devuelve el valor de la posición actual.
         * P (Pre):  '*this' es un ListIter válido a un elemento de una lista no vacia.
         * Q (Post): devuelve el valor actual; no modifica nada (const).
         */
        const T& peek_current() const;

        /*
         * Indica si el iterador está en el último elemento.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): devuelve true si está en el último (o la lista está vacía),
         *           false en otro caso; no modifica nada (const).
         */
        bool at_last() const;

        /*
         * Indica si el iterador está en el primer elemento.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): devuelve true si está en el primero (o la lista está vacía),
         *           false en otro caso; no modifica nada (const).
         */
        bool at_first() const;

        /*
         * Inserta un valor justo después del elemento actual.
         * Si la lista está vacía, inserta el primer elemento y queda parado en él.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): agrega el valor detrás del actual sin mover la posición del
         *           iterador y devuelve true.
         */
        bool insert_after(const T&value);

        /*
         * Inserta un valor justo antes del elemento actual.
         * Si la lista está vacía, inserta el primer elemento y queda parado en él.
         * P (Pre):  '*this' es un ListIter válido.
         * Q (Post): agrega el valor delante del actual sin mover la posición del
         *           iterador y devuelve true.
         */
        bool insert_before(const T&value);

        /*
         * Elimina el elemento actual y devuelve el valor que guardaba.
         * P (Pre):  '*this' es un ListIter que apunta a un elemento válido en una lista no vacia.
         * Q (Post): saca el nodo actual, se para en el siguiente (o en el
         *           anterior si no había siguiente) y devuelve el valor que tenía.
         */
        T remove();
    };

    /*
     * Crea un iterador parado en el principio de la lista.
     * P (Pre):  '*this' es una List<T> válida.
     * Q (Post): devuelve un iterador nuevo parado en el primer elemento.
     */
    ListIter create_head();

    /*
     * Crea un iterador parado en el final de la lista.
     * P (Pre):  '*this' es una List<T> válida.
     * Q (Post): devuelve un iterador nuevo parado en el último elemento.
     */
    ListIter create_tail();


};


// ****************************************************************
//  SECCION DE IMPLEMENTACION!!!
//    Por tratarse de una clase template, la implementacion
//    debe ir en el mismo .h, debido a como funcionan
//    los templates en C++
// ****************************************************************



/* ---------------------------------------------------------------
 * Clase List: lista doblemente enlazada, dueña de sus nodos.
 * Reservá y liberá los nodos con new / delete (nunca malloc / free).
 * --------------------------------------------------------------- */

template<typename T>
List<T>::List() {

    // TODO: dejar la lista vacía (head, tail en nullptr y size en 0).

    this->head = nullptr;   // La "cabeza" de la lista apunta hacia un puntero nulo.
    this->tail = nullptr;   // La "cola" de la lista apunta hacia un puntero nulo.
    this->size = 0;    // El tamaño de la lista es 0 debido a que no tiene elementos.
}

template<typename T>
List<T>::List(const List<T> &other) {

    // TODO: crear la nueva lista, como una copia independiente de other

    this->head = nullptr;   // Configuramos la "cabeza" de la nueva lista en un puntero vacío al ser creada recién.
    this->tail = nullptr;  // Configuramos la "cola" de la nueva lista en un puntero vacío por el mismo motivo.
    this->size = 0;      // Definimos el tamaño de la lista en 0 ya que se encuentra vacía al inicio.

    const Node* actual = other.head;  // Creamos un nodo nuevo que apunte hacia la "cabeza" de la otra lista enlazada.
        
    while (actual != nullptr) {  // Recorremos los nodos de la lista enlazada a copiar hasta que obtengamos uno que apunte hacia un puntero vacío.
        insert_tail(actual->value);  // Copiamos los valores de la otra lista enlazada en la "cola" de la creada recientemente.
        actual = actual->next;   // Avanzamos hacia el siguiente nodo de la otra lista enlazada.  
    }
}

template<typename T>
List<T> &List<T>::operator=(const List<T> &other) {

    // TODO: reemplazar la lista actual con una copia independiente de other
    // La memoria anterior no debe perderse, sino liberarse correctamente

    if (this == &other) {  // Corroboramos si la dirección de memoria de la lista actual es la misma que la de la lista "other".
        return *this;  // Devolvemos la misma lista debido a que son iguales.
    }

    while (this->head != nullptr) {
        this->pop_head();  // Eliminamos la "cabeza" de la lista mientras la misma no sea "nullptr".
    }

    const Node* nodo_nuevo = other.head;
    while (nodo_nuevo != nullptr) {  // Comprobamos que el nodo que colocaremos en la lista no sea un puntero nulo.
        this->insert_tail(nodo_nuevo->value);  // Insertamos el valor del nuevo nodo en la "cola" de la lista.
        nodo_nuevo = nodo_nuevo->next;  // Actualizamos el nodo nuevo para que apunte a su siguiente.
    }

    return *this;  // Devolvemos la lista final.
}

template<typename T>
List<T>::~List() {

    // TODO: liberar todos los nodos que queden con delete.

    Node* nodo_actual = this->head;  // Definimos al nodo actual como la "cabeza" de la lista.

    while (nodo_actual != nullptr) {  // Comprobamos que el nodo actual sea distinto de "nullptr" (puntero nulo).
        Node* nodo_a_borrar = nodo_actual;  // Definimos el nodo a eliminar como el nodo actual.
        nodo_actual = nodo_actual->next;  // Movemos el puntero del nodo actual para que apunte a su siguiente.
        delete nodo_a_borrar;  // Eliminamos el nodo que era la anterior "cabeza" de la lista.
    }

    this->head = nullptr;  // Establecemos la "cabeza" de la lista en nullptr ya que se eliminaron todos los nodos.
    this->tail = nullptr;  // Establecemos la "cola" de la lista en nullptr debido a que se eliminaron todos los nodos.
    this->size = 0;  // Colocamos el tamaño de la lista en 0 dado que no tiene nodos.
}

template<typename T>
bool List<T>::is_empty() const {

    // TODO: devolver si la lista está vacía.

    if (this->size != 0) {  // Si el tamaño de la lista es distinta de 0, entonces no está vacía y devuelve "false".
        return false;
    }

    return true;  // Devuelve "true" si la lista no tiene elementos.
}

template<typename T>
size_t List<T>::length() const {

    // TODO: devolver la cantidad de elementos.

    return this->size;  // Devuelve el tamaño de la lista enlazada.
}

template<typename T>
void List<T>::insert_head(const T& value) {

    // TODO: reservar un nodo con new, enlazarlo al principio y
    // actualizar head/tail/size.

    Node* nuevo_nodo_inicio = new Node(value);  // Creamos un nuevo nodo.

    if (this->size == 0) {   // Comprobamos si la lista está vacía.
        nuevo_nodo_inicio->next = nullptr;
        nuevo_nodo_inicio->prev = nullptr;
        this->head = nuevo_nodo_inicio; // La "cabeza" de la lista pasa a ser el nuevo nodo.
        this->tail = nuevo_nodo_inicio; // La "cola" de la lista pasa a ser el nuevo nodo.
        this->size += 1;  // Agregamos 1 al tamaño de la lista luego de insertar el nuevo nodo.
    }

    else {  // Chequeamos el caso en el cual la lista no esta vacía de antemano.
        nuevo_nodo_inicio->prev = nullptr;
        nuevo_nodo_inicio->next = this->head;  // El puntero siguiente al nuevo nodo pasa a ser la "cabeza" del anterior nodo.
        this->head->prev = nuevo_nodo_inicio;  // El puntero previo al de la "cabeza" del nodo antiguo pasa a ser el nuevo nodo.
        this->head = nuevo_nodo_inicio;  // La "cabeza" de la lista pasa a ser el nuevo nodo incorporado.
        this->size += 1;  // Agregamos 1 al tamaño de la lista.
    }

}

template<typename T>
void List<T>::insert_tail(const T& value) {

    // TODO: reservar un nodo con new, enlazarlo al final y
    // actualizar head/tail/size.

    Node* nuevo_nodo_final = new Node(value);  // Creamos un nuevo nodo que insertaremos al final de la lista.

    if (this->size == 0) {  // Comprobamos si la lista está vacía
        nuevo_nodo_final->next = nullptr;
        nuevo_nodo_final->prev = nullptr;
        this->head = nuevo_nodo_final;  // La "cabeza" pasa a ser el nuevo nodo agregado.
        this->tail = nuevo_nodo_final;  // La "cola" pasa a ser igualmente el nuevo nodo agregado ya que la lista tiene un solo elemento.
        this->size += 1;  // Aumentamos 1 al tamaño de la lista.
    }

    else {  // Caso en el cual la lista no esta vacía.
        nuevo_nodo_final->next = nullptr;
        this->tail->next = nuevo_nodo_final; // El siguiente a la "cola" actual pasa a ser el nuevo nodo.
        nuevo_nodo_final->prev = this->tail;  // El anterior al nuevo nodo pasa a ser la "cola" antigua de la lista.
        this->tail = nuevo_nodo_final;  // Definimos la "cola" de la lista como el nuevo nodo agregado.
        this->size += 1;  // Agregamos 1 al tamaño de la lista.
    }
}

template<typename T>
T List<T>::pop_head() {

    // TODO: sacar el primer nodo (con delete), devolver su valor.

    Node* cabeza_antigua_a_borrar = this->head;  // Definimos el nodo (la "cabeza") de la lista que vamos a eliminar.
    T valor_del_nodo_antiguo = cabeza_antigua_a_borrar->value; // Guardamos el valor de la "cabeza" de la lista para poder devolverlo al final 
                                                              //  y no perderlo al momento de eliminar su nodo correspondiente.

    this->head = cabeza_antigua_a_borrar->next;  // Actualizamos el puntero de la "cabeza" de la lista para que apunte a su nodo siguiente.
    delete cabeza_antigua_a_borrar; // Borramos el nodo (nodo de la "cabeza" antigua de la lista) y liberamos su memoria.

    this->size -= 1;  // Restamos 1 al tamaño de la lista luego de eliminar el nodo.

    if (this->head != nullptr) {  // Verificamos que la nueva "cabeza" de la lista no sea un puntero nulo luego de eliminar el nodo antiguo.
                                  // (Caso en el que la lista tiene un solo elemento: Únicamente tenía al nodo "cabeza" antiguo.)

        this->head->prev = nullptr;  // Actualizamos el puntero previo de la "cabeza" de la lista a "nullptr"
    }                               // (puntero vacío ya que es el anterior a la nueva "cabeza").

    else {
        this->tail = nullptr;   // Definimos la "cola" de la lista en "nullptr" luego de eliminar el único nodo que había anteriormente.
    }
    
    return valor_del_nodo_antiguo;  // Devolvemos el valor de la "cabeza" antigua de la lista.
} 

template<typename T>
T List<T>::pop_tail() {

    // TODO: sacar el último nodo (con delete), devolver su valor.

    Node* cola_antigua_a_borrar = this->tail; // Definimos la "cola" de la lista que vamos a eliminar.
    T valor_del_nodo_final_antiguo = cola_antigua_a_borrar->value;  // Guardamos el valor de la "cola" de la lista para poder  
                                                //  devolverlo luego y no perderlo al momento de eliminar su nodo correspondiente.

    this->tail = cola_antigua_a_borrar->prev;  // Actualizamos el puntero de la "cola" de la lista para que apunte a su nodo anterior.
    delete cola_antigua_a_borrar;  // Eliminamos el nodo (nodo de la "cola" antigua de la lista) y liberamos su memoria.

    this->size -= 1;  // Restamos 1 al tamaño de la lista luego de eliminar el nodo.

    if (this->tail != nullptr) { // Verificamos que la nueva "cola" de la lista no sea un puntero nulo.
        this->tail->next = nullptr;  // Actualizamos el puntero siguiente a la "cola" de la lista para que apunte a "nullptr".
    }

    else {
        this->head = nullptr;  // Definimos la "cabeza" de la lista en "nullptr" luego de eliminar el único nodo que había anteriormente.
    }
    
    return valor_del_nodo_final_antiguo;  // Devolvemos el valor de la "cola" antigua de la lista.
}

template<typename T>
const T& List<T>::peek_head() const { 

    // TODO: devolver el valor del principio.

    return this->head->value;  // Devolvemos el valor del nodo que se encuentra en la "cabeza" de la lista.
}

template<typename T>
const T& List<T>::peek_tail() const {

    // TODO: devolver el valor del final.

    return this->tail->value;  // Devolvemos el valor del nodo que se encuentra en la "cola" de la lista.
}

template <typename T>
typename List<T>::ListIter List<T>::create_head() {

    // TODO: retornar un iterador parado en el principio de la lista.

    return ListIter(this, this->head);  // Devolvemos el iterador parado en la "cabeza" de la lista.
}

template <typename T>
typename List<T>::ListIter List<T>::create_tail() {

    // TODO: retornar un iterador parado en el final de la lista.

    return ListIter(this, this->tail);  // Retornamos el iterador situado en la "cola" de la lista.
}

/* ---------------------------------------------------------------
 * Clase ListIter: iterador sobre una List.
 * Se obtienen mediante los métodos fábrica List::create_head / List::create_tail
 * --------------------------------------------------------------- */

// Constructor privado, utilizado por los métodos fábrica create_head y create_tail
template <typename T>
List<T>::ListIter::ListIter(List *list, List::Node *start) {
    this->list = list;
    this->curr = start;
}

template <typename T>
bool List<T>::ListIter::forward() {
    if(curr != nullptr && curr->next != nullptr){ // Chequeo primero que el curr sea distinto de null asi descarto la lista vacia y ahi si miro que el siguiente tampoco sea null
        curr = curr->next; // Lo muevo al siguiente y devuelvo True
        return true;
    }
    return false; // El siguiente era Nullptr por lo que devuelvo False
}

template <typename T>
bool List<T>::ListIter::backward() {
    if(curr != nullptr && curr->prev != nullptr){ // Igual que el anterior pero esta vez con Prev
        curr = curr->prev; // Actualizo curr
        return true;
    }
    return false;
}

template <typename T>
const T&  List<T>::ListIter::peek_current() const {
    return curr->value; // Devuelvo el valor del actual
}

template <typename T>
bool List<T>::ListIter::at_last() const {
    if(curr != nullptr && curr->next == nullptr) return true; // Me fijo curr no null y si su sig es null estamos en la tail sino no
    return false;
}

template <typename T>
bool List<T>::ListIter::at_first() const {
    if(curr != nullptr && curr->prev == nullptr) return true; // Me fijo curr no null y si su prev es null estamos en el head sino no
    return false;
}

template <typename T>
bool List<T>::ListIter::insert_after(const T&value) {
    Node *nuevo = new Node(value);
    if(list->size == 0){ // Caso de lista vacia 
        nuevo->next = nullptr; //sig nullptr
        nuevo->prev = nullptr; //prev nullptr

        list->head = nuevo; // nuevo head = nuevo
        list->tail = nuevo; // nuevo tail = nuevo
        curr = nuevo; // act = nuevo

        list->size++; // agrando el size
        return true;
    }
    else{ // caso lista no vacia
        Node *next = curr->next; // guardo puntero al siguiente al curr
        curr->next = nuevo; // siguiente curr = nuevo
        nuevo->prev = curr; // el prev de nuevo es curr
        nuevo->next = next; // el sig de nuevo es el que era el next del curr originalmente
        if(next != nullptr){
            next->prev = nuevo; // si el sig de curr no es null su previo es el agregado
        }
        else{
            list->tail = nuevo; // sig de curr null entonces el nuevo es el tail
        }
        list->size++; // agrego 1 al tamaño de la lista
    }
    return true;
}

template <typename T>
bool List<T>::ListIter::insert_before(const T&value) {
    Node *nuevo = new Node(value);
    if(list->size == 0){ // Caso de lista vacia 
        nuevo->next = nullptr; //sig nullptr
        nuevo->prev = nullptr; //prev nullptr

        list->head = nuevo; // nuevo head = nuevo
        list->tail = nuevo; // nuevo tail = nuevo
        curr = nuevo; // act = nuevo

        list->size++; // agrando el size
        return true;
    }
    else{ // caso lista no vacia
        Node *prev = curr->prev; // gurado puntero al prev al curr
        curr->prev = nuevo; // prev curr = nuevo
        nuevo->next = curr; // el sig de nuevo es curr
        nuevo->prev = prev; // el prev de nuevo es el que era el prev del curr originalmente
        if(prev != nullptr){
            prev->next = nuevo; // si el prev de curr no es null su next es el agregado
        }
        else{
            list->head = nuevo; // prev de curr null entonces el nuevo es el head
        }
        list->size++; // agrego 1 al tamaño de la lista
    }
    return true;
}

template <typename T>
T List<T>::ListIter::remove() {
    Node *sig = curr->next; // Guardo siguiente de Curr
    Node *prev = curr->prev; // Guardo previo de Curr
    T value = curr->value; // Copio el Value del Curr
    if(prev != nullptr){ // Caso previo no null
        prev->next = sig; // prev sig = sig
    }
    else{
        list->head = sig; // si prev es null entonces el head es el sig
    }
    if(sig != nullptr){ // Caso sig no null
        sig->prev = prev; // sig prev es prev
    }
    else{
        list->tail = prev; // si sig es null entonces tail es prev
    }

    if(sig!= nullptr){
        curr = sig; // si sig no es null muevo el curr a sig
    }
    else{
        curr = prev; // al ser sig nulo muevo el curr a prev
    }

    list->size--; // resto uno al size
    delete curr; // borro el curr
    return value; // retorno el value del curr
}

#endif // TP2_H

