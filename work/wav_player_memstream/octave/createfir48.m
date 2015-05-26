%low 500 hz
low = [0.002611, 
0.003756, 
0.005057, 
0.006494, 
0.008038, 
0.009657, 
0.011313, 
0.012962, 
0.014559, 
0.016060, 
0.017418, 
0.018594, 
0.019549, 
0.020255, 
0.020688, 
0.020833, 
0.020688, 
0.020255, 
0.019549, 
0.018594, 
0.017418, 
0.016060, 
0.014559, 
0.012962, 
0.011313, 
0.009657, 
0.008038, 
0.006494, 
0.005057, 
0.003756, 
0.002611] ;

low = 2*low;

%high 2Khz
high = [0.002220, 
0.002367, 
0.001741, 
-0.000000, 
-0.003155, 
-0.007932, 
-0.014399, 
-0.022451, 
-0.031796, 
-0.041966, 
-0.052342, 
-0.062216, 
-0.070857, 
-0.077589, 
-0.081867, 
0.916667, 
-0.081867, 
-0.077589, 
-0.070857, 
-0.062216, 
-0.052342, 
-0.041966, 
-0.031796, 
-0.022451, 
-0.014399, 
-0.007932, 
-0.003155, 
-0.000000, 
0.001741, 
0.002367, 
0.002220] ;

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

fid = fopen('fir48.txt','w');
% fir lowpass
fprintf(fid,'const int16_t b_low_48k[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], lowr);
fprintf(fid,'0\n};\n\n');
% fir highpass
fprintf(fid,'const int16_t b_high_48k[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], highr);
fprintf(fid,'0\n};\n\n');
fclose(fid);

sum(low)
sum(lowr)

