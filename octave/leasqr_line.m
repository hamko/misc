function [a, b] = leasqr_line(varargin) % dir=1: upper limit, dir=0: lower limit
    if (size(varargin)(2)>3)
        global g_linecoeff; g_linecoeff = varargin{4};
    else
        global g_linecoeff; g_linecoeff = 10;
    end
    mx = varargin{1}; my_in = varargin{2}; dir = varargin{3};

    % Transfer to origin for optimization
    mean_mx = mean(mx);
    mx -= mean_mx;
    mean_my_in = mean(my_in);
    my_in -= mean_my_in;

    global g_vy_tobe; g_vy_tobe = my_in;
    global g_dir; g_dir = dir;

    %Inital Values
%    vp = polyfit(mx, my_in, 1);
    if (dir == 0)
        vp = [0, min(my_in)];
    else 
        vp = [0, max(my_in)];
    end

    %NLS
    [vyhat vp] = leasqr(mx, zeros(size(mx)), vp, @myfun );
    a = vp(1);
    b = vp(2);
   
    % yo = a * xo + b
    % (y - ym) = a * (x - xm) + b
    % y = a * x + ym - a * xm + b
    b += mean_my_in - a * mean_mx;
%    plot(mx+mean_mx, my_in+mean_my_in); hold on;
%    plot(mx+mean_mx,a*mx+b+mean_my_in); hold off;
%    plot(mx, my_in); hold on;
%    plot(mx,a*mx+b); hold off;
end

%Set Function
function vY = myfun(mX, vP)
    global g_vy_tobe; global g_dir; global g_linecoeff;
    a=vP(1); b=vP(2);
    vY = a*mX + b - g_vy_tobe;
    if (g_dir == 0) 
        vY(vY>0)=g_linecoeff*vY(vY>0); #下
    else
        vY(vY<0)=g_linecoeff*vY(vY<0); #上
    end
end
