#!/usr/bin/env pq

;; Vamos testar jeitos diferentes de sintaxe dos tipos, pra ver o que agrada mais

; Criação de novas structs
(defstruct Vec2
  :x int
  :y int
) ; 1
(defstruct Vec2
  [x int] :ou ['x int] :ou [x :: int]
  [y int] :ou ['y int] :ou [y :: int]
) ; 2
; 1 - mais simples de fazer tuplas, onde campos não têm nome
; 2 - mais verboso / mais fácil de ver cada campo separadamente

; Criação de structs literais
(let origem Vec2 { :x 0 :y 0 })	; 1
(let origem Vec2 { 0 0 })		; 2
(let origem { 0 0 })			; 3
(let origem (Vec2 0 0))			; 4
(let origem (Vec2 :x 0 :y 0))	; 5
(let origem (Vec2 { 0 0 }))	    ; 6
; Problemas de cada:
; 1 e 2 - compilador precisa saber que 'Tipo { vals }' é uma coisa só
; 3 - como saber que é 'Vec2', e não um '{ int int }'? Rola coalescência quando for usar?
; 4 e 5 - não é tão explícito que resultado é uma struct, que é algo que acho manero
; 6 - grandim

; Notação de metodo
(let x (+ V1.x V2.x)) ; prefiro esse
(let x (+ V1:x V2:x)) ; igual Lua, daí talvez role diferença entre metodo e acesso a campo

; Construção de metodo
(letf Vec2/origem-p (this :: Vec2)
	  (= this origem)) ; 1
(letf Vec2.origem-p ()
	  (= self origem)) ; 2
(letf origem-p (self :: Vec2)
	  (= self origem)) ; 3
; 1 - função no namespace pode ser chato se tiverem muitas funções
; 2 - "metodo" não deixa tão explícito o argumento self, mas bom que já o tem
;     com tipo certo
; 3 - função sem namespace é mais limpo, mas não rola notação de
;     metodo, e tem que ter cuidado pra ser acessível pelo namespace da struct
