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
#Ux = diag([0.00001605491 0.00321098377 0.00000380851 0.00076170347]);
Ux = diag([0.00001605491 0.00321098377 1 1]);

#### Linear Model
s=J*(m+M)+M*m*l*l;
# for Position Control
A=[
0, 1, 0, 0;
0, -B*(J+m*l*l)/s, -(m**2)*(l**2)*g/s, C*m*l/s;
0, 0, 0, 1;
0, B*m*l/s, (m+M)*m*l*g/s, -C*(m+M)/s;
];
b=[ 0; (J+m*(l**2))/s; 0; -m*l/s ];

stabilitymanifold = [];
i = 0;
n = 100000;
scale = [-1000, -500, -1000, -500];
for i = 1:n
    f = (ones(1, 4)-rand(1, 4)).*scale;
    p = poly(A-b*f);
    r = roots(p);
    if (r < 0)
        stabilitymanifold = [stabilitymanifold; r' f];
    endif
    i = i + 1;
    if (rem(i, 1000) == 0)
        printf("%d/%d\n", i, n);
    endif
endfor

dlmwrite('stab', stabilitymanifold, ' ');
