Mas ô gilzoide, e a sintaxe do rolê?
====================================
Depois vos conto.


```lisp
; como definir um valor no escopo local
(let a 1)

; por padrão, em PQ, valores são imutáveis
; se você precisar de variáveis, use "var"
(var a 1)
; para mudar valor de variáveis, manda um "set"
(set a 4)

; funções, passa também o nome das variáveis a dar bind
(defun oi ()
	(print "oi mundo"))
; claro, parâmetros
(defun oi (nome sobrenome)
	(print "oi " sobrenome ", " nome))
; e também parâmetros variáveis!
(defun oi (...)
	(print ...))
; e também funções variáveis!
(var dá-oi print)
(set dá-oi oi)

; criação de tipos: structs
(defstruct Point
	; campos, com tipagem estática, nome dos campos em keyword, opcional
	:x int
	:y int
)
; função/metodo. Note que o próprio ponto deve estar nos argumentos
; note também que acessar um elemento num tipo usa notação de '.'
(defun distancia [Point float] (P)
	(let x P.x)
	(let y P.y)
	(sqrt (+ (* x x) (* y y)))
)

; Para chamar uma função, pode se usar a função a partir do tipo, ...
(Point/distancia p)
; ... ou usando a notação de metodo '.' (que expande pra `(Point/distancia p)`)
(p:distancia)
; ou simplesmente chamando a função diretão, pq na real o overload é sacado de bouas
(distancia p)
```
