#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>

using namespace std;

// Funcion que elimina tildes de texto
string eliminarTildes(string texto) {
    for (size_t i = 0; i < texto.length(); i++) {
        if (texto[i] == 'á' || texto[i] == 'Á') texto[i] = 'a';
        else if (texto[i] == 'é' || texto[i] == 'É') texto[i] = 'e';
        else if (texto[i] == 'í' || texto[i] == 'Í') texto[i] = 'i';
        else if (texto[i] == 'ó' || texto[i] == 'Ó') texto[i] = 'o';
        else if (texto[i] == 'ú' || texto[i] == 'Ú') texto[i] = 'u';
    }
    return texto;
}

class Respuesta {
private:
    string texto;

public:
    Respuesta(string texto) : texto(eliminarTildes(move(texto))) {}
    string getTexto() const { return texto; }

    // Destructor
    ~Respuesta() {}
};

class ItemPregunta {
private:
    string enunciado;
    vector<Respuesta> respuestas;

public:
    ItemPregunta(string enunciado) : enunciado(eliminarTildes(move(enunciado))) {}

    void agregarRespuesta(const string& texto) { respuestas.emplace_back(eliminarTildes(texto)); }

    void mostrar() const {
        cout << "Pregunta: " << enunciado << endl;
        for (size_t i = 0; i < respuestas.size(); ++i) {
            cout << "  - Respuesta " << i + 1 << ": " << respuestas[i].getTexto() << endl;
        }
    }

    string getEnunciado() const { return enunciado; }

    vector<Respuesta>& getRespuestas() { return respuestas; }
    const vector<Respuesta>& getRespuestas() const { return respuestas; }

    void setEnunciado(const string& nuevoEnunciado) { enunciado = eliminarTildes(nuevoEnunciado); }

    ~ItemPregunta() {
        respuestas.clear();
    }
};


class Item {
private:
    string tipo;
    string taxonomia;
    vector<ItemPregunta> preguntas;

public:
    Item(string tipo, string taxonomia)
        : tipo(eliminarTildes(move(tipo))), taxonomia(eliminarTildes(move(taxonomia))) {}

    void agregarPregunta(const ItemPregunta& pregunta) { preguntas.push_back(pregunta); }

    void eliminarPregunta(int indice) {
        if (indice >= 0 && indice < preguntas.size()) {
            preguntas.erase(preguntas.begin() + indice);
            cout << "Pregunta eliminada exitosamente.\n";
        }
    }

    // 🔧 Este método ahora retorna el tiempo en minutos
    int mostrar(int numeroItem) const {
        cout << "Item(" << numeroItem << ") " << tipo << ", " << taxonomia << "\n";
        int minutos = 0;
        int tiempoPregunta = (tipo == "Verdadero/Falso") ? 4 : 7;

        for (size_t i = 0; i < preguntas.size(); ++i) {
            cout << "Pregunta: " << preguntas[i].getEnunciado() << " [" << tiempoPregunta << "min]" << endl;

            if (tipo != "Verdadero/Falso") {
                const vector<Respuesta>& respuestas = preguntas[i].getRespuestas();
                for (size_t j = 0; j < respuestas.size(); ++j) {
                    cout << "  - Respuesta " << j + 1 << ": " << respuestas[j].getTexto() << endl;
                }
            }

            minutos += tiempoPregunta;
        }

        return minutos;
    }

    string getTaxonomia() const { return taxonomia; }
    string getTipo() const { return tipo; }
    const vector<ItemPregunta>& getPreguntas() const { return preguntas; }

    ~Item() {
        preguntas.clear();
    }
};


class Profesor {
private:
    string nombre;

public:
    Profesor(string nombre) : nombre(eliminarTildes(move(nombre))) {}

    string getNombre() const { return nombre; }

    // Destructor
    ~Profesor() {}
};

class Materia {
private:
    string nombre;
    float ponderacion;

public:
    Materia(string nombre, float ponderacion)
        : nombre(eliminarTildes(move(nombre))), ponderacion(ponderacion) {}

