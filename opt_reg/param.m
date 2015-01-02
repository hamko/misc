global m g M l B C J k K Kr Ux ce; 

# Controller Parameter
#### E: -0.77, -2.4
#Q = diag([1e2,1,1e2,1]); # x xdot theta thetadot
#R = 1.0;
#### E: -2.0, -3.1
Q = diag([1e2,1,1e2,1]); # x xdot theta thetadot
R = 0.01;

# Control Enable
ce = 1;

# Simulator 
n = 100; # Run Time
initial_theta = 0;
#initial_theta = pi*3/4;

# Machine Parameter
m=3;              # [kg]
M=6;              # [kg]

# CoP : NOTE THAT L IS NOT THE LENGTH OF PENDIUM [m] 
# This is identified by turning the robot upside down and giving micro vibration
l=0.7;
# Visco between machine and stick [kg m^2/s] (0.2-2?)
# This is identified by turning the robot upside down and giving micro vibration
C=0.1;
# motor torque when voltage is maximum and vel is 0. [N]
# This is identified by running with support and calc. (m+M)a_max_v_0 = (m+M)a_v_0*max_v/v 
# where a_max_v_0 is acceleration when applying duty = 1.0 and vel = 0.
k=30;
# Visco between machine and ground which is for motor force control [kg/s] (0.1-2?)
# This is identified by running with support and calc. (m+M)a_max_v_0/Vf
# where Vf is velocity when applying duty = 1.0 and t = inf.
B=0.1;

J=m*(l**2)/3;     # inertia moment [kg m^2] 

# World Parameter
g=9.8066;

#### Linear Model
    s=J*(m+M)+M*m*l*l;
    A=[
    0, 1, 0, 0;
    0, -B*(J+m*l*l)/s, -(m**2)*(l**2)*g/s, C*m*l/s;
    0, 0, 0, 1;
    0, B*m*l/s, (m+M)*m*l*g/s, -C*(m+M)/s;
    ];
    b=[ 0; (J+m*(l**2))/s; 0; -m*l/s ];


#### Controller
# Linear Quadratic Controller
    [K, P, E] = lqr(A, b, Q, R);
    K;
    E;
# Ackermann's Pole Placement Method
#    Mc = [b A*b A*A*b A*A*A*b];
#    P=conv(conv(conv([1 +3], [1 +3]), [1 +3]), [1 +3]); # -3 -3 -3 -3
#    K = [0 0 0 1]*inv(Mc)*(P(1)*A*A*A*A+P(2)*A*A*A+P(3)*A*A+P(4)*A+P(5)*eye(4));
#    K

# Parameter to implement (transfer mat: sensor data->SI unit, ex. if robot encoder unit is 1mm, fill unit = [0.001 x x x])
# gDuty = -(below[1] * gAngle + below[2] * gOmega ...)
    Uu = diag([k]); # duty 1.0 is k
    Ux = diag([1 1 1 1]); # 'x' of the robot is as below
    Kr = inv(Uu)*K*Ux;

