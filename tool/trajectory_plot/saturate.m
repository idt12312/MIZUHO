function [ y ] = saturate( x, x_max )

x_abs = min(abs(x), abs(x_max));
y = x_abs * sign(x);

end

