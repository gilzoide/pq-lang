(letf array (tipo :: Type)
	  (struct :size i32 :type (pointer tipo)))

; Overload de uma função que itera sobre array/lista/null-terminated-array
; aplicando uma função a cada elemento, sem retorno
(letf iterate (f :: ['a nil]
			   arr :: !(array 'a))
	  (and (= arr.size 0)
		   ((f (@ arr 0))
			(iterate f (+ arr 1)))))
(letf iterate (f :: ['a nil]
			   lst :: !(list-of 'a))
	  (and lst
		   ((f lst.head)
			(iterate f lst.tail))))
(letf iterate (f :: ['a nil]
			   nt-arr :: !(nt-array 'a))
	  (and nt-arr
		   ((f (@ nt-arr 0))
			(iterate f (+ nt-arr 1)))))

(let string (Char.array))
(let c-string (Char.nt-pointer))
(let char-list (Char.list-of))
(letf new (lista :: char-list)
	  (char-list { nil nil }))

(letf to-char-list (str :: c-string)
	  (var lista (char-list.new))
	  (iterate lista.append str)
	  lista)
(letf to-char-list (str :: string)
	  (var lista (char-list.new))
	  (iterate lista.append str)
	  lista)
