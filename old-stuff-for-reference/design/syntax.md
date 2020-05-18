Mas ô gilzoide, e a sintaxe do rolê?
====================================
Depois vos conto.


```lisp
; como definir um valor no escopo local
(let a 1)
; dá também pra definir valores através de listas, útil pra funções que
; retornam mais de um valor
(let (x y) (ponto2D.getCoords))

; por padrão, em PQ, valores são imutáveis
; se você precisar de variáveis, use "var"
(var a 1)
; do mesmo jeito, definição de variáveis através de listas
(var (x y) (ponto2D.getCoords))

; 'let' e 'var' criam bindings pra valores/variáveis. Se quiser trocar o valor
; de uma variável já existente anteriormente (em um escopo de fora) use 'set'
(set a 3)


; funções, passa também o nome das variáveis a dar bind
(letf oi ()
	(print "oi mundo"))

; claro, parâmetros
(letf oi-pessoa (nome sobrenome)
	(print "oi " sobrenome ", " nome))

; e também parâmetros variáveis! Lembre-se que parâmetros variáveis vêm em lista
(letf oi-params (...)
	(print ...))

; funções, assim como valores, podem ser variáveis. Manda 'varf' xD
(varf saudacao)
(set saudacao oi)
(set saudacao oi-pessoa)


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
	; função estática, cria um ponto
	(new :: (int int Point) (_ (x y)
		({} :x x :y y)
	))
)
(var p (Point 1 2))
(set (p.x) 5)
; Para chamar um metodo, pode se usar a função do tipo (estática), ...
(Point.dist p)
; ... usando a função do próprio objeto (que provavelmente será a mesma do tipo), ...
(p.dist p)
; ... ou usando a notação de metodo ':' (que expande pra `(p.dist p)`). TODO trocar a notação de metodo, já que ':' usa bastante no lisp
(p:dist)
```
