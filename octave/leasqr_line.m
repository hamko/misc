function [a, b] = leasqr_line(varargin) % dir=1: upper limit, dir=0: lower limit
    if (size(varargin)(2)>3)
        global g_linecoeff; g_linecoeff = varargin{4};
    else
        global g_linecoeff; g_linecoeff = 10;
    end
    mx = varargin{1}; my_in = varargin{2}; dir = varargin{3};
%    global mx = mx_in;
%    global vy_tobe = my_in;
    global g_vy_tobe; g_vy_tobe = my_in;
    global g_dir; g_dir = dir;

    %Inital Values
    vp0 = [0; 0]; # だいぶエクストリームな初期値（0.98,0.44とかが本当の値）

    %NLS
    opt = optimset('Display', 'iter');
    [vyhat vp] = leasqr(mx, zeros(size(mx)), vp0, @myfun );
    a = vp(1);
    b = vp(2);
    disp('========== RESULTS ==========');
    vp
    vyhat

    plot(mx, g_vy_tobe);
    hold on;
    plot(mx, vp(1)*mx+vp(2))
    hold off;

    g_dir
end

%Set Function
function vY = myfun(mX, vP)
    global g_vy_tobe; global g_dir; global g_linecoeff;
    a=vP(1); b=vP(2);
    vY = a*mX + b - g_vy_tobe;
    if (g_dir == 0) 
        vY(vY>0)=g_linecoeff*vY(vY>0); #下
    elseif
        vY(vY<0)=g_linecoeff*vY(vY<0); #上
    end
end
