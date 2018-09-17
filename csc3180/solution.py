from heapq import heappush, heappop
import copy
import time
import random                           

def issolve(puzzlelist):                       # to estimate whether this puzzle has the solution to the destination
    reverse = 0
    puzzlecopy = []
    for i in puzzlelist:
        if i == 0:
            continue
        puzzlecopy.append(i)
    for i in puzzlelist:
        if i == 0:
            continue
        puzzlecopy.pop(0)
        for j in puzzlecopy:
            if i > j:
                reverse = reverse + 1
    if reverse%2 == 0:
        return True
    return False


def solve(puzzle):                          # to make the puzzle be the original situation[[1,2,3][4,5,6][7,8,0]] through the A* algorithm
    global PQ
    if isfinished(puzzle):
        route = []
        route.append(puzzle.moveway)
        while 1:
            if puzzle.father.moveway == None:
                return route
            else:
                route.insert(0,puzzle.father.moveway)
                puzzle = puzzle.father
    if ismoveup(puzzle):
        Up_puzzle = moveup(puzzle)
        ReduUp = abs(Up_puzzle.blockposition[0] - 2) + abs(Up_puzzle.blockposition[1] - 2)
        Up_manhattan = manhattan(Up_puzzle.value) - ReduUp
        Up_priority = Up_manhattan + Up_puzzle.move
        Up_puzzle.priority = Up_priority                           # to calculate the priority, the priority is added by the "manhattan" and the "moves"
        if puzzle.father != None:
            if Up_puzzle.value != puzzle.father.value:
                PQ.put(Up_puzzle,Up_puzzle.priority)
        else:
            PQ.put(Up_puzzle,Up_puzzle.priority)
    if ismovedown(puzzle):
        Down_puzzle = movedown(puzzle)
        ReduDown = abs(Down_puzzle.blockposition[0] - 2) + abs(Down_puzzle.blockposition[1] - 2)
        Down_manhattan = manhattan(Down_puzzle.value) - ReduDown
        Down_priority = Down_manhattan + Down_puzzle.move
        Down_puzzle.priority = Down_priority
        if puzzle.father != None:
            if Down_puzzle.value != puzzle.father.value:
                PQ.put(Down_puzzle,Down_puzzle.priority)
        else:
            PQ.put(Down_puzzle,Down_puzzle.priority)
    if ismoveleft(puzzle):
        Left_puzzle = moveleft(puzzle)
        ReduLeft = abs(Left_puzzle.blockposition[0] - 2) + abs(Left_puzzle.blockposition[1] - 2)
        Left_manhattan = manhattan(Left_puzzle.value) - ReduLeft
        Left_priority = Left_manhattan + Left_puzzle.move
        Left_puzzle.priority = Left_priority
        if puzzle.father != None:
            if Left_puzzle.value != puzzle.father.value:
                PQ.put(Left_puzzle,Left_puzzle.priority)
        else:
            PQ.put(Left_puzzle,Left_puzzle.priority)
    if ismoveright(puzzle):
        Right_puzzle = moveright(puzzle)
        ReduRight = abs(Right_puzzle.blockposition[0] - 2) + abs(Right_puzzle.blockposition[1] - 2)
        Right_manhattan = manhattan(Right_puzzle.value) - ReduRight
        Right_priority = Right_manhattan + Right_puzzle.move
        Right_puzzle.priority = Right_priority
        if puzzle.father != None:
            if Right_puzzle.value != puzzle.father.value:
                PQ.put(Right_puzzle,Right_puzzle.priority)
        else:
            PQ.put(Right_puzzle,Right_puzzle.priority)
    Next_puzzle = PQ.get()
    return Next_puzzle       

def form_puzzle(puzlist):
    PuzzleList = []
    for i in puzlist:
        for j in i:
            PuzzleList.append(j)        
    return PuzzleList

class Puzzle:                                                                  # We just use the Puzzle class to define a new puzzle, and in this Class, the puzzle has
    def __init__(self,value,father,moveway,blockposition,priority,move):       # six attribution: first: the "value" is the puzzle's value just like:[1,3,2],[5,8,6],[4,7,0]].
        self.value = value                                                     # And the second is "father": this attribution is a pointer which point to the last puzzle state.
        self.father = father                                                   # And the attribution moveway just represent which way of "ULDR" to get the current puzzle state.
        self.moveway = moveway                                                 # The next attribution blockposition is to express where the block(the 0 value,the blank position) 
        self.blockposition = blockposition                                     # are. The priority names its priority in the priority queue. Through compare all the puzzle's 
        self.priority = priority                                               # priority can determine which one will be put out from the priority queue. Finally, the move
        self.move = move                                                       # attribution is to calculate how many moves through the initial state to current state

    def __lt__(self, other):
        return self.priority < other.priority

class PriorityQueue:                                                           # define a priority queue, and we can take the puzzle out through comparing the priority of the
    def __init__(self):                                                        # the puzzle, and the one has the smaller priority will be taken out.
        self._queue = []

    def put(self, item, priority):
        heappush(self._queue, (priority, item)) 
  
    def get(self): 
        return heappop(self._queue)[-1]
    def getqueue(self):
        print(self._queue)

def isfinished(puzzle):                                                        # to judge whether the puzzle get the destination
    global GOAL
    if puzzle.value == GOAL:
        return True
    return False

                                                                               # the following four function are to estimate whether the block can move and judge which directions
                                                                               # can move.
