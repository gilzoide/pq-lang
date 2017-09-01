Tipos
=====
Por usar o _llvm_, todos os tipos em _pq_ derivam dos seguintes tipos: Inteiros
(i1, i8, i16, i32, i64, talvez i128), pontos flutuantes (half, float, double,
talvez fp128), ponteiros (incluindo ponteiros para funções), vetores (_array_ e
_vector_) e structs.


Tipos númericos
---------------
Inteiros com sinal: i8, i16, i32, i64.

Inteiros sem sinal: u8, u16, u32, u64.

Inteiros com tamanho suficiente para representar o tamanho de qualquer
_container_, respectivamente com sinal e sem sinal: isize e usize.

Números de ponto flutuante: half, float, double.


Arrays
------
_Arrays_ são que nem em _C_: ponteiros que dão acesso direto à memória.
Diferente de _C_, porém, _arrays_ padrão em _pq_ mantêm informação sobre seu
tamanho. Sendo assim, um _array_ do tipo _T_ é definido como uma _struct_ com o
seu tamanho e um ponteiro para o tipo. O tamanho usado é do tipo `i32` ou `i64`
e depende da plataforma, o correspondente ao tipo `usize`em _pq_.

Definindo um _array_ de _T_:
- _pq_: `(array T)` ou `(T.array)`
- _llvm_: `{ i32, T* }` ou `{ i64, T* }`


Null Terminated Arrays
----------------------
Há um segundo tipo de _array_ padrão em _pq_: _null terminated arrays_ (vetores
terminados em valor nulo), abreviado para _nt-array_. Comum em C,
principalmente na representação de _strings_, esses vetores apresentam conteúdo
válido até encontrar o primeiro valor nulo (`zeroinitializer`). São
implementados com simples ponteiros, mas em _pq_ é considerado um tipo
completamente diferente (com razão: as funções).

Definindo um _nt-array_ de _T_:
- _pq_: `(nt-array T)` ou `(T.nt-array)`
- _llvm_: `T*`


Listas
------
Listas simplesmente encadeadas de elementos, muito usado em programação
funcional. Contêm nós com um elemento (cabeça, ou `head`) e a cauda, ou `tail`.
Como são definidas por _structs_ nomeadas, são sempre passadas por referência
para funções, sendo a lista vazia um ponteiro nulo.

Definindo uma lista de _T_:
- _pq_: `(list T)` ou `(T.list)`
- _llvm_: `%list.T = type { T, %list.T* }`


Tuplas
------
Tuplas são pacotes de valores ordenados, assim como em _Haskell_, _C++_ ou
_Rust_. São implementadas por _structs_ não nomeadas, e são passadas por valor
a funções quando imutáveis. São úteis para possibilitar múltiplo retorno em
funções. Em _pq_ uma struct de tipos representa um tipo tupla.

Representando uma tupla de dois números inteiros:
- _pq_: `{ i32, i32 }`
- _llvm_: `{ i32, i32 }`


Structs
-------
_Structs_ são usadas para definir pacotes de valores com nomes, com maior
refinamento. _Structs_ são sempre nomeadas, e seu nome é o tipo usado. São
sempre passadas por referência para funções, mesmo que imutáveis (_pq_ garante
que as funções que a usam só aceitam valores imutáveis). Todos os seus campos
podem ser nomeados com uma _keyword_, e uma função é criada como _getter_ para
cada campo, possibilitando linguagens como C acessarem os campos com
facilidade, já que o tipo em _llvm_ não possui essa informação.

Definindo uma _struct_ que expressa um ponto em um plano 2D, com coordenadas
usando ponto flutuante chamada "Ponto2D":
- _pq_: `(defstruct Ponto2D :x float :y float)`
- _llvm_: `%Ponto2D = type { float, float }`.


Estendendo structs
------------------
Outro conceito interessante sobre _structs_ é que essas podem ser estendidas
com novos campos, formando outro tipo. Quem curte programação orientada a
objetos sabe do que estou falando =]

Como _structs_ são sempre passadas por referência (ponteiros) para funções,
basta um `bitcast` e _llvm_ pode considerar uma variável do tipo estendido como
sendo do original, dando uma sensação de polimorfismo interessante (cuidado: _pq_
__NÃO USA__ polimorfismo de verdade, com _virtual/dispatch table_). Sendo
assim, __qualquer__ função definida para o tipo original pode ser usada no tipo
estendido, se esse não tiver definido uma implementação específica para tal.

Definindo o tipo "Ponto3D" que estende o tipo "Ponto2D" com mais uma coordenada:
- _pq_: `(defstruct Ponto3D :extends Ponto2D :z float)`
- _llvm_: `%Ponto3D = type { float, float, float }`


Funções
-------
Funções sendo tipos de primeira classe, têm tipo definido. Somente tipos de
funções com tipagem estática podem ser definidos. A ordem dos tipos segue a do
_Haskell_, onde o último tipo escrito é o tipo de retorno. Em _pq_ um _array_ de
tipos representa um tipo função.

Representando o tipo função que recebe dois números inteiros e retorna um
número de ponto flutuante:
- _pq_: `[i32 i32 double]`
- _llvm_: `double (i32, i32)`


Ponteiro
--------
Para interoperar com _C_, tem que existir o tipo "ponteiro", que representa um
ponteiro qualquer, o `void*` do _C_, ou `i8*` do _llvm_. O tipo `pointer` faz
esse papel. Ele é automaticamente convertido (usando `bitcast`) para um
ponteiro de qualquer tipo, e qualquer ponteiro é automaticamente convertido
para ele quando passado a funções, notavelmente a função `free`. Cuidado com o
SEGFAULT!
