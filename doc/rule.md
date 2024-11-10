# Hive Game



Hive是一款两人抽象棋类游戏。玩家竞争以控制蜂巢，试图围困对手的蜂后以赢得游戏。

##　游戏开始
每位玩家将自己的颜色的所有棋子（白色或黑色）放入储备区。可用的棋子包括：

> - 1 只蜂后
> - 2 只甲虫
> - 2 只蜘蛛
> - 3 只蚂蚁
> - 3 只蚱蜢

此外，如果游戏包含扩展棋子，还有：
> - 1 只瓢虫
> - 1 只蚊子
> - 1 只药丸虫
游戏开始时，游戏区域为空。

## 玩家回合
在玩家的回合中，玩家可以执行以下两种动作之一：

> 1. 从储备区将一个棋子放置到游戏区域
> 2. 移动游戏区域内的任意一个棋子


游戏开始时，玩家只能放置棋子，直到他们放置了蜂后。此外，蜂后必须在玩家的第四回合放置；如果一个玩家在三回合后还没有放置蜂后，他们必须在第四回合放置。
如果玩家能够进行合法移动，他们必须这样做。如果玩家无法进行任何合法移动，他们必须跳过。

### 放置棋子
要放置一个棋子，玩家从储备区取出一个棋子并将其放置在游戏区域内。棋子必须至少通过一个完整侧面接触自己的一个棋子，并且不能接触对手的任何棋子。
例外：当然，放置的第一个棋子不会接触任何棋子，第二个放置的棋子必然接触第一个棋子（对方颜色的）。
第二个例外：当甲虫位于一个或多个棋子上方时，“棋子堆[的]颜色由甲虫决定。”

### 移动棋子
要移动一个棋子，玩家选择游戏区域内的一个棋子并根据其移动规则进行移动。必须始终遵循以下黄金规则：

相邻棋子必须始终共享一个完整的侧面，而不是部分侧面或仅在一点接触。想象一个六边形网格；每个棋子占据网格上的一个这样的空间。
蜂巢规则：蜂巢在任何时候都不能被断开。当一个玩家移动一个棋子时，其余的棋子必须形成一个连续的群体，并且移动的棋子最终也必须与群体相邻。
移动自由规则：一个棋子必须能够物理移动到目的地，一次一个空间。在大多数情况下，这意味着一个棋子必须“滑动”到目的地。一些棋子配置可能会留下一个太窄的间隙，以至于棋子无法通过。爬山的棋子也可能受此规则限制，如果有两个高堆留下一个狭窄的间隙。
#### 棋子移动规则
1. 蜂后：滑动到相邻空间。如果一个玩家的蜂后被围困（见下文），该玩家输掉游戏。
2. 甲虫：滑动或爬升到相邻空间。甲虫可以爬过其他棋子；如果相邻空间被一个棋子（任何颜色）占据，玩家可以将他们的甲虫移动到棋子上方。下面的棋子被固定住，直到甲虫移开。可以有甲虫爬在其他甲虫上方，形成一个更高的堆叠。如果甲虫在同一水平移动，它滑动。
3. 蜘蛛：在蜂巢周围滑动三个空间。蜘蛛不能在同一回合内转回来。有时蜘蛛在移动过程中可能与蜂巢的多个部分相邻；只要它不是向后移动，它可以继续在蜂巢的任何一侧爬行。
4. 蚂蚁：滑动到任何地方。只要遵守黄金规则，蚂蚁可以移动到任何地方。
5. 蚱蜢：跳过一行棋子。蚱蜢选择一个方向，然后向那个方向跳跃，落在第一个空位上。它必须跳过至少一个棋子（任何颜色，也可以是蚱蜢）。
6. 瓢虫：爬升两个空间然后回落。像蜘蛛一样，瓢虫移动三个空间：第一和第二在蜂巢上方，第三必须将其带回地面。
7. 蚊子：像它接触的任何棋子一样移动。蚊子可以选择它相邻的任何棋子，并复制其移动。它可以像甲虫一样爬上蜂巢；然后，在蜂巢上方时，它是一个甲虫（即使它没有接触任何甲虫），直到它移动到地面水平。只与另一只蚊子相邻的蚊子不能移动。棋子堆对于蚊子来说被视为一个甲虫（它不能复制甲虫下面的棋子）。如果药丸虫也在玩，与药丸虫相邻的蚊子可以复制药丸虫的特殊能力。
8. 药丸虫：滑动一个空间，或者将相邻的棋子移动到药丸虫旁边的另一个空间。如果药丸虫决定移动另一个棋子，它选择一个相邻的棋子，将其移动到药丸虫上，然后将其移回另一个空位。药丸虫不能移动前一回合刚移动过的棋子，并且移动的棋子在下一回合不能移动。药丸虫也不能移动棋子堆中的任何棋子。药丸虫还必须遵守黄金规则（特别是，如果移动棋子会破坏蜂巢，或者间隙太窄，则不能移动棋子）。

## 游戏结束
1. 当一个蜂后在所有六个侧面被围困（被任何颜色的棋子）时，游戏结束。被围困的蜂后的拥有者输掉游戏。
2. 如果两只蜂后同时被围困，游戏为平局。此外，如果游戏以重复移动结束，玩家可以同意平局。

## DLC
- 瓢虫：使用瓢虫棋子。
- 蚊子：使用蚊子棋子。
- 药丸虫：使用药丸虫棋子。

## 比赛开局规则：蜂后不能在第一回合放置。