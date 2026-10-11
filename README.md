# Geodesic Surfaces


## Структура
* `main.cpp` 
* `plot_results.py` 

## Тест точності (нахилене коло сфери, T = 2π)
* N = 60 кроків: похибка = 4.52 × 10⁻⁶
* N = 600 кроків: похибка = 4.56 × 10⁻¹⁰

## Запуск
```bash
g++ -O3 main.cpp -o geodesics && ./geodesics
python plot_results.py
```
