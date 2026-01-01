# $${\Huge \color{red}A\color{green}d\color{red}v\color{green}e\color{red}n\color{green}t\ \color{red}o\color{green}f\ \color{red}C\color{green}o\color{red}d\color{green}e🎅}$$

Welcome to my cozy little workshop of Advent of Code solutions.  
Each year gets its own Christmas magic. 🎄✨

---

## $${\Large \color{gold}⭐\ Years}$$

- **2015** — $${\color{red}Incomplete}$$ (Found it enjoyable and went back for earlier years)  
- **2024** — $${\color{red}Incomplete}$$ 
- **2025** — $${\color{green}Complete}$$  
  My first *fully structured* AoC with:
  - `main.c`, `pt1.c`, `pt2.c`
  - standardized input files (`input.in`, `test.in`)
  - a reusable Makefile template  
  - and **Blackbox** logging integrated from Day 1 ✨

---

## $${\Large \color{green}📁\ Workshop\ Layout}$$

```
.
├── 2015/     → First year
├── 2024/     → Old attempt (kept, but ignored)
├── 2025/     → Current
└── README.md → You are here!
```

Each year folder is completely self-contained.

---

## $${\Large \color{green}🔧\ Dependency:\ Blackbox}$$

<img src="https://github.com/user-attachments/assets/00e305bc-7d20-4af1-a3e5-c794c76b60b5" width="180" alt="BlackBox Logo" />

This repo uses my lightweight C logging library **Blackbox**:

[![Blackbox Logger](https://img.shields.io/badge/Blackbox-Logger-gold?style=for-the-badge&logo=c&logoColor=black)](https://github.com/abnore/BlackBox)

Install it before building the 2025 solutions:

```
git clone https://github.com/abnore/blackbox
cd blackbox
make
sudo make install
```

(Or build and place the library locally if you prefer.)

Once installed, the Christmas elves will whisper debug messages directly to your console. 🎅📜✨

---

## $${\Large \color{lightblue}❄️\ Running\ a\ 2025\ Day}$$

Build:

```
make
```

Run with the test file:

```
make test
```

Run with the real input:

```
make run
```

Debug mode:

```
make run-debug
make test-debug
```

---

## $${\Huge \color{gold}Happy\ Advent\ of\ Code!}$$

May your loops be tight, and your off-by-one errors be forgiven by Santa. 🎅✨❄️
