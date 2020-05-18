Log de ideias de implementação que surgem
=========================================
Malz demorar pra começar...

- 2016-10-08: Fazer um interpretador em Lua, JIT compilando as coisas
  estaticamente tipadas pelo LLVM, daí as informações de tipo, expansão de
  macros e resolução de chamadas (MRO ou overload) fica a cargo do
  interpretador. Módulos feitos com tipagem estática podem ter seu bitcode
  compilado pra .ll ou .bc, ou mesmo compilado pra código nativo.
- Há um tempo: Usar LLVM pra JIT compilar os rolê, pra ficar rápido e poder
  facilmente rodar coisas compiladas em outras linguagens (a partir do LLVM
  também).
- 2016-05-06: Outra ideia pra gerenciamento de memória é cada objeto manter
  qual o primeiro escopo que o referencia, que daí quando sair desse escopo,
  destruir o objeto. Pode rolar até diferentes Allocator/Deallocator, pra quem
  não quiser não usar isso =P
- 2016-05-06: shared\_ptr tira um tanto bom da semântica da aplicação, e faz
  com que tenhamos mais trabalho.  Backend de qualquer jeito já vai cuidar
  disso com carinho, a treta é pros DEV.
- 2016-05-05: shared\_ptr são chatos de mexer, mas overloadar o operador delete
  aparentemente não é tão boa ideia, visto que o destrutor sempre é chamado antes.
  Qual outra alternativa? Talvez object pool...
- 2016-05-04: Xupa shared\_ptr, fazer a contagem de referência de Atoms nela
  mesma, overloadando o operador delete (tava complicando muito o trem)
