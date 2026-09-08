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
}

template<typename T>
List<T>::~List() {
    // TODO: liberar todos los nodos que queden con delete.
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
        this->head = nuevo_nodo; // La cabeza de la lista pasa a ser el nuevo nodo.
        this->tail = nuevo_nodo; // La cola de la lista pasa a ser el nuevo nodo.
        this->size += 1;  // Agregamos 1 al tamaño de la lista luego de insertar el nuevo nodo.
    }

    else {  // Chequeamos el caso en cual la lista no esta vacía de antemano.
        nuevo_nodo->next = this->head;  // El puntero siguiente al nuevo nodo pasa a ser la cabeza del anterior nodo.
        this->head->prev = nuevo_nodo;  // El puntero previo al de la cabeza del nodo antiguo pasa a ser el nuevo nodo.
        this->head = nuevo_nodo;  // La "cabeza" de la lista pasa a ser el nuevo nodo incorporado.
        this->size += 1;  // Agregamos 1 al tamaño de la lista.
    }

    delete nuevo_nodo;
}

template<typename T>
void List<T>::insert_tail(const T& value) {

    // TODO: reservar un nodo con new, enlazarlo al final y
    // actualizar head/tail/size.

    Node* nuevo_nodo_final = new Node(value);  // Creamos un nuevo nodo que insertaremos al final de la lista.

    if (this->size == 0) {
        this->head = nuevo_nodo_final;
        this->tail = nuevo_nodo_final;
        this->size += 1;
    }

    else {
        this->tail->next = nuevo_nodo_final;
        nuevo_nodo_final->prev = this->tail;
        this->tail = nuevo_nodo_final;
        this->size += 1;
    }
}

template<typename T>
T List<T>::pop_head() {

    // TODO: sacar el primer nodo (con delete), devolver su valor.

    T valor_del_nodo_inicial = this->head->value;  // Guardamos el valor de la "cabeza" de la lista para poder devolverlo al final y no perderlo
                                                  //  al momento de eliminar su nodo correspondiente.

    delete this->head;  // Borramos el nodo (nodo de la "cabeza" antigua de la lista) y liberamos su memoria.

    this->head = this->head->next;  // Actualizamos el puntero de la "cabeza" de la lista para que apunte a su nodo siguiente.
    this->head->prev = nullptr;   // Actualizamos el puntero previo de la "cabeza" de la lista a "nullptr" 
                                 // (puntero vacío ya que es el anterior a la nueva "cabeza").
    
    return valor_del_nodo_inicial;  // Devolvemos el valor de la "cabeza" antigua de la lista.
}

template<typename T>
T List<T>::pop_tail() {

    // TODO: sacar el último nodo (con delete), devolver su valor.

    T valor_del_nodo_final = this->tail->value;  // Guardamos el valor de la "cola" de la lista para poder devolverlo luego y no perderlo al 
                                                //  momento de eliminar su nodo correspondiente.

    delete this->tail;  // Eliminamos el nodo (nodo de la "cola" antigua de la lista) y liberamos su memoria.

    this->tail = this->tail->prev;  // Actualizamos el puntero de la "cola" de la lista para que apunte a su nodo anterior.
    this->tail->next = nullptr;  // Actualizamos el puntero siguiente a la "cola" de la lista para que apunte a "nullptr".

    return valor_del_nodo_final;  // Devolvemos el valor de la "cola" antigua de la lista.
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
}

template <typename T>
typename List<T>::ListIter List<T>::create_tail() {
    // TODO: retornar un iterador parado en el final de la lista.
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
    // TODO: avanzar una posición si se puede.
    return false;
}

template <typename T>
bool List<T>::ListIter::backward() {
    // TODO: retroceder una posición si se puede.
    return false;
}

template <typename T>
const T&  List<T>::ListIter::peek_current() const {
    // TODO: devolver el valor actual
}

template <typename T>
bool List<T>::ListIter::at_last() const {
    // TODO: devolver si el iterador está en el último elemento.
    return false;
}

template <typename T>
bool List<T>::ListIter::at_first() const {
    // TODO: devolver si el iterador está en el primer elemento.
    return false;
}

template <typename T>
bool List<T>::ListIter::insert_after(const T&value) {
    // TODO: insertar un valor detrás del actual con new.
    return false;
}

template <typename T>
bool List<T>::ListIter::insert_before(const T&value) {
    // TODO: insertar un valor delante del actual con new.
    return false;
}

template <typename T>
T List<T>::ListIter::remove() {
    // TODO: sacar el nodo actual (con delete), reposicionar el iterador
    // y devolver el valor que tenía.
}

#endif // TP2_H

