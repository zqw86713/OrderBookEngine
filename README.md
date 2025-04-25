
---

## ✅ `README.md` for `OrderBookEngine`

```markdown
# 🏛️ OrderBookEngine — C++17 Multithreaded Trading Engine

**OrderBookEngine** is a high-performance simulation of a Level-2 limit order book written in modern C++17.  
The project is designed to mimic the behavior of production-grade trading systems, featuring real-world mechanisms like price-time priority, partial fills, and thread-safe order management.

---

## 🚀 Features

- 📈 **Limit Order Matching** — Simulates Level-2 price-time priority queueing
- ⛓ **Multithreaded Execution** — Uses `std::mutex` and `std::lock_guard` for thread safety
- ✅ **Partial Fill Support** — Handles split execution based on quantity availability
- 🧪 **Unit Test Support** — Integrates with GoogleTest for future validation
- 📦 **Modern Build System** — Configured using CMake + Conan for dependency management
- 🐳 **Container-Ready** — Dockerfile included for portable builds and testing

---

## 🧱 Core Components

| File               | Description                                 |
|--------------------|---------------------------------------------|
| `order.h/.cpp`     | Order class: includes price, quantity, side, timestamp |
| `order_book.h/.cpp`| OrderBook class: manages buy/sell queues and matching |
| `main.cpp`         | Sample executable to simulate order flow and matching |
| `CMakeLists.txt`   | Build configuration (C++17, warnings, test integration) |
| `tests/`           | (Optional) GoogleTest test cases for order logic |

---

## 🧪 Example Output

```
Matching orders...
Matched: 10 units @ 99.50
Matched: 5 units @ 99.00
Done.
```

---

## 🔧 Build Instructions

### ▶️ Prerequisites

- CMake ≥ 3.14
- C++17-compatible compiler (GCC, Clang, MSVC)
- Conan (for dependency management)
- [Optional] GoogleTest

### ⚙️ Build Steps

```bash
mkdir build && cd build
conan install .. --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## 📦 Run

```bash
./OrderBookEngine
```

---

## 🧪 Unit Tests (optional)

```bash
./test_order_book
```

*Make sure you’ve linked GoogleTest if tests are enabled.*

---

## 💡 Motivation

This project was developed as a self-initiated backend + quant systems simulation, to explore:

- Order matching logic used in high-frequency trading
- C++ concurrency patterns in real-time execution engines
- Scalable backend infrastructure for financial systems

---

## 🛡 License

This project is licensed under the MIT License. See [`LICENSE`](LICENSE) for more information.

---

## 🙋‍♂️ Author

**Qingwei Zhang**  
Backend Engineer | Quant Developer Aspirant  
[GitHub: zqw86713](https://github.com/zqw86713)

```
