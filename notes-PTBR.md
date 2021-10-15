# O Sudoku
Para o problema do Sudoku, temos um tabuleiro de N x N espaços, dividos em B x B blocos, sendo B a raiz quadrada de N (normalmente um tabuleiro 9x9 com blocos em setores 3x3). Alguns dos espaços já estarão preenchidos com números e os outros nós devemos preencher de forma a fazer com que os espaços em cada setor BxB tenham os números de 1 até N sem repetição e, com que nenhum número se repita em uma linha ou coluna.

# Estratégia de busca

### Ideia geral que poderá ser utilizada
Podemos usar uma estratégia de "Hill-Climbing" com backtracking, pois não queremos que, caso cheguemos em um ponto que não seja solução o programa desiste de buscas outras opções. De forma mais genérica, podemos pensar nessa estratégia de busca heurística como um DFS informada, ou seja, a escolha do próimo nó a ser visitado no grafo de estados do problema é baseado em heurísticas que nós definimos. Nesse cenário, nós estimamos qual o melhor espaço para ser o próximo avaliado (usando uma heurística); atribuímos, então, o melhor valor possível para aquele espaço (com base em outro heurística) e seguindos tentando resolver o tabuleiro. Caso cheguemos em um estado inválido, ou seja, quando não completamos o tabuleiro e não podemos inserir valor algum em um dos espaços, fazemos o backtracking e continuamos a busca.

"Os métodos de busca local, que é o caso do 'hill climbing', são usados para resolver problemas de otimização discreta iniciados a partir de uma configuração inicial e movendo-se repetidamente para uma melhor configuração vizinha."

### Ideias de heurísticas
**O objetivo aqui é sempre tentar reduzir a quantidade de backtrakings realizados para chegar na resposta, fazendo com que a escolha do próximo estado visitado seja a melhor possível**
1. Podemos reduzir a quantidade de valores candidatos em cada espaço vazio ao analisar quais valores já foram inseridos na mesma coluna, linha e bloco referente àquele espaço. Com isso, diminui-se o espaço de busca -> sempre que um novo valor é atribuído a um espaço, atualizamos os valores já utilizados a coluna, linha e bloco referente aquele espaço
2. Outra regra que podemos usar é: se um valor só é válido em um espaço (ou seja, mais nenhum espaço na mesma linha, coluna ou bloco pode assumir esse valor), então ele pode ser vinculado àquele espaço.
3. Se o soduku não foi finalizado, pegamos todos os espaços não preenchidos e ordenamos eles de acordo com a quantidade de valores candidatos para cada posição (quanto menos valores candidatos, mais será a prioridade daquele espaço -> "minimum-remaining-values heuristic")
    * Caso mais de um espaço tenha o mínimo de valores candidatos dentre todos, como forma de desempate podemo usar uma heurística de "most-constraining-variable", ou seja, o espaço cujo valor influenciará em mais espaços (que ainda precisam ser preenchidos) será a preferência

-> Pensar em possíveis funções de avaliação (que levam em consideração o futuro) e possíveis funções de custo (que levam em consideração o passado) mais genéricas ----> não sei se fazem tanto sentido para esse problema, mas, de certa forma, a escolha do próximo espaço a analisar (como está escrito em 1) usa uma função de avaliação; um pensamento similar existe para os itens 2 e 3

# Estratégias e referências que podem ser exploradas também...
Estratégia 3 e 4 aqui + lista de referências: https://medium.com/@davidcarmel/solving-sudoku-by-heuristic-search-b0c2b2c5346e -> não melhoram desempenho apararentemente (por serem regras que demandam bastante processamento extra), mas podem ser boas inspirações; as referências no fim da publicação são interessantes

# Casos de teste prontos (exemplos de Sudoku Solvers)
- https://github.com/davidcarmel/SudokuSolver
- https://github.com/t-dillon/tdoku (https://github.com/t-dillon/tdoku/blob/master/benchmarks/README.md)
- http://sudopedia.enjoysudoku.com/Test_Cases.html