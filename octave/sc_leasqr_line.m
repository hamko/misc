x=[   85   86   87   88   89   90   91   92   93   94   95];
y=[  7622   7622   7624   7627   7625   7626   7625   7625   7626   7626   7626];
%y=rand(size(x));

plot(x, y); hold on;
[a, b] = leasqr_line(x, y, 0, 4);
plot(x,a*x+b);
i = 4;
[a, b] = leasqr_line(x, y, 1, i)
plot(x,a*x+b); 
hold off;

pause
