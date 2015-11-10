clear
#global verbose; verbose = true;

%Load Data
mx = [0:0.1:10]';
global vy_tobe = 5*rand(size(mx)) + mx;

%Set Function
function vY = myfun( mX, vP )
    global vy_tobe;
    a=vP(1); b=vP(2);
    vY = a*mX + b - vy_tobe;
    #    vY(vY<0)=10*vY(vY<0); #上
    vY(vY>0)=10*vY(vY>0); #下
endfunction

%Inital Values
vp0 = [-100; 0]; # だいぶエクストリームな初期値（0.98,0.44とかが本当の値）

%NLS
opt = optimset('Display', 'iter');
[vyhat vp] = leasqr( mx, zeros(size(mx)), vp0, @myfun );

disp('========== RESULTS ==========');
vp
vyhat

plot(mx, vy_tobe);
hold on;
plot(mx, vp(1)*mx+vp(2))
hold off;
