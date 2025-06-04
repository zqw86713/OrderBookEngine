# OrderBookEngine

A high-performance, modular order book engine written in C++17. This project simulates exchange-style order matching and demonstrates practical system architecture, data structure mastery, and modern C++ practices—ideal for backend and quantitative developer interviews.

---

## ✨ Features

- 🧾 **Order Book**
  - Supports `BUY` and `SELL` limit orders
  - Maintains price-time priority via custom comparators (`std::set`)
  - Fast order ID lookups using `std::unordered_map`

- ⚙️ **Matching Engine**
  - Decoupled matching logic via a dedicated `MatchingEngine` class
  - Executes partial or full fills based on bid/ask priority
  - Returns structured `Trade` objects with buyer/seller IDs, price, quantity, and timestamp

- 🧪 **Test-Ready Architecture**
  - Structured to support Google Test and CMake integration
  - Includes a runnable `main.cpp` demonstrating the matching engine in action

---

## 📂 Project Structure

```
├── OrderBook.h # Order management: add/cancel/modify/query
├── MatchingEngine.h/.cpp # Order matching logic
├── main.cpp # Sample usage of OrderBook and MatchingEngine
├── CMakeLists.txt # (Upcoming) Build configuration with gtest support
└── README.md # Project overview
```

---

## 🧠 Sample Output

Trade: B1 buys from S1 at $99.5 for 8 units
Trade: B1 buys from S2 at $100 for 2 units
Trade: B2 buys from S2 at $100 for 5 units



---

## 🔭 Planned Features

| Feature | Description |
|--------|-------------|
| 🧪 Google Test | Add unit test coverage for core modules |
| 💡 Market Orders | Support for non-price-constrained orders |
| ⏱️ Latency Metrics | Measure and log matching latency with `std::chrono` |
| 🧵 Multithreading | Simulate concurrent order flow for HFT-style stress testing |
| 📊 Trade Log | Export trade execution to CSV/JSON for backtesting or analysis |
| 🌐 API / CLI | Add REST interface or command-line control for end-to-end simulation |

---

## 🎯 Project Goals

This engine was built as a foundational simulation for:
- Interview preparation (Google, Citadel, HRT, Jane Street, etc.)
- Showcasing backend development skills in C++
- Practicing real-time matching logic used in trading infrastructure
- Long-term expansion toward quantitative strategy simulation and HFT modeling

---

## 👤 Author

**Qingwei Zhang**  
Backend Developer | Aspiring Quantitative Developer  
[GitHub Profile](https://github.com/zqw86713)
