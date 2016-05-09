Log de ideias de implementação que surgem
=========================================
Malz demorar pra começar...

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
