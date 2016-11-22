omega_dot = 50;
omega_max = 7;
omega_start = 0;
omega_end = 0;
theta_end = -pi/2;
dt = 0.005;

v_const = 0.3;

[theta, omega] = trapezoid(omega_dot, omega_max, omega_start, omega_end, theta_end, dt);

N = length(theta);
t = 0:dt:dt*(N-1);

figure;
subplot(2,1,1); 
grid on;
plot(t,theta,'.');

subplot(2,1,2); 
grid on;
plot(t,omega,'.');


pos_cur = [0;0;];
pos = pos_cur';

for i = 1:N
    R = [cos(theta(i)), -sin(theta(i)); sin(theta(i)), cos(theta(i))];
    pos_cur = pos_cur + R*[0;v_const]*dt;
    pos = [pos; pos_cur'];
end



x_end = pos(end,1);
y_end = pos(end,2);
if y_end < 0.090
    straight_len = 0.090 - y_end;
else 
    straight_len = 0;
end

N_straight = floor(straight_len / v_const / dt);
pos_straight = 0:v_const*dt:v_const*dt*(N_straight-1);
figure;
grid on;
hold on;
plot(0*ones(N_straight,1), pos_straight','.','MarkerSize',10);
plot(pos(:,1),pos(:,2)+straight_len,'.','MarkerSize',10);
plot(pos_straight'+x_end, (y_end+straight_len)*ones(N_straight,1),'.','MarkerSize',10);