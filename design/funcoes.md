Tipos de funções
================
_Pq_ é uma linguagem funcional, e há várias formas que essas funções podem
tomar. Os seguintes tipos de função podem ser usados em _pq_:


Funções do PQ
-------------
Funções definidas em código _pq_, com tipagem estática opcional.
São compiladas usando llvm, e estarão no código objeto final. Se não houver
anotação de tipo, a função será instanciada para cada tipo que precisar, a la
_C++_ templates. Podem receber argumentos variáveis por `va_args`.


Macros do PQ
------------
Como um bom _Lisp_, _pq_ terá macros, que são funções que recebem símbolos e
constroem novas funções retornando uma lista de símbolos. São executadas em
tempo de compilação, e podem utilizar quaisquer funções ou macros que precisar
para isso. Podem receber argumentos variáveis em uma lista dinâmica.


Tipos do PQ
-----------
Tipos em _pq_ podem ser usados como função para converter um valor para esse
tipo. É útil para inicialização de _structs_, para que estas não
sejam confundidas com uma tupla com os mesmos campos, por exemplo.


Funções nativas (_llvm_)
------------------------
Com o uso do _llvm_, é possível usar funções nativas já compiladas, ou funções
definidas em módulos _llvm_, que podem ser geradas de outras linguagens de
programação, como _C_, _C++_, _Rust_, etc.

Para funções nativas, é necessária a tipagem estática, declarada pelo
programador. É necessária linkagem com a biblioteca utilizada.

Para funções do _llvm_, a tipagem estática já é garantida, e o código fonte
automaticamente incluído.


Macros _llvm_
-------------
Realizam geração de código _llvm_ interno à função que a chama, utilizadas para
construções básicas como `if` e `while`, por exemplo. São funções _Lua_ que
recebem um `LLVMBuilder` já posicionado corretamente e a lista de argumentos
passados, construindo o código necessário.

Note que isso torna _pq_ uma linguagem independente de palavras reservadas,
simplificando a gramática e possibilitando uma infinidade de construções
interessantes através da metaprogramação.


Funções _Lua_
-------------
Como o interpretador/compilador é em _Lua_, código pq pode chamar funções
implementadas em _Lua_, desde que no modo interpretado. Para chamar funções
_Lua_ em código compilado _pq_, utilize a API própria.
