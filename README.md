# Desdemona
Reversi game (created for homework)

![image](https://user-images.githubusercontent.com/48797351/189120383-5b7831f7-e833-41ec-b696-e740e7110a8c.png)

αβ法をもちいた1人用オセロプログラムです。大学の課題で作りました。

知られざる開発秘話については[こちら](https://qiita.com/reika727/items/b027f4c20e2c946ea102)からどうぞ。

## 操作

左クリックで石を打ちます。右ドラッグで視点移動します。

## 起動オプション

- ```-level=<level>```

敵のレベルを設定します（デフォルト値は0）。このレベルはαβ法の読みの深さに相当します。私のパソコンでは快適にプレイできるのはレベル5程度までです。

- ```-second```

指定すると後手になります（デフォルトでは先手）。

## cui

```
./othello_cui.out
 abcdefgh
1........
2........
3........
4...wb...
5...bw...
6........
7........
8........
--------
> d3
 abcdefgh
1........
2........
3...b....
4...bb...
5...bw...
6........
7........
8........
--------
enemy thinking...
enemy: c5
 abcdefgh
1........
2........
3...b....
4...bb...
5..www...
6........
7........
8........
--------
>
```

"cui"フォルダにはcui用のプログラムが入っています。プロンプトに"a1"や"c2"といった感じで座標を打ち込み、試合を行います。こちらが先手、相手のレベルは4で固定です。
あくまでも試験用に作っただけのガバガバプログラムなので、試合を中断する方法はありません。途中でゲームを辞める場合はCtrl-Cで終わらせてください。
