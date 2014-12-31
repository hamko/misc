param; # load machine parameter

function y = u(x, t)
    global k K;
    r=3.0; 
#    r=t;
    y = -K*[x(1)-r; x(2); x(3); x(4)];
    duty = y / k;
    printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", y, duty, x(2), -(x(1)-r)*K(1), -x(2)*K(2), -x(3)*K(3), -x(4)*K(4));
endfunction

# x xdot theta thetadot
function dx = eom(x, t)
    global m g M l B C J ce;

    # calc. accelation
    if (ce) u = u(x,t); else u = 0; endif 
    S = [M+m, m*l*cos(x(3));
         m*l*cos(x(3)),J+m*(l**2)];
    T = [u+m*l*(x(4)**2)*sin(x(3))-B*x(2);
         m*l*g*sin(x(3))-C*x(4)];
    accel = S\T;

    # e.o.m
    dx(1) = x(2); dx(2) = accel(1);
    dx(3) = x(4); dx(4) = accel(2);
endfunction


#Simulator
    t = linspace(0, 0.06*n, n)
    x0 = [0; 0; initial_theta; 0];
    xx1 = lsode("eom", x0, t);

    grid on;
    for i=1:n
        x1 = xx1(i,1); y1 = 0;
        x2 = x1 + l*sin(xx1(i,3)); y2 = l*cos(xx1(i,3));

        title(sprintf("x1=%.1f, th1=%.1f, x2=%.1f, th2=%.1f", xx1(i,1),xx1(i,3)));
        hold off; plot([-5.5,5.5],[5.5,5.5]);
        hold on; plot([-5.5,5.5],[-0.1,-0.1]);
        hold on; plot([-5.5,5.5],[l+0.1,l+0.1]);
        hold on; plot([x1,x2],[y1,y2]);

        pause(0.04);
    endfor
