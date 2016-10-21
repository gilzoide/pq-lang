(let numero-magico 42) ; resposta do jogo

;; versão iterativa
(var escolha) ; variável com escolha do usuário
(while (!= escolha numero-magico)
       (set escolha (scanf "%d"))
       (print (case
                ((< escolha numero-magico) "Número muito pequeno")
                ((> escolha numero-magico) "Número muito grande")
                (otherwise "Número certo, parabéns =]"))))

;; versão recursiva
(letf joga ()
      (let escolha (scanf "%d"))
      (let {msg ganhou} (case
                          ((< escolha numero-magico) {"Número muito pequeno" nil})
                          ((> escolha numero-magico) {"Número muito grande" nil})
                          (otherwise {"Número certo, parabéns =]" t})))
      (print msg)
      (or ganhou (joga)))
(joga)



; teste de closures
(letf range (inicio fim)
      (var i inicio)
      (while (< i fim)
             (yield i)
             (++ i)))
