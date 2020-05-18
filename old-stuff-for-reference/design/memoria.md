Gerenciamento de memória
========================
Como acontece(rá) o gerenciamento de memória no PQ?

A ideia é que haja um _object pool_ pra podermos reaproveitar objetos pequenos,
como Ints, Cons e talz. Ao criar, vê se já tem uma memória sobrando, se não
cria. Ao destruir, diz que não tá mais usando, e rola um `reset` no objeto, pra
ficar pronto pra próxima.

Como a maioria dos valores são imutáveis, basta passar o mesmo ponteiro do
objeto pra lá e pra cá que tá tudo certo. Ao transferir valores entre variáveis
(as que podem variar mesmo), rola a passagem por valor, que é onde a função
Atom::clone entra em ação.

A treta mesmo é na hora de destruir objetos, quando um valor pode ser
descartado? As opções clássicas são __contagem de referência__ e __garbage
collection__. Contar referências, pra automatizar tudo, é meio complexo.
Garbage collection é um pouco mais difícil de implementar, pode dar uns delay
no rolê e talz. Daí a ideia é que como valores estão atrelados a seus escopos,
o primeiro (menor índice) que mantiver uma referência a um objeto é quem deve
retorná-lo ao _pool_. Ao criar um Atom, tal índice deve ser settado com o
índice do escopo atual. Esse valor só deve ser trocado se um Atom for retornado
por uma função, já que essa é a hora que o índice do escopo atual muda
