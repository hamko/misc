param; # load machine parameter

global flag;
flag = 0;
function y = u(x, t)
    global k K Kv Kr Uu Ux is_pos_control is_easy_vel_control flag;

    # Position Controller
    if (is_pos_control == 1) 
        r=[1; 0; 0; 0]; # Objective Point
        if (is_easy_vel_control) K(1)=0; endif  # for easy Velocity Control
        y = -K*(x-r); # calc. feedback force
        duty = -inv(Uu)*K*Ux * (inv(Ux)*x-inv(Ux)*r); # in real the later part will be raw data
        printf("%.2f %.2f %.2f %.2f %.2f %.2f\n", t, y, -(x(1)-r(1))*K(1), -x(2)*K(2), -x(3)*K(3), -x(4)*K(4));
    else
        # Velocity Controller
        xv = x(2:4);
        if (t > 3.0)
            flag = 1;
        endif 
        if (!flag)
            rv=[1; 0; 0];
        else 
            rv=[0.0; 0; 0];
        endif
        y = -Kv*(xv-rv);
        printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f\n", t, xv(1), rv(1), y, -(xv(1)-rv(1))*Kv(1), -xv(2)*Kv(2), -xv(3)*Kv(3));
    endif
endfunction

# x xdot theta thetadot
function dx = eom(x, t)
    global m g M l B C J ce count;

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
    t = linspace(0, 0.05*n, n);
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
        hold on; plot([x2,x3],[y2,y3], "r");
        hold on; scatter(x2, y2, 0.1);

        hold on; scatter(-5.5, t(i), 0.1);

        pause(0.01);
    endfor
