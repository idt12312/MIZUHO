function [ x, v ] = trapezoid( a, v_max, v1, v3, L, dt )

dir = sign(L);
L = abs(L);
v1 = abs(v1);
v3 = abs(v3);

N1 = floor(-(2*v1-sqrt(2*v1^2 + 2*v3^2 + 4*L*a))/(2*a*dt));
N3 = floor(-(2*v3-sqrt(2*v1^2 + 2*v3^2 + 4*L*a))/(2*a*dt));

a = (L - dt*(N1*v1 + N3*v3)) / (0.5*dt^2*(N1^2+N3^2));


if v1+a*N1*dt < v_max
    N2 = floor(0);
else
    N1 = floor((v_max - v1)/(dt*a));
    N3 = floor((v_max - v3)/(dt*a));
    
    L2 = L - (0.5*a*((N1*dt)^2+(N3*dt)^2) + v1*N1*dt+v3*N3*dt);
    N2 = floor(L2/(dt*v_max));
    v_max = L2/(dt*N2);
end



cnt = 0;
x = 0;
v = v1;
while cnt <= N1+N2+N3
    if cnt <= N1
        v_cur = v1 + a*dt*cnt;
    elseif cnt <= N1+N2
        v_cur = v_max;
    elseif cnt <= N1+N2+N3
        v_cur = v3 + a*dt*(N1+N2+N3-cnt);
    else
        v_cur = v3;
    end
    cnt = cnt + 1;

    
    %%v_sat = saturate(v_cur, v_max);
   
    
    x = [x; x(end)+v_cur*dt];
    v = [v; v_cur];
end

x = dir*x;
v = dir*v;

end

