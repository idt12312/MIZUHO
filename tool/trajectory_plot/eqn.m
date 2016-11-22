syms dt L N1 N2 v1 v2 a

eq1 = L == N1*dt*v1 + N2*dt*v2 + 0.5*(N1*dt)^2*a + 0.5*(N2*dt)^2*a;
eq2 = v1 + N1*dt*a == v2 + N2*dt*a;

S = solve([eq1, eq2], [N1, N2]);

N1_ans = simplify(S.N1);
N2_ans = simplify(S.N2);