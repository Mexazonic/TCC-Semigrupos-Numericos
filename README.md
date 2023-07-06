# Árvore de Semigrupos Numéricos - TCC

Este repositório contém o código-fonte e os arquivos relacionados ao algoritmo de exploração da árvore de semigrupos numéricos desenvolvido para o Trabalho de Conclusão de Curso (TCC) na Universidade de Brasília.

## Requisitos

Para executar o algoritmo, certifique-se de ter os seguintes requisitos instalados:

- Sistema Operacional: Ubuntu 16
- Cilk++

## Imagem Docker

Uma imagem Docker contendo o ambiente necessário para executar o algoritmo está disponível no Docker Hub. Você pode acessá-la através do seguinte link: [Imagem Docker](https://hub.docker.com/repository/docker/mexazonic/tcc2/general)

## Instruções de Execução

Siga as instruções abaixo para executar o algoritmo:

1. Execute o comando `make clean` para limpar o ambiente.
2. Para compilar o algoritmo, utilize o comando `make travessia_arvore MAX_GENUS=50`.
3. Para executar o algoritmo, utilize o comando `./travessia_arvore`.

## Informações do Autor e Orientador

- Aluno: Bruno Henrique Sousa Duarte
- Orientador: Prof. Dr. Matheus Bernardini de Souza
- Banca: Prof. Dr. Victor do Nascimento Martins, Prof. Dr. Edson Alves da Costa Júnior

**Observação:** Certifique-se de fornecer os valores adequados para os parâmetros MAX_GENUS ao compilar e executar o algoritmo, de acordo com sua necessidade.
