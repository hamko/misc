x=[1:50];
y=rand(1,50).*x;
[a_u, b_u] = leasqr_line(x, y, 1);
[a_l, b_l] = leasqr_line(x, y, 0);

plot(x, y); hold on;
plot(x, a_u*x+b_u);
plot(x, a_l*x+b_l); hold off;

pause
