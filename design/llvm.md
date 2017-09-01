Interpretação, LLVM, JIT, compilação
====================================
O uso da infraestrutura LLVM para compiladores possibilita ao _pq_ várias
propriedades interessantes:

- Geração de código de máquina, maior velocidade de execução, maior gama de
  ferramentas auxiliares já existentes
- Total interoperabilidade com outras linguagens que também usem o LLVM através
  de sua representação intermediária, em especial C (maiores testes)
- Uso de funções já compiladas em bibliotecas compartilhadas (.so, .dll, .dylib),
  possibilitando um incrível reúso de ferramental já pronto


Claro que há alguns poréns também:

- Sistema de tipos estático do LLVM dificulta algumas coisas da dinamicidade
  usual dos Lisps, que será resolvido com a interpretação/JIT compilação do
  código
- Certa dificuldade na implementação, além de estar sujeito a mudanças no LLVM
  em si


Imagino que os pontos bons compensem, então bora lá!


Interpretação
-------------
Ao rodar o comando `pq` em um código, o comportamento padrão é de
interpretar/JIT compilar e executar o código. Desse modo, é possível criar
_scripts_ em _pq_, ou mesmo testes de unidade em bibliotecas.

Interpretar o código possibilita fazer as coisas mais dinâmicas, sem anotação
de tipos e sem preocupação com criar uma função _main_ como ponto de entrada.
Todos os comandos são executados, mesmo que estejam fora de funções: o que não
é possível em C, por exemplo. Ao compilar o código, somente as declarações
de variáveis globais, tipo e funções têm seu código gerado.


Compilação
----------
Compilar módulos em código intermediário LLVM (IR, arquivos .ll ou .bc) é muito
interessante, possibilitando códigos em PQ serem usados em outras linguagens,
como C/C++ por exemplo.

Mas ali atrás falou que ia interpretar pra ser dinâmico, então vai compilar o
quê? A resposta pra isso é simples: o que quer que não precise de dinamicidade,
ou que puder ser instanciado estaticamente, vira código LLVM IR. Sendo assim,
passagens completamente dinâmicas são ignoradas, talvez soltando um _warning_
que não estará no código final.

Itens dinâmicos contemplam, entre outros:

- Funções sem anotação de tipo gerais, porém instâncias dela podem ser
  compiladas para tipos específicos a la _template_ do C++
- Código como listas em runtime, função _eval_ e afins
- Macros, que avaliadas em tempo de compilação tá resolvido problema
- Funções que usam informações da interpretação, como info sobre os tipos,
  escopos, pacotes, funções definidas em Lua
