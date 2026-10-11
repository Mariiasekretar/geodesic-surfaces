import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def plot_sphere():
    df = pd.read_csv('sphere.csv')
    fig = plt.figure(figsize=(8, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    u, v = np.mgrid[0:np.pi:50j, 0:2*np.pi:50j]
    x = np.sin(u) * np.cos(v)
    y = np.sin(u) * np.sin(v)
    z = np.cos(u)
    ax.plot_surface(x, y, z, color='gray', alpha=0.1, edgecolor='none')
    
    ax.plot(df['x'], df['y'], df['z'], color='red', linewidth=2.5)
    ax.set_axis_off()
    plt.savefig('figures/sphere_geodesics.png', bbox_inches='tight', dpi=150)
    plt.close()

def plot_torus():
    df = pd.read_csv('torus.csv')
    fig = plt.figure(figsize=(8, 8))
    ax = fig.add_subplot(111, projection='3d')
    
    R, r = 3.0, 1.0
    u, v = np.mgrid[0:2*np.pi:50j, 0:2*np.pi:50j]
    x = (R + r * np.cos(v)) * np.cos(u)
    y = (R + r * np.cos(v)) * np.sin(u)
    z = r * np.sin(v)
    ax.plot_surface(x, y, z, color='gray', alpha=0.1, edgecolor='none')
    
    ax.plot(df['x'], df['y'], df['z'], color='blue', linewidth=1.5)
    ax.set_axis_off()
    ax.set_zlim(-3, 3)
    plt.savefig('figures/torus_geodesics.png', bbox_inches='tight', dpi=150)
    plt.close()

if __name__ == '__main__':
    import os
    os.makedirs('figures', exist_ok=True)
    plot_sphere()
    plot_torus()