    string getNombre() const { return nombre; }
    float getPonderacion() const { return ponderacion; }
    void setPonderacion(float nuevaPonderacion) { ponderacion = nuevaPonderacion; }

    // Destructor
    ~Materia() {}
};

class Prueba {
private:
    string titulo;
    string fechaCreacion;
    Profesor profesor;
    Materia materia;
    vector<Item> items;

    string obtenerFechaActual() {
        time_t t = time(nullptr);
        tm* fechaLocal = localtime(&t);

        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", fechaLocal);

        return string(buffer);
    }

public:
    Prueba(string titulo, const Profesor& profesor, const Materia& materia)
        : titulo(eliminarTildes(move(titulo))), profesor(profesor), materia(materia), fechaCreacion(obtenerFechaActual()) {}

    void agregarItem(const Item& item) { items.push_back(item); }

    const vector<Item>& getItems() const { return items; }
    vector<Item>& getEditableItems() { return items; }
    const string& getTitulo() const { return titulo; }

    void mostrarPrueba() const {
        static int conteoSolemne = 1;
        cout << "Solemne " << conteoSolemne++ << endl;
        cout << "Profesor(a): " << profesor.getNombre() << endl;
        cout << materia.getNombre() << endl;
        cout << "Ponderacion: " << materia.getPonderacion() << "%" << endl;
        cout << "Fecha de creacion: " << fechaCreacion << "\n" << endl;

        int tiempoTotal = 0;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << endl;
            tiempoTotal += items[i].mostrar(i + 1);
        }

        cout << "\nTiempo de Resolucion: " << tiempoTotal << " minutos" << endl;
    }


    // Destructor
    ~Prueba() {
        items.clear(); // Limpia los items al liberar la prueba
    }
};

class Menu {
private:
    vector<Prueba> pruebas;

    int obtenerEntradaValida(int min, int max) const {
        int opcion;
        while (true) {
            cin >> opcion;
            if (cin.fail() || opcion < min || opcion > max) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Ingrese un numero valido: ";
            } else {
                return opcion;
            }
        }
    }

    void pausaMenu() const {
        cout << "\nPresione ENTER para regresar al menu principal...";
        cin.ignore();
        cin.get();
    }

