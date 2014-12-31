global m g M l B C J k K ce; 

# Controller Parameter
Q = diag([1e2,1,1e2,1]); # x xdot theta thetadot
R = 1.0;

# Control Enable
ce = 1;

# Simulator 
n = 100; # Run Time
initial_theta = 0;
#initial_theta = pi*3/4;

# Machine Parameter
m=3;              # [kg]
M=6;              # [kg]
l=1.6;            # CoP [m] (NOTE: This will also be identified by experiment)
J=m*(l**2)/3;     # inertia moment [kg m^2] 

# Visco between machine and stick [kg m^2/s] (0.2-2?)
# This is idenified by turning the robot upside down and giving micro vibration
C=0.1;
# motor torque when voltage is maximum and vel is 0. [N]
# This is identified by running with support and calc. (m+M)a_max_v_0 = (m+M)a_v_0*max_v/v 
# where a_max_v_0 is acceleration when applying duty = 1.0 and vel = 0.
k=100;
# Visco between machine and ground which is for motor force control [kg/s] (0.1-2?)
# This is identified by running with support and calc. (m+M)a_max_v_0/Vf
# where Vf is velocity when applying duty = 1.0 and t = inf.
B=0.1;


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


#### Calc. Controller Parameter
    [K, P, E] = lqr(A, b, Q, R);
