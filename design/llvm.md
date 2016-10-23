LLVM e JIT
==========
A infraestrutura LLVM para compiladores possibilita ao PQ várias propriedades
interessantes:

- Geração de código de máquina, maior velocidade de execução, maior gama de
  ferramentas auxiliares já existentes
- Total interoperabilidade com outras linguagens que também usem o LLVM através
  de sua representação intermediária, em especial C/C++ (maiores testes)
- Uso de funções já compiladas em bibliotecas compartilhadas (.so, .dll, .dylib),
  possibilitando um incrível reúso de ferramental já pronto


Claro que há alguns poréns também:

- Sistema de tipos estático do LLVM dificulta algumas coisas como a dinamicidade
  usual dos Lisps, que será resolvido com a interpretação do código
- Certa dificuldade na implementação, além de estar sujeito a mudanças no LLVM
  em si


Imagino que os pontos bons compensem, então bora lá!
