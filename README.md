# Aloca e Libera

## Instruções

1. Crie uma pasta com seu nome completo, separado por hífens, todas as letras em minúsculas

    Exemplo: eduardo-heredia
             maria-eduarda-silva

2. Dentro desta pasta, crie um arquivo main.c com o seguinte conteúdo

  ```c
    #include <stdio.h>
    int main() {}
  ```

3. Faça o commit de sua contribuição. Seu commit deve ser ASSINADO.

4. Submeta um Pull Request 

5. No arquivo main.c
    - Crie uma lista duplamente encadeada usando alocação dinâmica em C
    - Submeta um Pull Request. Seu commit deve ser ASSINADO.

6. Crie as funções aloca() e libera()
    - Crie uma função aloca com a mesma interface de malloc
    - Crie uma função libera com a mesma interface de free
    - libera e free deverão alocar/desalocar memória de um buffer global de 16KB
    - Atualize sua lista encadeada para usar aloca e libera no lugar de malloc e free
      (óbvio que não poderá usar malloc e free dentro de aloca e libera)
