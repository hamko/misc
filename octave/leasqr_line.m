function [a, b] = leasqr_line(varargin) % dir=1: upper limit, dir=0: lower limit
if (size(varargin)(2)>3)
    global g_linecoeff; g_linecoeff = varargin{4};
else
    global g_linecoeff; g_linecoeff = 10;
end
mx = varargin{1}; my_in = varargin{2}; dir = varargin{3};
if (size(mx, 1) > size(mx, 2))
    mx = mx';
end
if (size(my_in, 1) > size(my_in, 2))
    my_in = my_in';
end

% Transfer to origin for optimization
mean_mx = mean(mx);
mx -= mean_mx;
absmax_mx = max(abs(mx));
mx /= absmax_mx;
mean_my_in = mean(my_in);
my_in -= mean_my_in;
absmax_my_in = max(abs(my_in));
my_in /= absmax_my_in;


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
min_vysq = 1e8;
search_dir=[1,0;-1,0;0,1;0,-1];
for i = [1:50]
    vp_cand = vp + search_dir(mod(i, 4)+1, :) / 10;
    vy = myfun(mx, vp_cand);
    if (min_vysq > vy*vy')
        min_vysq = vy*vy';
        vp = vp_cand;
    end
end
a_prev = vp(1);
b_prev = vp(2);

% yo = a * xo + b
% (y - ym) / ys = a * (x - xm) / xs + b
% y=ys*a(x-xm)/xs+ys*b+ym
% y=ys*a*x/xs-ys*a*xm/xs+ys*b+ym
a = +absmax_my_in * a_prev / absmax_mx;
b = -absmax_my_in * a_prev * mean_mx / absmax_mx + absmax_my_in * b_prev + mean_my_in;
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
