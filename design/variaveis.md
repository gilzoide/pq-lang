Valores e Variáveis
===================
Em _pq_, variáveis são imutáveis por padrão. Isso quer dizer que os valores
guardados normalmente não podem ser modificados, o que é comum na programação
funcional e fundamental no cálculo lambda. Quando variáveis são imutáveis, muitas
otimizações podem ser feitas, e há maior segurança quanto ao determinismo do
programa, incluindo acessos concorrentes.

Para definir uma ligação de um valor a um nome (variável imutável), basta usar
a construção `(let nome valor)`. Para definir uma variável mutável, usa-se a
construção `(var nome valor)`.


Argumentos de funções
---------------------
Quando uma função é criada em _pq_ e compilada pelo _llvm_, variáveis passadas
por argumento têm o seguinte comportamento:
- Tipos nativos (ints, floats, ponteiros) e tuplas (structs não nomeadas)
  imutáveis: são passadas por valor
- Structs nomeadas: são passadas por referência, mesmo que imutáveis
- Quaisquer variáveis mutáveis: passadas por referência
