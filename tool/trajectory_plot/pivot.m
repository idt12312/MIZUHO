omega_dot = 10;
omega_max = pi;
omega_start = 0;
omega_end = 0;
theta_end = -pi/2;
dt = 0.005;

[theta, omega] = trapezoid(omega_dot, omega_max, omega_start, omega_end, theta_end, dt);

N = length(theta);
t = 0:dt:dt*(N-1);

figure;
subplot(2,1,1); 
grid on;
plot(t,theta);

subplot(2,1,2); 
grid on;
plot(t,omega);
