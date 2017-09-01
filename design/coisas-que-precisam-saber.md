Coisas que precisam saber (_pq_ explicado rapidão)
==================================================

Implementação
-------------
A implementação do interpretador/compilador será em [_Lua_](http://lua.org/),
utilizando [_LPeg_](http://www.inf.puc-rio.br/~roberto/lpeg/) para o _parsing_
e [_LLVM_](http://llvm.org/) para a compilação. A escolha do _Lua_ deve ao fato
da linguagem ser leve o suficiente, portável e muito flexível, bom pra escrever
as abstrações malucas que um interpretador/compilador maluco que quer rodar em
qualquer máquina precisa.

Interpretado vs JIT compilado vs Compilado
------------------------------------------
_Pq_ usa _LLVM_, compila pra código _LLVM IR_ ou nativo, mas só se você pedir.
O padrão é ele interpretar/JIT compilar os rolê, que assim _pq_ pode ser usado
tanto pra _scripts_ e pequenos _snippets_ quanto pra sistemas fodões baixo
nível que escovam bits e precisam de alta performance.

Cuidado que nem tudo pode ser compilado pra código de máquina, pois muitas
certezas precisam ser dadas pra compilar. A regra é que tudo compilado pode ser
usado também no modo interpretado, mas construções do modo interpretado só
podem ser usadas no próprio modo interpretado ou em tempo de compilação
(dentro de macros ou construções _top level_ que não geram código).

Sintaxe
-------
Estilo _Lisp_: `(print "Oi mundão!")`

Mais sobre isso depois (em tempo, não nesse documento).

Tipos
-----
Há dois tipos de tipos:
- __Estáticos__: tipos do LLVM, compilados e estáticos (dâ):
  + __Inteiros__:
    * com sinal - i8, i16, i32, i64, i128, isize (tipo o do _Rust_)
    * sem sinal - u8, u16, u32, u64, u128, usize (tipo o do _Rust_)
  + __Reais__: half, float, double, fp128
  + __Tuplas__: pacotes de variáveis, sem nomes pros campos. São passados por
    valor quando imutáveis. Úteis pra múltiplos retornos em funções
  + __Structs__: pacotes de variáveis com um namespace próprio, nomes opcionais
    pra seus campos. São passados por referência mesmo quando imutáveis (claro
    que rola um atributo _readonly_ pra otimizações)
  + __Arrays__: vetor contíguo na memória de tamanho conhecido de variáveis de
    mesmo tipo. Permitem indexação direta. Implementado como ponteiro + tamanho
  + __Null Terminated Arrays (nt-arrays)__: vetor contíguo de variáveis de
    mesmo tipo. Permite indexação direta, mas índices só são válidos até que
    se encontre um terminador nulo (`zeroinitializer`). Implementado como
    um simples ponteiro. Sim, _strings_ do _C_
  + __Lista simplesmente encadeada__: elemento + ponteiro do próximo nó, um
    clássico da programação funcional. Sendo uma Struct (com nome bonito), é
    sempre passada por referência, sendo ponteiro nulo (`null`) uma lista vazia
  + __Ponteiro__: um ponteiro qualquer, correspondente ao `void *` do _C_. Pode
    ser usado no lugar de qualquer tipo de ponteiro, e qualquer tipo de
    ponteiro pode ser usado em seu lugar (_cast_ livre de encheção de saco)

- __Dinâmicos__, só quando interpretado:
  + __Tipo__: sim, o tipo "Tipo", com informções sobre o tal do tipo, como
    campos, funções definidas, nome, blablabla
  + __Lista__: as listas dinâmicas com valores de qualquer tipo, que formam
    códigos nos _Lisps_
  + __Macro__: macros são que nem funções, mas sem tipagem estática, e só são
    avaliadas em tempo de interpretação/compilação. Não é permitida a aplicação
    parcial de macros, nem passagem dessas como parâmetro para funções.
	São variádicas por padrão.
  + __Escopo__: _namespaces_, _tables_ do _Lua_ que guardam valores indexados
    por seu símbolo correspondente. Em geral podem ser indexadas implicitamente
	(tipos são indexados pelo _overload_, escopo léxico e módulos importados
	indexados só pelo uso do símbolo diretão)
  + __Table__: _table_ do _Lua_, pra guardar valores e talz. São diferentes de
    escopos por precisarem sempre de indexação explícita; suas chaves podem ser
    quaisquer valores
  + __Símbolo__: _strings_ internas do _Lua_ usadas pra guardar os nomes das
    variáveis, funções, e tudo mais
  + __Keywords__: símbolos prefixados com ":" usados somente como ponto de
    referência dentro de macros e nomes de campos em _structs_
  + __Nil__: nulo, um valor que na real tem valor nenhum. É usado o `nil` do
    _Lua_ pra isso. Único tipo dinâmico que é falso

Verdade ou conseq... digo, falsidade
------------------------------------
Assim como em _C_, qualquer valor com todos os _bits_ zerados é falso. O resto
é verdadeiro, simples assim. O único valor interpretado que é falso é o `nil`.
Na verdade, o identificador `nil` poderá ser usado em qualquer lugar, denotando
um valor falso (de conteúdo `zeroinitializer`) do tipo que precisar. Um tipo
_bool_ existirá, mas somente para facilitar a interoperabilidade com outras
linguagens.

É possível também que haja algum tipo onde cada _bit_ seja considerado um valor
booleano diferente.

Tipos de Funções
----------------
Tipos de chamadas possíveis em _pq_:
- __Funções do pq__: funções definidas em código _pq_. Se alguma construção
  dinâmica for usada, a função não poderá ser compilada. Podem ser variádicas,
  mas devem usar `va-list` e família para isso
- __Macro do pq__: macros definidas em código _pq_, que recebem uma lista com
  os síbolos passados e devem retornar uma lista com o código a ser colocado
  no lugar da chamada
- __Funções nativas__: funções já compiladas pra código de máquina nativo.
  Precisam ter a tipagem definida pelo programador antes de poderem ser usadas
  (código objeto não sabe quem é que tipo)
- __Funções LLVM__: funções definidas em módulos do _LLVM IR_, tanto textual
  (arquivos `.ll`) quanto _bitcode_ (arquivos `.bc`). A tipagem já é garantida,
  e otimizações podem ser executadas em tempo de _linkagem_. É bacana que dá
  pra compilar códigos de outras linguagens (tipo _C_ com _clang_) e usar
  diretão no _pq_
- __Funções do Lua__: funções definidas em código _Lua_, disponíveis somente no
  modo interpretado. Podem ser usadas pros mais diversos propósitos, um
  importante sendo a comunicação com o interpretador (`let`, `var`, `set`,
  `import`...).  Se quiser usar _Lua_ a partir de código compilado, use a API
  própria
- __Macros LLVM__: funções _Lua_ que recebem uma lista com os símbolos passados
  e um `LLVMBuilder`, e são responsáveis por construir o código compilado
  necessário, como o `if` e `while`. Implementar construções assim faz com que
  _pq_ seja livre de palavras reservadas e tenha uma gramática bem mais simples.
  Se for usada no modo interpretado, não haverá um `LLVMBuilder` definido, o
  que pode quebrar o código se não tratado (macros _builtin_ sempre tratarão
  esse caso)

Aplicação parcial (_currying_)
------------------------------
Inspirado pelo _Haskell_, _pq_ vai realizar aplicação parcial de funções se
essas não receberem o número mínimo de argumentos. Isso facilita bastante o uso
da programação funcional, deixando o código bem limpo e inteligível (é só não
abusar). _Closures_ serão implementadas como funções parcialmente aplicadas.

Aplicação parcial não é suportada em chamadas a macros.

Overloads
---------
Pra que o trem fique maneiro e bem abstraído, funções em _pq_ funcionarão por
overloads (maomen que nem _C++_ e _Java_). Isso quer dizer que várias funções
de mesmo nome podem ser criadas, desde que essas tenham tipos diferentes (conta
somente tipos de parâmetros, nada de retorno).

Como a aplicação parcial é executada por padrão, só serão permitidos _overloads_
com o mesmo número de argumentos. Se algum precisar de mais informação,
considere o uso de tuplas.

Pra poder usar cada uma dessas funções até mesmo em _C_, vai rolar um leve
_mangling_ nos nomes das funções, mas nada de `_Z1f...` (feio e ilegível demais).

Extensão de _structs_
---------------------
_Structs_ diferentes podem vir de uma mesma estrutura base. Pensando nisso,
_pq_ permite a construção de tipos novos a partir de algum já existente,
copiando os campos já existentes no tipo original e adicionando mais os que
precisar.

Como _structs_ são sempre passadas por referência, qualquer função definida
para o tipo original pode ser usada pelo tipo estendido, basta um _cast_ no
ponteiro. Sendo assim, se um _overload_ não for implementado para um tipo
estendido, o do tipo original é chamado.

Documentação
------------
Assim como em _Python_, é possível documentar elementos do código com strings,
salvas em algum lugar especial, podendo ser usadas no modo interpretado.
Isso facilita a padronização e extração de documentação direto do código.

Erros
-----
No começo, nenhum tratamento especial será dado para erros (estilo _C_: crie
seu próprio). Por outro lado, _LLVM_ permite uso de exceções em código
compilado, podendo ser incluído posteriormente na linguagem.

Gerenciamento de memória
------------------------
No começo, nenhum sistema de gerenciamento de memória será implementado
(estilo _C_: te vira). Por outro lado, _LLVM_ permite uso de
_Garbage Collectors_ em código compilado, podendo ser incluído posteriormente
na linguagem.
