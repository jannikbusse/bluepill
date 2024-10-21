import numpy as np
import matplotlib.pyplot as plt

def plot_math_function(func, n):
    x = np.linspace(0, n, 400)
    y = func(x)
    
    plt.figure(figsize=(10, 6))
    plt.plot(x, y, label=f'{func.__name__}(x)')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f'Plot of {func.__name__} from 0 to {n}')
    plt.legend()
    plt.grid(True)
    plt.show()

# Example usage:

def func(x):
    return (1 + np.sin((x / 4096) * 6.28318530717)) * 0.5
plot_math_function(func, 4096)