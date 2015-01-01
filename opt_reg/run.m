param; # load machine parameter

function y = u(x, t)
    global k K Kr Ux;
    # Objective Point
    r=[0; 0; 0; 0]; 
#    r(1)=3
    if (t < 0.06*100)
        r(1)=3;
    elseif (t < 0.06*200)
        r(1)=0;
    elseif (t < 0.06*300)
        r(1)=-3;
    endif

    # calc. feedback force
    y = -K*(x-r);
    # NOTE: Imitating robot controller, the following line can copy to REAL ROBOT!
    # in real the later part will be raw data
    duty = -Kr * (inv(Ux)*x-inv(Ux)*r); 

#    printf("%.2f %.2f %.2f %.2f %.2f %.2f\n", y, duty, -(x(1)-r)*K(1), -x(2)*K(2), -x(3)*K(3), -x(4)*K(4));
    printf("%.2f %.2f\n", y, duty);
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
    t = linspace(0, 0.06*n, n);
    x0 = [0; 0; initial_theta; 0];
    xx1 = lsode("eom", x0, t);

    grid on;
    for i=1:n
        x1 = xx1(i,1); y1 = 0;
        x2 = x1 + l*sin(xx1(i,3)); y2 = l*cos(xx1(i,3));
        x3 = x1 + 2*l*sin(xx1(i,3)); y3 = 2*l*cos(xx1(i,3));

        title(sprintf("x1=%.1f, th1=%.1f, x2=%.1f, th2=%.1f", xx1(i,1),xx1(i,3)));
        hold off; plot([-5.5,5.5],[5.5,5.5]);
        hold on; plot([-5.5,5.5],[-0.1,-0.1]);
        hold on; plot([-5.5,5.5],[l*2+0.1,l*2+0.1]);
        hold on; plot([x1,x2],[y1,y2]);
        hold on; scatter(x2, y2, 0.1);
        hold on; plot([x2,x3],[y2,y3], "r");

        pause(0.04);
    endfor
