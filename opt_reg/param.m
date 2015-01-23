global m g M l B C J k K Kv Kr Ux Uu ce is_easy_vel_control is_pos_control; 

# Control Enable
ce = 1;

# Simulator 
n = 100; # Run Time
initial_theta = 0;
#initial_theta = pi*3/4;

enable_ackermann = 0;           # if you want Ackermann control method, set 1
is_pos_control = 1;             # if you want position control, set 1
    is_easy_vel_control = 0;    # if you use easy_vel_control set 1. Make sure that is_pos_control = 1

# Machine Parameter
m=5.38;              # [kg]
M=1.12;              # [kg]

# CoP : NOTE THAT L IS NOT THE LENGTH OF PENDIUM [m] 
# This is identified by turning the robot upside down and giving micro vibration
l=0.556;
# Visco between machine and stick [kg m^2/s] (0.2-2?)
# This is identified by turning the robot upside down and giving micro vibration
C=0.848;
# Visco between machine and ground which is for motor force control [kg/s] (0.1-2?)
# This is identified by running with support and calc. (m+M)a_max_v_0/Vf
# where Vf is velocity when applying duty = 1.0 and t = inf.
B=18.057;

# motor torque when voltage is maximum and vel is 0. [N]
# This is identified by running with support and calc. (m+M)a_max_v_0 = (m+M)a_v_0*max_v/v 
# where a_max_v_0 is acceleration when applying duty = 1.0 and vel = 0.
k=56.530;

# inertia moment [kg m^2] 
J=m*(l**2)/3;     

# World Parameter
g=9.8066;

# unit parameter
Uu = diag([k]); # the force when duty is 1.0
Ux = diag([0.00001605491 0.00321098377 0.00000380851 0.00076170347]);

#### Linear Model
    s=J*(m+M)+M*m*l*l;
    if (is_pos_control)
        # for Position Control
        A=[
        0, 1, 0, 0;
        0, -B*(J+m*l*l)/s, -(m**2)*(l**2)*g/s, C*m*l/s;
        0, 0, 0, 1;
        0, B*m*l/s, (m+M)*m*l*g/s, -C*(m+M)/s;
        ];
        b=[ 0; (J+m*(l**2))/s; 0; -m*l/s ];
    else 
        # for Velocity Control
        Av=[
        -B*(J+m*l*l)/s, -(m**2)*(l**2)*g/s, C*m*l/s;
        0, 0, 1;
        B*m*l/s, (m+M)*m*l*g/s, -C*(m+M)/s;
        ];
        bv=[ (J+m*(l**2))/s; 0; -m*l/s ];
    endif



#### Controller
# Linear Quadratic Controller
    if (is_pos_control)
        if (!enable_ackermann)
            # Controller Parameter
            Q = diag([1e2,1,1e2,1]); # x xdot theta thetadot
            R = 1.0;
            #Q = diag([1e2,1,1e2,1]); # x xdot theta thetadot
            #R = 0.01;

            [K, P, E] = lqr(A, b, Q, R); # for Position Control
            K
            E
            K_for_robot=K*Ux
        else 
            Mc = [b A*b A*A*b A*A*A*b];
            P=conv(conv(conv([1 +3], [1 +3]), [1 +3]), [1 +3]); # -3 -3 -3 -3
            K = [0 0 0 1]*inv(Mc)*(P(1)*A*A*A*A+P(2)*A*A*A+P(3)*A*A+P(4)*A+P(5)*eye(4));
            K;
        endif # Parameter to implement (transfer mat: sensor data->SI unit, ex. if robot encoder unit is 1mm, fill unit = [0.001 x x x])
        # gDuty = -(Kr[1] * gAngle + Kr[2] * gOmega ...)
    else 
        if (!enable_ackermann)
            Qv = diag([1e5,1e2,1]); # x xdot theta thetadot
            Rv = 0.1;
            [Kv, Pv, Ev] = lqr(Av, bv, Qv, Rv); # for Velocity Control
            Kv;
            Ev;
        else 
            Mc = [bv Av*bv Av*Av*bv];
            P=conv(conv([1 +3], [1 +3]), [1 +3]); # -3 -3 -3
            Kv = [0 0 1]*inv(Mc)*(P(1)*Av*Av*Av+P(2)*Av*Av+P(3)*Av+P(4)*eye(3));
            Kv;
        endif
    endif


