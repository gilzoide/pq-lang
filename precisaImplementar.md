O que é preciso implementar pro PQ ficar pronto?
================================================

- Parser
- Tipos
- Escopos / namespaces
- Gerenciador de chamada de overload (que descobre qual função que é)
- Funções builtin
- Ponte de dados nativos entre Lua e LLVM
- ExecutionEngine pra rodar as treta, mesmo quando compilando (pode ser preciso
  em uma macro)
- Gerenciamento de memória (além do mais, vai ter GC quando compilado?)
- Interpretador
- Debugger bacana
