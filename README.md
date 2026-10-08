# ⚫ Gomoku

**Gomoku** is an AI project developed at **42 School**: a game of Gomoku (five in a row) on a 19×19 board, with an AI opponent driven by a **Minimax** search with **alpha-beta pruning**, written in **C++** with an **SFML** interface.

> Team project with [GaetanMo](https://github.com/GaetanMo).

---

## 🎯 Rules

- Players take turns placing stones on the intersections of a 19×19 board.
- **Win** by aligning **5 stones**, or by **capturing 5 pairs** of the opponent's stones.
- **Capture**: flanking exactly two enemy stones (`X O O X`) removes them from the board.

---

## 🧠 The AI

- **Minimax with alpha-beta pruning** to explore future moves while cutting branches that cannot change the result.
- **Heuristic evaluation** scoring each position from alignments (length, open or closed ends, holes) and capture threats.
- **Candidate moves sorted by potential**: only the most promising moves are explored, which keeps the search deep and fast.
- **Incremental scoring and make/undo moves**: the board is updated in place instead of being copied at every node, and the score is updated move by move instead of being recomputed from scratch.

---

## 🎮 Features

- 3 game modes: **Player vs Player**, **Player vs AI** and **AI vs AI**
- Graphical interface with **SFML**: menu, board, captures, scores
- Per-player **timers**, including the AI's thinking time (running in a separate thread so the display stays live)
- **Move suggestions** for the human player in Player vs AI mode

---

## ⚙️ Usage

```bash
make run    # downloads SFML 2.6.1, compiles and launches the game
```

Requires Linux with `c++` (C++17), `make` and `wget`.

---

## 🛠️ Tech stack

- C++17 (`-O3`)
- SFML 2.6.1
- Makefile
