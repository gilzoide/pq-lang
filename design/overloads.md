Overloads
=========
Para facilitar a programação, e ainda possibilitar a notação de chamada de
método estilo POO, _pq_ suporta (e incentiva) o uso de _overloads_. Isso quer
dizer que várias funções podem ter o mesmo nome, desde que operem em tipos
diferentes.

Sempre que uma função é chamada, se não especificado pelo tipo, a resolução de
qual será chamada é:

1. _Overload_ to tipo específico passado como argumento
2. _Overload_ de algum dos tipos que o argumento estende
3. Função geral sem tipagem definida, que será instanciada para o tipo passado
   como argumento

Como _overloads_ são especificados pelo tipo, não são válidos para macros.

Exemplo:
```lisp
(defstruct Ponto2D :x float :y float)
(defstruct Ponto3D :extends Ponto2D :z float)

; Considere essa chamada da função `magnitude`
(magnitude ponto)
; Se ambos Ponto2D e Ponto3D implementam a função `magnitude`, o overload do tipo de `ponto` será chamado
; Se somente Ponto2D implementa a função, e `ponto` é uma instância de Ponto3D, a função `Ponto2D/magnitude` será chamada

; Se ambos os tipos implementam a função, é possível forçar a chamada da função usando Ponto2D:
(Ponto2D/magnitude ponto)
```