public:
    void mostrarMenu() const {
        cout << "\n===== Menu Principal =====\n";
        cout << "1. Crear Evaluacion.\n";
        cout << "2. Actualizar Item.\n";
        cout << "3. Borrar Items.\n";
        cout << "4. Informacion de los Items.\n";
        cout << "5. Buscar segun Taxonomia.\n";
        cout << "6. Salir\n";
        cout << "===Seleccione una opcion===\n";
    }

    void lanzar() {
        int opcion;
        do {
            mostrarMenu();
            opcion = obtenerEntradaValida(1, 6);
            switch (opcion) {
                case 1: crearPrueba(); break;
                case 2: actualizarItem(); break;
                case 3: borrarItem(); break;
                case 4: mostrarPruebas(); break;
                case 5: consultarItemsPorTaxonomia(); break;
                case 6: cout << "Saliendo... ¡Adios!\n"; break;
            }
        } while (opcion != 6);
    }

    void crearPrueba() {
    string tituloPrueba;

    // Validar el título de la prueba
    do {
        // Limpiar buffer antes de usar getline para evitar conflictos
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "=== Ingrese el titulo del Examen ===\n";
        getline(cin, tituloPrueba);

        // Verificar si el título está vacío o contiene solo espacios
        if (tituloPrueba.empty() || all_of(tituloPrueba.begin(), tituloPrueba.end(), ::isspace)) {
            cout << "Error: El titulo no puede estar vacio ni contener solo espacios. Intente nuevamente.\n";
            tituloPrueba.clear(); // Asegurar que el título se limpie para evitar valores inválidos
        }
    } while (tituloPrueba.empty());

    // Validar el nombre del profesor
    string nombreProfesor;
    do {
        cout << "=== Ingrese el nombre del profesor(a) ===\n";
        getline(cin, nombreProfesor);

        if (nombreProfesor.empty() || all_of(nombreProfesor.begin(), nombreProfesor.end(), ::isspace)) {
            cout << "Error: El nombre del profesor no puede estar vacio ni contener solo espacios. Intente nuevamente.\n";
            nombreProfesor.clear(); // Limpiar entrada inválida
        }
    } while (nombreProfesor.empty());

    Profesor profesor(nombreProfesor); // Crear objeto Profesor con validación correcta

    // Validar el nombre de la materia
    string nombreMateria;
    do {
        cout << "=== Cual Materia Pertenece===\n";
        getline(cin, nombreMateria);

        if (nombreMateria.empty() || all_of(nombreMateria.begin(), nombreMateria.end(), ::isspace)) {
            cout << "Error: El nombre de la materia no puede estar vacio ni contener solo espacios. Intente nuevamente.\n";
            nombreMateria.clear(); // Limpiar entrada inválida
        }
    } while (nombreMateria.empty());

    // Validar la ponderación como un número flotante positivo
    float ponderacion;
    do {
        cout << "=== Ingrese la ponderacion de la materia===\n";
        while (!(cin >> ponderacion) || ponderacion <= 0.0f) {
            cout << "Error: Ingrese un numero valido mayor a 0.\n";
            cin.clear(); // Limpiar el estado de error del flujo
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorar la entrada incorrecta
        }
    } while (ponderacion <= 0.0f);
    cin.ignore(); // Consumir el salto de línea tras validar la entrada numérica

    Materia materia(nombreMateria, ponderacion); // Crear objeto Materia con valores válidos
    Prueba nuevaPrueba(tituloPrueba, profesor, materia);

    // Ciclo para agregar items
    while (true) {
        cout << "\n=== Nuevo Item ===\n";
        cout << "1) Verdadero/Falso\n";
        cout << "2) Opcion Multiple\n";
        cout << "3) Salir\n";
        cout << "===Seleccione que tipo de pregunta desea agregar al item.===\n";

        int tipoItem = obtenerEntradaValida(1, 3);
        if (tipoItem == 3) {
            pruebas.push_back(nuevaPrueba);
            cout << "Saliendo de la creacion de la prueba...\n";
            return;
        }

        string tipo = (tipoItem == 1) ? "Verdadero/Falso" : "Opcion Multiple";

        // Validar taxonomia
        cout << "========================================\n";
        cout << "1) Crear\n2) Evaluar\n3) Analizar\n4) Aplicar\n5) Entender\n6) Recordar\n";
        cout << "===Ingrese la taxonomia del item.===\n";
        int taxonomiaItem = obtenerEntradaValida(1, 6);
        string taxonomia;
        switch (taxonomiaItem) {
            case 1: taxonomia = "crear"; break;
            case 2: taxonomia = "evaluar"; break;
            case 3: taxonomia = "analizar"; break;
            case 4: taxonomia = "aplicar"; break;
            case 5: taxonomia = "entender"; break;
            case 6: taxonomia = "recordar"; break;
        }

        Item nuevoItem(tipo, taxonomia);

        // Agregar preguntas al item
        cout << "=== Cuantas preguntas desea agregar al item [Max 50]===\n";
        int numPreguntas = obtenerEntradaValida(1, 50);

        cin.ignore(); // Limpiar el búfer después de leer numPreguntas

        for (int i = 0; i < numPreguntas; ++i) {
            string enunciadoPregunta;
            do {
                cout << "Ingrese la pregunta #" << i + 1 << ": ";
                getline(cin, enunciadoPregunta);

                if (enunciadoPregunta.empty() || all_of(enunciadoPregunta.begin(), enunciadoPregunta.end(), ::isspace)) {
                    cout << "Error: La pregunta no puede estar vacía ni solo contener espacios. Intente nuevamente.\n";
                    cin.clear();
                }
            } while (enunciadoPregunta.empty());

            ItemPregunta nuevaPregunta(enunciadoPregunta);
            // Si la pregunta es de "Opción Múltiple", agregar respuestas
            if (tipo == "Opcion Multiple") {
                const int respuestas = 4;
                for (int j = 0; j < respuestas; ++j) {
                    string respuestaTexto;
                    do {
                        cout << "Respuesta #" << j + 1 << ": ";
                        getline(cin, respuestaTexto);

                        if (respuestaTexto.empty() || all_of(respuestaTexto.begin(), respuestaTexto.end(), ::isspace)) {
                            cout << "Error: La respuesta no puede estar vacía ni solo contener espacios. Intente nuevamente.\n";
                            respuestaTexto.clear();
                        }
                    } while (respuestaTexto.empty());
                    nuevaPregunta.agregarRespuesta(respuestaTexto);
                }
            } else {
                nuevaPregunta.agregarRespuesta("Verdadero");
                nuevaPregunta.agregarRespuesta("Falso");
            }

            nuevoItem.agregarPregunta(nuevaPregunta);
        }

        nuevaPrueba.agregarItem(nuevoItem);

        // Preguntar si desea agregar mas items
        cout << "Desea agregar otro item?\n1) Si\n2) No\n";
        int continuar = obtenerEntradaValida(1, 2);
        if (continuar == 2) {
            pruebas.push_back(nuevaPrueba);
            cout << "Prueba creada y guardada exitosamente.\n";
            return;
        }
    }
}

    void mostrarPruebas() const {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
        } else {
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << "\nExamen" << i + 1 << ":\n";
                pruebas[i].mostrarPrueba();
            }
        }
        pausaMenu();
    }

    void consultarItemsPorTaxonomia() const {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
            pausaMenu();
            return;
        }

        // Lista de taxonomias disponibles
        vector<string> taxonomiasDisponibles = {"crear", "evaluar", "analizar", "aplicar", "entender", "recordar"};

        cout << "\n===== Buscar segun Taxonomia =====\n";
        for (size_t i = 0; i < taxonomiasDisponibles.size(); ++i) {
            cout << i + 1 << ") " << taxonomiasDisponibles[i] << endl;
        }
        cout << taxonomiasDisponibles.size() + 1 << ") Salir\n";

        int opcion = obtenerEntradaValida(1, taxonomiasDisponibles.size() + 1);
        if (opcion == taxonomiasDisponibles.size() + 1) {
            cout << "Saliendo...\n";
            return;
        }

        // Obtener la taxonomia seleccionada
        string taxonomiaSeleccionada = taxonomiasDisponibles[opcion - 1];
        cout << "\n===" << taxonomiaSeleccionada << "===\n";

        // Buscar preguntas agrupadas por taxonomia
        bool hayResultados = false;
        for (size_t i = 0; i < pruebas.size(); ++i) {
            const auto& items = pruebas[i].getItems();

            vector<ItemPregunta> preguntasFiltradas;

            for (const auto& item : items) {
                if (item.getTaxonomia() == taxonomiaSeleccionada) {
                    hayResultados = true;
                    cout << "" << i + 1 << " " << pruebas[i].getTitulo() << ":\n";
                    for (const auto& pregunta : item.getPreguntas()) {
                        cout << "  - " << pregunta.getEnunciado() << endl;
                    }
                }
            }
        }

        if (!hayResultados) {
            cout << "No se encontraron preguntas con la taxonomia seleccionada.\n";
        }

        pausaMenu();
    }

    void borrarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
            return;
        }

        while (true) {
            cout << "Seleccione la prueba donde desea borrar un item:\n";
            for (size_t i = 0; i < pruebas.size(); ++i) {
                cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
            }

            int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
            auto& pruebaSeleccionada = pruebas[indicePrueba];
            auto& items = pruebaSeleccionada.getEditableItems();

            if (items.empty()) {
                cout << "La prueba seleccionada no tiene items.\n";
                break;
            }

            cout << "Prueba seleccionada: " << pruebaSeleccionada.getTitulo() << "\nItems disponibles:\n";
            for (size_t i = 0; i < items.size(); ++i) {
                cout << "  Item #" << i + 1 << " - Taxonomia: " << items[i].getTaxonomia() << endl;
            }

            cout << "Ingrese el numero del item que desea borrar: ";
            int numeroItem = obtenerEntradaValida(1, items.size());
            cout << "¿Esta seguro que quiere eliminar este item?\n1) Si\n2) No\n";
            int confirmacion = obtenerEntradaValida(1, 2);

            if (confirmacion == 1) {
                items.erase(items.begin() + (numeroItem - 1));
                cout << "Item eliminado exitosamente.\n";

                if (items.empty()) {
                    cout << "La prueba \"" << pruebaSeleccionada.getTitulo() << "\" ya no tiene items. Se eliminara automaticamente.\n";
                    pruebas.erase(pruebas.begin() + indicePrueba);
                    break;
                }
            } else {
                cout << "Accion cancelada. El item no se elimino.\n";
                continue;
            }

            cout << "¿Desea hacer algo mas?\n";
            cout << "1) Borrar otro item\n";
            cout << "2) Seleccionar otra prueba\n";
            cout << "3) Regresar al menu principal\n";
            int opcion = obtenerEntradaValida(1, 3);

            if (opcion == 1) continue;
            if (opcion == 2) return;
            break;
        }
        pausaMenu();
    }

    void actualizarItem() {
        if (pruebas.empty()) {
            cout << "No se han creado pruebas aun.\n";
            return;
        }

        cout << "Seleccione la prueba donde desea actualizar un item:\n";
        for (size_t i = 0; i < pruebas.size(); ++i) {
            cout << i + 1 << ") " << pruebas[i].getTitulo() << endl;
        }

        int indicePrueba = obtenerEntradaValida(1, pruebas.size()) - 1;
        auto& pruebaSeleccionada = pruebas[indicePrueba];

        auto& items = pruebaSeleccionada.getEditableItems();
        if (items.empty()) {
            cout << "La prueba seleccionada no tiene items.\n";
            pausaMenu();
            return;
        }

        cout << "Seleccione el item que desea actualizar:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ") Item #" << i + 1 << " - Tipo: " << items[i].getTipo() << endl;
        }

        int indiceItem = obtenerEntradaValida(1, items.size()) - 1;
        auto& itemSeleccionado = items[indiceItem];

        cout << "\nSeleccione una accion:\n";
        cout << "1) Agregar Pregunta\n";
        cout << "2) Salir\n";
        int accion = obtenerEntradaValida(1, 2);

        switch (accion) {
            case 1: {
                cout << "Ingrese el enunciado de la nueva pregunta: ";
                cin.ignore();
                string preguntaTexto;
                getline(cin, preguntaTexto);

                ItemPregunta nuevaPregunta(preguntaTexto);

                if (itemSeleccionado.getTipo() == "Verdadero/Falso") {
                    nuevaPregunta.agregarRespuesta("Verdadero");
                    nuevaPregunta.agregarRespuesta("Falso");
                    cout << "Respuestas 'Verdadero' y 'Falso' añadidas automaticamente.\n";
                } else if (itemSeleccionado.getTipo() == "Opcion Multiple") {
                    cout << "¿Cuantas respuestas desea agregar?: ";
                    int numRespuestas = obtenerEntradaValida(1, 10);

                    for (int i = 0; i < numRespuestas; ++i) {
                        cout << "Ingrese la respuesta #" << i + 1 << ": ";
                        string respuestaTexto;
                        cin.ignore();
                        getline(cin, respuestaTexto);
                        nuevaPregunta.agregarRespuesta(respuestaTexto);
                    }
                } else {
                    cout << "Tipo de item desconocido. No se pudo agregar la pregunta.\n";
                    break;
                }

                itemSeleccionado.agregarPregunta(nuevaPregunta);
                cout << "Pregunta agregada exitosamente.\n";
                break;
            }
            case 2:
                cout << "Saliendo de la actualizacion...\n";
                return;
        }
        pausaMenu();
    }

    // Destructor
    ~Menu() {
        pruebas.clear(); // Libera la memoria asociada a las pruebas
    }
};

int main() {
    Menu menu;
    menu.lanzar();
    return 0;
}