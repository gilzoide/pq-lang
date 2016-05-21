Mas ô gilzoide, e a sintaxe do rolê?
====================================
Depois vos conto.


```lisp
; como definir uma variável no escopo local
(set a 1)

; por padrão, em PQ, valores são imutáveis
; se você precisar de variáveis, use "setvar"
(setvar a 1)


; funções, passa também o nome das variáveis a dar bind
(setf oi ()
	(print "oi mundo"))

; claro, parâmetros
(setf oi (nome sobrenome)
	(print "oi " sobrenome ", " nome))

; e também parâmetros variáveis!
(setf oi (...)
	(print ...))


; criação de tipos
; não são exatamente structs, nem classes, pense em protótipos
; alguma hora a gente explica direito
(newtype Point
	; atributos, com possível tipagem estática
	(x :: int)
	(y :: int)
	; função/metodo. Note que o próprio ponto deve estar nos argumentos
	; note também que acessar um elemento num tipo usa notação de '.'
	(distancia :: (Point float) (_ (P)
		(sqrt (+ (* P.x P.x) (* P.y P.y)))
	))
	; todas funções são estáticas, então xupa anotação
	(parse (_ (str :: string)
		; todo implementae
		"implemente-me"
	))
)
; Para chamar um metodo, pode se usar a função do tipo (estática), ...
(Point.dist p)
; ... usando a função do próprio objeto (que provavelmente será a mesma do tipo), ...
(p.dist p)
; ... ou usando a notação de metodo ':' (que expande pra `(p.dist p)`)
(p:dist)
```
