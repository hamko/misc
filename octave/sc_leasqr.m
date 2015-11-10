global verbose; verbose = true;

%Load Data
mx = [0:0.1:10]';
vy = mx.*mx;

%Set Function
function vY = myfun( mX, vP )
a=vP(1); b=vP(2); c=vP(3);
vY = a*mX(:,1).*mX(:,1) + b*mX(:,1) + c;
endfunction

%Inital Values
vp0 = [0.9; 0; 0];

%NLS
opt = optimset('Display', 'iter');
[vyhat vp] = leasqr( mx, vy, vp0, @myfun );
ssr = (vy-vyhat)'*(vy-vyhat);

disp('========== RESULTS ==========');
vp
ssr

%Output Results
myfile = fopen('output_nls.txt', 'w');
fprintf(myfile, '========== RESULTS ==========\n');
fprintf(myfile, 'vp = %12.7f\n', vp);
fprintf(myfile, 'SSR = %12.7f\n', ssr);
fclose(myfile);
