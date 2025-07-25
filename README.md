# 🍽️ Philosophers - 42 Porto

A solution to the Dining Philosophers problem using threads and mutexes in C, demonstrating concurrent programming principles.

[![Code Size](https://img.shields.io/github/languages/code-size/mfortuna23/philosophers?style=flat)](https://github.com/mfortuna23/philosophers)

## 🌟 Key Features
### My Implementation
- **Deadlock-free solution** using mutexes and careful resource timing
- **Starvation prevention** through fair scheduling algorithm
- **Real-time monitoring** of each philosopher's state (eat/think/sleep)
- **Configurable parameters**:
  ```bash
  ./philo 5 800 200 200 [7]
  # 5 philos | 800ms die timer | 200ms eat | 200ms sleep | (optional) 7 meals
  ```
