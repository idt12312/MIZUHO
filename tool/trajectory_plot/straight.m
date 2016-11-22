ax = 2.0;
v_max = 1.0;
v_start = 0;
v_end = 0.4;
x_end = 1.0;
dt = 0.005;

[x, v] = trapezoid(ax, v_max, v_start, v_end, x_end, dt);

N = length(x);
t = 0:dt:dt*(N-1);

figure;
subplot(2,1,1); 
grid on;
plot(t,x);

subplot(2,1,2); 
grid on;
plot(t,v);
