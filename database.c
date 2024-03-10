#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definizioni delle strutture di dati
struct Persona {
    char name[20];
    char surname[50];
    char address[100];
    int age;
};

typedef struct IndexNodeString {
    char *value;
    struct Persona *record;
    struct IndexNodeString *left;
    struct IndexNodeString *right;
} IndexNodeString;

typedef struct IndexNodeInt {
    int value;
    struct Persona *record;
    struct IndexNodeInt *left;
    struct IndexNodeInt *right;
} IndexNodeInt;

typedef struct {
    IndexNodeString *name;
    IndexNodeString *surname;
    IndexNodeString *address;
    IndexNodeInt *age;
} Database;

// Prototipi delle funzioni
void insertStringIndexNode(IndexNodeString **root, char *value, struct Persona *record);
void insertIntIndexNode(IndexNodeInt **root, int value, struct Persona *record);
struct Persona *findStringIndexNode(IndexNodeString *root, char *value);
struct Persona *findIntIndexNode(IndexNodeInt *root, int value);
void freeStringIndexTree(IndexNodeString *root);
void freeIntIndexTree(IndexNodeInt *root);
void freeDatabase(Database *database);

// Implementazione delle funzioni

// Inserisce una stringa in un nodo di indice
void insertStringIndexNode(IndexNodeString **root, char *value, struct Persona *record) {
    if (*root == NULL) {
        *root = (IndexNodeString *)malloc(sizeof(IndexNodeString));
        (*root)->value = strdup(value);
        (*root)->record = record;
        (*root)->left = (*root)->right = NULL;
    } else {
        int cmp = strcmp(value, (*root)->value);
        if (cmp < 0) {
            insertStringIndexNode(&((*root)->left), value, record);
        } else if (cmp > 0) {
            insertStringIndexNode(&((*root)->right), value, record);
        }
    }
}

// Inserisce un intero in un nodo di indice
void insertIntIndexNode(IndexNodeInt **root, int value, struct Persona *record) {
    if (*root == NULL) {
        *root = (IndexNodeInt *)malloc(sizeof(IndexNodeInt));
        (*root)->value = value;
        (*root)->record = record;
        (*root)->left = (*root)->right = NULL;
    } else {
        if (value < (*root)->value) {
            insertIntIndexNode(&((*root)->left), value, record);
        } else if (value > (*root)->value) {
            insertIntIndexNode(&((*root)->right), value, record);
        }
    }
}

// Trova una stringa in un nodo di indice
struct Persona *findStringIndexNode(IndexNodeString *root, char *value) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(value, root->value);
    if (cmp == 0) {
        // Trovato
        return root->record;
    } else if (cmp < 0) {
        return findStringIndexNode(root->left, value);
    } else {
        return findStringIndexNode(root->right, value);
    }
}

// Trova un intero in un nodo di indice
struct Persona *findIntIndexNode(IndexNodeInt *root, int value) {
    if (root == NULL) {
        return NULL;
    }

    if (value == root->value) {
        // Trovato
        return root->record;
    } else if (value < root->value) {
        return findIntIndexNode(root->left, value);
    } else {
        return findIntIndexNode(root->right, value);
    }
}

void insert(Database *database, struct Persona *persona) {
    insertStringIndexNode(&(database->name), persona->name, persona);
    insertStringIndexNode(&(database->surname), persona->surname, persona);
    insertStringIndexNode(&(database->address), persona->address, persona);
    insertIntIndexNode(&(database->age), persona->age, persona);
}

struct Persona *findByName(Database *database, char *name) {
    return findStringIndexNode(database->name, name);
}

// Funzione per liberare la memoria di un albero di indici di stringhe
void freeStringIndexTree(IndexNodeString *root) {
    if (root == NULL) {
        return;
    }

    freeStringIndexTree(root->left);
    freeStringIndexTree(root->right);
    free(root->value);
    free(root);
}

// Funzione per liberare la memoria di un albero di indici di interi
void freeIntIndexTree(IndexNodeInt *root) {
    if (root == NULL) {
        return;
    }

    freeIntIndexTree(root->left);
    freeIntIndexTree(root->right);
    free(root);
}

// Funzione per liberare la memoria del database e degli indici
void freeDatabase(Database *database) {
    freeStringIndexTree(database->name);
    freeStringIndexTree(database->surname);
    freeStringIndexTree(database->address);
    freeIntIndexTree(database->age);
}

// Funzione principale per testare il codice
int main() {
    // Esempio di utilizzo della libreria
    Database myDatabase;
    memset(&myDatabase, 0, sizeof(Database));  // Inizializza tutto a NULL

    // Creazione di un record
    struct Persona person;
    strcpy(person.name, "Sherlock");
    strcpy(person.surname, "Holmes");
    strcpy(person.address, "221B di Baker Street");
    person.age = 25;

    // Inserimento del record nel database
    insert(&myDatabase, &person);

    // Ricerca del record per nome
    struct Persona *foundPerson = findByName(&myDatabase, "Sherlock");

    // Verifica se il record è stato trovato
    if (foundPerson != NULL) {
        printf("Record trovato:\n");
        printf("Nome: %s\n", foundPerson->name);
        printf("Cognome: %s\n", foundPerson->surname);
        printf("Indirizzo: %s\n", foundPerson->address);
        printf("Età: %d\n", foundPerson->age);
    } else {
        printf("Record non trovato.\n");
    }

    // Libera la memoria allocata per il database
    freeDatabase(&myDatabase);

    return 0;
}
