%low 500 hz
low = [0.002752, 
0.003977, 
0.005376, 
0.006929, 
0.008605, 
0.010369, 
0.012179, 
0.013987, 
0.015743, 
0.017396, 
0.018896, 
0.020195, 
0.021253, 
0.022034, 
0.022514, 
0.022676, 
0.022514, 
0.022034, 
0.021253, 
0.020195, 
0.018896, 
0.017396, 
0.015743, 
0.013987, 
0.012179, 
0.010369, 
0.008605, 
0.006929, 
0.005376, 
0.003977, 
0.002752] ;
low = low*2;

%high 2Khz
high = [0.002843, 
0.003550, 
0.003589, 
0.002519, 
-0.000087, 
-0.004568, 
-0.011109, 
-0.019680, 
-0.030005, 
-0.041562, 
-0.053611, 
-0.065271, 
-0.075604, 
-0.083727, 
-0.088918, 
0.909297, 
-0.088918, 
-0.083727, 
-0.075604, 
-0.065271, 
-0.053611, 
-0.041562, 
-0.030005, 
-0.019680, 
-0.011109, 
-0.004568, 
-0.000087, 
0.002519, 
0.003589, 
0.003550, 
0.002843] ;


[lowh,w] = freqz(low,1,20480);
%midh = freqz (mid, 1, w);
highh = freqz (high, 1, w);

%plot(w,(abs(midh)+1.3*abs(highh)+1.5*abs(lowh)));
%plot(w,abs(lowh));
plot(w,abs(highh));

%wavy = 1.8*lowy + 0.1*midy + 1.5*highy;
%wavy = highy + lowy + midy;

%wavwrite (wavy, Fs, "gl_out.wav");

lowr = int32(round(low*(2^15)));
highr = int32(round(high*(2^15)));
%lowr=lowr';
%save 'demo.txt' lowr -ascii

fid = fopen('fir44.txt','w');
% fir lowpass
fprintf(fid,'const int16_t b_low_44k1[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], lowr);
fprintf(fid,'0\n};\n\n');
% fir highpass
fprintf(fid,'const int16_t b_high_44k1[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], highr);
fprintf(fid,'0\n};\n\n');
fclose(fid);

sum(low)
sum(lowr)

