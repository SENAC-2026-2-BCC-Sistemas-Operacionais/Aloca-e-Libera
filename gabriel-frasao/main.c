#include "lista-circular-duplamente-ligada.c"

int main() {
  Node *list = list_new(1);

  list_insert(&list, 2);
  list_insert(&list, 3);
  list_insert(&list, 4);
  list_insert(&list, 5);
  list_insert(&list, 6);
  list_insert(&list, 7);

  list_print(list);

  list_remove(&list, 3);
  list_print(list);

  Node *node = list_search(list, 7);
  if (node == NULL) {
    printf("Search for 7 returned NULL\n");
  } else {
    printf("Search for 7 returned %d\n", node->value);
  }

  node = list_search(list, 3);
  if (node == NULL) {
    printf("Search for 3 returned NULL\n");
  } else {
    printf("Search for 3 returned %d\n", node->value);
  }

  list_free(list);
}
