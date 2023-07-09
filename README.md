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

## Flags Utilizadas

1. **CXXFLAGS**:
   - `-std=c++11`: Especifica o padrão de linguagem C++ a ser usado (C++11 neste caso).
   - `-fcilkplus`: Ativa o suporte a Cilk Plus, uma extensão para programação paralela em C++.
   - `-g`: Inclui informações de depuração no binário gerado.
   - `-Wall`: Ativa avisos detalhados do compilador para possíveis problemas no código.
   - `-O3`: Habilita otimizações de compilação de alto nível, visando melhorar o desempenho.

2. **TARGET_ARCH**:
   - `-march=native`: Indica ao compilador que otimize o código para a arquitetura nativa da máquina em que está sendo compilado. Isso inclui instruções específicas do processador.
   - `-mtune=native`: Otimiza o código para a arquitetura nativa da máquina em que será executado, levando em consideração características de desempenho específicas do processador.

3. **LDLIBS**:
   - `-lcilkrts`: Especifica a biblioteca de tempo de execução do Cilk Plus (`cilkrts`) a ser vinculada ao binário.

Essas flags são usadas durante o processo de compilação e vinculação para definir as opções de compilação, otimização e vinculação do projeto. O objetivo é produzir um binário otimizado e compatível com o suporte a Cilk Plus, permitindo a execução paralela de código.

## Informações do Autor e Orientador

- Aluno: Bruno Henrique Sousa Duarte
- Orientador: Prof. Dr. Matheus Bernardini de Souza
- Banca: Prof. Dr. Victor do Nascimento Martins, Prof. Dr. Edson Alves da Costa Júnior

**Observação:** Certifique-se de fornecer os valores adequados para os parâmetros MAX_GENUS ao compilar e executar o algoritmo, de acordo com sua necessidade.

