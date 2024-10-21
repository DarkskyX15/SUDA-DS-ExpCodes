<!--
 * @Date: 2024-10-21 16:52:56
 * @Author: DarkskyX15
 * @LastEditTime: 2024-10-21 16:57:33
-->

# Requirement

自行实现栈和队列类。（队列需要为循环队列）

# 题目1：小猫钓鱼纸牌游戏

A和B两个同学玩简单的纸牌游戏，每人手里有n张牌，两人轮流出牌并依次排列在桌面上，每次出掉手里的第1张牌，出牌后如果发现桌面上有跟刚才打出的牌的数字相同，则把到相同的那张牌结束的全部牌按次序放在自己手里牌的末尾。  
当一人手中牌先出完时，游戏结束，对方获胜。  

如n为5，A手里的牌依次为2 3 5 6 ，B手里的牌依次为1 5 4 2。  
A出2，B出1；此时，桌子上从前往后依次为21，A手里是356，B手里是542；  
A出3，B出5；此时，桌子上从前往后依次为2135，A手里是56，B手里是42；  
接着A出5，发现前面有一张5，则把两个5都拿掉，这时他手里有6 5 5；桌子上的牌依次为2 1 3；接着B出4，桌子上的牌是2134，他手里的牌是2；  
接着A出6， A手里剩55，B出2，发现前面有2，全部收走到自己手里，它手上的牌即是：264312桌子上没有牌；依次类推，直到某人先出完牌为止，则对方是胜者。  

编写程序，模拟显示出桌子上和A、B两位同学手里牌的变化过程，并判断谁是胜者以及此时胜者手里的牌的状态。  

如上例中，每次A和B各出一张牌后，桌上和手里牌的变化状态如下所示：

```plaintext
the cards on the desk:2 1
the cards in A's hand:3 5 6
the cards in B's hand:5 4 2
************************
the cards on the desk:2 1 3 5
the cards in A's hand:5 6
the cards in B's hand:4 2
************************
the cards on the desk:2 1 3 4
the cards in A's hand:6 5 5
the cards in B's hand:2
************************
the cards on the desk:
the cards in A's hand:5 5
the cards in B's hand:2 6 4 3 1 2
************************
the cards on the desk:5 2
the cards in A's hand:5
the cards in B's hand:6 4 3 1 2
************************
the cards on the desk:6
the cards in A's hand:5 2 5
the cards in B's hand:4 3 1 2
************************
the cards on the desk:6 5 4
the cards in A's hand:2 5
the cards in B's hand:3 1 2
************************
the cards on the desk:6 5 4 2 3
the cards in A's hand:5
the cards in B's hand:1 2
************************
the cards on the desk:6 1
the cards in A's hand:5 3 2 4 5
the cards in B's hand:2
************************
the cards on the desk:6 1 5 2
the cards in A's hand:3 2 4 5
the cards in B's hand:
************************
```

胜者为A，A剩下的牌从上到下依次为3 2 4 5

# 题目2：火车车厢重排（选做）

（参考101教材3.4.3）  

一列挂有 n 节车厢的货运列车途径 n 个车站，计划在行车途中将各节车厢停放在不同的车站。假设 n 个车站的编号从1 到 n，货运列车按照从第n 站到第 1 站的顺序经过这些车站，且将与车站编号相同的车厢卸下。  
货运列车的各节车厢以随机顺序入轨，为方便列车在各个车站卸掉相应的车厢，则须重排这些车厢，使得各车厢从前往后依次编号为1到n，这样在每个车站只需卸掉当前最后一节车厢即可。  
车厢重排可通过转轨站完成。一个转轨站包含 1 个入轨 （I）， 1 个出轨（O）和 k 个位于入轨和出轨之间缓冲轨（Hi）。  

请设计合适的程序来实现火车车厢的重排。
