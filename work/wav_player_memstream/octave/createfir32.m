%low 500 hz
low = [0.003125, 
0.004644, 
0.006437, 
0.008484, 
0.010752, 
0.013190, 
0.015741, 
0.018331, 
0.020883, 
0.023315, 
0.025544, 
0.027492, 
0.029088, 
0.030274, 
0.031004, 
0.031250, 
0.031004, 
0.030274, 
0.029088, 
0.027492, 
0.025544, 
0.023315, 
0.020883, 
0.018331, 
0.015741, 
0.013190, 
0.010752, 
0.008484, 
0.006437, 
0.004644, 
0.003125];

low = 1.5*low;

%high 2Khz
high = [0.001202, 
0.003348, 
0.006215, 
0.009183, 
0.011263, 
0.011218, 
0.007792, 
-0.000000, 
-0.012597, 
-0.029674, 
-0.050063, 
-0.071841, 
-0.092579, 
-0.109728, 
-0.121046, 
0.875000, 
-0.121046, 
-0.109728, 
-0.092579, 
-0.071841, 
-0.050063, 
-0.029674, 
-0.012597, 
-0.000000, 
0.007792, 
0.011218, 
0.011263, 
0.009183, 
0.006215, 
0.003348, 
0.001202] ;



[lowh,w] = freqz(low,1,20480);
%midh = freqz (mid, 1, w);
highh = freqz (high, 1, w);

%plot(w,(abs(midh)+1.3*abs(highh)+1.5*abs(lowh)));
%plot(w,abs(lowh));
plot(w,log10(abs(highh)));

%wavy = 1.8*lowy + 0.1*midy + 1.5*highy;
%wavy = highy + lowy + midy;

%wavwrite (wavy, Fs, "gl_out.wav");

lowr = int32(round(low*(2^15)));
highr = int32(round(high*(2^15)));
%lowr=lowr';
%save 'demo.txt' lowr -ascii

fid = fopen('fir32.txt','w');
% fir lowpass
fprintf(fid,'const int16_t b_low_32k[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], lowr);
fprintf(fid,'0\n};\n\n');
% fir highpass
fprintf(fid,'const int16_t b_high_32k[FIR_ORDER] = {\n');
fprintf(fid,[repmat('%g ,',1,size(I,2)),'\n'], highr);
fprintf(fid,'0\n};\n\n');
fclose(fid);

sum(low)
sum(lowr)