def ismoveup(puzzle):
    if puzzle.blockposition == [0,0] or puzzle.blockposition == [0,1] or puzzle.blockposition == [0,2]:
        return False
    return True

def ismovedown(puzzle):
    if puzzle.blockposition == [2,0] or puzzle.blockposition == [2,1] or puzzle.blockposition == [2,2]:
        return False
    return True

def ismoveleft(puzzle):
    if puzzle.blockposition == [0,0] or puzzle.blockposition == [1,0] or puzzle.blockposition == [2,0]:
        return False
    return True

def ismoveright(puzzle):
    if puzzle.blockposition == [0,2] or puzzle.blockposition == [1,2] or puzzle.blockposition == [2,2]:
        return False
    return True
                                                                               # the next four function are to move the puzzle to create a new puzzle, and also remain the old puzzle.
def moveup(puzzle):
    Position = puzzle.blockposition
    Priority = puzzle.priority
    A = Position[0]
    B = Position[1]
    New_puzzle_value = copy.deepcopy(puzzle.value)
    New_puzzle = Puzzle(New_puzzle_value,puzzle,'U',Position,Priority,0)
    New_puzzle.value[A][B] = New_puzzle.value[A-1][B]
    New_puzzle.value[A-1][B] = 0
    New_puzzle.blockposition = [puzzle.blockposition[0]-1,puzzle.blockposition[1]]
    New_puzzle.move = puzzle.move + 1
    return New_puzzle

def movedown(puzzle):
    Position = puzzle.blockposition
    A = Position[0]
    B = Position[1]
    New_puzzle_value = copy.deepcopy(puzzle.value)
    New_puzzle = Puzzle(New_puzzle_value,puzzle,'D',puzzle.blockposition,puzzle.priority,0)
    New_puzzle.value[A][B] = New_puzzle.value[A+1][B]
    New_puzzle.value[A+1][B] = 0
    New_puzzle.blockposition = [New_puzzle.blockposition[0]+1,New_puzzle.blockposition[1]]
    New_puzzle.move = puzzle.move + 1
    return New_puzzle

def moveleft(puzzle):
    Position = puzzle.blockposition
    A = Position[0]
    B = Position[1]
    New_puzzle_value = copy.deepcopy(puzzle.value)
    New_puzzle = Puzzle(New_puzzle_value,puzzle,'L',puzzle.blockposition,puzzle.priority,0)
    New_puzzle.value[A][B] = New_puzzle.value[A][B-1]
    New_puzzle.value[A][B-1] = 0
    New_puzzle.blockposition = [New_puzzle.blockposition[0],New_puzzle.blockposition[1]-1]
    New_puzzle.move = puzzle.move + 1
    return New_puzzle

def moveright(puzzle):
    Position = puzzle.blockposition
    A = Position[0]
    B = Position[1]
    New_puzzle_value = copy.deepcopy(puzzle.value)
    New_puzzle = Puzzle(New_puzzle_value,puzzle,'R',puzzle.blockposition,puzzle.priority,0)
    New_puzzle.value[A][B] = New_puzzle.value[A][B+1]
    New_puzzle.value [A][B+1] = 0
    New_puzzle.blockposition = [New_puzzle.blockposition[0],New_puzzle.blockposition[1]+1]
    New_puzzle.move = puzzle.move + 1
    return New_puzzle


def manhattan(puzzle_value):                                                  # this function is to calculate the manhattan value
    manhattan = 0
    init_position = {1:[0,0],2:[0,1],3:[0,2],4:[1,0],5:[1,1],6:[1,2],7:[2,0],8:[2,1],0:[2,2]}
    for i in range(0,3):
        for j in range(0,3):
            manhattan +=(abs(init_position[puzzle_value[i][j]][0] - i) + abs(init_position[puzzle_value[i][j]][1] - j))
    return manhattan


def rand():
    total = 9
    li = [i for i in range(total)]
    res = []
    num = 9
    for i in range(1,num):
      t = random.randint(i,total-1)
      res.append(li[t])
      li[t], li[i] = li[i], li[t]
    return res


PQ = PriorityQueue()
GOAL = [[1,2,3],[4,5,6],[7,8,0]]
def solution(puzzle):                                                                                  
    puzzlelist = form_puzzle(puzzle)                                              # the next are to test whether the function can execute, just like the test_solution function,
    init_priority = manhattan(puzzle)                                             # and if you want to test the function, you can just change the argument of solution(puzzle)
    init_position = []
    for i in range(len(puzzle)):
        rowpuzzle = puzzle[i]
        for j in range(len(rowpuzzle)):
            value = rowpuzzle[j]
            if value == 0:
                init_position = [i,j]
                break
    puzzlefather = Puzzle(None,None,None,None,None,None)
    puzzle_start = Puzzle(puzzle,puzzlefather,None,init_position,init_priority,0)
    count = 0
    if issolve(puzzlelist):
        while 1:
            count = count + 1
            solve(puzzle_start)
            if isfinished(puzzle_start):
                print(solve(puzzle_start))
                break
            puzzle_start = solve(puzzle_start)
    else:
        print("This puzzle has no solution to the destination")  
puzzlevalue=[[3,2,1],[5,6,7],[4,8,0]]
solution(puzzlevalue)
