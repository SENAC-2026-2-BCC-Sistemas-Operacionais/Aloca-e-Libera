#include<stdio.h>

typedef struct List List;
struct List{
    int size;
    int i;
    int buffer[16000];
};

typedef struct Mem Mem;
struct Mem{
    List owners;
    char buffer[16000];
};



Mem createMem(){
    Mem newMem;
    newMem.owners.size = 0;
    newMem.owners.i = 0;
    return newMem;
}

Mem mem;

void* customAlloc(int size);
void* customAlloc(int size){
    if (mem.owners.i + size > 16000) {
        return NULL; // Out of memory
    }

    void* allocated_ptr = &mem.buffer[mem.owners.i];

    mem.owners.buffer[mem.owners.size] = mem.owners.i;
    mem.owners.size++;

    mem.owners.i += size;
    return allocated_ptr;
}

void customFree(void* ptr);
void customFree(void* ptr){
    if (ptr == NULL) return;

    int offset = (char*)ptr - mem.buffer;

    if (offset < 0 || offset >= mem.owners.i) {
        printf("Error: Invalid pointer passed to free.\n");
        return;
    }

    int found_idx = -1;
    for (int j = 0; j < mem.owners.size; j++) {
        if (mem.owners.buffer[j] == offset) {
            found_idx = j;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Error: Pointer was not allocated by our malloc.\n");
        return;
    }

    if (found_idx == mem.owners.size - 1) {
        mem.owners.i = mem.owners.buffer[found_idx]; // Roll back the bump pointer
    }

    for (int j = found_idx; j < mem.owners.size - 1; j++) {
        mem.owners.buffer[j] = mem.owners.buffer[j + 1];
    }
    mem.owners.size--; // Decrement allocation count
}

int main(){
    mem = createMem();

    int* num1 = (int*)customAlloc(sizeof(int));
    *num1 = 42;
    printf("Allocated num1 at offset %d\n", (char*)num1 - mem.buffer);

    double* arr = (double*)customAlloc(10 * sizeof(double));
    arr[0] = 3.14;
    printf("Allocated arr at offset %d\n", (char*)arr - mem.buffer);

    printf("Total allocations before free: %d\n", mem.owners.size);
    printf("Total bytes used before free: %d\n\n", mem.owners.i);

    printf("Freeing arr...\n");
    customFree(arr);

    printf("Total allocations after freeing arr: %d\n", mem.owners.size);
    printf("Total bytes used after freeing arr: %d\n\n", mem.owners.i);

    printf("Freeing num1...\n");
    customFree(num1);

    printf("Total allocations after freeing num1: %d\n", mem.owners.size);
    printf("Total bytes used after freeing num1: %d\n", mem.owners.i); // i remains the same

    return 0;
}
