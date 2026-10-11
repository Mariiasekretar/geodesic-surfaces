#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

const double R_SPHERE = 1.0;
const double R_TORUS = 3.0;
const double r_TORUS = 1.0;

struct State {
    double u, v;
    double du, dv;
};

State sphere_derivs(const State& s) {
    State d;
    d.u = s.du;
    d.v = s.dv;
    d.du = std::sin(s.u) * std::cos(s.u) * s.dv * s.dv;
    d.dv = -2.0 * (1.0 / std::tan(s.u)) * s.du * s.dv;
    return d;
}

State torus_derivs(const State& s) {
    State d;
    d.u = s.du;
    d.v = s.dv;
    double denom = R_TORUS + r_TORUS * std::cos(s.v);
    d.du = (2.0 * r_TORUS * std::sin(s.v) * s.du * s.dv) / denom;
    d.dv = -(denom * std::sin(s.v) * s.du * s.du) / r_TORUS;
    return d;
}

template <typename F>
State rk4_step(const State& s, double dt, F derivs) {
    State k1 = derivs(s);
    
    State s2 = {s.u + 0.5 * dt * k1.u, s.v + 0.5 * dt * k1.v, s.du + 0.5 * dt * k1.du, s.dv + 0.5 * dt * k1.dv};
    State k2 = derivs(s2);
    
    State s3 = {s.u + 0.5 * dt * k2.u, s.v + 0.5 * dt * k2.v, s.du + 0.5 * dt * k2.du, s.dv + 0.5 * dt * k2.dv};
    State k3 = derivs(s3);
    
    State s4 = {s.u + dt * k3.u, s.v + dt * k3.v, s.du + dt * k3.du, s.dv + dt * k3.dv};
    State k4 = derivs(s4);
    
    State next;
    next.u  = s.u  + (dt / 6.0) * (k1.u  + 2.0 * k2.u  + 2.0 * k3.u  + k4.u);
    next.v  = s.v  + (dt / 6.0) * (k1.v  + 2.0 * k2.v  + 2.0 * k3.v  + k4.v);
    next.du = s.du + (dt / 6.0) * (k1.du + 2.0 * k2.du + 2.0 * k3.du + k4.du);
    next.dv = s.dv + (dt / 6.0) * (k1.dv + 2.0 * k2.dv + 2.0 * k3.dv + k4.dv);
    return next;
}

double run_verification_test(int steps) {
    double T = 2.0 * M_PI;
    double dt = T / steps;
    double alpha = M_PI / 4.0; 
    
    State s = {M_PI / 2.0, 0.0, -std::sin(alpha), std::cos(alpha)};
    State init = s;

    for (int i = 0; i < steps; ++i) {
        s = rk4_step(s, dt, sphere_derivs);
    }

    double x_init = R_SPHERE * std::sin(init.u) * std::cos(init.v);
    double y_init = R_SPHERE * std::sin(init.u) * std::sin(init.v);
    double z_init = R_SPHERE * std::cos(init.u);

    double x_end = R_SPHERE * std::sin(s.u) * std::cos(s.v);
    double y_end = R_SPHERE * std::sin(s.u) * std::sin(s.v);
    double z_end = R_SPHERE * std::cos(s.u);

    return std::sqrt(std::pow(x_end - x_init, 2) + std::pow(y_end - y_init, 2) + std::pow(z_end - z_init, 2));
}

void run_simulation(const std::string& filename, State init, double T, double dt, bool is_sphere) {
    std::ofstream file(filename);
    file << "x,y,z\n";
    
    State s = init;
    int steps = static_cast<int>(T / dt);
    
    for (int i = 0; i <= steps; ++i) {
        double x, y, z;
        if (is_sphere) {
            x = R_SPHERE * std::sin(s.u) * std::cos(s.v);
            y = R_SPHERE * std::sin(s.u) * std::sin(s.v);
            z = R_SPHERE * std::cos(s.u);
        } else {
            x = (R_TORUS + r_TORUS * std::cos(s.v)) * std::cos(s.u);
            y = (R_TORUS + r_TORUS * std::cos(s.v)) * std::sin(s.u);
            z = r_TORUS * std::sin(s.v);
        }
        file << x << "," << y << "," << z << "\n";
        s = is_sphere ? rk4_step(s, dt, sphere_derivs) : rk4_step(s, dt, torus_derivs);
    }
}

int main() {
    std::cout << "=== VERIFICATION TEST ===" << std::endl;
    std::cout << "N = 60  steps error: " << run_verification_test(60) << std::endl;
    std::cout << "N = 600 steps error: " << run_verification_test(600) << std::endl;
    std::cout << "=========================" << std::endl;

    double alpha = M_PI / 4.0; 
    State sphere_init = {M_PI / 2.0, 0.0, -std::sin(alpha), std::cos(alpha)};
    run_simulation("sphere.csv", sphere_init, 2.0 * M_PI, 0.01, true);

    State torus_init = {0.0, 0.0, 0.15, 0.5};
    run_simulation("torus.csv", torus_init, 50.0, 0.01, false);

    return 0;
}
